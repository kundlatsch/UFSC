function y = fPn(n, vec, x)
    y = vec(1)*x + vec(2);
    for i = 3 : n+1
        y = y*x + vec(i);
    end
end