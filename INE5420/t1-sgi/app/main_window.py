from PyQt5 import QtCore, QtGui, QtWidgets

from app.new_wireframe_window import NewWireframeWindow
from app.transform_window import TransformWindow
from app.utils import (
    CoordinatesRepresentation,
)
from app.math_functions import transform_coordinates, build_window_normalizations
from app.clipping import clip
from app.config import (
    DEFAULT_X_MAX,
    DEFAULT_X_MIN,
    DEFAULT_Y_MAX,
    DEFAULT_Y_MIN,
    SHIFT_FACTOR,
)

from app.obj_handler import ObjLoader, ObjWriter
from app.wireframe import Wireframe, Curve, WireframeGroup


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.resize(860, 640)
        self.display_file = []
        self.newWireframeWindowDialog = NewWireframeWindow(self)
        self.transformWindowDialog = TransformWindow(self)
        self.default_x_max = DEFAULT_X_MAX
        self.default_x_min = DEFAULT_X_MIN
        self.default_y_max = DEFAULT_Y_MAX
        self.default_y_min = DEFAULT_Y_MIN
        self.scale_acumulator = 0
        self.window_coordinates = CoordinatesRepresentation(
            -self.default_x_max / 2,
            -self.default_y_max / 2,
            self.default_x_max / 2,
            self.default_y_max / 2,
            factor=SHIFT_FACTOR,
        )
        self.viewport_coordinates = CoordinatesRepresentation(
            self.default_x_min + 20,
            self.default_y_min + 20,
            self.default_x_max - 20,
            self.default_y_max - 20,
        )
        self.wireframe_count = 0
        self.acc_rotation_degrees_X = 0
        self.acc_rotation_degrees_Y = 0
        self.acc_rotation_degrees_Z = 0
        self.acc_x_shift = 0
        self.acc_y_shift = 0
        self.window_transformations_matrix = None
        self.desnormalization_matrix = None
        self.line_clipping_method = "no-clipping"
        self.prepare_normalization_matrix()
        self.prepare_desnormalization_matrix()
        self.setup()

    def setup(self):
        self.debuggerTextBrowser = QtWidgets.QTextBrowser(self)
        self.debuggerTextBrowser.setGeometry(QtCore.QRect(220, 430, 610, 195))
        self.debuggerTextBrowser.setObjectName("debuggerTextBrowser")
        self.displayFileLabel = QtWidgets.QLabel(self)
        self.displayFileLabel.setGeometry(QtCore.QRect(10, 20, 81, 31))
        self.displayFileLabel.setObjectName("displayFileLabel")
        self.listWidget = QtWidgets.QListWidget(self)
        self.listWidget.setGeometry(QtCore.QRect(10, 50, 111, 153))
        self.listWidget.setObjectName("listWidget")
        self.newPushButton = QtWidgets.QPushButton(self)
        self.newPushButton.setGeometry(QtCore.QRect(130, 50, 75, 34))
        self.newPushButton.setObjectName("newPushButton")
        self.deletePushButton = QtWidgets.QPushButton(self)
        self.deletePushButton.setGeometry(QtCore.QRect(130, 80, 75, 34))
        self.deletePushButton.setObjectName("deletePushButton")
        self.clearPushButton = QtWidgets.QPushButton(self)
        self.clearPushButton.setGeometry(QtCore.QRect(130, 110, 75, 34))
        self.clearPushButton.setObjectName("clearPushButton")
        self.refreshPushButton = QtWidgets.QPushButton(self)
        self.refreshPushButton.setGeometry(QtCore.QRect(130, 140, 75, 34))
        self.refreshPushButton.setObjectName("refreshPushButton")
        self.transformPushButton = QtWidgets.QPushButton(self)
        self.transformPushButton.setGeometry(QtCore.QRect(130, 170, 75, 34))
        self.transformPushButton.setObjectName("transformPushButton")

        self.savePushButton = QtWidgets.QPushButton(self)
        self.savePushButton.setGeometry(QtCore.QRect(10, 210, 95, 24))
        self.savePushButton.setObjectName("savePushButton")
        self.loadPushButton = QtWidgets.QPushButton(self)
        self.loadPushButton.setGeometry(QtCore.QRect(110, 210, 95, 24))
        self.loadPushButton.setObjectName("loadPushButton")

        self.navigationLabel = QtWidgets.QLabel(self)
        self.navigationLabel.setGeometry(QtCore.QRect(10, 250, 111, 18))
        self.navigationLabel.setObjectName("navigationLabel")
        self.rotationLabel = QtWidgets.QLabel(self)
        self.rotationLabel.setGeometry(QtCore.QRect(10, 370, 58, 18))
        self.rotationLabel.setObjectName("rotationLabel")
        self.zoomLabel = QtWidgets.QLabel(self)
        self.zoomLabel.setGeometry(QtCore.QRect(10, 490, 58, 18))
        self.zoomLabel.setObjectName("zoomLabel")

        self.inPushButton = QtWidgets.QPushButton(self)
        self.inPushButton.setGeometry(QtCore.QRect(115, 260, 35, 20))
        self.inPushButton.setObjectName("inPushButton")
        self.outPushButton = QtWidgets.QPushButton(self)
        self.outPushButton.setGeometry(QtCore.QRect(150, 260, 35, 20))
        self.outPushButton.setObjectName("outPushButton")

        self.upPushButton = QtWidgets.QPushButton(self)
        self.upPushButton.setGeometry(QtCore.QRect(60, 280, 51, 34))
        self.upPushButton.setObjectName("upPushButton")
        self.downPushButton = QtWidgets.QPushButton(self)
        self.downPushButton.setGeometry(QtCore.QRect(60, 310, 51, 34))
        self.downPushButton.setObjectName("downPushButton")
        self.rightPushButton = QtWidgets.QPushButton(self)
        self.rightPushButton.setGeometry(QtCore.QRect(110, 300, 51, 34))
        self.rightPushButton.setObjectName("rightPushButton")
        self.leftPushButton = QtWidgets.QPushButton(self)
        self.leftPushButton.setGeometry(QtCore.QRect(10, 300, 51, 34))
        self.leftPushButton.setObjectName("leftPushButton")
        self.zoomInPushButton = QtWidgets.QPushButton(self)
        self.zoomInPushButton.setGeometry(QtCore.QRect(10, 510, 51, 34))
        self.zoomInPushButton.setObjectName("zoomInPushButton")
        self.zoomInPushButton.setEnabled(False)
        self.zoomOutPushButton = QtWidgets.QPushButton(self)
        self.zoomOutPushButton.setGeometry(QtCore.QRect(70, 510, 51, 34))
        self.zoomOutPushButton.setObjectName("zoomOutPushButton")
        self.zoomOutPushButton.setEnabled(False)
        self.rotateXPushButton = QtWidgets.QPushButton(self)
        self.rotateXPushButton.setGeometry(QtCore.QRect(10, 430, 51, 34))
        self.rotateXPushButton.setObjectName("rotateXPushButton")
        self.rotateYPushButton = QtWidgets.QPushButton(self)
        self.rotateYPushButton.setGeometry(QtCore.QRect(70, 430, 51, 34))
        self.rotateYPushButton.setObjectName("rotateYPushButton")
        self.rotateZPushButton = QtWidgets.QPushButton(self)
        self.rotateZPushButton.setGeometry(QtCore.QRect(130, 430, 51, 34))
        self.rotateZPushButton.setObjectName("rotateZPushButton")
        self.degreesLabel = QtWidgets.QLabel(self)
        self.degreesLabel.setGeometry(QtCore.QRect(20, 400, 58, 18))
        self.degreesLabel.setObjectName("degreesLabel")
        self.degreesEdit = QtWidgets.QTextEdit(self)
        self.degreesEdit.setGeometry(QtCore.QRect(80, 390, 91, 31))
        self.degreesEdit.setObjectName("degreesEdit")

        self.clippingLabel = QtWidgets.QLabel(self)
        self.clippingLabel.setGeometry(QtCore.QRect(10, 570, 180, 18))
        self.clippingLabel.setObjectName("clippingLabel")
        self.clippingComboBox = QtWidgets.QComboBox(self)
        self.clippingComboBox.setGeometry(QtCore.QRect(10, 600, 180, 25))
        self.clippingComboBox.setObjectName("clippingComboBox")
        self.clippingComboBox.addItem("")
        self.clippingComboBox.addItem("")
        self.clippingComboBox.addItem("")

        # self.textEdit.setEnabled(False)
        self.line = QtWidgets.QFrame(self)
        self.line.setGeometry(QtCore.QRect(10, 10, 196, 20))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.line_2 = QtWidgets.QFrame(self)
        self.line_2.setGeometry(QtCore.QRect(10, 230, 196, 20))
        self.line_2.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_2.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.line_3 = QtWidgets.QFrame(self)
        self.line_3.setGeometry(QtCore.QRect(10, 350, 196, 20))
        self.line_3.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.line_4 = QtWidgets.QFrame(self)
        self.line_4.setGeometry(QtCore.QRect(10, 470, 196, 20))
        self.line_4.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.line_5 = QtWidgets.QFrame(self)
        self.line_5.setGeometry(QtCore.QRect(10, 550, 196, 20))
        self.line_5.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_5.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_5.setObjectName("line_5")
        self.displayFrame = QtWidgets.QLabel(self)
        self.displayFrame.setGeometry(QtCore.QRect(222, 20, 630, 380))
        self.displayFrame.setAutoFillBackground(False)
        self.displayFrame.setStyleSheet("background-color: rgb(255, 255, 255);")
        canvas = QtGui.QPixmap(630, 380)
        canvas.fill(QtGui.QColor("white"))
        self.displayFrame.setPixmap(canvas)
        self.painter = QtGui.QPainter(self.displayFrame.pixmap())
        self.displayFrame.setObjectName("displayFrame")

        self.retranslateUi()
        self.dialogs = list()
        QtCore.QMetaObject.connectSlotsByName(self)
        self.set_buttons_actions()
        self.draw_native_objects()
        self.show()

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("MainWindow", "Computer Graphics"))
        self.displayFileLabel.setText(_translate("MainWindow", "Display File"))
        self.newPushButton.setText(_translate("MainWindow", "New"))
        self.deletePushButton.setText(_translate("MainWindow", "Delete"))
        self.clearPushButton.setText(_translate("MainWindow", "Clear"))
        self.refreshPushButton.setText(_translate("MainWindow", "Refresh"))
        self.transformPushButton.setText(_translate("MainWindow", "Transform"))
        self.navigationLabel.setText(_translate("MainWindow", "Navigation"))
        self.rotationLabel.setText(_translate("MainWindow", "Rotation"))
        self.zoomLabel.setText(_translate("MainWindow", "Zoom"))
        self.upPushButton.setText(_translate("MainWindow", "Up"))
        self.downPushButton.setText(_translate("MainWindow", "Down"))
        self.rightPushButton.setText(_translate("MainWindow", "Right"))
        self.leftPushButton.setText(_translate("MainWindow", "Left"))
        self.inPushButton.setText(_translate("MainWindow", "In"))
        self.outPushButton.setText(_translate("MainWindow", "Out"))
        self.zoomInPushButton.setText(_translate("MainWindow", "+"))
        self.zoomOutPushButton.setText(_translate("MainWindow", "-"))
        self.rotateXPushButton.setText(_translate("MainWindow", "X"))
        self.rotateYPushButton.setText(_translate("MainWindow", "Y"))
        self.rotateZPushButton.setText(_translate("MainWindow", "Z"))
        self.degreesLabel.setText(_translate("MainWindow", "Degrees:"))
        self.savePushButton.setText(_translate("MainWindow", "Save"))
        self.loadPushButton.setText(_translate("MainWindow", "Load"))
        self.clippingComboBox.setItemText(0, _translate("MainWindow", "No Clipping"))
        self.clippingComboBox.setItemText(
            1, _translate("MainWindow", "Cohen-Sutherland")
        )
        self.clippingComboBox.setItemText(2, _translate("MainWindow", "Liang-Barsky"))
        self.clippingLabel.setText(_translate("MainWindow", "Line clipping method"))

    def console_print(self, string):
        self.debuggerTextBrowser.append(string)
        print(string)

    def console_clear(self):
        self.debuggerTextBrowser.clear()

    def set_buttons_actions(self):
        self.newPushButton.clicked.connect(self.new_wireframe_window)
        self.deletePushButton.clicked.connect(self.delete_wireframe)
        self.clearPushButton.clicked.connect(self.clear_display_file)
        self.clearPushButton.clicked.connect(self.clear_canvas)
        self.refreshPushButton.clicked.connect(self.refresh_canvas)
        self.leftPushButton.clicked.connect(self.shift_window_left)
        self.rightPushButton.clicked.connect(self.shift_window_right)
        self.upPushButton.clicked.connect(self.shift_window_up)
        self.downPushButton.clicked.connect(self.shift_window_down)
        self.inPushButton.clicked.connect(self.scale_window_in)
        self.outPushButton.clicked.connect(self.scale_window_out)
        self.transformPushButton.clicked.connect(self.transform_window)
        self.loadPushButton.clicked.connect(self.load_obj_file)
        self.savePushButton.clicked.connect(self.save_obj_file)
        self.rotateXPushButton.clicked.connect(self.rotate_window_x)
        self.rotateYPushButton.clicked.connect(self.rotate_window_y)
        self.rotateZPushButton.clicked.connect(self.rotate_window_z)
        self.clippingComboBox.currentIndexChanged.connect(
            self.select_line_clipping_method
        )
        self.console_print("WELCOME!")

    def new_wireframe_window(self):
        self.newWireframeWindowDialog.new_window()

    def transform_window(self):
        if len(self.display_file) > 0:
            current_row = self.listWidget.currentRow()
            active_wireframe = self.display_file[current_row]
            self.transformWindowDialog.new_window(active_wireframe)

    def delete_wireframe(self):
        if len(self.display_file) > 0:
            item = self.listWidget.currentRow()
            self.listWidget.takeItem(item)
            wireframe = self.display_file.pop(item)
            self.console_print(f"{wireframe.name} deleted!")
            self.redraw_wireframes()

    def clear_display_file(self):
        self.listWidget.setCurrentRow(0)
        wireframes = len(self.display_file)
        for i in range(wireframes):
            item = self.listWidget.currentRow()
            self.listWidget.takeItem(item)
            self.display_file.pop(item)
        self.scale_window_by_step(0)
        self.refresh_canvas()
        self.console_print("Canvas cleared")

    def clear_canvas(self):
        self.displayFrame.pixmap().fill(QtGui.QColor("white"))
        self.displayFrame.update()
        self.draw_native_objects()

    def load_obj_file(self):
        file_name = QtWidgets.QFileDialog.getOpenFileName(
            self, "Open obj file", "./", "Obj files (*.obj)"
        )[0]

        if not file_name:
            return

        loader = ObjLoader(
            file_name,
            self.wireframe_count,
            self.window_coordinates,
            self.window_transformations_matrix,
        )
        new_wireframes = loader.wireframes
        for wireframe in new_wireframes:
            self.display_file.append(wireframe)
            self.draw_wireframe(wireframe)
            self.listWidget.insertItem(self.wireframe_count, wireframe.name)
            self.wireframe_count += 1
        self.draw_native_objects()
        self.console_print("Obj loaded")

    def save_obj_file(self):
        file_name, ok = QtWidgets.QInputDialog.getText(
            self, "Text Input Dialog", "Enter scene name:"
        )
        if ok:
            writer = ObjWriter(
                self.display_file, file_name, self.desnormalization_matrix
            )
            writer.create_obj()
            self.console_print("Scene saved")
        else:
            self.console_print("Invalid scene name")

    line_clipping_methods = {0: "no-clipping", 1: "cohen-sutherland", 2: "liang-barsky"}

    def select_line_clipping_method(self):
        index = self.clippingComboBox.currentIndex()
        method = self.line_clipping_methods[index]
        self.line_clipping_method = method
        self.redraw_wireframes()
        self.console_print(f"Clipping method changed to {method}")

    def draw_line(self, x1, y1, x2, y2, color):
        # self.console_print(f"Drawning new line! Points={(x1, y1)}, {(x2, y2)}")
        self.displayFrame.update()
        self.painter.setPen(QtGui.QPen(color, 5))
        self.painter.drawLine(x1, y1, x2, y2)

    def prepare_coordinates_to_draw(self, wireframe):
        should_clip = True

        if self.line_clipping_method == "no-clipping":
            should_clip = False

        wireframe.transform_coordinates()

        coordinates = [[(x, y) for (x, y, z) in wireframe.transformed_coordinates]]
        if should_clip:
            is_visible, coordinates = clip(
                wireframe, coordinates[0], method=self.line_clipping_method
            )
        else:
            is_visible = True

        return is_visible, coordinates

    def _draw_wireframe(self, wireframe):
        fill_points = []

        is_visible, coordinates = self.prepare_coordinates_to_draw(wireframe)

        if is_visible:
            # When clipping, it might return multiple objects
            for inside_coordinates in coordinates:
                range_to_draw = len(inside_coordinates)
                if isinstance(wireframe, Curve):
                    range_to_draw -= 1
                for index in range(range_to_draw):
                    x1, y1 = inside_coordinates[index]
                    xvp1, yvp1 = transform_coordinates(
                        x1,
                        y1,
                        self.window_coordinates,
                        self.viewport_coordinates,
                    )
                    x2, y2 = inside_coordinates[(index + 1) % len(inside_coordinates)]
                    xvp2, yvp2 = transform_coordinates(
                        x2,
                        y2,
                        self.window_coordinates,
                        self.viewport_coordinates,
                    )
                    self.draw_line(xvp1, yvp1, xvp2, yvp2, wireframe.color)
                    if wireframe.filled:
                        fill_points.append((xvp1, yvp1))

                if fill_points:
                    # Create QPoints just to use filling primitive from pyqt
                    polygon = QtGui.QPolygonF()
                    for point in fill_points:
                        new_point = QtCore.QPointF(point[0], point[1])
                        polygon.append(new_point)
                    path = QtGui.QPainterPath()
                    path.addPolygon(polygon)
                    self.painter.setBrush(wireframe.color)
                    self.painter.drawPath(path)

    def draw_wireframe(self, wireframe):
        if isinstance(wireframe, WireframeGroup):
            for w in wireframe.wireframes:
                self._draw_wireframe(w)
        else:
            self._draw_wireframe(wireframe)

    def draw_native_objects(self):
        offset = 20
        x1 = offset
        y1 = offset
        x2 = self.viewport_coordinates.x_max
        y2 = self.viewport_coordinates.y_max
        window_rectangle = [(x1, y1), (x2, y1), (x2, y2), (x1, y2)]
        for index in range(len(window_rectangle)):
            x1, y1 = window_rectangle[index]
            x2, y2 = window_rectangle[(index + 1) % len(window_rectangle)]
            self.draw_line(x1, y1, x2, y2, QtCore.Qt.red)

    def set_navigation_default_paramaters(self):
        self.console_print("Navigation parameters reseted!")
        self.window_coordinates.x_max = self.default_x_max / 2
        self.window_coordinates.x_min = -self.default_x_max / 2
        self.window_coordinates.y_max = self.default_y_max / 2
        self.window_coordinates.y_min = -self.default_y_max / 2
        self.scale_acumulator = 0
        self.acc_rotation_degrees_X = 0
        self.acc_rotation_degrees_Y = 0
        self.acc_rotation_degrees_Z = 0
        self.acc_x_shift = 0
        self.acc_y_shift = 0

    def shift_window_left(self):
        self.console_print("Window left shift!")
        self.acc_x_shift += 5
        self.redraw_wireframes()

    def shift_window_right(self):
        self.console_print("Window right shift!")
        self.acc_x_shift -= 5
        self.redraw_wireframes()

    def shift_window_up(self):
        self.console_print("Window up shift!")
        self.acc_y_shift -= 5
        self.redraw_wireframes()

    def shift_window_down(self):
        self.console_print("Window down shift!")
        self.acc_y_shift += 5
        self.redraw_wireframes()

    def scale_window_by_step(self, step):
        self.scale_acumulator += step
        scale_factor = 1 + self.scale_acumulator
        self.console_print(f"Scale factor updated to {scale_factor}")
        self.window_coordinates.x_max = self.default_x_max * scale_factor
        self.window_coordinates.y_max = self.default_y_max * scale_factor

    def scale_window_in(self):
        self.console_print("Zoom in!")
        self.scale_window_by_step(-0.05)
        self.redraw_wireframes()

    def scale_window_out(self):
        self.console_print("Zoom out!")
        self.scale_window_by_step(0.05)
        self.redraw_wireframes()

    def rotate_window_x(self):
        degrees = self.degreesEdit.toPlainText()
        try:
            self.acc_rotation_degrees_X -= float(degrees)
            self.console_print(f"X rotation degrees {self.acc_rotation_degrees_X}")
            self.redraw_wireframes()
        except ValueError:
            self.console_print("Invalid or missing degrees value")

    def rotate_window_y(self):
        degrees = self.degreesEdit.toPlainText()
        try:
            self.acc_rotation_degrees_Y -= float(degrees)
            self.console_print(f"Y rotation degrees {self.acc_rotation_degrees_Y}")
            self.redraw_wireframes()
        except ValueError:
            self.console_print("Invalid or missing degrees value")

    def rotate_window_z(self):
        degrees = self.degreesEdit.toPlainText()
        try:
            self.acc_rotation_degrees_Z -= float(degrees)
            self.console_print(f"Z rotation degrees {self.acc_rotation_degrees_Z}")
            self.redraw_wireframes()
        except ValueError:
            self.console_print("Invalid or missing degrees value")

    def refresh_canvas(self):
        self.console_print("Refresh canvas")
        self.prepare_desnormalization_matrix()
        self.set_navigation_default_paramaters()
        self.redraw_wireframes()

    def prepare_normalization_matrix(self):
        height = self.window_coordinates.y_max - self.window_coordinates.y_min
        width = self.window_coordinates.x_max - self.window_coordinates.x_min
        self.window_transformations_matrix = build_window_normalizations(
            self.acc_x_shift,
            self.acc_y_shift,
            width,
            height,
            self.acc_rotation_degrees_X,
            self.acc_rotation_degrees_Y,
            self.acc_rotation_degrees_Z,
        )

    def prepare_desnormalization_matrix(self):
        height = self.window_coordinates.y_max - self.window_coordinates.y_min
        width = self.window_coordinates.x_max - self.window_coordinates.x_min
        matrix = build_window_normalizations(
            -self.acc_x_shift,
            -self.acc_y_shift,
            4 / width,
            4 / height,
            -self.acc_rotation_degrees_X,
            -self.acc_rotation_degrees_Y,
            -self.acc_rotation_degrees_Z,
        )
        self.desnormalization_matrix = matrix

    def redraw_wireframes(self):
        self.console_print(f"Drawning {len(self.display_file)} wireframes")
        self.clear_canvas()
        self.prepare_normalization_matrix()
        for wireframe in self.display_file:
            wireframe.update_aux_values(
                self.window_coordinates, self.window_transformations_matrix
            )
            self.draw_wireframe(wireframe)
        self.draw_native_objects()
