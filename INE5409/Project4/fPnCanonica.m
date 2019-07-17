function y = fPnCanonica(n, a, x)
%Pn(x)= a(1)+a(2)*x+a(3)*x^2+...+a(n+1)*x^n
    for r = 1 : length(x)
        y(r) = a(n+1)*x(r) + a(n ) ;
        for i = n-1:-1:1
            y(r) = y(r)*x(r) + a(i);
        end
    end
    
end