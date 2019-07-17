function [x m]= fRaizesPn(vec)
    n = length(vec) - 1;
    vec(2:n+1) = vec(2:n+1)/vec(1); %Nomalizaçaõ do 1º coeficiente
    vec(1) = 1;
    xi = fLocalizaRaizPn(n, vec);
    
    k= 0;
    while n > 0
        k++;
        [x(k), m(k)] = fNewtonPn(n, vec, xi(k));
        for j = 1 : m(k)
            [n, vec] = fReduzGrau(n, vec, x(k));
        end
    end
    x = transpose(x);
    m = transpose(m);
end