from PyQt5 import QtCore, QtWidgets
from app.utils import transformations_codes, build_transformation_string


class TransformWindow(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super().__init__()
        self.resize(560, 360)
        self.setObjectName("TransformWindow")
        self.partnerDialog = parent
        self.next_id = 0
        self.setup()

    def setup(self):
        self.transformLabel = QtWidgets.QLabel(self)
        self.transformLabel.setGeometry(QtCore.QRect(10, 10, 151, 17))
        self.transformLabel.setObjectName("transformLabel")
        self.listWidget = QtWidgets.QListWidget(self)
        self.listWidget.setGeometry(QtCore.QRect(10, 30, 171, 261))
        self.listWidget.setObjectName("listWidget")

        self.deleteTransformationPushButton = QtWidgets.QPushButton(self)
        self.deleteTransformationPushButton.setGeometry(QtCore.QRect(10, 300, 171, 25))
        self.deleteTransformationPushButton.setObjectName(
            "deleteTransformationPushButton"
        )

        self.tabWidget = QtWidgets.QTabWidget(self)
        self.tabWidget.setGeometry(QtCore.QRect(200, 30, 351, 261))
        self.tabWidget.setObjectName("tabWidget")

        self.rotationTab = QtWidgets.QWidget()
        self.rotationTab.setObjectName("rotationTab")
        self.rotationComboBox = QtWidgets.QComboBox(self.rotationTab)
        self.rotationComboBox.setGeometry(QtCore.QRect(20, 20, 281, 25))
        self.rotationComboBox.setObjectName("rotationComboBox")
        self.rotationComboBox.addItem("")
        self.rotationComboBox.addItem("")
        self.rotationComboBox.addItem("")
        self.rotationText = QtWidgets.QTextEdit(self.rotationTab)
        self.rotationText.setGeometry(QtCore.QRect(100, 60, 80, 21))
        self.rotationText.setObjectName("rotationText")
        self.rotationLabel = QtWidgets.QLabel(self.rotationTab)
        self.rotationLabel.setGeometry(QtCore.QRect(30, 60, 71, 17))
        self.rotationLabel.setObjectName("rotationLabel")
        self.rotationXTextEdit = QtWidgets.QTextEdit(self.rotationTab)
        self.rotationXTextEdit.setGeometry(QtCore.QRect(100, 90, 80, 21))
        self.rotationXTextEdit.setObjectName("rotationXTextEdit")
        self.rotationYLabel = QtWidgets.QLabel(self.rotationTab)
        self.rotationYLabel.setGeometry(QtCore.QRect(78, 112, 16, 17))
        self.rotationYLabel.setObjectName("rotationYLabel")
        self.rotationXLabel = QtWidgets.QLabel(self.rotationTab)
        self.rotationXLabel.setGeometry(QtCore.QRect(78, 92, 16, 17))
        self.rotationXLabel.setObjectName("rotationXLabel")
        self.rotationYTextEdit = QtWidgets.QTextEdit(self.rotationTab)
        self.rotationYTextEdit.setGeometry(QtCore.QRect(100, 110, 80, 21))
        self.rotationYTextEdit.setObjectName("rotationYTextEdit")
        self.rotationZLabel = QtWidgets.QLabel(self.rotationTab)
        self.rotationZLabel.setGeometry(QtCore.QRect(78, 132, 16, 17))
        self.rotationZLabel.setObjectName("rotationZLabel")
        self.rotationZTextEdit = QtWidgets.QTextEdit(self.rotationTab)
        self.rotationZTextEdit.setGeometry(QtCore.QRect(100, 130, 80, 21))
        self.rotationZTextEdit.setObjectName("rotationZTextEdit")
        self.disable_edit_rotation_point(True)
        self.tabWidget.addTab(self.rotationTab, "")

        self.translationTab = QtWidgets.QWidget()
        self.translationTab.setObjectName("translationTab")
        self.translationXTextEdit = QtWidgets.QTextEdit(self.translationTab)
        self.translationXTextEdit.setGeometry(QtCore.QRect(40, 20, 80, 21))
        self.translationXTextEdit.setObjectName("translationXTextEdit")
        self.translationXLabel = QtWidgets.QLabel(self.translationTab)
        self.translationXLabel.setGeometry(QtCore.QRect(20, 20, 16, 17))
        self.translationXLabel.setObjectName("translationXLabel")
        self.translationYTextEdit = QtWidgets.QTextEdit(self.translationTab)
        self.translationYTextEdit.setGeometry(QtCore.QRect(40, 50, 80, 21))
        self.translationYTextEdit.setObjectName("translationYTextEdit")
        self.translationYLabel = QtWidgets.QLabel(self.translationTab)
        self.translationYLabel.setGeometry(QtCore.QRect(20, 50, 16, 17))
        self.translationYLabel.setObjectName("translationYLabel")
        self.translationZTextEdit = QtWidgets.QTextEdit(self.translationTab)
        self.translationZTextEdit.setGeometry(QtCore.QRect(40, 80, 80, 21))
        self.translationZTextEdit.setObjectName("translationZTextEdit")
        self.translationZLabel = QtWidgets.QLabel(self.translationTab)
        self.translationZLabel.setGeometry(QtCore.QRect(20, 80, 16, 17))
        self.translationZLabel.setObjectName("translationZLabel")
        self.tabWidget.addTab(self.translationTab, "")

        self.scaleTab = QtWidgets.QWidget()
        self.scaleTab.setObjectName("scaleTab")
        self.scalingXTextEdit = QtWidgets.QTextEdit(self.scaleTab)
        self.scalingXTextEdit.setGeometry(QtCore.QRect(40, 20, 80, 21))
        self.scalingXTextEdit.setObjectName("scalingXTextEdit")
        self.scalingYLabel = QtWidgets.QLabel(self.scaleTab)
        self.scalingYLabel.setGeometry(QtCore.QRect(20, 50, 16, 17))
        self.scalingYLabel.setObjectName("scalingYLabel")
        self.scalingXLabel = QtWidgets.QLabel(self.scaleTab)
        self.scalingXLabel.setGeometry(QtCore.QRect(20, 20, 16, 17))
        self.scalingXLabel.setObjectName("scalingXLabel")
        self.scalingYTextEdit = QtWidgets.QTextEdit(self.scaleTab)
        self.scalingYTextEdit.setGeometry(QtCore.QRect(40, 50, 80, 21))
        self.scalingYTextEdit.setObjectName("scalingYTextEdit")
        self.scalingZTextEdit = QtWidgets.QTextEdit(self.scaleTab)
        self.scalingZTextEdit.setGeometry(QtCore.QRect(40, 80, 80, 21))
        self.scalingZTextEdit.setObjectName("scalingZTextEdit")
        self.scalingZLabel = QtWidgets.QLabel(self.scaleTab)
        self.scalingZLabel.setGeometry(QtCore.QRect(20, 80, 16, 17))
        self.scalingZLabel.setObjectName("scalingZLabel")
        self.tabWidget.addTab(self.scaleTab, "")

        self.reflectionTab = QtWidgets.QWidget()
        self.reflectionTab.setObjectName("reflectionTab")
        self.reflectionXCheckBox = QtWidgets.QCheckBox(self.reflectionTab)
        self.reflectionXCheckBox.setGeometry(QtCore.QRect(20, 20, 92, 23))
        self.reflectionXCheckBox.setObjectName("reflectionXCheckBox")
        self.reflectionYCheckBox = QtWidgets.QCheckBox(self.reflectionTab)
        self.reflectionYCheckBox.setGeometry(QtCore.QRect(20, 50, 92, 23))
        self.reflectionYCheckBox.setObjectName("reflectionYCheckBox")
        self.reflectionZCheckBox = QtWidgets.QCheckBox(self.reflectionTab)
        self.reflectionZCheckBox.setGeometry(QtCore.QRect(20, 80, 92, 23))
        self.reflectionZCheckBox.setObjectName("reflectionYCheckBox")
        self.reflectionOriginCheckBox = QtWidgets.QCheckBox(self.reflectionTab)
        self.reflectionOriginCheckBox.setGeometry(QtCore.QRect(20, 110, 92, 23))
        self.reflectionOriginCheckBox.setObjectName("reflectionOriginCheckBox")
        self.tabWidget.addTab(self.reflectionTab, "")

        self.addTransformationPushButton = QtWidgets.QPushButton(self)
        self.addTransformationPushButton.setGeometry(QtCore.QRect(380, 300, 171, 25))
        self.addTransformationPushButton.setObjectName("addTransformationPushButton")

        self.transformPushButton = QtWidgets.QPushButton(self)
        self.transformPushButton.setGeometry(QtCore.QRect(380, 330, 171, 25))
        self.transformPushButton.setObjectName("transformPushButton")

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)
        self.tabWidget.setCurrentIndex(0)
        self.connect_actions()

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.transformLabel.setText(
            _translate("TransformWindow", "Transform Wireframe")
        )
        self.deleteTransformationPushButton.setText(
            _translate("TransformWindow", "Delete Transformation")
        )
        self.translationXLabel.setText(_translate("TransformWindow", "X:"))
        self.translationYLabel.setText(_translate("TransformWindow", "Y:"))
        self.translationZLabel.setText(_translate("TransformWindow", "Z:"))
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.translationTab),
            _translate("TransformWindow", "Translation"),
        )
        self.reflectionXCheckBox.setText(_translate("TransformWindow", "X"))
        self.reflectionYCheckBox.setText(_translate("TransformWindow", "Y"))
        self.reflectionZCheckBox.setText(_translate("TransformWindow", "Z"))
        self.reflectionOriginCheckBox.setText(_translate("TransformWindow", "Origin"))
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.reflectionTab),
            _translate("TransformWindow", "Reflection"),
        )
        self.rotationComboBox.setItemText(
            0, _translate("TransformWindow", "Rotate about the center of the object")
        )
        self.rotationComboBox.setItemText(
            1, _translate("TransformWindow", "Rotate about the origin")
        )
        self.rotationComboBox.setItemText(
            2, _translate("TransformWindow", "Rotate about a point")
        )
        self.rotationLabel.setText(_translate("TransformWindow", "Degrees:"))
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.rotationTab),
            _translate("TransformWindow", "Rotation"),
        )
        self.scalingYLabel.setText(_translate("TransformWindow", "Y:"))
        self.scalingXLabel.setText(_translate("TransformWindow", "X:"))
        self.scalingZLabel.setText(_translate("TransformWindow", "Z:"))
        self.rotationYLabel.setText(_translate("TransformWindow", "Y:"))
        self.rotationXLabel.setText(_translate("TransformWindow", "X:"))
        self.rotationZLabel.setText(_translate("TransformWindow", "Z:"))
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.scaleTab),
            _translate("TransformWindow", "Scaling"),
        )
        self.addTransformationPushButton.setText(
            _translate("TransformWindow", "Add Transformation")
        )
        self.transformPushButton.setText(_translate("TransformWindow", "Transform"))

    def new_window(self, active_wireframe):
        self.wireframe = active_wireframe
        self.update_transformations_list()
        self.show()

    def update_transformations_list(self):
        self.listWidget.clear()
        self.next_id = 0
        for code in self.wireframe.transformations_codes:
            if code[0] != "r_rt":
                item = build_transformation_string(code)
                self.listWidget.insertItem(self.next_id, item)
                self.next_id += 1

    def add_last_n_transformations_to_list(self, n):
        codes = self.wireframe.transformations_codes[-n:]
        for code in codes:
            transformation = transformations_codes[code[0]]
            item = build_transformation_string(code)
            self.listWidget.insertItem(self.next_id, item)
            self.next_id += 1

    def connect_actions(self):
        self.addTransformationPushButton.clicked.connect(self.add_transformation)
        self.deleteTransformationPushButton.clicked.connect(self.delete_transformation)
        self.transformPushButton.clicked.connect(self.transform)
        self.listWidget.currentItemChanged.connect(self.show_transformation)
        self.rotationComboBox.currentIndexChanged.connect(self.select_rotation)

    def transform(self):
        self.partnerDialog.redraw_wireframes()
        self.partnerDialog.console_print(f"Transformations applied")

    def delete_transformation(self):
        transformations = len(self.wireframe.transformations_codes)
        if transformations > 0:
            item = self.listWidget.currentRow()
            self.listWidget.takeItem(item)
            transformation = self.wireframe.transformations_codes.pop(item)
            name = f"{transformations_codes[transformation[0]]}"
            self.partnerDialog.console_print(f"{name} removed!")

    def add_transformation(self):
        active_tab = self.tabWidget.currentIndex()
        self.tab_index_to_function[active_tab][0](self)

    def add_rotation(self):
        degrees = self.rotationText.toPlainText()
        if degrees == "":
            return
        if self.rotationComboBox.currentIndex() == 0:
            # Center of object
            self.wireframe.transformations_codes.append(("rt", [float(degrees), ()]))
        elif self.rotationComboBox.currentIndex() == 1:
            # Origin
            self.wireframe.transformations_codes.append(
                ("rt", [float(degrees), (0, 0, 0)])
            )
        elif self.rotationComboBox.currentIndex() == 2:
            # Point
            try:
                x = float(self.rotationXTextEdit.toPlainText())
                y = float(self.rotationYTextEdit.toPlainText())
                z = float(self.rotationYTextEdit.toPlainText())
                self.wireframe.transformations_codes.append(
                    ("rt", [float(degrees), (x, y, z)])
                )
            except ValueError:
                self.partnerDialog.console_print(
                    "Please inform valid X, Y and Z values to rotation about a point"
                )
                return
        self.add_last_n_transformations_to_list(1)

    def show_rotation(self, degrees, xyz):
        self.rotationText.setText(str(degrees))
        if xyz == ():
            self.rotationComboBox.setCurrentIndex(0)
            self.rotationXTextEdit.setText("")
            self.rotationYTextEdit.setText("")
            self.disable_edit_rotation_point(True)
        elif xyz == (0, 0, 0):
            self.rotationComboBox.setCurrentIndex(1)
            self.rotationXTextEdit.setText("0")
            self.rotationYTextEdit.setText("0")
            self.disable_edit_rotation_point(True)
        else:
            self.rotationComboBox.setCurrentIndex(2)
            self.rotationXTextEdit.setText(str(xyz[0]))
            self.rotationYTextEdit.setText(str(xyz[1]))
            self.rotationZTextEdit.setText(str(xyz[2]))
            self.disable_edit_rotation_point(False)

    def select_rotation(self):
        if self.rotationComboBox.currentIndex() == 2:
            self.disable_edit_rotation_point(False)
        else:
            self.disable_edit_rotation_point(True)

    def disable_edit_rotation_point(self, boolean):
        self.rotationXTextEdit.setDisabled(boolean)
        self.rotationYTextEdit.setDisabled(boolean)
        self.rotationZTextEdit.setDisabled(boolean)

    def add_translation(self):
        x_text = self.translationXTextEdit.toPlainText()
        y_text = self.translationYTextEdit.toPlainText()
        z_text = self.translationZTextEdit.toPlainText()
        x = float(x_text) if x_text != "" else 0
        y = float(y_text) if y_text != "" else 0
        z = float(z_text) if z_text != "" else 0
        self.wireframe.transformations_codes.append(("tr", [x, y, z]))
        self.add_last_n_transformations_to_list(1)

    def show_translation(self, x, y, z):
        self.translationXTextEdit.setText(str(x))
        self.translationYTextEdit.setText(str(y))
        self.translationZTextEdit.setText(str(z))

    def add_scaling(self):
        x_text = self.scalingXTextEdit.toPlainText()
        y_text = self.scalingYTextEdit.toPlainText()
        z_text = self.scalingZTextEdit.toPlainText()
        x = float(x_text) if x_text != "" else 1
        y = float(y_text) if y_text != "" else 1
        z = float(z_text) if z_text != "" else 1
        self.wireframe.transformations_codes.append(("sc", [x, y, z]))
        self.add_last_n_transformations_to_list(1)

    def show_scaling(self, x, y, z):
        self.scalingXTextEdit.setText(str(x))
        self.scalingYTextEdit.setText(str(y))
        self.scalingZTextEdit.setText(str(z))

    def add_reflection(self):
        count = 0
        if self.reflectionXCheckBox.isChecked():
            self.wireframe.transformations_codes.append(("rf", ["x"]))
            count += 1
        if self.reflectionYCheckBox.isChecked():
            self.wireframe.transformations_codes.append(("rf", ["y"]))
            count += 1
        if self.reflectionZCheckBox.isChecked():
            self.wireframe.transformations_codes.append(("rf", ["z"]))
            count += 1
        if self.reflectionOriginCheckBox.isChecked():
            self.wireframe.transformations_codes.append(("rf", ["origin"]))
            count += 1
        if count > 0:
            self.add_last_n_transformations_to_list(count)

    def show_reflection(self, rtype):
        self.reflectionXCheckBox.setChecked(False)
        self.reflectionYCheckBox.setChecked(False)
        self.reflectionZCheckBox.setChecked(False)
        self.reflectionOriginCheckBox.setChecked(False)
        if rtype == "x":
            self.reflectionXCheckBox.setChecked(True)
        elif rtype == "y":
            self.reflectionYCheckBox.setChecked(True)
        elif rtype == "z":
            self.reflectionZCheckBox.setChecked(True)
        elif rtype == "origin":
            self.reflectionOriginCheckBox.setChecked(True)

    tab_index_to_function = {
        0: (add_rotation, show_rotation),
        1: (add_translation, show_translation),
        2: (add_scaling, show_scaling),
        3: (add_reflection, show_reflection),
    }

    transformation_name_to_index = {
        "Rotation": 0,
        "Translation": 1,
        "Scaling": 2,
        "Reflection": 3,
    }

    def show_transformation(self):
        item = self.listWidget.currentItem()
        try:
            item_name = item.text().split()[0]
            new_index = self.transformation_name_to_index[item_name]
            self.tabWidget.setCurrentIndex(new_index)

            row = self.listWidget.currentRow()
            transformation_code = self.wireframe.transformations_codes[row]
            if transformation_code != "r_rt":
                params = transformation_code[1]
                print(params)
                self.tab_index_to_function[new_index][1](self, *params)
        except AttributeError:
            self.partnerDialog.console_print("Error parsing current transformation")
