module Main (main) where

import Formas

main = do
    print (area (Retangulo 5 2))
    print (area (Circulo 5))
    print (area (Trapezio 8 5 4))