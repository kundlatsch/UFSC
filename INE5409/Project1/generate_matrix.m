function [A B] = generate_matrix(n)
    # Declaracao da matriz

    for i = 1
        # x(i)+x(i+1)=1.50;
        A(i,i) = 1;
        A(i,i+1) = 1;
        B(i,1) = 1.50;
    end

    for i = 2 : n/2
        # x(i-1)+4x(i)+x(i+1)=1.00;
        A(i,i-1) = 1;
        A(i,i) = 4;
        A(i,i+1) = 1;
        B(i,1) = 1.00;
    end

    for i = (n/2) + 1 : n - 1
        # x(i-1)+5x(i)+x(i+1)=2.00;
        A(i,i-1) = 1;
        A(i,i) = 5;
        A(i,i+1) = 1;
        B(i,1) = 2.00;
    end

    for i = n
        # x(i-1)+x(i)=3.00;
        A(i,i-1) = 1;
        A(i,i) = 1;
        B(i,1) = 3.00;
    end

end	 	  	 	      	    	      	   	    		  	 	
