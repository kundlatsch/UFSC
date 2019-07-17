busca :: [Int] -> Int -> Bool
busca [] x = False
busca [a] x = if a == x
    then 
        True
    else 
        False
busca (a:b) x = if a == x
    then
        True
    else
        busca b x

main = do
    let lista = [2, 6, 10]
    let numero = 1
    print(busca lista numero)
