function yp = fPnGregoryNewton(n, tp)

%% Pn(t) = y(1) + dy1 + (dy1)²*(t-t1)(t-t2) + ... + (dy1)^n*(t-t1)*...*(t-tj)
%%          a1     a2     a3                          an    
    
    [a, t] = fCoefPnGregoryNewton(n, [-1, 1]);
    
    for r = 1 : length(tp)
        yp(r) = a(1);
        for k = 1 : n
            aux = 1;
            for j = 1: k
                aux *= (tp(r) - t(j));
            end
            yp(r) += a(k+1) * aux; 
        end
    end
end