function X = fNewtonSistemasNaoLineares(a, tol, x , y)
    k = 0;
    n = size(a,1);
    Dx(1:n) = 5*tol;
    lambda = 0.01;
    while max(abs(Dx)) > tol && k < 4000
        k += 1;
        %matrix jacob
        for j = 1:n
            Xnovo = a;
            Xnovo(j) = Xnovo(j) + Dx(j);
            for i = 1:n
               A(i,j) = ( fFa(Xnovo, x, y)(i) - fFa(a, x , y)(i) )/Dx(j);
            end
        end
        A;
        B = -transpose(fFa(a, x, y));
        Dx = A\B;
        X = a + Dx * lambda;
        a = X;
    end
    DxMaximo = max(abs(Dx));
    k;
end
	 	  	     	    	      			    	  	 	
