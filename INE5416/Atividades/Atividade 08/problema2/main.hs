import Pontos

main = do
    print $ distancia  (Ponto2D 5 2) (Ponto2D 5 3)
    print $ colineares (Ponto2D 5 2) (Ponto2D 5 3) (Ponto2D 5 4)
    print $ formaTriangulo (Ponto2D 4 2) (Ponto2D 5 3) (Ponto2D 5 4)