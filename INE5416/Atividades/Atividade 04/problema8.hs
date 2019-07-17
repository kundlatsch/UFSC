bhaskaraA :: Float -> Float -> Float -> Float
bhaskaraA n1 n2 n3 = ((-n2 + (((n2 * n2) - (4 * n1 * n3)) ** 1/2)) / 2 * n1)

bhaskaraS :: Float -> Float -> Float -> Float
bhaskaraS n1 n2 n3 = ((-n2 - (((n2 * n2) - (4 * n1 * n3)) ** 1/2)) / 2 * n1)

main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n2String :: Float)
    print(bhaskaraA n1 n2 n3)
    print(bhaskaraS n1 n2 n3)