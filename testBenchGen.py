import os
import sys
import re
import subprocess
from pathlib import Path

class testBenchGen(object):
    def openReplaceFile(self, fileName, instName, instLine):
        fd_rd = open(fileName, "rt")
        path_name = "../test_bench"
        test_path = Path(path_name)
        if not test_path.is_dir() :
            os.mkdir(path_name)

        fd_wr = open("../test_bench/test_bench_{}.c".format(instName), "w")
        str = fd_rd.read()
        str = str.replace("{###place_holder###}", "{}".format(instLine))
        fd_wr.write(str)
        fd_wr.close()
        fd_rd.close()
        subprocess.Popen("cd ../test_bench && gedit test_bench_{0}.c && \
                gcc test_bench_{0}.c -static -o test_bench_{0} && \
                ./test_bench_{0} | grep 'output' > test_bench_{0}_output.log &&\
                cat test_bench_{0}_output.log \
                    ".format(instName), 
                shell=True, preexec_fn=os.setsid)
        return str

if __name__ == "__main__":
    test = testBenchGen()
    test.openReplaceFile(sys.argv[1], sys.argv[2])
