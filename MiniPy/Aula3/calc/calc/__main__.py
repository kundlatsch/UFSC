import sys
from .errors import (
    calculate, 
    CalculatorSyntaxError, 
    NotEnaughtValuesError, 
    TooManyValuesError,
)


if __name__ == '__main__':
    command = ''
    while command != "exit":
        command = input('-> ')
        try:
            output = calculate(command)
        except (CalculatorSyntaxError, NotEnaughtValuesError,
                TooManyValuesError) as e:
            print(e, file=sys.stderr)
        else:
            print(output)
