function x = fNewtonPnM1(n, vec, xi)
    tol = 1e-14;
    dx=2.*tol;
    k=0;
    while abs(dx) > tol && k<50
        k++;
        r = fResto(vec, n, xi, n+1); % Cálcula totos os restos, via n+1 divisões sintéticas
        dx = -r(1)/r(2);
        x = xi + dx;
        % [k, r(1), r(2), dx, x]
        xi = x;
    end
end