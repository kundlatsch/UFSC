data Arvore = Null | No Int Arvore Arvore

minhaArvore :: Arvore
minhaArvore = No 52 (No 32 (No 12 Null Null) (No 35 Null Null)) (No 56 (No 55 Null Null) (No 64 Null Null))

somaElementos :: Arvore -> Int
somaElementos Null = 0
somaElementos (No n esq dir) = n + (somaElementos esq) + (somaElementos dir)

buscaElemento :: Arvore -> Int -> Bool
buscaElemento Null _ = False
buscaElemento (No n esq dir) x 
    | (n == x) = True                           
    | otherwise = (buscaElemento esq x) || (buscaElemento dir x)

limiteSup :: Int
limiteSup = 1000 --Define um limite superior para o maior número

minimo :: Int -> Int -> Int
minimo x y | (x < y) = x
           | otherwise = y

minimoElemento :: Arvore -> Int
minimoElemento Null = limiteSup 
minimoElemento (No n esq dir) = 
    minimo n (minimo (minimoElemento esq) (minimoElemento dir))

-- funções exercício

ocorrenciasElemento :: Arvore -> Int -> Int
ocorrenciasElemento (No n Null Null) x = if n == x 
  then 1
  else 0
ocorrenciasElemento (No n esq dir) x
  | (n == x) = 1 +  (ocorrenciasElemento esq x) + (ocorrenciasElemento dir x)
  | otherwise = (ocorrenciasElemento esq x) + (ocorrenciasElemento dir x)

maioresQueElemento :: Arvore -> Int -> Int
maioresQueElemento (No n Null Null) x
  | (x < n) = 1
  | otherwise = 0
maioresQueElemento (No n esq dir) x
  | (x < n) = 1 + (maioresQueElemento esq x) + (maioresQueElemento dir x)
  | otherwise = (maioresQueElemento esq x) + (maioresQueElemento dir x)

quantidade :: Arvore -> Int
quantidade (No n Null Null) = 1
quantidade (No n esq dir) = 1 + (quantidade esq) + (quantidade dir)

mediaElementos :: Arvore -> Float
mediaElementos (No n esq dir) = fromIntegral(somaElementos(No n esq dir)) / fromIntegral(quantidade(No n esq dir))

elementos :: Arvore -> [Int]
elementos (No n Null Null) = [n]
elementos (No n esq dir) = [n] ++ (elementos esq) ++ (elementos dir)

main = do 
  putStrLn (show (ocorrenciasElemento minhaArvore 55))
  putStrLn (show (maioresQueElemento minhaArvore 15))
  putStrLn (show (mediaElementos minhaArvore))
  putStrLn (show (quantidade minhaArvore))
  putStrLn (show (elementos minhaArvore))
