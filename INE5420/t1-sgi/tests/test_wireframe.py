from numpy.testing import assert_allclose
from app.wireframe import Wireframe
from app.math_functions import (
    build_translation_matrix,
    build_scaling_matrix,
    build_rotation_matrix,
    build_reflection_matrix,
    transformations_functions_dict,
)
import pytest


@pytest.mark.skip(reason="no way of currently testing this")
def setup_class(points):
    return Wireframe(points, 0, "foo")


# Pure transformation calculations
@pytest.mark.skip(reason="no way of currently testing this")
def test_point_translation_A():
    w = setup_class([(1, 3)])
    t = ("tr", [-3, 2])
    w.transformations_codes.append(t)
    w.apply_transformations_to_points()
    expected_points = [(-2, 5)]
    assert w.transformed_coordinates == expected_points


@pytest.mark.skip(reason="no way of currently testing this")
def test_point_translation_B():
    w = setup_class([(-2, 5)])
    t = ("tr", [3, -2])
    w.transformations_codes.append(t)
    w.apply_transformations_to_points()
    expected_points = [(1, 3)]
    assert w.transformed_coordinates == expected_points


@pytest.mark.skip(reason="no way of currently testing this")
def test_point_scaling_C():
    w = setup_class([(4, 5)])
    s = ("sc", [0.5, 0.5])
    w.transformations_codes.append(s)
    w.apply_transformations_to_points()
    expected_points = [(2, 2.5)]
    assert w.transformed_coordinates == expected_points


@pytest.mark.skip(reason="no way of currently testing this")
def test_point_scaling_D():
    w = setup_class([(2, 2.5)])
    s = ("sc", [2, 2])
    w.transformations_codes.append(s)
    w.apply_transformations_to_points()
    expected_points = [(4, 5)]
    assert w.transformed_coordinates == expected_points


@pytest.mark.skip(reason="no way of currently testing this")
def test_point_rotation_E():
    w = setup_class([(2, 2.5)])
    r = ("rt", [30])
    w.transformations_codes.append(r)
    w.apply_transformations_to_points()
    expected_points = [(2.98, 1.16)]
    assert_allclose(w.transformed_coordinates, expected_points, rtol=1e-2, atol=0)


@pytest.mark.skip(reason="no way of currently testing this")
def test_point_rotation_F():
    w = setup_class([(2.98, 1.16)])
    r = ("rt", [-30])
    w.transformations_codes.append(r)
    w.apply_transformations_to_points()
    expected_points = [(2, 2.5)]
    assert_allclose(w.transformed_coordinates, expected_points, rtol=1e-2, atol=0)


@pytest.mark.skip(reason="no way of currently testing this")
def test_transformations_compositions_G():
    w = setup_class([(1, 3)])
    t = ("tr", [-3, 2])
    w.transformations_codes.append(t)
    t = ("tr", [3, -2])
    w.transformations_codes.append(t)
    w.apply_transformations_to_points()
    expected_points = [(1, 3)]
    assert w.transformed_coordinates == expected_points


@pytest.mark.skip(reason="no way of currently testing this")
def test_transformations_compositions_H():
    w = setup_class([(4, 5)])
    s = ("sc", [0.5, 0.5])
    w.transformations_codes.append(s)
    s = ("sc", [2, 2])
    w.transformations_codes.append(s)
    w.apply_transformations_to_points()
    expected_points = [(4, 5)]
    assert w.transformed_coordinates == expected_points


@pytest.mark.skip(reason="no way of currently testing this")
def test_transformations_compositions_I():
    w = setup_class([(2, 2.5)])
    r = ("rt", [30])
    w.transformations_codes.append(r)
    r = ("rt", [-30])
    w.transformations_codes.append(r)
    w.apply_transformations_to_points()
    expected_points = [(2, 2.5)]
    assert_allclose(w.transformed_coordinates, expected_points, rtol=1e-2, atol=0)


# ----------------------------------------------------------------


@pytest.mark.skip(reason="no way of currently testing this")
def test_transformations_compositions_G2():
    w = setup_class([(1, 3), (2, 8)])
    t = ("tr", [-3, 2])
    w.transformations_codes.append(t)
    t = ("tr", [3, -2])
    w.transformations_codes.append(t)
    w.transform_coordinates()
    expected_points = [(1, 3), (2, 8)]
    assert w.transformed_coordinates == expected_points
