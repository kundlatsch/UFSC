import Data.Function

fibonacci :: Int -> Int
fibonacci = fix (\f n -> if n > 2 then f(n-1) + f(n-2) else 1)

main = do
    n1String <- getLine
    let n1 = (read n1String :: Int)
    print (fibonacci n1)