function x = fLocalizaRaizesPnM1(vec)
    n = length(vec) - 1;
    vec(2:n+1) = vec(2:n+1)/vec(1); %Nomalizaçaõ do 1º coeficiente
    vec(1) = 1;
    xi = fLocalizaRaizPn(n, vec);
    
    for i=1 : length(xi)
        x(end+1) = fNewtonPnM1(n, vec, xi(i));
    end
    x = transpose(x);
end