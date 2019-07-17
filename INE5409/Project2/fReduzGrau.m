function [n, vec] = fReduzGrau(n, vec, x)

    b = [vec(1)];

    for i = 2 : n+1
        b(i) = vec(i) + x*b(i-1);
    end
    
    vec = b(1:n);
    n = n-1;
end