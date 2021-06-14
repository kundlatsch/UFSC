import sys
from PyQt5 import QtGui
from PyQt5 import QtCore, QtGui, QtWidgets
from app.wireframe import Wireframe, BezierCurve, BSplineCurve, BicubicSurface
from app.utils import Shape


class NewWireframeWindow(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super().__init__()
        self.resize(560, 360)
        self.partnerDialog = parent
        self.display_file = self.partnerDialog.display_file
        self.points = []
        self.color = QtCore.Qt.black
        self.setup()

    def setup(self):
        self.newWireframeLabel = QtWidgets.QLabel(self)
        self.newWireframeLabel.setGeometry(QtCore.QRect(10, 10, 151, 17))
        self.newWireframeLabel.setObjectName("newWireframeLabel")

        self.tabWidget = QtWidgets.QTabWidget(self)
        self.tabWidget.setGeometry(QtCore.QRect(200, 30, 351, 261))
        self.tabWidget.setObjectName("tabWidget")

        self.newPointsListWidget = QtWidgets.QListWidget(self)
        self.newPointsListWidget.setGeometry(QtCore.QRect(10, 30, 171, 261))
        self.newPointsListWidget.setObjectName("newPointsListWidget")
        self.deletePointPushButton = QtWidgets.QPushButton(self)
        self.deletePointPushButton.setGeometry(QtCore.QRect(10, 300, 171, 25))
        self.deletePointPushButton.setObjectName("deletePointPushButton")
        self.colorPickerPushButton = QtWidgets.QPushButton(self)
        self.colorPickerPushButton.setGeometry(QtCore.QRect(200, 300, 171, 25))
        self.colorPickerPushButton.setObjectName("pickColorPushButton")
        self.addNewPointPushButton = QtWidgets.QPushButton(self)
        self.addNewPointPushButton.setGeometry(QtCore.QRect(380, 300, 171, 25))
        self.addNewPointPushButton.setObjectName("addNewPointPushButton")
        self.drawPolygonPushButton = QtWidgets.QPushButton(self)
        self.drawPolygonPushButton.setEnabled(True)
        self.drawPolygonPushButton.setGeometry(QtCore.QRect(380, 330, 171, 25))
        self.drawPolygonPushButton.setObjectName("drawPolygonPushButton")

        self.wireframeTab = QtWidgets.QWidget()
        self.setPointsWireframeLabel = QtWidgets.QLabel(self.wireframeTab)
        self.setPointsWireframeLabel.setGeometry(QtCore.QRect(10, 10, 81, 18))
        self.setPointsWireframeLabel.setObjectName("setPointsWireframeLabel")
        self.newXWireframeLabel = QtWidgets.QLabel(self.wireframeTab)
        self.newXWireframeLabel.setGeometry(QtCore.QRect(30, 45, 21, 18))
        self.newXWireframeLabel.setObjectName("newXWireframeLabel")
        self.newYWireframeLabel = QtWidgets.QLabel(self.wireframeTab)
        self.newYWireframeLabel.setGeometry(QtCore.QRect(30, 85, 16, 16))
        self.newYWireframeLabel.setObjectName("newYWireframeLabel")
        self.newXWireframeTextEdit = QtWidgets.QTextEdit(self.wireframeTab)
        self.newXWireframeTextEdit.setGeometry(QtCore.QRect(50, 35, 51, 31))
        self.newXWireframeTextEdit.setObjectName("newXWireframeTextEdit")
        self.newYWireframeTextEdit = QtWidgets.QTextEdit(self.wireframeTab)
        self.newYWireframeTextEdit.setGeometry(QtCore.QRect(50, 75, 51, 31))
        self.newYWireframeTextEdit.setObjectName("newYWireframeTextEdit")
        self.newZWireframeLabel = QtWidgets.QLabel(self.wireframeTab)
        self.newZWireframeLabel.setGeometry(QtCore.QRect(30, 125, 16, 16))
        self.newZWireframeLabel.setObjectName("newZWireframeLabel")
        self.newZWireframeTextEdit = QtWidgets.QTextEdit(self.wireframeTab)
        self.newZWireframeTextEdit.setGeometry(QtCore.QRect(50, 115, 51, 31))
        self.newZWireframeTextEdit.setObjectName("newZWireframeTextEdit")
        self.newZWireframeTextEdit.setText("0")
        self.fillCheckBox = QtWidgets.QCheckBox(self.wireframeTab)
        self.fillCheckBox.setGeometry(QtCore.QRect(30, 165, 120, 18))
        self.fillCheckBox.setObjectName("fillCheckBox")
        self.fillCheckBox.setEnabled(False)
        self.tabWidget.addTab(self.wireframeTab, "")

        self.bezierTab = QtWidgets.QWidget()
        self.setPointsBezierLabel = QtWidgets.QLabel(self.bezierTab)
        self.setPointsBezierLabel.setGeometry(QtCore.QRect(10, 10, 81, 18))
        self.setPointsBezierLabel.setObjectName("setPointsBezierLabel")
        self.newXCurveLabel = QtWidgets.QLabel(self.bezierTab)
        self.newXCurveLabel.setGeometry(QtCore.QRect(30, 45, 21, 18))
        self.newXCurveLabel.setObjectName("newXCurveLabel")
        self.newYCurveLabel = QtWidgets.QLabel(self.bezierTab)
        self.newYCurveLabel.setGeometry(QtCore.QRect(30, 85, 16, 16))
        self.newYCurveLabel.setObjectName("newYCurveLabel")
        self.newXCurveTextEdit = QtWidgets.QTextEdit(self.bezierTab)
        self.newXCurveTextEdit.setGeometry(QtCore.QRect(50, 35, 51, 31))
        self.newXCurveTextEdit.setObjectName("newXCurveTextEdit")
        self.newYCurveTextEdit = QtWidgets.QTextEdit(self.bezierTab)
        self.newYCurveTextEdit.setGeometry(QtCore.QRect(50, 75, 51, 31))
        self.newYCurveTextEdit.setObjectName("newYCurveTextEdit")
        self.newZCurveLabel = QtWidgets.QLabel(self.bezierTab)
        self.newZCurveLabel.setGeometry(QtCore.QRect(30, 125, 16, 16))
        self.newZCurveLabel.setObjectName("newZCurveLabel")
        self.newZCurveTextEdit = QtWidgets.QTextEdit(self.bezierTab)
        self.newZCurveTextEdit.setGeometry(QtCore.QRect(50, 115, 51, 31))
        self.newZCurveTextEdit.setObjectName("newZCurveTextEdit")
        self.newZCurveTextEdit.setText("0")
        self.curveComboBox = QtWidgets.QComboBox(self.bezierTab)
        self.curveComboBox.setGeometry(QtCore.QRect(30, 195, 142, 25))
        self.curveComboBox.setObjectName("curveComboBox")
        self.curveComboBox.addItem("Bezier")
        self.curveComboBox.addItem("B-Spline")
        self.accuracyCurveLabel = QtWidgets.QLabel(self.bezierTab)
        self.accuracyCurveLabel.setGeometry(QtCore.QRect(30, 160, 81, 16))
        self.accuracyCurveLabel.setObjectName("accuracyLabel")
        self.accuracyCurveTextEdit = QtWidgets.QTextEdit(self.bezierTab)
        self.accuracyCurveTextEdit.setGeometry(QtCore.QRect(100, 155, 71, 31))
        self.accuracyCurveTextEdit.setObjectName("accuracyTextEdit")
        self.accuracyCurveTextEdit.setText("20.0")
        self.tabWidget.addTab(self.bezierTab, "")

        self.bicubicSurfaceTab = QtWidgets.QWidget()
        self.setPointsBicubicSurfaceLabel = QtWidgets.QLabel(self.bicubicSurfaceTab)
        self.setPointsBicubicSurfaceLabel.setGeometry(QtCore.QRect(10, 10, 81, 18))
        self.setPointsBicubicSurfaceLabel.setObjectName("setPointsBicubicSurfaceLabel")
        self.newXBicubicSurfaceLabel = QtWidgets.QLabel(self.bicubicSurfaceTab)
        self.newXBicubicSurfaceLabel.setGeometry(QtCore.QRect(30, 45, 21, 18))
        self.newXBicubicSurfaceLabel.setObjectName("newXBicubicSurfaceLabel")
        self.newYBicubicSurfaceLabel = QtWidgets.QLabel(self.bicubicSurfaceTab)
        self.newYBicubicSurfaceLabel.setGeometry(QtCore.QRect(30, 85, 16, 16))
        self.newYBicubicSurfaceLabel.setObjectName("newYBicubicSurfaceLabel")
        self.newXBicubicSurfaceTextEdit = QtWidgets.QTextEdit(self.bicubicSurfaceTab)
        self.newXBicubicSurfaceTextEdit.setGeometry(QtCore.QRect(50, 35, 51, 31))
        self.newXBicubicSurfaceTextEdit.setObjectName("newXBicubicSurfaceTextEdit")
        self.newYBicubicSurfaceTextEdit = QtWidgets.QTextEdit(self.bicubicSurfaceTab)
        self.newYBicubicSurfaceTextEdit.setGeometry(QtCore.QRect(50, 75, 51, 31))
        self.newYBicubicSurfaceTextEdit.setObjectName("newYBicubicSurfaceTextEdit")
        self.newZBicubicSurfaceLabel = QtWidgets.QLabel(self.bicubicSurfaceTab)
        self.newZBicubicSurfaceLabel.setGeometry(QtCore.QRect(30, 125, 16, 16))
        self.newZBicubicSurfaceLabel.setObjectName("newZBicubicSurfaceLabel")
        self.newZBicubicSurfaceTextEdit = QtWidgets.QTextEdit(self.bicubicSurfaceTab)
        self.newZBicubicSurfaceTextEdit.setGeometry(QtCore.QRect(50, 115, 51, 31))
        self.newZBicubicSurfaceTextEdit.setObjectName("newZBicubicSurfaceTextEdit")
        self.newZBicubicSurfaceTextEdit.setText("0")
        self.bicubicSurfaceComboBox = QtWidgets.QComboBox(self.bicubicSurfaceTab)
        self.bicubicSurfaceComboBox.setGeometry(QtCore.QRect(30, 195, 142, 25))
        self.bicubicSurfaceComboBox.setObjectName("bicubicSurfaceComboBox")
        self.bicubicSurfaceComboBox.addItem("Bezier")
        self.bicubicSurfaceComboBox.addItem("B-Spline")
        self.accuracyBicubicSurfaceLabel = QtWidgets.QLabel(self.bicubicSurfaceTab)
        self.accuracyBicubicSurfaceLabel.setGeometry(QtCore.QRect(30, 160, 81, 16))
        self.accuracyBicubicSurfaceLabel.setObjectName("bicubicSurfaceLabel")
        self.accuracyBicubicSurfaceTextEdit = QtWidgets.QTextEdit(
            self.bicubicSurfaceTab
        )
        self.accuracyBicubicSurfaceTextEdit.setGeometry(QtCore.QRect(100, 155, 71, 31))
        self.accuracyBicubicSurfaceTextEdit.setObjectName("bicubicSurfaceTextEdit")
        self.accuracyBicubicSurfaceTextEdit.setText("20.0")
        self.tabWidget.addTab(self.bicubicSurfaceTab, "")

        self.retranslateUi()
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(self)
        self.set_buttons_actions()

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.newWireframeLabel.setText(_translate("Form", "List of Points"))
        self.setWindowTitle(_translate("Form", "New Wireframe"))
        self.drawPolygonPushButton.setText(_translate("Form", "Draw"))
        self.addNewPointPushButton.setText(_translate("Form", "Add Point"))
        self.deletePointPushButton.setText(_translate("Form", "Delete Point"))
        self.colorPickerPushButton.setText(_translate("Form", "Pick Color"))
        self.setPointsWireframeLabel.setText(_translate("Form", "Set points:"))
        self.setPointsBezierLabel.setText(_translate("Form", "Set points:"))
        self.setPointsBicubicSurfaceLabel.setText(_translate("Form", "Set points:"))
        self.newXWireframeLabel.setText(_translate("Form", "X:"))
        self.newYWireframeLabel.setText(_translate("Form", "Y:"))
        self.newZWireframeLabel.setText(_translate("Form", "Z:"))
        self.newXCurveLabel.setText(_translate("Form", "X:"))
        self.newYCurveLabel.setText(_translate("Form", "Y:"))
        self.newZCurveLabel.setText(_translate("Form", "Z:"))
        self.newXBicubicSurfaceLabel.setText(_translate("Form", "X:"))
        self.newYBicubicSurfaceLabel.setText(_translate("Form", "Y:"))
        self.newZBicubicSurfaceLabel.setText(_translate("Form", "Z:"))
        self.fillCheckBox.setText(_translate("Form", "Fill Polygon"))
        self.accuracyCurveLabel.setText(_translate("Form", "Accuracy:"))
        self.accuracyBicubicSurfaceLabel.setText(_translate("Form", "Accuracy:"))
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.wireframeTab),
            _translate("Form", "Wireframe"),
        )
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.bezierTab),
            _translate("Form", "Curve"),
        )
        self.tabWidget.setTabText(
            self.tabWidget.indexOf(self.bicubicSurfaceTab),
            _translate("Form", "Bicubic Surface"),
        )

    def new_window(self):
        self.points = []
        self.set_text_draw_button()
        self.show()

    def add_new_point(self):
        active_tab = self.tabWidget.currentIndex()

        if active_tab == 0:
            xTextEdit = self.newXWireframeTextEdit
            yTextEdit = self.newYWireframeTextEdit
            zTextEdit = self.newZWireframeTextEdit
        if active_tab == 1:
            xTextEdit = self.newXCurveTextEdit
            yTextEdit = self.newYCurveTextEdit
            zTextEdit = self.newZCurveTextEdit
        if active_tab == 2:
            xTextEdit = self.newXBicubicSurfaceTextEdit
            yTextEdit = self.newYBicubicSurfaceTextEdit
            zTextEdit = self.newZBicubicSurfaceTextEdit
        try:
            x = float(xTextEdit.toPlainText())
            y = float(yTextEdit.toPlainText())
            z = float(zTextEdit.toPlainText())
        except ValueError:
            self.partnerDialog.console_print("Invalid or empty value on X, Y or Z")
            return
        # self.partnerDialog.console_print(f"Points={(x, y)}")
        self.points.append((x, y, z))
        self.partnerDialog.console_print(f"Points after append={self.points}")
        xTextEdit.clear()
        yTextEdit.clear()
        zTextEdit.clear()
        point_id = len(self.points) - 1
        point_str = f"Point {point_id}: {x}, {y}, {z}"
        self.newPointsListWidget.insertItem(point_id, point_str)
        self.set_text_draw_button()
        if len(self.points) > 2:
            self.fillCheckBox.setEnabled(True)

    def add_new_wireframe(self):
        active_tab = self.tabWidget.currentIndex()
        if len(self.points) > 0:
            wireframe_id = self.partnerDialog.wireframe_count

            if active_tab == 0:
                # Wireframe tab
                wireframe = Wireframe(
                    self.points,
                    wireframe_id,
                    self.color,
                    self.partnerDialog.window_coordinates,
                    self.partnerDialog.window_transformations_matrix,
                )
                if self.fillCheckBox.isChecked():
                    wireframe.filled = True

            if active_tab == 1:
                # Curve tab
                accuracy = None
                try:
                    accuracy = float(self.accuracyCurveTextEdit.toPlainText())
                except ValueError:
                    self.partnerDialog.console_print(
                        "Invalid or empty value on accuracy, using default value"
                    )

                args = [
                    self.points,
                    wireframe_id,
                    self.color,
                    self.partnerDialog.window_coordinates,
                    self.partnerDialog.window_transformations_matrix,
                ]

                if self.curveComboBox.currentIndex() == 0:
                    # Bezier

                    if len(self.points) % 3 != 1 or len(self.points) < 3:
                        self.partnerDialog.console_print(
                            "Invalid number of points to draw bezier curve"
                        )
                        return

                    accuracy = accuracy if accuracy else 20.0
                    args.append(accuracy)
                    wireframe = BezierCurve(*args)

                if self.curveComboBox.currentIndex() == 1:
                    # B-Spline

                    if len(self.points) < 4:
                        self.partnerDialog.console_print(
                            "Invalid number of points to draw B-Spline curve"
                        )
                        return

                    accuracy = accuracy if accuracy else 0.1
                    args.append(accuracy)
                    wireframe = BSplineCurve(*args)

            if active_tab == 2:

                if len(self.points) != 16:
                    self.partnerDialog.console_print("Invalid number of points")
                    return

                accuracy = None
                try:
                    accuracy = float(self.accuracyCurveTextEdit.toPlainText())
                except ValueError:
                    self.partnerDialog.console_print(
                        "Invalid or empty value on accuracy, using default value"
                    )
                    accuracy = 20.0

                curve_type = ""
                if self.bicubicSurfaceComboBox.currentIndex() == 0:
                    curve_type = "Bezier"
                elif self.bicubicSurfaceComboBox.currentIndex() == 1:
                    curve_type = "B-Spline"

                args = [
                    self.points,
                    wireframe_id,
                    self.color,
                    self.partnerDialog.window_coordinates,
                    self.partnerDialog.window_transformations_matrix,
                    curve_type,
                    accuracy,
                ]
                wireframe = BicubicSurface(*args)

            self.display_file.append(wireframe)
            self.partnerDialog.draw_wireframe(wireframe)
            self.partnerDialog.draw_native_objects()
            # self.partnerDialog.redraw_wireframes()
            self.partnerDialog.listWidget.insertItem(wireframe_id, wireframe.name)
            self.partnerDialog.wireframe_count += 1
            self.newPointsListWidget.clear()
            self.partnerDialog.console_print(f"New wireframe added={wireframe.name}")

        self.fillCheckBox.setChecked(False)
        self.fillCheckBox.setEnabled(False)
        self.newZWireframeTextEdit.setText("0")
        self.newZCurveTextEdit.setText("0")
        self.close()

    def delete_active_point(self):
        item = self.newPointsListWidget.currentRow()
        self.newPointsListWidget.takeItem(item)
        try:
            self.points.pop(item)
        except IndexError:
            self.partnerDialog.console_print("There is no point to remove")
            return
        self.set_text_draw_button()
        self.partnerDialog.console_print(f"Point {item} deleted!")

    def pick_color(self):
        color = QtWidgets.QColorDialog.getColor()
        if color.isValid():
            self.color = color
            self.partnerDialog.console_print("Color updated")

    def set_buttons_actions(self):
        self.addNewPointPushButton.clicked.connect(self.add_new_point)
        self.drawPolygonPushButton.clicked.connect(self.add_new_wireframe)
        self.deletePointPushButton.clicked.connect(self.delete_active_point)
        self.colorPickerPushButton.clicked.connect(self.pick_color)
        self.tabWidget.currentChanged.connect(self.set_text_draw_button)
        self.curveComboBox.currentIndexChanged.connect(self.set_default_curve_accuracy)

    def set_text_draw_button(self):
        active_tab = self.tabWidget.currentIndex()
        if active_tab == 0:
            self.drawPolygonPushButton.setText(f"Draw {Shape(len(self.points)).name}")
        if active_tab == 1:
            if len(self.points) == 0:
                self.drawPolygonPushButton.setText(f"Draw Nothing")
            else:
                self.drawPolygonPushButton.setText(f"Draw Curve")
        if active_tab == 2:
            if len(self.points) == 0:
                self.drawPolygonPushButton.setText(f"Draw Nothing")
            else:
                self.drawPolygonPushButton.setText(f"Draw Surface")

    def set_default_curve_accuracy(self):
        if self.curveComboBox.currentIndex() == 0:
            self.accuracyCurveTextEdit.setText("20.0")
        if self.curveComboBox.currentIndex() == 1:
            self.accuracyCurveTextEdit.setText("0.1")
        self.accuracyBicubicSurfaceTextEdit.setText("20.0")
