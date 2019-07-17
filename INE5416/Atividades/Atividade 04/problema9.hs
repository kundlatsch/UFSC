distancia :: Float -> Float -> Float -> Float -> Float -> Float -> Float
distancia a1 a2 a3 b1 b2 b3 = ((b1 - a1) ** 2) + ((b2 - a2) ** 2) + ((b3 - a3) ** 2) ** 1/2


main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    n4String <- getLine
    n5String <- getLine
    n6String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n3String :: Float)
    let n4 = (read n4String :: Float)
    let n5 = (read n5String :: Float)
    let n6 = (read n6String :: Float)
    print(distancia n1 n2 n3 n4 n5 n6)