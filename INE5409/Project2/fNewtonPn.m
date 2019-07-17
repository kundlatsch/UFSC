function [x, m] = fNewtonPn(n, vec, xi)
    tol = 1e-14;
    dx=2.*tol;
    k=0;
    while abs(dx) > tol && k<50
        k++;
        r = fResto(vec, n, xi, n+1); % Cálcula totos os restos, via n+1 divisões sintéticas
        m = fMultiplicidade(r);
        % dx = -r(1)/r(2); 
        dx = -r(m)/(m*r(m+1));
        x = xi + dx;
        % [k, r(1), r(2), dx, x]
        xi = x;
        
    end
end