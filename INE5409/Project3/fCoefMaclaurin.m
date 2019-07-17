function c = fCoefMaclaurin(n)
    aux = pi/4;
    c(1) = 1/sqrt(2);
    
    for i = 1 : n
        c(i+1) = aux^(i) / factorial(i) * c(1) * (-1)^((i*i + i)/2); 
    end
end