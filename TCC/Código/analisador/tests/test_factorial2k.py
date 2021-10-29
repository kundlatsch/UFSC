from pytest import approx
from src.factorial2k import (
    effects_table_method,
    variation_allocation,
    nonlinear_regression,
)


# 2K^2 FACTORIAL DESIGN TESTS #
FACTORS_2K2 = 2
RESULTS_2K2 = [[15], [45], [25], [75]]
EFFECTS_2K2 = effects_table_method(FACTORS_2K2, RESULTS_2K2)
ALLOCATION_2K2 = variation_allocation(EFFECTS_2K2.copy())


def test_effects2k2():
    assert EFFECTS_2K2 == [40.0, 20.0, 10.0, 5.0]


def test_variation2k2():
    assert ALLOCATION_2K2 == approx([76, 19, 5], rel=1e-1, abs=1)


# 2K^n FACTORIAL DESIGN TESTS #
FACTORS_2KN = 3
RESULTS_2KN = [[14], [22], [10], [34], [46], [58], [50], [86]]
EFFECTS_2KN = effects_table_method(FACTORS_2KN, RESULTS_2KN)
ALLOCATION_2KN = variation_allocation(EFFECTS_2KN.copy())


def test_effects2kN():
    assert EFFECTS_2KN == [40.0, 10.0, 5.0, 20.0, 5.0, 2.0, 3.0, 1.0]


def test_variation2kN():
    assert ALLOCATION_2KN == approx([18, 4, 71, 4, 1, 2, 0], rel=1e-1, abs=1)


# VERIFYING THE ASSUMPTIONS


def test_nonlinear_regression():
    assert nonlinear_regression(EFFECTS_2K2, FACTORS_2K2) == [15.0, 45.0, 25.0, 75.0]
