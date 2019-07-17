primeiros :: Int -> [t] -> [t]
primeiros c [a] = [a]
primeiros c (a:b) = if (c - 1) /= 0
  then
    [a] ++ primeiros (c - 1) b
  else
    [a]

main = do
  nString <- getLine
  let n = (read nString :: Int)
  let lista = [1 .. 10]
  print(primeiros n lista)