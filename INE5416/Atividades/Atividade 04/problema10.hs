maior :: Float -> Float -> Float -> Float
maior a b c = 
    if (a > b) && (a > c) then a
    else if (b > a) && (b > c) then b
    else c


main = do
  n1String <- getLine
  n2String <- getLine
  n3String <- getLine
  let n1 = (read n1String :: Float)
  let n2 = (read n2String :: Float)
  let n3 = (read n3String :: Float)
  print(maior n1 n2 n3)