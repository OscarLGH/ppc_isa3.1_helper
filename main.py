
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow
from mainForm import Ui_Form

from testBenchGen import testBenchGen
from gem5CodeGen import gem5_code_gen
from gem5CodeInsert import gem5CodeInsert
from gem5DebugRun import gem5_debug_run
 
class MyMainForm(QMainWindow, Ui_Form):
    def __init__(self, parent=None):
        super(MyMainForm, self).__init__(parent)
        self.setupUi(self)
        self.code = None
        self.run = None


if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWin = MyMainForm()

    def generate_code(win):
        Mnemonics = win.lineEdit_mnemonics.text()
        pseudocode = win.textEdit_Pseudocode.toPlainText()
        PO = win.lineEdit_PO.text()
        XO = win.lineEdit_XO.text()
        format = win.comboBox_Format.currentText()
        format2 = win.comboBox_Format_2.currentText()
        comboBox_Src1RegType = win.comboBox_Src1RegType.currentText()
        comboBox_Src2RegType = win.comboBox_Src2RegType.currentText()
        comboBox_Src3RegType = win.comboBox_Src3RegType.currentText()
        comboBox_DstRegType = win.comboBox_DstRegType.currentText()
        comboBox_Src1ElemType = win.comboBox_Src1ElemType.currentText()
        comboBox_Src2ElemType = win.comboBox_Src2ElemType.currentText()
        comboBox_Src3ElemType = win.comboBox_Src3ElemType.currentText()
        comboBox_DstElemType = win.comboBox_DstElemType.currentText()
        

        op_bits = 128
        if (comboBox_DstElemType == "uint8_t" or comboBox_DstElemType == "int8_t"):
            op_bits = 8
        if (comboBox_DstElemType == "uint16_t" or comboBox_DstElemType == "int16_t"):
            op_bits = 16
        if (comboBox_DstElemType == "uint32_t" or comboBox_DstElemType == "int32_t" or comboBox_DstElemType == "float"):
            op_bits = 32
        if (comboBox_DstElemType == "uint64_t" or comboBox_DstElemType == "int64_t" or comboBox_DstElemType == "double"):
            op_bits = 64

        isa_format = ""
        if (format == "VA Form"):
            isa_format = "VA_XO"
        if (format == "VX Form"):
            isa_format = "VX_XO"
        if (format == "XX1 Form"):
            isa_format = "XX1_XO"
        if (format == "XX2 Form"):
            isa_format = "XX2_XO"
        if (format == "XX3 Form"):
            isa_format = "XX3_XO"

        if (format == "VA Form" or format == "VX Form" or format == "VC Form"):
            src1_idx = "VRA + 32"
            src2_idx = "VRB + 32"
            src3_idx = "VRC + 32"
            dst_idx = "VRT + 32"

        if (format == "XX1 Form" or format == "XX2 Form" or format == "XX3 Form"):
            src1_idx = "32 * XX_AX + XX_A"
            src2_idx = "32 * XX_BX + XX_B"
            src3_idx = "32 * XX_CX + XX_C"
            dst_idx = "32 * XX_TX + XX_T"

        if (win.code == None) :
            win.code = gem5_code_gen()
        else:
            win.code.save_code(win.textEdit_generatedCode.toPlainText())

        code_str = win.code.open_replace_template("gem5_code_template.isa", PO, isa_format, format2, XO, Mnemonics,pseudocode,
                                        src1_idx, src2_idx, src3_idx, dst_idx, comboBox_Src1ElemType, comboBox_Src2ElemType, comboBox_Src3ElemType, comboBox_DstElemType,
                                        "{:.0f}".format(128 / op_bits))
        win.textEdit_generatedCode.setText(code_str)

    def generate_test(win):
        format = win.comboBox_Format.currentText()
        Mnemonics = win.lineEdit_mnemonics.text()
        machineCode = win.MachineCode.isChecked()
        PO = win.lineEdit_PO.text()
        XO = win.lineEdit_XO.text()
        if (PO == ""):
            PO = "0"
        if (XO == ""):
            XO = "0"

        disassemble_inst = ""
        disassemble_inst_same_operand = ""
        po_code = int(PO, 10)
        xo_code = int(XO, 10)
        vrt = 3
        vra = 0
        vrb = 1
        vrc = 2

        inst_type = ""
        if (format == "VA Form"):
            disassemble_inst = "{0} %%v3, %%v0, %%v1, %%v2".format(Mnemonics)
            disassemble_inst_same_operand = "{0} %%v4, %%v4, %%v4, %%v4".format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (vrc << 6) | (xo_code)
            inst_type = "vector"
        if (format == "VX Form"):
            disassemble_inst = "{0} %%v3, %%v0, %%v1".format(Mnemonics)
            disassemble_inst_same_operand = "{0} %%v4, %%v4, %%v4".format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code)
            inst_type = "vector"
        if (format == "VC Form"):
            disassemble_inst = "{0} %%v3, %%v0, %%v1".format(Mnemonics)
            disassemble_inst_same_operand = "{0} %%v4, %%v4, %%v4".format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 1) | 0x1
            inst_type = "vector"
        if (format == "XX1 Form"):
            disassemble_inst = '{0} %%vs3 \\n""{0} %%vs35 \\n'.format(Mnemonics)
            disassemble_inst_same_operand = "{0} %%vs4 \\n""{0} %%vs36 \\n".format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 1) | 0x1
            inst_type = "scalar"
        if (format == "XX2 Form"):
            disassemble_inst = '{0} %%vs3, %%vs0 \\n""{0} %%vs35, %%vs32 \\n'.format(Mnemonics)
            disassemble_inst_same_operand = '{0} %%vs4, %%vs4 \\n""{0} %%vs36, %%vs36 \\n'.format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 2) | 0x3
            inst_type = "scalar"
        if (format == "XX3 Form"):
            disassemble_inst = '{0} %%vs3, %%vs0, %%vs1 \\n""{0} %%vs35, %%vs32, %%vs33 \\n'.format(Mnemonics)
            disassemble_inst_same_operand = '{0} %%vs4, %%vs4, %%vs4 \\n""{0} %%vs36, %%vs36, %%vs36 \\n'.format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 3) | 0x7
            inst_type = "scalar"
        if (format == "FA Form"):
            disassemble_inst = '{0} %%f3, %%f0, %%f1, %%f2'.format(Mnemonics)
            disassemble_inst_same_operand = '{0} %%f4, %%f4, %%f4, %%f4'.format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 3) | 0x7
            inst_type = "fp"
        if (format == "FX Form"):
            disassemble_inst = '{0} %%f3, %%f0, %%f1'.format(Mnemonics)
            disassemble_inst_same_operand = '{0} %%f4, %%f4, %%f4'.format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 3) | 0x7
            inst_type = "fp"
        if (format == "FC Form"):
            disassemble_inst = '{0} %%f3, %%f0, %%f1'.format(Mnemonics)
            disassemble_inst_same_operand = '{0} %%f4, %%f4, %%f4'.format(Mnemonics)
            inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code << 3) | 0x7
            inst_type = "fp"
        if (machineCode) :
            disassemble_inst = "/*{}*/.long 0x{:08X}".format(disassemble_inst, inst_word)

        test = testBenchGen()
        if (inst_type == "vector"):
            test_str = test.openReplaceFile("vector.c", Mnemonics, disassemble_inst, disassemble_inst_same_operand)

        if (inst_type == "scalar"):
            test_str = test.openReplaceFile("scalar.c", Mnemonics, disassemble_inst, disassemble_inst_same_operand)
            
        if (inst_type == "fp"):
            test_str = test.openReplaceFile("fp.c", Mnemonics, disassemble_inst, disassemble_inst_same_operand)
        #win.textEdit_generatedCode.setText(test_str)
        print(test_str)

    def insertCode(win):
        code = win.textEdit_generatedCode.toPlainText()
        insert = gem5CodeInsert()
        insert.openInsertFile(code, "decoder.isa")

    def run(win):
        mnemonics = win.lineEdit_mnemonics.text()
        compile = win.Checkbox_Compile.isChecked()
        
        
        test_only = win.Checkbox_TestOnly.isChecked()
        if (win.run == None):
            win.run = gem5_debug_run()

        win.run.gem5_debug_run(mnemonics, compile, test_only)

    def commit_file(win):
        if (win.run == None):
            print("insert code and click 'run' first!")
        else:
            win.run.gem5_commit_file()

    def sync_operands(win):
        if (win.Checkbox_OperandSync.isChecked()):
            index = win.comboBox_DstElemType.currentIndex()
            win.comboBox_Src1ElemType.setCurrentIndex(index)
            win.comboBox_Src2ElemType.setCurrentIndex(index)
            win.comboBox_Src3ElemType.setCurrentIndex(index)

    def sync_idx(win):
        format = win.comboBox_Format.currentText()
        if (format == "VA Form" or format == "VX Form" or format == "VC Form"):
            win.comboBox_Src1RegType.setCurrentIndex(0)
            win.comboBox_Src2RegType.setCurrentIndex(0)
            win.comboBox_Src3RegType.setCurrentIndex(0)
            win.comboBox_DstRegType.setCurrentIndex(0)

        if (format == "XX1 Form" or format == "XX2 Form" or format == "XX3 Form"):
            win.comboBox_Src1RegType.setCurrentIndex(1)
            win.comboBox_Src2RegType.setCurrentIndex(1)
            win.comboBox_Src3RegType.setCurrentIndex(1)
            win.comboBox_DstRegType.setCurrentIndex(1)

        if (format == "FX Form" or format == "FA Form" or format == "FC Form"):
            win.comboBox_Src1RegType.setCurrentIndex(3)
            win.comboBox_Src2RegType.setCurrentIndex(3)
            win.comboBox_Src3RegType.setCurrentIndex(3)
            win.comboBox_DstRegType.setCurrentIndex(3)
            win.comboBox_Format_2.setCurrentIndex(1)
    
    myWin.pushButton_GenCode.clicked.connect(lambda :generate_code(myWin))
    myWin.pushButton_GenTest.clicked.connect(lambda :generate_test(myWin))
    myWin.pushButton_InsertCode.clicked.connect(lambda :insertCode(myWin))
    myWin.pushButton_Debug.clicked.connect(lambda :run(myWin))
    myWin.pushButton_Commit.clicked.connect(lambda :commit_file(myWin))
    myWin.comboBox_DstElemType.currentIndexChanged.connect(lambda :sync_operands(myWin))
    myWin.comboBox_Format.currentIndexChanged.connect(lambda :sync_idx(myWin))
    myWin.show()

    sys.exit(app.exec_())
