ocorrencias :: [Int] -> Int -> Int
ocorrencias [] x = 0
ocorrencias [y] x = if x == y
    then 1
    else 0
ocorrencias (a:b) x = if x == a
    then 1 + ocorrencias b x
    else 0 + ocorrencias b x

main = do
    let lista = [2, 6, 10, 2]
    let numero = 2
    print(ocorrencias lista numero)
