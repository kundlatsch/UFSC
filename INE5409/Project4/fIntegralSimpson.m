function Sn = fIntegralSimpson(n, a, b, f)
    if (mod(n,2) != 0)
        n++;
    end
    h = (b-a)/n;
    x = a:h:b;
    y = f(x);
    
    Sn = (h/3)*(y(1) + y(n+1) + 4*sum(y(2:2:n)) + 2*sum(y(3:2:n-1)));


end