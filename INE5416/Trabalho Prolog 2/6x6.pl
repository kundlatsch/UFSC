validvalue(A) :-
    member(A, [1, 2, 3, 4, 5, 6]).

validate([]).
validate([H|T]) :-
    not(member(H, T)),
    validate(T).

maiorQue(A, B) :-
    validvalue(A),
    validvalue(B),
    A>B.

menorQue(A, B) :-
    validvalue(A),
    validvalue(B),
    A<B.

maiorMaior(A, B, C) :-
    maiorQue(A, B),
    maiorQue(B, C).

maiorMenor(A, B, C) :-
    maiorQue(A, B),
    menorQue(B, C).

menorMaior(A, B, C) :-
    menorQue(A, B),
    maiorQue(B, C).

menorMenor(A, B, C) :-
    menorQue(A, B),
    menorQue(B, C).

menorMenorMenor(A, B, C, D, E, F) :-
    menorQue(A, B),
    menorQue(C, D),
    menorQue(E, F).

menorMenorMaior(A, B, C, D, E, F) :-
    menorQue(A, B),
    menorQue(C, D),
    maiorQue(E, F).

menorMaiorMaior(A, B, C, D, E, F) :-
    menorQue(A, B),
    maiorQue(C, D),
    maiorQue(E, F).

maiorMaiorMaior(A, B, C, D, E, F) :-
    maiorQue(A, B),
    maiorQue(C, D),
    maiorQue(E, F).

maiorMaiorMenor(A, B, C, D, E, F) :-
    maiorQue(A, B),
    maiorQue(C, D),
    menorQue(E, F).

maiorMenorMenor(A, B, C, D, E, F) :-
    maiorQue(A, B),
    menorQue(C, D),
    menorQue(E, F).

maiorMenorMaior(A, B, C, D, E, F) :-
    maiorQue(A, B),
    menorQue(C, D),
    maiorQue(E, F).

menorMaiorMenor(A, B, C, D, E, F) :-
    menorQue(A, B),
    maiorQue(C, D),
    menorQue(E, F).

solve(Solution) :-
    Solution=[[A1, A2, A3, A4, A5, A6], [B1, B2, B3, B4, B5, B6], [C1, C2, C3, C4, C5, C6], [D1, D2, D3, D4, D5, D6], [E1, E2, E3, E4, E5, E6], [F1, F2, F3, F4, F5, F6]],

    % problema 6X6 Nr. 4
    % LINHA1
    menorMaiorMaior(A1,
                    A2,
                    A3,
                    A4,
                    A5,
                    A6),
    validate([A1, A2, A3, A4, A5, A6]),

    % COLUNA1
    menorMaior(A1, B1, C1),
    menorMaior(D1, E1, F1),
    validate([A1, B1, C1, D1, E1, F1]),

    % LINHA2
    maiorMaiorMaior(B1,
                    B2,
                    B3,
                    B4,
                    B5,
                    B6),
    validate(
             [ B1,
               B2,
               B3,
               B4,
               B5,
               B6
             ]),

    % COLUNA2
    maiorMaior(A2, B2, C2),
    menorMaior(D2, E2, F2),
    validate(
             [ A2,
               B2,
               C2,
               D2,
               E2,
               F2
             ]),

    % LINHA3
    menorMenorMenor(C1,
                    C2,
                    C3,
                    C4,
                    C5,
                    C6),
    validate(
             [ C1,
               C2,
               C3,
               C4,
               C5,
               C6
             ]),

    % COLUNA3
    menorMaior(A3, B3, C3),
    maiorMenor(D3, E3, F3),

    % LINHA4
    menorMenorMenor(D1,
                    D2,
                    D3,
                    D4,
                    D5,
                    D6),
    validate(
             [ D1,
               D2,
               D3,
               D4,
               D5,
               D6
             ]),

    % COLUNA4
    menorMenor(A4, B4, C4),
    maiorMenor(D4, E4, F4),
    validate(
             [ A4,
               B4,
               C4,
               D4,
               E4,
               F4
             ]),

    % linha5
    maiorMenorMenor(E1,
                    E2,
                    E3,
                    E4,
                    E5,
                    E6),
    validate(
             [ E1,
               E2,
               E3,
               E4,
               E5,
               E6
             ]),

    % coluna5
    menorMenor(A5, B5, C5),
    menorMenor(D5, E5, F5),
    validate(
             [ A5,
               B5,
               C5,
               D5,
               E5,
               F5
             ]),

    % linha6
    maiorMenorMaior(F1,
                    F2,
                    F3,
                    F4,
                    F5,
                    F6),
    validate(
             [ F1,
               F2,
               F3,
               F4,
               F5,
               F6
             ]),

    % coluna6
    maiorMenor(A6, B6, C6),
    maiorMaior(D6, E6, F6),
    validate(
             [ A6,
               B6,
               C6,
               D6,
               E6,
               F6
             ]),

    % verifica se não há elementos repetidos nas grades
    validate([A1, A2, B1, B2, C1, C2]),
    validate([A3, A4, B3, B4, C3, C4]),
    validate([A5, A6, B5, B6, C5, C6]),
    validate(
             [ D1,
               D2,
               E1,
               E2,
               F1,
               F2
             ]),
    validate(
             [ D3,
               D4,
               E3,
               E4,
               F3,
               F4
             ]),
    validate(
             [ D5,
               D6,
               E5,
               E6,
               F5,
               F6
             ]).
