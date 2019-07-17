-- |a - b | < c < a + b

verificar :: Float -> Float -> Float -> Bool
verificar n1 n2 n3 = if (n1 - n2 < n3) && (n3 < n1 + n2)
    then
        True
    else
        False

triangulo :: Float -> Float -> Float -> String
triangulo n1 n2 n3 = if (verificar n1 n2 n3 && verificar n1 n3 n2 && verificar n2 n3 n1)
    then
        
main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n2String :: Float)
    print(media n1 n2 n3)