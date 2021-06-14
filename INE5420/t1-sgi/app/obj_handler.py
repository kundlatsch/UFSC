import pathlib
from PyQt5 import QtGui, QtCore

from app.wireframe import Wireframe, WireframeGroup


class ObjLoader:
    def __init__(
        self, file_path, wireframe_index, normalization_values, window_transformations
    ):
        self.vertices = []
        self.mtl = None
        self.mtl_dict = {}
        self.mtl_parsing = ""
        self.obj_parsing = ""
        self.wireframes = []
        self.wireframes_dict = {}
        self.wireframe_index = wireframe_index
        self.normalization_values = normalization_values
        self.window_transformations = window_transformations
        self.file_path = pathlib.Path(file_path)
        self.window = None
        self.load(file_path)

    def vertice_handler(self, args):
        x = float(args[0])
        y = float(args[1])
        z = float(args[2])
        vertex = (x, y, z)
        self.vertices.append(vertex)

    def mtllib_handler(self, args):
        file = args[0]
        file_path = self.file_path.parent / file
        self.parse_file(file_path, self.mtl_parser)

    def usemtl_handler(self, args):
        mtl = args[0]
        if mtl in self.mtl_dict:
            self.mtl = self.mtl_dict[mtl]

    def object_name_handler(self, args):
        name = args[0]
        if name == "":
            name = "generic_object"
        self.obj_parsing = name
        self.wireframes_dict[name] = []
        self.mtl = None

    def object_build_handler(self, args):
        points = []
        for vertex in args:
            if vertex[0] == "-":
                index = int(vertex)
                points.append(self.vertices[index])
            else:
                index = int(vertex) - 1
                points.append(self.vertices[index])
        self.build_wireframe(points)

    def face_handler(self, args):
        points = []
        for arg in args:
            raw_index = arg.split("/")[0]
            if raw_index[0] == "-":
                index = int(raw_index)
                points.append(self.vertices[index])
            else:
                index = int(raw_index) - 1
                points.append(self.vertices[index])
        self.build_wireframe(points)

    def build_wireframe(self, vertices):
        if not self.mtl:
            self.mtl = QtCore.Qt.black
        wireframe = Wireframe(
            vertices,
            self.wireframe_index,
            self.mtl,
            self.normalization_values,
            self.window_transformations,
        )

        wireframe.name = self.obj_parsing + str(self.wireframe_index)
        self.wireframes_dict[self.obj_parsing].append(wireframe)
        self.wireframe_index += 1

    def window_handler(self, args):
        position_vector = (args[0], args[1])
        size_vector = (args[2], args[3])
        self.window = (position_vector, size_vector)

    def ignore(self, args):
        pass

    obj_parser = {
        "v": vertice_handler,
        "mtllib": mtllib_handler,
        "usemtl": usemtl_handler,
        "o": object_name_handler,
        "p": object_build_handler,
        "l": object_build_handler,
        "w": window_handler,
        "f": face_handler,
        "vt": ignore,
        "vn": ignore,
        "g": object_name_handler,
        "s": ignore,
        "#": ignore,
    }

    def newmtl_handler(self, args):
        self.mtl_parsing = args[0]

    def Kd_handler(self, args):
        r = int(float(args[0]) * 255)
        g = int(float(args[1]) * 255)
        b = int(float(args[2]) * 255)
        color = QtGui.QColor(r, g, b)
        self.mtl_dict[self.mtl_parsing] = color
        self.mtl_parsing = ""

    mtl_parser = {
        "newmtl": newmtl_handler,
        "Kd": Kd_handler,
        "map_Kd": ignore,
        "Ka": ignore,
        "Ks": ignore,
        "Tf": ignore,
        "Ni": ignore,
        "Ns": ignore,
        "illum": ignore,
        "d": ignore,
        "#": ignore,
    }

    def load(self, file_path):
        self.parse_file(file_path, self.obj_parser)
        for group in self.wireframes_dict:
            if len(self.wireframes_dict[group]) == 1:
                wireframe = self.wireframes_dict[group].pop()
                self.wireframes.append(wireframe)
            else:
                wireframes_group = self.wireframes_dict[group]
                wireframe = WireframeGroup(
                    wireframes_group,
                    group,
                    self.normalization_values,
                    self.window_transformations,
                )
                self.wireframes.append(wireframe)

    def parse_file(self, file_path, parse_dict):
        with open(file_path) as file:
            for line in file.readlines():
                split = line.split()
                try:
                    statement = split[0]
                    args = split[1:]
                    if statement in parse_dict:
                        parse_dict[statement](self, args)
                except IndexError:
                    # blank line
                    pass


class ObjWriter:
    def __init__(self, wireframes_list, scene_name, desnormalization_matrix):
        self.wirefames_list = wireframes_list
        self.scene_name = scene_name
        self.desnormalization_matrix = desnormalization_matrix
        print(self.desnormalization_matrix)

    def create_obj(self):
        obj_lines = []
        mtl_lines = []
        obj_lines.append(f"mtllib {self.scene_name}.mtl\n")

        for wireframe in self.wirefames_list:
            obj_list, mtl_list = wireframe.to_obj(self.desnormalization_matrix)
            obj_lines += obj_list
            mtl_lines += mtl_list

        file_path = str(pathlib.Path().absolute() / "obj" / self.scene_name)
        obj_name = file_path + ".obj"
        mtl_name = file_path + ".mtl"
        self.write_file(obj_name, obj_lines)
        self.write_file(mtl_name, mtl_lines)

    def write_file(self, file_name, lines):
        with open(file_name, "w") as writer:
            for line in lines:
                writer.write(line)
