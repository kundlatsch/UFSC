type Nome = String
type Discipina = String
type Nota = Float
type Aluno = (Nome, Discipina, Nota, Nota, Nota)

aluno :: Int -> Aluno
aluno 1 = ("Bob", "Paradigmas", 10, 9, 1)
aluno 2 = ("Tom", "Redes", 5, 7, 6)
aluno 3 = ("Arthur", "Formais", 4, 7, 8)
aluno 4 = ("Diana", "Paradigmas", 2, 4, 5)
aluno 5 = ("Maria", "Calculo", 5, 5, 8)

getNome :: Aluno -> String
getNome (n, _, _, _, _) = n

getMediaAluno :: Int -> Float
getMediaAluno a = calculaMediaAluno(aluno a)

calculaMediaAluno :: Aluno -> Float
calculaMediaAluno (_, _, a, b, c) = (a + b + c) / 3

getMediaTurma :: Int -> Float
getMediaTurma a = (somaMediaTurma a) / (fromIntegral a)

somaMediaTurma :: Int -> Float
somaMediaTurma 0 = 0
somaMediaTurma a = getMediaAluno(a) + somaMediaTurma(a - 1)

main = do
    putStrLn(show(getNome(aluno 1)))
    putStrLn(show(getMediaAluno 1))
    putStrLn(show(getMediaTurma 5))