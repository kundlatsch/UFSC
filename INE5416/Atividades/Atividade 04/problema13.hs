mmc :: Integral a => a -> a -> a
mmc a b | a == 0 = 0
        | b == 0 = 0
        | a == b = a
        | otherwise = div (a * b) (mdc a b) 
		
main do:
	n1String <- getLine
    n2String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    print(mmc n1 n2)