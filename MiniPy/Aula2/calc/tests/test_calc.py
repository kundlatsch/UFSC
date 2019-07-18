from calc import calculate

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