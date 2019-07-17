function [ A B ] = crout_decomp(A,B)
    n = size(A,1);
    for k = 1 : n 
        for i = k : n
            sum = 0;
            for r = 1 : k-1
                sum += A(i,r) * A(r,k);
            end
            A(i,k) = A(i,k) - sum;
        end
    
        for j = k+1 : n
            sum = 0;
                for r = 1 : k-1
                    sum += A(k,r) * A(r,j);
                end
            A(k,j) = ( A(k,j) - sum ) / A(k,k);
        end
    end
    
end	 	  	  	  		     	   		  	 	
