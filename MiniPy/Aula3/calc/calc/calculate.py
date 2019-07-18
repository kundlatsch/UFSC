import re

from .operations import OPERATIONS

from .errors import(
    CalculatorSyntaxError,
    TooManyValuesError,
)

FLOAT_RE = re.compile(r'-?(([0-9]+)|([0-9]+\.[0-9]+))')

def calculate(command: str):
    elements = command.split()

    stack = []

    for element in elements:
        if FLOAT_RE.fullmatch(element):
            stack.append(float(element))

        elif element == 'exit':
            return 'program exit'

        elif element in OPERATIONS:
           OPERATIONS[element](stack)

        else:
            raise CalculatorSyntaxError(f'Invalid token "{element}"')

    try:
        [result] = stack
    except ValueError as e:
        raise TooManyValuesError('Too many values remaining')

    return result
