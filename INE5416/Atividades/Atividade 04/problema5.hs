media :: Float -> Float -> Float -> String
media n1 n2 n3 = if ((n1 + n2 + n3) / 3) >= 6
    then
        "Aprovado"
    else
        "Reprovado"

main = do
    n1String <- getLine
    n2String <- getLine
    n3String <- getLine
    let n1 = (read n1String :: Float)
    let n2 = (read n2String :: Float)
    let n3 = (read n2String :: Float)
    print(media n1 n2 n3)