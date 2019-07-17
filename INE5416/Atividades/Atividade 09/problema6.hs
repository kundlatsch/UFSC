maior :: Float -> Float -> Float
maior a b = (\y z -> if y > z then y else z) a b

main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n3String :: Float)
    print ( ( \x y z -> if (x > y && x > z) then x else maior y z ) n1 n2 n3 )