import os
import sys
import re
from pathlib import Path

class gem5CodeGen(object):
    def openReplaceTemplate(self, templateFileName, po,
                        bitfields, format, xo, mnemonics, comment, src1, src2,
                        src3, dst, iterations):
        fd_rd = open(templateFileName, "rt")
        str = fd_rd.read()
        str = str.replace('"###PO###"', po)
        str = str.replace('"###BITFIELDS###"', bitfields)
        str = str.replace('"###FORMAT###"', format)
        str = str.replace('"###XO###"', xo)
        str = str.replace('"###MNEMONICS###"', mnemonics)
        str = str.replace('"###COMMENT###"', comment)
        str = str.replace('"###VRA###"', src1)
        str = str.replace('"###VRB###"', src2)
        str = str.replace('"###VRC###"', src3)
        str = str.replace('"###VRT###"', dst)
        str = str.replace('"###ITERATIONS###"', iterations)
        fd_rd.close()
        return str

if __name__ == "__main__":
    test = gem5CodeGen()
    test.openReplaceTemplate(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4],
                        sys.argv[5], sys.argv[6], sys.argv[7], sys.argv[8],
                        sys.argv[9]
                        )