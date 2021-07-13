import os
import sys
import re
import signal
import subprocess
from pathlib import Path

class gem5DebugRun(object):
    def __init__(self, parent=None):
        self.make_process = None
        
    def gem5DebugRun(self, instName, compile, debug):
        gcc_make_cmdout = os.popen("cd ../test_bench && gcc test_bench_{}.c -static -o test_bench_{} && sync".format(instName, instName))
        print(gcc_make_cmdout.read())
        c_exec_cmdout = os.popen("cd ../test_bench && ./test_bench_{} | grep 'output' > test_bench_{}_output.log && sync".format(instName, instName))
        print(c_exec_cmdout.read())

        if (compile):
            os.popen("cp ../gem5/src/arch/power/isa/decoder.isa ../gem5/src/arch/power/isa/decoder.isa.bak")
            os.popen("cp decoder.isa ../gem5/src/arch/power/isa/decoder.isa && sync")
            if (self.make_process != None):
                os.killpg(os.getpgid(self.make_process.pid), signal.SIGTERM)
                self.make_process.kill()

        if (debug):
            self.make_process = subprocess.Popen("cd ../gem5 && \
            	scons build/POWER/gem5.opt -j160 && \
            	build/POWER/gem5.opt --debug-flags=O3CPUAll,Registers configs/example/se.py -c ../test_bench/test_bench_{} \
                | grep 'output'> gem5_{}_output.log && sync && cat gem5_{}_output.log && cd .. && \
                echo 'comparing results:\n' && \
                diff gem5/gem5_{}_output.log test_bench/test_bench_{}_output.log && \
                cp gem5/src/arch/power/isa/decoder.isa.bak gem5/src/arch/power/isa/decoder.isa".format(instName, instName, instName, instName, instName), shell=True, preexec_fn=os.setsid)
            #print(gem5_make_cmdout.read())
        else:
            self.make_process = subprocess.Popen("cd ../gem5 && \
            	scons build/POWER/gem5.opt -j160 && \
            	build/POWER/gem5.opt --debug-flags=O3CPUAll,Registers configs/example/se.py -c ../test_bench/test_bench_{} \
                | grep 'output'> gem5_{}_output.log && sync && cd .. && \
                diff gem5/gem5_{}_output.log test_bench/test_bench_{}_output.log &&".format(instName, instName, instName, instName), shell=True, preexec_fn=os.setsid)

if __name__ == "__main__":
    test = gem5DebugRun()
    test.gem5DebugRun(sys.argv[1], argv[2] == "1", argv[3] == "1")
