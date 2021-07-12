import os
import sys
import re
from pathlib import Path

class gem5CodeInsert(object):
    def openInsertFile(self, code, fileName):

        code_lines = code.split("\n")
        ret1 = re.match("    (.*): decode (.*) {", code_lines[1])
        ret2 = re.match("        format (.*) {", code_lines[2])
        place_holder = "// ###{}: decode {} {}###".format(ret1.group(1), ret1.group(2), ret2.group(1))
        print(place_holder)

        #os.popen("cp ../gem5/src/arch/power/isa/decoder.isa . && sync")
        fileName = "../gem5/src/arch/power/isa/decoder.isa"
        fd_rd = open(fileName, "rt+")

        isa = fd_rd.readlines()
        fd_rd.close()
        space = ""
        for eachline in isa:
            if place_holder in eachline:
                print(eachline)
                ret = re.match("(\s*)(.*)###", eachline)
                if ret != None:
                    #print(ret.groups())
                    space = ret.group(1)
        #print("space:{}".format(space))

        space_cnt = len(space)
        #print(space_cnt)
        space_code = ""
        for i in range(0, space_cnt - 12):
            space_code += ' '

        insert_code = "\n"
        for eachline in code_lines[3:-4]:
            eachline = "{}{}\n".format(space_code, eachline)
            insert_code += eachline
        print(insert_code)

        fd_rd = open(fileName, "rt+")
        isa = fd_rd.read()

        isa = isa.replace(place_holder, "{}{}{}\n".format(insert_code, space, place_holder))
        fd_rd.close()

        fd_wr = open("decoder.isa", "wt")
        fd_wr.write(isa)

        fd_wr.close()

        return str

    def findBlock(self, lines, str):
        i = 0
        bracket_match = 0
        flag = False
        flag1 = False
        start = 0
        end = 0
        for line in lines:
            i = i + 1
            if (str in line):
                flag = True
                start = i
            if (flag) :
                for j in range(0, len(line)) :

                    if (line[j] == '{'):
                        flag1 = True
                        bracket_match = bracket_match - 1

                    if (line[j] == '}'):
                        bracket_match = bracket_match + 1

                    if (flag1 and bracket_match == 0):
                        end = i
                        return start,end

        return -1,-1


if __name__ == "__main__":
    test = gem5CodeInsert()
    test.openInsertFile(sys.argv[1], sys.argv[2])
