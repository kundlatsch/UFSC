function x = fNewtonNum(f, xi, tol)
dx=2.*tol;k=0;
    while abs(dx) > tol && k<10
    k++;
        aux = f(xi);
        dx = -aux*dx/(f(xi+dx) - aux);
        x = xi + dx;
        xi = x;
    end
end