format long

x = [1.0 2.1  3.0 3.9 5.0 6.0  7.2 8.1  9.2 10.4];
% y é a função medida
y = [1.5 0.1 -0.4 0.2 1.7 2.2 1.6  0.4 -0.4  0.5];
% pontos para plotar
xp = min(x):0.1:max(x);


printf('-------\nAjuste Polinomial\n')
n = 7

% ajuste polinomial ao menor desvio quadrático
a = fCalculaCoefAjustePn(x, y, n);
%a = polyfit(x, y, n)

%funcao pra calcular o grafico do metodo
y_polinomial = fPnCanonica(n, a, xp);

DesvioModulo = sum(abs(fPnCanonica(n, a, x) .- y))/length(x)


printf('-------\nAjuste para g(x) = a(1)*sen(x) + a(2)*cos(x)\n')

a2 = fCalculaCoefAjustegx1(x, y);
y_gx = fgx1(a2, xp);

DesvioModulo = sum(abs(fgx1(a2, x).-y))/length(x)


printf('-------\nAjuste metodo de Newton\n')
a3 = [2 1 1 1.2 1 1 1];
aNewton = fNewtonSistemasNaoLineares(a3, 0.001, x, y);
y_newton = fgx2(aNewton, xp);

DesvioModulo = sum(abs(fgx2(aNewton, x).-y))/length(x)

%DesvioModulo = sum(abs(y_newton .-y))/length(x)

printf('-------\nerf(x) pelos metodos dos Trapezios, Gauss e Simpson:\n')
xi = 1;

Ve = erf(xi)
Va = erfx2(xi)
erros = abs(Ve .- Va)



plot(xp, y_polinomial, "-r", xp, y_gx, "-g", xp, y_newton, "-b", x, y, '*', 16, 0)

legend('Ajuste Polinomial                  ','Ajuste com seno e cosseno              ','Ajuste metodo de Newton                 ', 'Pontos Experimentais                     ');

