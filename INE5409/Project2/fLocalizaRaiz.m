function Xi=fLocalizaRaiz(f,init,final)
    step = 0.1;
    fLimit = 5*step; 
    a = init;
    while a < final
        b = a+step;
        fA = f(a);
        fB = f(b);
        if  fA * fB < 0 && fA < fLimit && fB < fLimit
            Xi(end+1) = (a+b) * 0.5;
        end
        a = b;
    
    end
end