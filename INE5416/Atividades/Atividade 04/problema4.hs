--Recebe um parâmetro Float e retorna String
my_xor :: Bool -> Bool -> Bool
my_xor n1 n2 = (n1 && not n2) || (not n1 && n2)

main = do
    n1String <- getLine
    n2String <- getLine
    let n1 = (read n1String :: Bool)
    let n2 = (read n2String :: Bool)

    print(my_xor n1 n2)