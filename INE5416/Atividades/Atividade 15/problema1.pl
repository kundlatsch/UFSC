genitor(pam, bob).
genitor(tom, bob).
genitor(tom, liz).

genitor(bob, ana).
genitor(bob, pat).

genitor(liz,bill).

genitor(pat, jim).

mulher(pam).
mulher(liz).
mulher(pat).
mulher(ana).
homem(tom).
homem(bob).
homem(jim).
homem(bill).

pai(X,Y) :- genitor(X,Y), homem(X).
mae(X,Y) :- genitor(X,Y), mulher(X).

avo(AvoX, X) :- genitor(GenitorX, X), genitor(AvoX, GenitorX), homem(AvoX).
avoh(AvohX, X) :- genitor(GenitorX, X), genitor(AvohX, GenitorX), mulher(AvohX).
irmao(X,Y) :- genitor(PaiAmbos, X), genitor(PaiAmbos, Y), X \== Y, homem(X).
irma(X,Y) :- genitor(PaiAmbos, X), genitor(PaiAmbos, Y), X \== Y, mulher(X).
irmaos(X,Y) :- (irmao(X,Y); irma(X,Y)), X \== Y.

ascendente(X,Y) :- genitor(X,Y). %recursão - caso base
ascendente(X,Y) :- genitor(X, Z), ascendente(Z, Y). %recursão - passo recursivo

% Questão 1

tio(X, Y) :- genitor(GenitorY, Y), irmao(X, GenitorY). 
	% ?- tio(bob, bill) = true

tia(X, Y) :- genitor(GenitorY, Y), irma(X, GenitorY). 
	% ?- tia(liz, ana) = true

primo(X, Y) :- 	genitor(GenitorY, Y), genitor(GenitorX, X), 
				irmaos(GenitorX, GenitorY), homem(X). 
	% ?- primo(bill, ana) = true

prima(X, Y) :- 	genitor(GenitorY, Y), genitor(GenitorX, X), 
				irmaos(GenitorX, GenitorY), mulher(X). 
	% ?- prima(pat, bill) = true

prima(X, Y) :- 	genitor(GenitorY, Y), genitor(GenitorX, X), 
				irmaos(GenitorX, GenitorY). 
	% ?- prima(ana, bill) = true

bisavo(X, Y) :- avo(X, GenitorY), genitor(GenitorY, Y).
	% ?- avo(tom, jim) = true

bisavoh(X, Y) :- avoh(X, GenitorY), genitor(GenitorY, Y).
	% ?- avo(pam, jim) = true

descendente(X,Y) :- genitor(Y,X).
descendente(X,Y) :- genitor(Z,X), descendente(Y,Z).
	% ?- descendente(ana,bob) = true, descendente(bob,ana) = false

feliz(X) :- genitor(X, Y), filho(Y,X).
filho(X, Y) :- genitor(Y, X).
	%? - feliz(bob) = true, feliz(jim) = false

% Método para verificar se X é meio irmão de Y, ou seja,
% se X e Y possuem apenas um genitor em comum.
meioIrmao(X, Y) :- 	genitor(GenitorAmbos, X), genitor(GenitorAmbos, Y),
					genitor(GenitorX, X), genitor(GenitorY, Y), 
					GenitorX \== GenitorY, X \== Y.
	% ?- meioIrmao(bob,liz) = true