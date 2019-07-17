no(52, 
    no(32,
        no(12,nil,nil),
        no(35,nil,nil)), 
    no(56,
        no(55,
            no(55,nil,nil),
            nil),
        no(64,nil,nil))).
        
soma(nil, 0).
soma(no(N, ArvE, ArvD),Soma) :-
    soma(ArvE,SomaEsq),
    soma(ArvD,SomaDir),
    Soma is N + SomaEsq + SomaDir.
    
membro(N, no(N, _, _)) :- !.
membro(N, no(_, ArvE, ArvD)) :-
    membro(N, ArvE), !;
    membro(N, ArvD).
    
min(X,Y,X) :- X =< Y, !.
min(X,Y,Y).

minimo(nil,1000). %INF = 1000
minimo(no(N, ArvE, ArvD),Min) :-
    minimo(ArvE, MinE),
    minimo(ArvD, MinD),
    min(MinE, MinD, MinED),
    min(N, MinED, Min).
    
caminho(Y, no(Y, _, _), [Y]) :- !.
caminho(Y, no(K, ArvE, ArvD), [K|Caminho]) :-
    caminho(Y, ArvE, Caminho), !;
    caminho(Y, ArvD, Caminho).