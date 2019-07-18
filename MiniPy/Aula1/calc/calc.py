command = ''

OPERATIONS = ('+', '-', '*', '/')

class CalculatorSyntaxError(Exception):
    pass

def calculate(command: str):
    elements = command.split()

    stack = []

    for element in elements:
        if element.isdigit():
            stack.append(int(element))
        elif element in OPERATIONS:
            rhs = stack.pop()
            lhs = stack.pop()
            if element == '+':
                stack.append(lhs + rhs)
            if element == '-':
                stack.append(lhs - rhs)
            if element == '*':
                stack.append(lhs * rhs)
            if element == '/':
                stack.append(lhs / rhs)
        else:
            raise CalculatorSyntaxError(f'Invalid token "{element}"')

    [result] = stack

    return result

while True:
    command = input('>> ')
    print(calculate(command))
