soma :: [Int] -> Int
soma [] = 0
soma (a:b) = a + soma(b)

main = do
	entradaString <- getLine
	let entrada = (read entradaString :: Int)
	let lista = [1 .. entrada]
	print(soma lista)