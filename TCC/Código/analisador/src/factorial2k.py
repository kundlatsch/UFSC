import pyDOE2 as pd
import numpy as np
from itertools import combinations
import string


def create_sign_table(factors):
    # This combination string is used to create the factorial_string,
    # because pyDOE2 input is the comlumns that we want to create in
    # the signal table. Here we are using it to generate the 2k^N table,
    # so we need the combination of N comlumns.

    combination_string = ""
    for _, letter in zip(range(0, factors), string.ascii_lowercase):
        combination_string += letter

    _combinations = [
        "".join(l)
        for i in range(len(combination_string))
        for l in combinations(combination_string, i + 1)
    ]

    factors_string = " ".join(_combinations)
    factorial_columns = pd.fracfact(factors_string)

    image_column = np.ones((2 ** factors, 1))
    sign_table = np.hstack((image_column, factorial_columns))
    return sign_table


def effects_table_method(factors, results):
    results_column = np.array(results)
    sign_table = create_sign_table(factors)

    tpf = 2 ** factors
    coefficients = [
        float(np.dot(sign_table[:, i], results_column) / tpf) for i in range(tpf)
    ]

    return coefficients


def variation_allocation(effects):
    # The first elements represents q0, and is not important here
    effects.pop(0)

    allocation = [4 * effect * effect for effect in effects]
    total_variation = sum(allocation)

    percentages = [(100 * a) / total_variation for a in allocation]

    return percentages


def get_residuals(results, effects, factors):
    predicted_results = nonlinear_regression(effects, factors)
    return [float(predicted_results[i] - results[i]) for i in range(len(results))]


def nonlinear_regression(coefficients, factors):
    sign_table = create_sign_table(factors)
    return [regression_eq(coefficients, r) for r in sign_table]


def regression_eq(coefficients, independent_values):
    map(lambda x: np.array(x), (independent_values, coefficients))
    return sum(np.multiply(coefficients, independent_values))
