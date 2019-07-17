data Ponto = Ponto2D Float Float | Ponto3D Float Float Float

distancia :: Ponto -> Ponto -> Float
distancia (Ponto2D a1 b1) (Ponto2D a2 b2) = (((a2 - a1) ** 2) + ((b2 - b1) ** 2)) ** 1/2
distancia (Ponto3D a1 b1 c1) (Ponto3D a2 b2 c2) = (((a2 - a1) ** 2) + ((b2 - b1) ** 2) + ((c2 - c1) ** 2)) ** 1/2

main = do 
    putStrLn (show ( distancia (Ponto2D 2 4) (Ponto2D 3 4) ) )

    putStrLn (show ( distancia (Ponto3D 2 4 4) (Ponto3D 3 4 10) ) )