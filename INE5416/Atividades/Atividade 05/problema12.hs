apagar :: Int -> [t] -> [t]
apagar c [] = []
apagar c (a:b) = if (c - 1) /= 0
  then
    apagar (c - 1) b
  else
    b

main = do
  nString <- getLine
  let n = (read nString :: Int)
  let lista = [1 .. 10]
  print(apagar n lista)