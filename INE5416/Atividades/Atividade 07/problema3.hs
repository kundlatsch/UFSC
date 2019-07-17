class (Integral x) => MeuInt x where
    bigger :: x -> x -> x
    smaller :: x -> x -> x
    par :: x -> Bool
    impar :: x -> Bool
    primo :: x -> Bool
    mdc :: x -> x -> x
    (===) :: x -> x -> Bool

    bigger a b  | a > b = a
                | otherwise = b

    smaller a b | a == (bigger a b) = b
                | otherwise = a

    par x   | x `mod` 2 == 0 = True
            | otherwise = False

    impar x = not(par x)
    
    primo 0 = True
    primo x | x `mod` (x - 1) /= 0 = primo((x - 1)::Integer)
            | otherwise = False

    x === y | abs(x - y) <= 1 = True
            | otherwise = False

instance MeuInt Integer
instance MeuInt Int

main = do
    --print (bigger (4::Integer) (12::Integer))
    --print (smaller (4::Integer) (12::Integer))
    --print (par (4::Integer))
    --print (impar (4::Integer))
    --print ((4::Integer) === (8::Integer))
    print (primo (2::Integer))