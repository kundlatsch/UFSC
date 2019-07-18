from math import sqrt
from .errors import NotEnaughtValuesError

def add(a, b):
	return a + b

def sub(a, b):
	return a - b

def mult(a, b):
	return a * b

def div(a, b):
	return a / b

def sqrt(x):
	return sqrt(x)

def binary_op(f):
	def execute(stack):
		try:
			rhs = stack.pop()
			lhs = stack.pop()
		except IndexError as e:
			raise NotEnaughtValuesError('Not enaught operands.') from e
		stack.append(f(lhs, rhs))
	return execute

def unary_op(f):
	def execute(stack):
		try:
			x = stack.pop()
		except IndexError as e:
			raise NotEnaughtValuesError('Not enaught operands.') from e
		stack.append(f(x))
	return execute

OPERATIONS = {
	'+': binary_op(add),
	'-': binary_op(sub), 
	'*': binary_op(mult), 
	'/': binary_op(div),
	'sqrt': unary_op(sqrt)
}