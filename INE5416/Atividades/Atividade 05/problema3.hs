menor :: [Int] -> Int
menor [] = 0
menor [x] = x
menor (a:b) = if a > menor(b)
	then
		menor(b)
	else
		a

main = do
	let lista = [1 .. 10]
	print(menor lista)

