module Pontos (Ponto (Ponto2D), distancia, colineares, formaTriangulo) where

data Ponto = Ponto2D Float Float
            deriving Show

distancia :: Ponto -> Ponto -> Float
distancia (Ponto2D a1 b1) (Ponto2D a2 b2) = ((((a2 - a1) ** 2) + ((b2 - b1) ** 2)) ** 1/2)

colineares :: Ponto -> Ponto -> Ponto -> Bool
colineares (Ponto2D a1 b1) (Ponto2D a2 b2) (Ponto2D a3 b3)  | ((a1 * b2) + (b1 * a3) + (a2 * b3) + (-(a3 * b2) - (b3 * a1) - (a2 * b1))) == 0 = True
                                                            | otherwise = False

formaTriangulo :: Ponto -> Ponto -> Ponto -> Bool
formaTriangulo (Ponto2D a1 b1) (Ponto2D a2 b2) (Ponto2D a3 b3) = not (colineares (Ponto2D a1 b1) (Ponto2D a2 b2) (Ponto2D a3 b3))