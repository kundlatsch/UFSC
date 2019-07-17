module Pilha (Pilha (Stack), emptyStack, push, pop, top) where

data Pilha t = Stack [t]
    deriving (Eq,Show)
    
push :: Pilha t -> t -> Pilha t
push (Stack s) x = Stack (x:s)

pop :: Pilha t -> Pilha t
pop (Stack []) = error "Empty"
pop (Stack (x:s)) = Stack s

top :: Pilha t -> t
top (Stack []) = error "Empty"
top (Stack (x:s)) = x

emptyStack :: Pilha t
emptyStack = Stack []