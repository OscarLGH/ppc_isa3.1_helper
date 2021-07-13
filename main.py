
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow
from mainForm import Ui_Form

from testBenchGen import testBenchGen
from gem5CodeGen import gem5_code_gen
from gem5CodeInsert import gem5CodeInsert
from gem5DebugRun import gem5DebugRun
 
class MyMainForm(QMainWindow, Ui_Form):
    def __init__(self, parent=None):
        super(MyMainForm, self).__init__(parent)
        self.setupUi(self)
        self.code = None


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

        if (win.code == None) :
            win.code = gem5_code_gen()
        else:
            win.code.save_code(win.textEdit_generatedCode.toPlainText())

        code_str = win.code.open_replace_template("gem5_code_template.isa", PO, isa_format, format2, XO, Mnemonics,pseudocode,
                                        comboBox_Src1ElemType, comboBox_Src2ElemType, comboBox_Src3ElemType, comboBox_DstElemType,
                                        "{:.0f}".format(128 / op_bits), "", "", "")
        win.textEdit_generatedCode.setText(code_str)

    def generate_test(win):
        format = win.comboBox_Format.currentText()
        Mnemonics = win.lineEdit_mnemonics.text()
        machineCode = win.MachineCode.isChecked()
        PO = win.lineEdit_PO.text()
        XO = win.lineEdit_XO.text()

        disassemble_inst = ""
        if (format == "VA Form"):
            disassemble_inst = "{} %%v3, %%v0, %%v1, %%v2".format(Mnemonics)
        if (format == "VX Form"):
            disassemble_inst = "{} %%v3, %%v0, %%v1".format(Mnemonics)
        if (machineCode) :
            po_code = int(PO, 10)
            xo_code = int(XO, 10)
            vrt = 3
            vra = 0
            vrb = 1
            vrc = 2
            if (format == "VA Form"):
                inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (vrc << 6) | (xo_code)
            if (format == "VX Form"):
                inst_word = (po_code << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (xo_code)
            disassemble_inst = "/*{}*/.long 0x{:08X}".format(disassemble_inst, inst_word)

        test = testBenchGen()
        test_str = test.openReplaceFile("vector.c", Mnemonics, disassemble_inst)
        #win.textEdit_generatedCode.setText(test_str)
        print(test_str)

    def insertCode(win):
        code = win.textEdit_generatedCode.toPlainText()
        insert = gem5CodeInsert()
        insert.openInsertFile(code, "decoder.isa")

    def run(win):
        mnemonics = win.lineEdit_mnemonics.text()
        compile = win.Checkbox_Compile.isChecked()
        debug = win.Checkbox_Debug.isChecked()
        run = gem5DebugRun()
        run.gem5DebugRun(mnemonics, compile, debug)

    def sync_operands(win):
        if (win.Checkbox_OperandSync.isChecked()):
            index = win.comboBox_DstElemType.currentIndex()
            win.comboBox_Src1ElemType.setCurrentIndex(index)
            win.comboBox_Src2ElemType.setCurrentIndex(index)
            win.comboBox_Src3ElemType.setCurrentIndex(index)


    myWin.pushButton_GenCode.clicked.connect(lambda :generate_code(myWin))
    myWin.pushButton_GenTest.clicked.connect(lambda :generate_test(myWin))
    myWin.pushButton_InsertCode.clicked.connect(lambda :insertCode(myWin))
    myWin.pushButton_Debug.clicked.connect(lambda :run(myWin))
    myWin.comboBox_DstElemType.currentIndexChanged.connect(lambda :sync_operands(myWin))
    myWin.show()

    sys.exit(app.exec_())
