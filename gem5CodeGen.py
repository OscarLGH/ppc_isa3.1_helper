import os
import sys
import re

class gem5_code_gen(object):
    def __init__(self, parent=None):
        self.comment = ""
        self.code_seg_1 = ""
        self.code_seg_2 = ""
        self.code_seg_3 = ""

    def open_replace_template(self, template_file, po,
                        bitfields, format, xo, mnemonics, comment, src1_idx, src2_idx,
                        src3_idx, dst_idx, src1_type, src2_type,
                        src3_type, dst_type, iterations):
        fd_rd = open(template_file, "rt")
        str = fd_rd.read()
        str = str.replace('"###PO###"', po)
        str = str.replace('"###BITFIELDS###"', bitfields)
        str = str.replace('"###FORMAT###"', format)
        str = str.replace('"###XO###"', xo)
        str = str.replace('"###MNEMONICS###"', mnemonics)

        if (comment == ""):
            str = str.replace('"###COMMENT###"', "//comment")
        else:
            str = str.replace('"###COMMENT###"', comment)

        str = str.replace('"###REGA_IDX###"', src1_idx)
        str = str.replace('"###REGB_IDX###"', src2_idx)
        str = str.replace('"###REGC_IDX###"', src3_idx)
        str = str.replace('"###REGT_IDX###"', dst_idx)

        str = str.replace('"###REGA_TYPE###"', src1_type)
        str = str.replace('"###REGB_TYPE###"', src2_type)
        str = str.replace('"###REGC_TYPE###"', src3_type)
        str = str.replace('"###REGT_TYPE###"', dst_type)

        if (self.code_seg_1 == ""):
            str = str.replace('"###CODE_SEG_1###"', "//code seg 1")
        else:
            str = str.replace('"###CODE_SEG_1###"', self.code_seg_1)
        str = str.replace('"###ITERATIONS###"', iterations)
        if (self.code_seg_2 == ""):
            str = str.replace('"###CODE_SEG_2###"', "auto tmp = vra_val[i] + vrb_val[i];")
        else:
            str = str.replace('"###CODE_SEG_2###"', self.code_seg_2)

        if (self.code_seg_3 == ""):
            str = str.replace('"###CODE_SEG_3###"', "//code seg 3")
        else:
            str = str.replace('"###CODE_SEG_3###"', self.code_seg_3)

        fd_rd.close()
        return str

    def save_code(self, code):
        ret = re.findall("/\* CODE SEG 1 \*/\n\s*(.*)\n\s*/\* CODE SEG 1 END \*/", code, flags=re.DOTALL + re.MULTILINE)
        self.code_seg_1 = ret[0]
        ret = re.findall("/\* CODE SEG 2 \*/\n\s*(.*)\n\s*/\* CODE SEG 2 END \*/", code, flags=re.DOTALL + re.MULTILINE)
        self.code_seg_2 = ret[0]
        ret = re.findall("/\* CODE SEG 3 \*/\n\s*(.*)\n\s*/\* CODE SEG 3 END \*/", code, flags=re.DOTALL + re.MULTILINE)
        self.code_seg_3 = ret[0]


if __name__ == "__main__":
    test = gem5_code_gen()
    test.open_replace_template(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4],
                        sys.argv[5], sys.argv[6], sys.argv[7], sys.argv[8],
                        sys.argv[9], sys.argv[10], sys.argv[11], sys.argv[12],
                        sys.argv[13], sys.argv[14], sys.argv[15]
                        )
