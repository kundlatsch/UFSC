import Data.Matrix

main = do
	let a =  matrix 4 4 $ \(i,j) -> 2*i - j
	print a