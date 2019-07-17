main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n3String :: Float)
    print ( ( \x y z -> ((x + y + z) / 3) >= 6 ) n1 n2 n3 )