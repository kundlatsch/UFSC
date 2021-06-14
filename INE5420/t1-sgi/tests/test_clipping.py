import numpy as np
import pytest
from numpy.testing import assert_array_equal, assert_allclose
from app.clipping import cohen_sutherland, liang_barsky


def test_cohen_sutherland_both_out():
    is_visible, _, _ = cohen_sutherland((-1.2, -1.2), (1.2, -1.2))
    expected_result = False
    assert is_visible == expected_result


def test_cohen_sutherland_both_in():
    is_visible, _, _ = cohen_sutherland((-0.2, -0.2), (1, -1))
    expected_result = True
    assert is_visible == expected_result


def test_cohen_sutherland_left_out_right_in():
    is_visible, (x0, y0), (x1, y1) = cohen_sutherland((-1.2, -1), (1, -1))
    expected_result_visibility = True
    assert is_visible == expected_result_visibility
    assert (x0, y0) == (-1, -1)
    assert (x1, y1) == (1, -1)


def test_cohen_sutherland_right_out_left_in():
    is_visible, (x0, y0), (x1, y1) = cohen_sutherland((0, 0), (1.2, -1.2))
    expected_result_visibility = True
    assert is_visible == expected_result_visibility
    assert (x0, y0) == (0, 0)
    assert (x1, y1) == (1, -1)


def test_cohen_sutherland_top_out_left_out():
    is_visible, (x0, y0), (x1, y1) = cohen_sutherland((0, 1.2), (1.2, 0.8))
    expected_result_visibility = True
    assert is_visible == expected_result_visibility
    assert (x0, y0) == (0.6, 1)
    assert (x1, y1) == (1, 0.8666666666666667)


def test_liang_barsky_both_out():
    is_visible, _, _ = liang_barsky((-1.2, -1.2), (1.2, -1.2))
    expected_result = False
    assert is_visible == expected_result


def test_liang_barsky_both_in():
    is_visible, (x0, y0), (x1, y1) = liang_barsky((-0.2, -0.2), (1, -1))
    expected_result = True
    assert is_visible == expected_result
    assert (x0, y0) == (-0.2, -0.2)
    assert (x1, y1) == (1, -1)


def test_liang_barsky_left_out_right_in():
    is_visible, (x0, y0), (x1, y1) = liang_barsky((-1.2, -1), (1, -1))
    expected_result_visibility = True
    assert is_visible == expected_result_visibility
    assert (x0, y0) == (-1, -1)
    assert_allclose((x1, y1), (1, -1), rtol=1e-5, atol=0)
    # assert (x1, y1) == (1, -1)


def test_liang_barsky_right_out_left_in():
    is_visible, (x0, y0), (x1, y1) = liang_barsky((0, 0), (1.2, -1.2))
    expected_result_visibility = True
    assert is_visible == expected_result_visibility
    assert (x0, y0) == (0, 0)
    assert (x1, y1) == (1, -1)


def test_liang_barsky_top_out_left_out():
    is_visible, (x0, y0), (x1, y1) = liang_barsky((0, 1.2), (1.2, 0.8))
    expected_result_visibility = True
    assert is_visible == expected_result_visibility
    assert (x0, y0) == (0.6, 1)
    assert (x1, y1) == (1, 0.8666666666666667)
