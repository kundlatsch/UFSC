main = do
    print (map (\x -> if x `mod` 2 == 0 then True else False) [1,2,3,4])