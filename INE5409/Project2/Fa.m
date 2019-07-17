function  y = Fa(x)
    %% f1 = x(1)^3+x(2)^3-2 = 0
    y(1) = x(1)*x(1)*x(1) + x(2)*x(2)*x(2) - 2;
    
    %% f2 = sen(x(1))*cos(x(2))-0.45 = 0
    y(2) = sin(x(1))*cos(x(2)) - 0.45;
    
    y = transpose(y);
end