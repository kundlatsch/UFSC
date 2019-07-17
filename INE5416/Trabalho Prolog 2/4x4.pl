validvalue(A) :-
    member(A, [1, 2, 3, 4]).

validate([]).
validate([H|T]) :- not(member(H,T)), validate(T).

maiorQue(A, B) :-
    validvalue(A), validvalue(B),
    not(A<B).

menorQue(A, B) :-
    validvalue(A), validvalue(B),
    not(A>B).

solve(Solution) :-
    Solution = [
        [A1, A2, A3, A4],
        [B1, B2, B3, B4],
        [C1, C2, C3, C4],
        [D1, D2, D3, D4]
    ],

    % problema4x4 Nr. 1
    % LINHA1
    menorQue(A1,A2),
    menorQue(A3,A4),

    % LINHA2
    menorQue(B1,B2),
    maiorQue(B3,B4),

    % LINHA3
    maiorQue(C1,C2),
    menorQue(C3,C4),

    % LINHA4
    maiorQue(D1,D2),
    maiorQue(D3,D4),

    % COLUNA1
    maiorQue(A1,B1),
    maiorQue(C1,D1),

    % COLUNA2
    menorQue(A2,B2),
    menorQue(C2,D2),

    % COLUNA3
    menorQue(A3,B3),
    menorQue(C3,D3),

    % COLUNA4
    maiorQue(A4,B4),
    maiorQue(C4,D4),

    % Validação de linhas
    validate([A1, A2, A3, A4]),
    validate([B1, B2, B3, B4]),
    validate([C1, C2, C3, C4]),
    validate([D1, D2, D3, D4]),

    % Validação de blocos
    validate([A1, A2, B1, B2]),
    validate([A3, A4, B3, B4]),
    validate([C1, C2, D1, D2]),
    validate([C3, C4, D3, D4]),

    % Validação de colunas
    validate([A1, B1, C1, D1]),
    validate([A2, B2, C2, D2]),
    validate([A3, B3, C3, D3]),
    validate([A4, B4, C4, D4]).
