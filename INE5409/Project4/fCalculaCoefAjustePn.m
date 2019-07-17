function a = fCalculaCoefAjustePn(x, y, n);
    
    m = length(x);
    
    for i = 1 : n + 1
        for j = 1 : n + 1
            soma = 0;
            
            for k = 1 : m
                soma += x(k)**(i+j-2);
            end
            A(i,j) = soma;
        end
    end

    for i = 1 : n + 1
        soma = 0;
        for k = 1 : m
            soma += x(k)**(i-1)*y(k);
        end
        B(i) = soma;
    end
    B = transpose(B);
    a = A\B;
end