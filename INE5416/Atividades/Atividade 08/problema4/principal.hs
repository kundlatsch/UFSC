import Pilha (Pilha (Stack), emptyStack, push, pop, top)

main = do putStrLn (show (push (push emptyStack 1) 2))
          putStrLn (show (top (Stack [1,2,3,4,5])))
          putStrLn (show (pop (Stack [1,2,3,4,5])))
