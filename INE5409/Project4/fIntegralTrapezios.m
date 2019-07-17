function Tn = fIntegralTrapezios(n, a, b, f)
    h = (b-a)/n;
    x = a:h:b;
    y = f(x);
    
    Tn = 0.5*h*(y(1) + y(n+1) + 2*sum(y(2:n)));


end