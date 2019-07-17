function [a, t] = fCoefPnGregoryNewton(n, interval)
    h = (interval(2) - interval(1))/(n);
    t = interval(1) : h : interval(2);
    y = func(t);
    a(1) = y(1);
    
    for i = 1: n
        dd(i, 1) = (y(i+1) - y(i))/(t(i+1) - t(i));
    end

    for k = 2 : n
        for i = 1 : n+1-k
            dd(i, k) = (dd(i+1, k-1) - dd(i, k-1))/(t(i+k) - t(i));
        end
    end

    for k=1 : n
        a(k+1) = dd(1, k);
    end
end