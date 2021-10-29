from factorial2k import *
from data import RESULTS_PLANS_CREATED
import seaborn as sb
import pandas as pd
import matplotlib.pyplot as plt
from statistics import mean


def normalize(values):
    vmin = min(values)
    vmax = max(values)
    return [(v - vmin) / (vmax - vmin) for v in values]


def main():
    FACTORS_2KN = 4
    results = RESULTS_PLANS_CREATED
    EFFECTS_2KN = effects_table_method(FACTORS_2KN, results)

    sb.set(style="darkgrid")

    # 1 - Independent Errors
    # x = normalize(get_residuals(results, EFFECTS_2KN, FACTORS_2KN))
    # y = normalize(nonlinear_regression(EFFECTS_2KN, FACTORS_2KN)) #y_hat
    # plt.plot(x, y, 'bo')
    # plt.xlabel('Residuals', fontsize=10)
    # plt.ylabel('Predicted Value', fontsize=10)
    # plt.show()

    # 2 - Normally Distributed Errors
    # r = []
    # for re in results:
    #     r.append(re[0])
    # x = normalize(r)
    # y = normalize(nonlinear_regression(EFFECTS_2KN, FACTORS_2KN)) #y_hat
    # plt.plot([0,1], 'r')
    # plt.plot(x, y, 'bo')
    # plt.xlabel('Simulation Value', fontsize=10)
    # plt.ylabel('Predicted Value', fontsize=10)
    # plt.show()

    # 3 - Constant Standard Deviation of Errors
    x = [i for i in range(2 ** FACTORS_2KN)]
    y = normalize(nonlinear_regression(EFFECTS_2KN, FACTORS_2KN))  # y_hat
    m = mean(y)
    y2 = [m for i in range(2 ** FACTORS_2KN)]
    plt.plot(x, y, "bo")
    plt.plot(x, y2, "r--")
    plt.ylabel("Predicted Value", fontsize=10)
    plt.show()


if __name__ == "__main__":
    main()
