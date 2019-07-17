function r = fResto(a, n, xi, n_div)
    k = 0;
    r(n_div) = a(1);
    while(n > 0 && k < n_div)
        k++;

        b = [a(1)];
    
        for i = 2 : n+1
            b(i) = a(i) + xi*b(i-1);
        end
        
        r(k) = b(n+1);
    
        % Próxima divisão
        n = n - 1;
        a = b;
    end
end