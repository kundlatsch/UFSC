--Recebe um parâmetro Float e retorna String
absolute :: Float -> Float
absolute n = if n >= 0
    then
        n
    else
        -n

main = do
    nString <- getLine
    let n = (read nString :: Float)

    print(absolute n)