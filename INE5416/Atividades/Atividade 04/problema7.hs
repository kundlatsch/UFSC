-- Recebe um Int e retorna um Int
fibonacci :: Int -> Int
fibonacci 1 = 1
fibonacci 2 = 1
fibonacci n = fibonacci(n-1) + fibonacci(n-2)

main = do
    nString <- getLine
    let n = (read nString :: Int)
    print (fibonacci n)