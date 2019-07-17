baskara :: Float -> Float -> Float -> (Float, Float)
baskara a b c =
        let delta = sqrt (b^2 - 4*a*c)
        in ( (-b-delta)/(2*a), (-b+delta)/(2*a) )

main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n3String :: Float)
    print (baskara n1 n2 n3)