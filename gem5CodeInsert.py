import os
import sys
import re
from pathlib import Path

class gem5CodeInsert(object):
    def openInsertFile(self, code, fileName):
        fd_rd = open(fileName, "rt+")
        str = fd_rd.read()

        fd_rd.close()
        return str

if __name__ == "__main__":
    test = gem5CodeInsert()
    test.openInsertFile(sys.argv[1], sys.argv[2], sys.argv[3])