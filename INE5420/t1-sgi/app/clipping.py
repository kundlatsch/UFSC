import numpy as np
from copy import deepcopy
from app.wireframe import Curve


def get_cohen_sutherland_point_code(
    value, min_value, max_value, lower_code, upper_code
):
    if value > max_value:
        return upper_code
    if value < min_value:
        return lower_code
    return 0


def cohen_sutherland(p1, p2):
    top = 8
    bottom = 4
    left = 1
    right = 2

    x_min = -1
    y_min = -1
    x_max = 1
    y_max = 1

    p0_code = 0
    x0, y0 = p1
    p0_code += get_cohen_sutherland_point_code(x0, x_min, x_max, left, right)
    p0_code += get_cohen_sutherland_point_code(y0, y_min, y_max, bottom, top)

    p1_code = 0
    x1, y1 = p2
    p1_code += get_cohen_sutherland_point_code(x1, x_min, x_max, left, right)
    p1_code += get_cohen_sutherland_point_code(y1, y_min, y_max, bottom, top)
    while True:

        # Both out from window
        if p0_code & p1_code != 0:
            return (False, (x0, y0), (x1, y1))
        # Both in
        if p0_code | p1_code == 0:
            return (True, (x0, y0), (x1, y1))

        x = 0
        y = 0

        outside_code = max(p0_code, p1_code)
        if outside_code & left == left:
            y = y0 + (y1 - y0) * (x_min - x0) / (x1 - x0)
            x = x_min
        if outside_code & right == right:
            y = y0 + (y1 - y0) * (x_max - x0) / (x1 - x0)
            x = x_max

        if outside_code & top == top:
            x = x0 + (x1 - x0) * (y_max - y0) / (y1 - y0)
            y = y_max

        if outside_code & bottom == bottom:
            x = x0 + (x1 - x0) * (y_min - y0) / (y1 - y0)
            y = y_min

        if outside_code == p0_code:
            x0 = x
            y0 = y
            p0_code = 0
            p0_code += get_cohen_sutherland_point_code(x0, x_min, x_max, left, right)
            p0_code += get_cohen_sutherland_point_code(y0, y_min, y_max, bottom, top)
        else:
            x1 = x
            y1 = y
            p1_code = 0
            p1_code += get_cohen_sutherland_point_code(x1, x_min, x_max, left, right)
            p1_code += get_cohen_sutherland_point_code(y1, y_min, y_max, bottom, top)

    return (False, None, None)


def liang_barsky(point_1, point_2):
    x_min = -1
    y_min = -1
    x_max = 1
    y_max = 1

    x0, y0 = point_1
    x1, y1 = point_2

    p1 = -(x1 - x0)
    p2 = -p1
    p3 = -(y1 - y0)
    p4 = -p3

    q1 = x0 - x_min
    q2 = x_max - x0
    q3 = y0 - y_min
    q4 = y_max - y0

    pk = list(zip([p1, p2, p3, p4], [q1, q2, q3, q4]))
    initial_inside_check = any([p == 0 and q < 0 for (p, q) in pk])
    if initial_inside_check:
        return (False, None, None)

    r_negative = [(q / p) for (p, q) in pk if p < 0]
    u1 = max(0, max(r_negative, default=0))

    r_positive = [(q / p) for (p, q) in pk if p > 0]
    u2 = min(1, min(r_positive, default=1))

    # Completly outside
    if u1 > u2:
        return (False, None, None)

    new_x0 = x0 + u1 * p2
    new_y0 = y0 + u1 * p4
    new_x1 = x0 + u2 * p2
    new_y1 = y0 + u2 * p4

    return (True, (new_x0, new_y0), (new_x1, new_y1))


def w_a_get_window_index(window_vertices, point, code):
    x, y = point
    # The index from window vertices list must be the
    # right before the next window vertice
    if x == 1:
        # Right, so right bottom
        index = window_vertices.index(((1, -1), 0))
        window_vertices.insert(index, (point, code))
    if x == -1:
        # Left, so left top
        index = window_vertices.index(((-1, 1), 0))

        window_vertices.insert(index, (point, code))
    if y == 1:
        # Top, so right top
        index = window_vertices.index(((1, 1), 0))
        window_vertices.insert(index, (point, code))
    if y == -1:
        # Bottom, so left bottom
        index = window_vertices.index(((-1, -1), 0))
        window_vertices.insert(index, (point, code))
    return window_vertices


def is_point_outside_window(points):
    return np.any((points < -1) | (points > 1))


def is_point_inside_window(points):
    return not is_point_outside_window(np.array(points))


def weiler_atherton(object_coordinates):

    # Check if there's any point inside the window
    all_outside = np.all(
        [is_point_outside_window(np.array(c)) for c in object_coordinates]
    )
    if all_outside:
        return False, [None]

    # 0 = original
    # 1 = enter
    # 2 = exit

    # First step: build two lists of vertices
    window_vertices = [((-1, 1), 0), ((1, 1), 0), ((1, -1), 0), ((-1, -1), 0)]
    object_vertices = [(c, 0) for c in object_coordinates]

    number_points = len(object_vertices)
    enter_points = []
    # Second step: calculate intersections
    for index in range(number_points):
        p0 = object_coordinates[index]
        p1 = object_coordinates[(index + 1) % number_points]

        is_visible, new_p0, new_p1 = cohen_sutherland(p0, p1)
        if is_visible:
            if new_p1 != p1:
                # Exit point
                point_index = object_vertices.index((p0, 0)) + 1
                # Append new point right after the original
                object_vertices.insert(point_index, (new_p1, 2))
                window_vertices = w_a_get_window_index(window_vertices, new_p1, 2)

            if new_p0 != p0:
                # Enter point
                point_index = object_vertices.index((p0, 0)) + 1
                # Append new point right after the original
                object_vertices.insert(point_index, (new_p0, 1))
                enter_points.append((new_p0, 1))
                window_vertices = w_a_get_window_index(window_vertices, new_p0, 1)

    new_polygons = []
    new_points = []
    # If enter points is empty, return the own coordinates
    if enter_points != []:
        # Repeat until empty enter points
        while enter_points != []:
            # Get first point from enter
            reference_point = enter_points.pop(0)
            rf_p, _ = reference_point
            inside_points = [rf_p]
            point_index = object_vertices.index(reference_point) + 1
            new_points.append(reference_point)

            obj_len = len(object_vertices)
            for aux_index in range(obj_len):
                (p, c) = object_vertices[(point_index + aux_index) % obj_len]
                new_points.append((p, c))
                inside_points.append(p)
                if c != 0:
                    break

            last_point = new_points[-1]
            point_index = window_vertices.index(last_point)
            window_len = len(window_vertices)
            for aux_index in range(window_len):
                (p, c) = window_vertices[(point_index + aux_index) % window_len]
                new_points.append((p, c))
                inside_points.append(p)
                if c != 0:
                    break

            new_polygons.append(inside_points)
        coordinates = new_polygons
    else:
        coordinates = [object_coordinates]

    # print(f"Coordinates after weiler_atherton={coordinates}")
    return True, coordinates


def clip_points(point):
    return np.clip(np.array(point), -1, 1)


def clip(wireframe, coordinates, method=None):
    # Apply point clipping
    if wireframe.number_points == 1:
        coord_aux = np.array(coordinates[0])
        if is_point_outside_window(coord_aux):
            # print(f"Coords {coord_aux} not visible!")
            is_visible = False
            return is_visible, [None]
        else:
            # print(f"Coords {coord_aux} visible!")
            is_visible = True
            return is_visible, [[coord_aux]]

    # Apply line clipping
    if wireframe.number_points == 2:
        p1 = coordinates[0]
        p2 = coordinates[1]
        if method == "liang-barsky":
            is_visible, new_p1, new_p2 = liang_barsky(p1, p2)
        if method == "cohen-sutherland":
            is_visible, new_p1, new_p2 = cohen_sutherland(p1, p2)
        return is_visible, [[new_p1, new_p2]]

    # Apply point clipping to curve
    if isinstance(wireframe, Curve):
        coordinates = list(map(clip_points, coordinates))
        return True, [coordinates]

    # Apply polygon clipping
    is_visible, coordinates = weiler_atherton(coordinates)
    return is_visible, coordinates
