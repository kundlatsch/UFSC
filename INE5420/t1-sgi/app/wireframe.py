import numpy as np
from functools import reduce
from PyQt5 import QtGui

from app.utils import Shape
from app.math_functions import (
    transformations_functions_dict,
    build_homogeneous_coordinates,
    calculate_object_center,
    multiply_coordinates_by_transformations,
    normalize_point,
    calculate_bezier_points,
    calculate_bspline_parameters,
    calculate_curve_points_to_surface,
)


class WireframeGroup:
    def __init__(self, wireframes, name, normalization_values, window_transformations):
        self.wireframes = wireframes
        self.name = name
        self.normalization_values = normalization_values
        self.window_transformations = window_transformations
        self.transformations_codes = []
        self.center = None

    def update_aux_values(self, normalization_values, window_transformations):
        self.normalization_values = normalization_values
        self.window_transformations = window_transformations

        for wireframe in self.wireframes:
            wireframe.update_aux_values(
                self.normalization_values, self.window_transformations
            )
            wireframe.transformations_codes = self.transformations_codes

        # self.calculate_group_center()

        # for wireframe in self.wireframes:
        #     wireframe.group_center = self.center

    def calculate_group_center(self):
        concatenated_matrix = self.wireframes[0].transformed_coordinates
        for wirefame in self.wireframes[1:]:
            concatenated_matrix = np.concatenate(
                (concatenated_matrix, wirefame.transformed_coordinates), axis=0
            )
        c_x, c_y, c_z = calculate_object_center(concatenated_matrix)
        self.center = (c_x, c_y, c_z)


class Wireframe:
    def __init__(
        self, coordinates, index, color, normalization_values, window_transformations
    ):
        self.coordinates = coordinates
        self.number_points = len(self.coordinates)
        self.polygon_type = Shape(self.number_points).name
        self.name = f"{self.polygon_type}_{index}"
        self.color = color
        self.transformations_codes = []
        self.transformed_coordinates = []
        self.normalization_values = normalization_values
        self.window_angle = 0
        self.window_x_shift_acc = 0
        self.window_y_shift_acc = 0
        self.window_transformations = window_transformations
        self.window_width = (
            self.normalization_values.x_max - self.normalization_values.x_min
        )
        self.window_height = (
            self.normalization_values.y_max - self.normalization_values.y_min
        )
        self.center = None
        self.group_center = None
        self.filled = False
        self.visible = True
        self.transform_coordinates()

    # def calculate_object_center(self):
    #     self.center = tuple(np.array(self.transformed_coordinates).mean(axis=0))

    def update_aux_values(self, normalization_values, window_transformations):
        self.normalization_values = normalization_values
        self.window_transformations = window_transformations
        self.window_width = (
            self.normalization_values.x_max - self.normalization_values.x_min
        )
        self.window_height = (
            self.normalization_values.y_max - self.normalization_values.y_min
        )

    def needs_translation(self, code):
        return code in ["rt"]

    def transform_coordinates(self):
        coord_aux = build_homogeneous_coordinates(self.coordinates)
        coord_aux = multiply_coordinates_by_transformations(
            coord_aux, self.window_transformations
        )
        for (code, params) in self.transformations_codes:
            t_aux = []
            if self.needs_translation(code):
                if code in ["rt"]:
                    if len(params[1]) > 0:
                        translate_x, translate_y, translate_z = normalize_point(
                            params[1], self.window_width / 2, self.window_height / 2
                        )
                    else:

                        (
                            translate_x,
                            translate_y,
                            translate_z,
                            _,
                        ) = calculate_object_center(coord_aux)
                    params = [params[0]] * 3
                else:
                    (
                        translate_x,
                        translate_y,
                        translate_z,
                        _,
                    ) = calculate_object_center(coord_aux)
                t_aux.append(
                    transformations_functions_dict["tr"](
                        -translate_x, -translate_y, -translate_z
                    )
                )
                t_aux.append(transformations_functions_dict[code](*params))
                t_aux.append(
                    transformations_functions_dict["tr"](
                        translate_x, translate_y, translate_z
                    )
                )
            else:
                if code == "tr":
                    x_normalized, y_normalized, z_normalized = normalize_point(
                        params, self.window_width / 2, self.window_height / 2
                    )
                    params = [x_normalized, y_normalized, z_normalized]
                t_aux.append(transformations_functions_dict[code](*params))

            composition_matrix = reduce(np.dot, t_aux)
            coord_aux = multiply_coordinates_by_transformations(
                coord_aux, composition_matrix
            )

        # self.center = calculate_object_center(coord_aux)
        # Remove last column and map the points to tuple
        self.transformed_coordinates = list(map(tuple, coord_aux[:, :-1]))
        # print("normalized coordinates=", self.transformed_coordinates)

    def to_obj(self, desnormalization_matrix):
        coord_aux = build_homogeneous_coordinates(self.transformed_coordinates)
        desnormalized_coordinates = multiply_coordinates_by_transformations(
            coord_aux, desnormalization_matrix
        )
        print(coord_aux, "->>>", desnormalization_matrix)
        obj_list = []
        mtl_list = []

        obj_list.append(f"o {self.name}")
        obj_list.append(f"usemtl {self.name}_mtl")

        for vertex in desnormalized_coordinates:
            obj_list.append(f"v {vertex[0]} {vertex[1]} 0.0")

        f = ["f"]
        points = [f"-{n+1}" for n in range(self.number_points)]
        f += points[::-1]
        f_line = " ".join(f)
        obj_list.append(f_line)

        mtl_list.append(f"newmtl {self.name}_mtl")
        r, g, b, a = (0, 0, 0, 0)
        try:
            r, g, b, a = self.color.getRgb()
        except AttributeError:
            r, g, b, a = QtGui.QColor(self.color).getRgb()
        mtl_list.append(f"Kd {r/255} {g/255} {b/255} {a/255}")

        obj_list = [obj + "\n" for obj in obj_list]
        mtl_list = [mtl + "\n" for mtl in mtl_list]
        return (obj_list, mtl_list)


class Curve(Wireframe):
    def __init__(
        self,
        base_points,
        index,
        color,
        normalization_values,
        window_transformations,
        accuracy,
    ):
        self.base_points = base_points
        self.accuracy = accuracy
        self.coordinates = self.build_curve_coordinates()
        Wireframe.__init__(
            self,
            self.coordinates,
            index,
            color,
            normalization_values,
            window_transformations,
        )
        self.name = self.build_curve_name(index)

    def build_curve_name(self, index):
        return ""

    def build_curve_coordinates(self):
        return []


class BezierCurve(Curve):
    def __init__(
        self,
        base_points,
        index,
        color,
        normalization_values,
        window_transformations,
        accuracy=20,
    ):
        Curve.__init__(
            self,
            base_points,
            index,
            color,
            normalization_values,
            window_transformations,
            accuracy,
        )

    def build_curve_name(self, index):
        return f"Bezier_Curve_{index}"

    def build_curve_coordinates(self):
        bezier_points = []
        minimum_points = 4
        # Bezier Curves needs, at minimum, 4 points.
        # So, create chunks of size 4 and calculate
        # the bezier points to each of them.
        for i in range(0, len(self.base_points), minimum_points - 1):
            points = self.base_points[i : i + minimum_points]
            if len(points) != minimum_points:
                break
            else:
                bezier_points.append(
                    [
                        # Calculate bezier points, using the blending functions,
                        # for each point. The number of points are specified
                        # by the accuracy. This will build the curve like
                        #  a polygon with (acc - 1) polygons
                        calculate_bezier_points(points, t)
                        for t in np.linspace(0, 1, num=int(self.accuracy))
                    ]
                )
        flattened_bezier = [item for sublist in bezier_points for item in sublist]
        return [(x, y, z) for (x, y, z) in flattened_bezier]


class BSplineCurve(Curve):
    def __init__(
        self,
        base_points,
        index,
        color,
        normalization_values,
        window_transformations,
        accuracy=0.001,
    ):
        Curve.__init__(
            self,
            base_points,
            index,
            color,
            normalization_values,
            window_transformations,
            accuracy,
        )

    def build_curve_name(self, index):
        return f"B-Spline_Curve_{index}"

    def build_curve_coordinates(self):
        # P0...Pm control points, where m >= 3
        # generate m - 2 cubic segments
        spline_points = []
        iterations = int(1 / self.accuracy)
        number_points = len(self.base_points)
        minimum_points = 4

        # Create a rolling window with size 4
        for i in range(0, number_points):
            upper_bound = i + minimum_points

            # If out of boundaries, break
            if upper_bound > number_points:
                break
            points = self.base_points[i:upper_bound]

            # Calculate parameters for each 4 control points
            delta_x, delta_y = calculate_bspline_parameters(points, self.accuracy)
            x = delta_x[0]
            y = delta_y[0]
            spline_points.append((x, y, 0))
            for _ in range(0, iterations):
                x += delta_x[1]
                delta_x[1] += delta_x[2]
                delta_x[2] += delta_x[3]

                y += delta_y[1]
                delta_y[1] += delta_y[2]
                delta_y[2] += delta_y[3]

                spline_points.append((x, y, 0))
        return spline_points


class BicubicSurface(Curve):
    def __init__(
        self,
        base_points,
        index,
        color,
        normalization_values,
        window_transformations,
        curve_type,
        accuracy=20,
    ):
        self.curve_type = curve_type
        Curve.__init__(
            self,
            base_points,
            index,
            color,
            normalization_values,
            window_transformations,
            accuracy,
        )

    def build_curve_name(self, index):
        return f"{self.curve_type}_Bicubic_Surface_{index}"

    def build_curve_coordinates(self):
        bezier_points = []

        gbx = []
        gby = []
        gbz = []

        for i in range(0, len(self.base_points), 4):
            points = self.base_points[i : i + 4]
            aux = list(zip(*points))
            gbx.append(aux[0])
            gby.append(aux[1])
            gbz.append(aux[2])

        for s in np.linspace(0, 1, num=int(self.accuracy)):
            for t in np.linspace(0, 1, num=int(self.accuracy)):
                x_new = calculate_curve_points_to_surface(gbx, s, t, self.curve_type)
                y_new = calculate_curve_points_to_surface(gby, s, t, self.curve_type)
                z_new = calculate_curve_points_to_surface(gbz, s, t, self.curve_type)
                point = (x_new, y_new, z_new)
                bezier_points.append(point)

        return bezier_points
