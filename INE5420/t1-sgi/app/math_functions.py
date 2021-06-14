import numpy as np
from functools import reduce
from app.utils import get_reflection_indexes, transformations_codes
from app.config import (
    X_MAX_TRANSLATED,
    X_MIN_TRANSLATED,
    Y_MAX_TRANSLATED,
    Y_MIN_TRANSLATED,
    MAX_NORMALIZED_VALUE,
    MIN_NORMALIZED_VALUE,
)


def x_viewport_transform(
    x_window, x_window_min, x_window_max, x_viewport_min, x_viewport_max
):
    x_window_min = -1
    x_window_max = 1
    return ((x_window - x_window_min) / (x_window_max - x_window_min)) * (
        x_viewport_max - x_viewport_min
    ) + 20


def y_viewport_transform(
    y_window, y_window_min, y_window_max, y_viewport_min, y_viewport_max
):
    y_window_min = -1
    y_window_max = 1
    return (1 - ((y_window - y_window_min) / (y_window_max - y_window_min))) * (
        y_viewport_max - y_viewport_min
    ) + 20


def build_translation_matrix(Tx, Ty, Tz):
    """
    Build translation matrix as:
        [1   0  0  0]
        [0   1  0  0]
        [0   0  1  0]
        [Tx  Ty Tz 1]
    """
    matrix = np.identity(4)
    matrix[3][0] = Tx
    matrix[3][1] = Ty
    matrix[3][2] = Tz
    return matrix


def build_scaling_matrix(Sx, Sy, Sz):
    """
    Build scaling matrix as:
        [Sx 0  0  0]
        [0  Sy 0  0]
        [0  0  Sz 0]
        [0  0  0  1]
    """
    matrix = np.identity(4)
    matrix[0][0] = Sx
    matrix[1][1] = Sy
    matrix[2][2] = Sz
    return matrix


def build_rotation_matrix(dX, dY, dZ):
    """
    Build rotation matrix as composition from:

    Rx = [1  0       0       0]
         [0  cos(dX) sen(dX) 0]
         [0 -sen(dX) cos(dX) 0]
         [0  0       0       1]

    Ry = [cos(dY) 0 -sen(dY) 0]
         [0       1 0        0]
         [sen(dY) 0 cos(dY)  0]
         [0       0 0        1]

    Rz = [cos(dZ)  sen(dZ) 0 0]
         [-sen(dZ) cos(dZ) 0 0]
         [0        0       1 0]
         [0        0       0 1]
    """
    Rx = np.identity(4)
    Rx[1][1] = np.cos(np.deg2rad(dX))
    Rx[1][2] = np.sin(np.deg2rad(dX))
    Rx[2][1] = -np.sin(np.deg2rad(dX))
    Rx[2][2] = np.cos(np.deg2rad(dX))

    Ry = np.identity(4)
    Ry[0][0] = np.cos(np.deg2rad(dY))
    Ry[0][2] = -np.sin(np.deg2rad(dY))
    Ry[2][0] = np.sin(np.deg2rad(dY))
    Ry[2][2] = np.cos(np.deg2rad(dY))

    Rz = np.identity(4)
    Rz[0][0] = np.cos(np.deg2rad(dZ))
    Rz[0][1] = np.sin(np.deg2rad(dZ))
    Rz[1][0] = -np.sin(np.deg2rad(dZ))
    Rz[1][1] = np.cos(np.deg2rad(dZ))

    return reduce(np.dot, [Rx, Ry, Rz])


def build_reflection_matrix(over):
    """
    Build reflection matrix as:
        [1  0  0]
        [0  1  0]
        [0  0  1],
    where over determine where to
    apply the reflection: x, y or origin.
    """
    matrix = np.identity(3)
    for index in get_reflection_indexes(over):
        matrix[index] = -1
    return matrix


def bezier_blending_functions(t):
    """
    Given Bezier matrix as:
        [-1  3 -3  1]
        [ 3 -6  3  0]
        [-3  3  0  0]
        [ 1  0  0  0],
    and vector T as:
        [t³  t²  t 1],
    return Bézier blending function as
        [(1 - t)³  ]
        [3t(1 - t)²]
        [3t²(1 - t)]
        [t3        ]
    """
    return np.array(
        [(1 - t) ** 3, 3 * t * ((1 - t) ** 2), 3 * (t ** 2) * (1 - t), t ** 3]
    )


def parameter_vector(n):
    return np.array([n ** 3, n ** 2, n, 1])


def bezier_matrix():
    """
    Return Bezier matrix as:
        [-1  3 -3  1]
        [ 3 -6  3  0]
        [-3  3  0  0]
        [ 1  0  0  0],
    """
    return np.array(
        [
            [-1, 3, -3, 1],
            [3, -6, 3, 0],
            [-3, 3, 0, 0],
            [1, 0, 0, 0],
        ]
    )


def calculate_bezier_points(points, t):

    blending_functions = bezier_blending_functions(t)
    return np.dot(blending_functions, points)


def calculate_curve_points_to_surface(points, s, t, curve):
    s_vector = parameter_vector(s)
    t_vector = parameter_vector(t)
    if curve == "Bezier":
        m = bezier_matrix()
    else:
        m = build_bspline_matrix()
    return reduce(
        np.dot,
        [s_vector, m, points, m.T, t_vector.T],
    )


def build_bspline_matrix():
    """
    Build B-Spline matrix as
        [ -1/6  1/2  -1/2  1/6]
        [  1/2   -1   1/2    0]
        [ -1/2    0   1/2    0]
        [  1/6  2/3   1/6    0]
    """
    return np.array(
        [
            [-1 / 6, 1 / 2, -1 / 2, 1 / 6],
            [1 / 2, -1, 1 / 2, 0],
            [-1 / 2, 0, 1 / 2, 0],
            [1 / 6, 2 / 3, 1 / 6, 0],
        ]
    )


def calculate_initial_differences(delta, a, b, c, d):
    """
    Calculate initial differences vector as
        [ d              ]
        [ aδ³  + bδ² + cδ]
        [ 6aδ³ + 2bδ²    ]
        [ 6aδ³           ]
    """
    delta_2 = delta ** 2
    delta_3 = delta ** 3
    return [
        d,
        a * delta_3 + b * delta_2 + c * delta,
        6 * a * delta_3 + 2 * b * delta_2,
        6 * a * delta_3,
    ]


def calculate_bspline_parameters(points, delta):
    MBS = build_bspline_matrix()

    GBS_x = []
    GBS_y = []
    for (x, y, _) in points:
        GBS_x.append(x)
        GBS_y.append(y)

    GBS_x = np.array([GBS_x]).T
    coeff_x = MBS.dot(GBS_x).T[0]
    init_diff_x = calculate_initial_differences(delta, *coeff_x)

    GBS_y = np.array([GBS_y]).T
    coeff_y = MBS.dot(GBS_y).T[0]
    init_diff_y = calculate_initial_differences(delta, *coeff_y)

    return init_diff_x, init_diff_y


transformations_functions_dict = {
    "rf": build_reflection_matrix,
    "rt": build_rotation_matrix,
    "r_rt": build_rotation_matrix,
    "sc": build_scaling_matrix,
    "r_sc": build_scaling_matrix,
    "tr": build_translation_matrix,
}


def build_homogeneous_coordinates(coordinates):
    ones = np.ones((len(coordinates), 1))
    return np.hstack((coordinates, ones))


def calculate_object_center(coordinates):
    return tuple(np.array(coordinates).mean(axis=0))


def multiply_coordinates_by_transformations(coordinates, transformations):
    return np.dot(coordinates, transformations)


def normalize_point(point, height, width):
    x, y, z = point
    return (x / height, y / width, z / width)


def build_window_normalizations(
    window_x_shift,
    window_y_shift,
    window_width,
    window_height,
    window_angle_x,
    window_angle_y,
    window_angle_z,
):
    translation_matrix = transformations_functions_dict["tr"](
        window_x_shift, window_y_shift, 0
    )
    rotation_matrix = transformations_functions_dict["rt"](
        window_angle_x, window_angle_y, window_angle_z
    )

    scaling_matrix = transformations_functions_dict["sc"](
        2 / window_width, 2 / window_height, 2 / window_width
    )

    composition = reduce(np.dot, [translation_matrix, rotation_matrix, scaling_matrix])

    return composition


def transform_coordinates(x, y, window_coordinates, viewport_coordinates):
    xvp = x_viewport_transform(
        x,
        window_coordinates.x_min,
        window_coordinates.x_max,
        viewport_coordinates.x_min,
        viewport_coordinates.x_max,
    )
    yvp = y_viewport_transform(
        y,
        window_coordinates.y_min,
        window_coordinates.y_max,
        viewport_coordinates.y_min,
        viewport_coordinates.y_max,
    )
    return (xvp, yvp)
