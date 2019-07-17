function [a b c d] = fSpline(x, y, hi, m)
%   A * S = B
%   Construindo A, para splines quadráticas nos extremos
%   First line
    A(1, 1) = 5*hi;
    A(1, 2) = hi;
    for i = 2: m-2
        A(i, i-1) = hi;
        A(i, i) = 4*hi
        A(i, i+1) = hi;
    end
%   Last line
    A(m-1, m-1) = 5*hi;
    A(m-1, m-2) = hi;
    
%   Construindo B
    for i=1:m-1
        B(i, 1) = 6*((y(i+2) - y(i+1))/hi - (y(i+1) - y(i))/hi);
    end
    
    S = B \ A;
    
%   Extreme elements
    S = [S(1), S, S(end)];
    
    for i=1:m
    	a(i)=(S(i+1)-S(i))/(6*hi);
    	b(i)= S(i)/2;
    	c(i)=(y(i+1)-y(i))/hi-(S(i+1)+2*S(i))*hi/6;
    	d(i)= y(i);
    end
end	 	  	 	      	     		     	 	     	     	 	
