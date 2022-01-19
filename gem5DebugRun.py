import os
import sys
import re
import signal
import subprocess
from pathlib import Path

class gem5_debug_run(object):
    def __init__(self, parent=None):
        self.make_process = None
        
    def gem5_debug_run(self, instName, compile, test_only):

        if (test_only) :
            cmd1 = ""
            cmd3 = ""
        else:
            cmd1 = "cp ../gem5/src/arch/power/isa/decoder.isa ../gem5/src/arch/power/isa/decoder.isa.bak && \
                    cp decoder.isa ../gem5/src/arch/power/isa/decoder.isa &&"
            cmd3 = "cp src/arch/power/isa/decoder.isa.bak src/arch/power/isa/decoder.isa &&"

        if (compile):
            cmd2 = "scons build/POWER/gem5.opt -j160 &&"
            if (self.make_process != None):
                os.killpg(os.getpgid(self.make_process.pid), signal.SIGTERM)
                self.make_process.kill()
        else:
            cmd2 = ""

        cmd_str = " {0} \
                    cd ../test_bench && \
                    gcc test_bench_{2}.c -static -mregnames -o test_bench_{2} && \
                    ./test_bench_{2} | grep 'output' > test_bench_{2}_output.log && \
                    cd ../gem5 && \
            	    {1} \
            	    {3} \
                    build/POWER/gem5.opt configs/example/se.py --cpu-type=DerivO3CPU \
                    --caches --l1d_size=1024 --l1i_size=1024 --l2cache --l2_size=1024 \
                    -c ../test_bench/test_bench_{2} \
                    | grep 'output'> gem5_{2}_output.log && \
                    cat gem5_{2}_output.log && \
                    cd .. && \
                    echo 'comparing results:\n' && \
                    diff gem5/gem5_{2}_output.log test_bench/test_bench_{2}_output.log -y -W 400; \
                    if [ $? -eq 0 ]; then echo '\e[32m\e[1mUnit test passed!\e[0m' ; else echo '\e[31m\e[1mUnit test failed!\e[0m'; fi ;\
                        ".format(cmd1, cmd2, instName, cmd3)
        #print(cmd_str)

        self.make_process = subprocess.Popen(cmd_str, shell=True, preexec_fn=os.setsid)

    def gem5_commit_file(self):
        cmd_str = "cp decoder.isa ../gem5/src/arch/power/isa/decoder.isa && echo '\e[32m\e[1mfile committed.\e[0m'"
        subprocess.Popen(cmd_str, shell=True, preexec_fn=os.setsid)

if __name__ == "__main__":
    test = gem5_debug_run()
    test.gem5_debug_run(sys.argv[1], argv[2] == "1", argv[3] == "1")
