from enum import Enum


class Shape(Enum):
    Nothing = 0
    Point = 1
    Line = 2
    Polygon = -1

    @classmethod
    def _missing_(cls, value):
        return Shape.Polygon


def get_reflection_indexes(over):
    return {"x": [(1, 1)], "y": [(0, 0)], "origin": [(1, 1), (0, 0)]}[over]


transformations_codes = {
    "rf": "Reflection",
    "rt": "Rotation",
    "sc": "Scaling",
    "tr": "Translation",
}


def build_transformation_string(transformation):
    return f"{transformations_codes[transformation[0]]} {transformation[1]}"


class CoordinatesRepresentation:
    def __init__(self, x_min, y_min, x_max, y_max, factor=0):
        self.x_min = x_min
        self.y_min = y_min
        self.x_max = x_max
        self.y_max = y_max
        self.x_shift_factor = calculate_coordinate_shift(self.x_max, self.x_min, factor)
        self.y_shift_factor = calculate_coordinate_shift(self.y_max, self.y_min, factor)


def calculate_coordinate_shift(max_coordinate, min_coordinate, step):
    size = max_coordinate - min_coordinate
    return size * step
