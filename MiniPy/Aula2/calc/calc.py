import sys

command = ''

def add(a, b):
    return a + b

def sub(a, b):
    return a - b

def mult(a, b):
    return a * b

def div(a, b):
    return a / b


OPERATIONS = {
    '+': add,
    '-': sub, 
    '*': mult, 
    '/': div,
}

class CalculatorSyntaxError(Exception):
    pass

class NotEnaughtValuesError(Exception):
    pass

class TooManyValuesError(Exception):
    pass

def calculate(command: str):
    elements = command.split()

    stack = []

    for element in elements:
        if element.isdigit():
            stack.append(int(element))

        elif element == 'exit':
            return 'program exit'

        elif element in OPERATIONS:
            try:
                rhs = stack.pop()
                lhs = stack.pop()
            except IndexError as e:
                raise NotEnaughtValuesError('Not enaught operands.') from e
            stack.append(OPERATIONS[element](lhs, rhs))

        else:
            raise CalculatorSyntaxError(f'Invalid token "{element}"')

    try:
        [result] = stack
    except ValueError as e:
        raise TooManyValuesError('Too many values remaining')

    return result

if __name__ == '__main__':
    while command != "exit":
        command = input('-> ')
        try:
            output = calculate(command)
        except (CalculatorSyntaxError, NotEnaughtValuesError,
                TooManyValuesError) as e:
            print(e, file=sys.stderr)
        else:
            print(output)
