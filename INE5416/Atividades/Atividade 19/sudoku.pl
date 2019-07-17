tabuleiro([
            [_, _, _, 4], 
            [_, 3, _, _], 
            [1, _, _, _], 
            [_, _, 2, _] 
          ]).

n(1).
n(2).
n(3).
n(4).

todosDiferentes([]).
todosDiferentes([H|T]) :- not(member(H,T)), todosDiferentes(T).

%Tento atribuir valores faltantes e verificar se todos são diferentes
completa([X1, X2, X3, X4]) :-
    n(X1), n(X2), n(X3), n(X4),
    todosDiferentes([X1, X2, X3, X4]).
    
solucao(TabuleiroSolucao) :-
    TabuleiroSolucao = tabuleiro([
            [X11, X12, X13, X14], 
            [X21, X22, X23, X24], 
            [X31, X32, X33, X34], 
            [X41, X42, X43, X44] 
          ]),
          
    %linhas
    completa([X11, X12, X13, X14]),
    completa([X21, X22, X23, X24]),
    completa([X31, X32, X33, X34]),
    completa([X41, X42, X43, X44]),
    
    %colunas
    completa([X11, X21, X31, X41]),
    completa([X12, X22, X32, X42]),
    completa([X13, X23, X33, X43]),
    completa([X14, X24, X34, X44]),
    
    %quadrados menores
    completa([X11, X12, X21, X22]),
    completa([X31, X32, X41, X42]),
    completa([X13, X14, X23, X24]),
    completa([X33, X34, X43, X44]).
