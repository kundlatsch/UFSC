inverte :: [t] -> [t]
inverte [] = []
inverte (a:b) = (inverte b) ++ [a]

main = do
    let lista = [2, 6, 10]
    print(inverte lista)
