% Função original
f_orig = @(x) cos(x);
%% Intervalo original
a = 0;
b = pi/2;


% Novo intervalo / normalizado [-1, +1]
% x(t) = (b - a)t/2 + (b + a)/2
% Nova função: f(x(t)) = cos(t/4*pi + pi/4) 

% Calculando pontos que serão usados para o plot/interpolações
tp = -1:0.01:1;
aux = pi/4;
xp = aux.*tp .+ aux;
ye = f_orig(xp);


printf("Interpolador Gregory-Newton\n");
% Grau
n = 2

yp = fPnGregoryNewton(n, tp);
erro_max_GN = max(abs(ye - yp));
printf("Erro máximo: %f\n", erro_max_GN);

printf("\n------------------\n");
printf("Interpolação por Spline Cúbica (quadrática nos extremos)\n");
m = 4
% Gerar intervalos
hi = (b-a)/m;
x_amostrais = a:hi:b
y_amostrais = f_orig(x_amostrais);

[a, b, c, d] = fSpline(x_amostrais, y_amostrais, hi, m);

i = 1;
for k=1:length(x_amostrais)
    if (x_amostrais(k) > i*hi)
        i++
    end
    y_splines(k)=a(i)*(xp(k)-x_amostrais(i))*(xp(k)-x_amostrais(i))*(xp(k)-x_amostrais(i))+b(i)*(xp(k)-x_amostrais(i))*(xp(k)-x_amostrais(i))+c(i)*(xp(k)-x_amostrais(i))+d(i);
end
length(y_splines)
length(x_amostrais)

printf("\n------------------\n");
printf("Aproximação por Maclaurin\n");
n = 3

c = fCoefMaclaurin(n)
%cW=[ 1/sqrt(2), -pi/(4*sqrt(2)), -pi^2/(32*sqrt(2)), pi^3/(384*sqrt(2)), pi^4/(6144*sqrt(2)), -pi^5/(122880*sqrt(2)) ] %Wolfran
y_mac = fPnCanonica(n, c, tp);
erro_max_MC = max(abs(y_mac - ye));
printf("Erro máximo: %f\n", erro_max_MC);

printf("\n------------------\n");
printf("Aproximação de Padé\n");
n = 3
m = 2
ypade = fPnmPade(n, m, tp);
erro_max_PD = max(abs(ypade - ye));
printf("Erro máximo: %f\n", erro_max_PD);


printf("\n------------------\n");
printf("Aproximação de Tchebychev\n");
  % Intervalo original
    a = 0;
    b = pi/2;
    n = 25;
    b = fCoefTchebyschev(n, a, b);
    yTchebyschev = fTchebyschev(n,b, tp);
    erro_max_Tb = max(abs(yTchebyschev - ye));
    printf("Erro máximo: %f\n", erro_max_Tb);

plot(xp, yp,"b; Interpolador Polinomial;",
     xp, ye, "r;Funcao Original;",
     xp, ypade, "g; Aproximador de Padé;",
     xp, y_mac, "m; Aproximador de Maclaurin;", 
     xp, y_splines, "k; Splines Cúbicas",
     xp, yTchebyschev, "y; Tchebyschev",
     0,2);
