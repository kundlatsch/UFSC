from pytest import approx

from calc.calculate import calculate

def test_basic_sum():
	assert calculate('2 2 +') == 4
	assert calculate('20 3 +') == 23
	assert calculate('0 2 +') == 2

def test_basic_sub():
	assert calculate('10 5 -') == 5
	assert calculate('10 20 -') == -10
	assert calculate('200 200 -') == 0

def test_basic_mult():
	assert calculate('6 3 *') == 18
	assert calculate('900 1 *') == 900
	assert calculate('10 0 *') == 0

def test_basic_div():
	assert calculate('1 10 /') == 0.1
	assert calculate('10 2 /') == 5
	assert calculate('0 9 /') == 0

def test_float_sum():
	assert calculate('2.2 2.5 +') == 4.7
	assert calculate('0.2 0 +') == 0.2
	assert calculate('10 0.125 +') == 10.125

def test_float_sub():
	assert calculate('5.2 1.1 -') == 4.1
	assert calculate('0 9.8 -') == -9.8
	assert calculate('120.45 0 -') == 120.45

def test_float_negative_sum():
	assert calculate('-2.2 2.5 +') == approx(0.3)

def test_sqrt():
	assert calculate('4 sqrt') == 2