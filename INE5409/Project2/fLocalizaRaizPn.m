function xi = fLocalizaRaizPn(n, vec)
    r = 1 + max(abs(vec(2:n+1)))/vec(1);

    init = -r;
    final = r;

    step = 1/(10*pi);
    a = init;
    xi = [];
    while a < final
        b = a+step;
        fA = fPn(n, vec, a);
        fB = fPn(n, vec, b);
        if  fA * fB <= 0
            xi(end+1) = (a+b) * 0.5;
        end
        a = b;
    end
    
    for i = length(xi)+1 : n
        xi(i) = complex((rand()-0.5)*0.5*r, (rand()-0.5)*0.5*r);
    end
    
    xi = transpose(xi);
end