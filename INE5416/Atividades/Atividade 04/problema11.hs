mdc :: Integral a => a -> a -> a
mdc a b | mod a b == 0 = b
        | mod b a == 0 = a
        | a > b = mdc b (mod a b)
        | a < b = mdc a (mod b a)

main do:
	n1String <- getLine
    n2String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    print(mdc n1 n2)