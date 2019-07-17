function X = gauss_seidel(A, B, n, Xi, tolerance)
    X = Xi;
    stop = tolerance + 1;
    lambda = 1.1; 
    aux = 1 - lambda;
    k = 0;
    while stop > tolerance && k < 100
        Xi = X;
        for i = 1
            # -> X(i) + X(i+1) = 1.5
            # -> X1 + X2 = B(1)
            # -> X1 = B(1) - X2
            X(i) = aux * Xi(i) + lambda * (B(i) - A(i,i+1) * X(i+1)) / A(i,i);
        end
        
        for i = 2 : n/2
            # -> X(i-1) + 4X(i) + X(i+1) = 1.00;
            # -> 4X(i) = B(i) - X(i-1) - X(i+1)
            X(i) = aux * Xi(i) + lambda * (B(i) - A(i,i-1) * X(i-1) - A(i,i+1) * X(i+1)) / A(i,i);
        end
        
        for i = (n/2) + 1 : n - 1
            # x(i-1)+5x(i)+x(i+1)=2.00;
            X(i) = aux * Xi(i) + lambda * (B(i) - A(i,i-1) * X(i-1) + A(i, i+1) * X(i+1)) / A(i, i);
        end
        
        for i = n
            #x(i-1)+x(i)=3.00;
            X(i) = aux * Xi(i) + lambda * (B(i) - (A(i,i-1) * X(i-1))) / A(i, i);
        end
        stop = max(abs(X .- Xi));
        k = k + 1;
    end
    numero_de_iteracoes = k
    X = transpose(X);
end	 	  	 	      	    	      	   	    		  	 	
