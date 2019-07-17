main = do
    n1String <- getLine
    n2String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    print ((\x y -> x /= y) n1 n2)