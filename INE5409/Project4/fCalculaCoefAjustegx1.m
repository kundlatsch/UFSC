function a = fCalculaCoefAjustegx1(x, y)
    A(1, 1) = sum(sin(x).*sin(x));
    A(1, 2) = sum(sin(x).*cos(x));
    A(2, 1) = A(1, 2);
    A(2, 2) = sum(cos(x).*cos(x));
    
    B(1) = sum(y.*sin(x));
    B(2) = sum(y.*cos(x));
    
    a = A\transpose(B);
end