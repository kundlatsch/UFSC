-- 2) 
--    Polimorfismo ad-hoc de coerção acontece 
--    quando o tipo é convertido implicitamente:

soma :: Float -> Float -> Float
soma x y = x + y + 1
main = do
print (soma 21 10)