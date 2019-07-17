--Recebe um parâmetro Float e retorna String
potencia :: Float -> Float -> Float
potencia n1 n2 = n1 ** n2

main = do
    n1String <- getLine
    n2String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    print(potencia n1 n2)