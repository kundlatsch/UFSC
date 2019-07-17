function Va = erfx2(x)
    
    f = @(z) exp(-z .* z);
    a = 0; b = x;
    Tn =  fIntegralTrapezios(2**18, a, b, f);
    m = 8;
    Gm = fIntegralGauss(m, a, b, f);
    Sn = fIntegralSimpson(2**11, a, b, f);
    Va(1) = (2/sqrt(pi))*Tn;
    Va(2) = (2/sqrt(pi))*Gm;
    Va(3) = (2/sqrt(pi))*Sn;


end