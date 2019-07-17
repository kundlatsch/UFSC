function y = fPnmPade(n, m, tp)
    a_mac = fCoefMaclaurin(n+m)
    [ap, bp] = fCalculaCoefPade(n, m, a_mac);
    for r = 1 : length(tp)
        y(r) = fPnCanonica(n, ap, tp(r))/fPnCanonica(m, bp, tp(r));
    end
    
    
end