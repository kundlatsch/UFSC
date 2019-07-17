maior :: [Int] -> Int
maior [] = 0
maior [x] = x
maior (a:b) = if a < maior(b)
    then
        maior(b)
    else
        a

menor :: [Int] -> Int
menor [] = 0
menor [x] = x
menor (a:b) = if a > menor(b)
    then
        menor(b)
    else
        a

diferencaMaiorMenor ::  [Int] -> Int
diferencaMaiorMenor (a:b) = maior(a:b) - menor(a:b)


main = do
    let lista = [1 .. 10]
    print(diferencaMaiorMenor lista)