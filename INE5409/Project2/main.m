% 1) Determine todos os zeros REAIS da função f(x) = x*x*x-exp(x) e seus resíduos 
%    Lembre-se de verificar se existem pontos de descontinuidades;

f = @(x) x .* x .* x - exp(x);
init=-100;
final=100;

raizes_iniciais = fLocalizaRaiz(f,init,final)

for k=1:length(raizes_iniciais)
    X(k)=fNewtonNum(f, raizes_iniciais(k), 1e-15);
    residuosf(k)=abs(f(X(k)));
end
raizes_newton = X
residuos_1 = residuosf

% 2) Determine todas as raízes e respectivas multiplicidades do polinômio de
%    grau 10 com os seguintes coeficientes em ordem decrescente de grau

a=[+1 -7  +20.95 -34.75 +34.5004 -20.5012 +6.7512 -0.9504  0  0  0];

raizesPnMultiplicidade1 = fLocalizaRaizesPnM1(a)

[x m] = fRaizesPn(a);

raizesPn = x
multiplicidadePn = m

% 3) - uma solução  pelo método de Newton com derivadas numéricas, para 'n'
%      eqs. genéricas (trabalhe com as funções na forma vetorial);
%    - o resíduo máximo das equações do sistema de  equações não lineares

xi = [1; 1];

n = length(xi);

raizesDoSistemaFa = fNewtonFa(n, xi)


 