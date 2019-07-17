function [n r t d] = optimize_matrix(A, B)
    n = size(A,1);
    
    for i = 1
        t(i) = NaN;
        r(i) = A(i,i);
        d(i) = A(i,i+1);
    end
    
    for i = 2 : n-1
        t(i) = A(i,i-1);
        r(i) = A(i,i);
        d(i) = A(i,i+1);
    end
    
    for i = n
        t(i) = A(i,i-1);
        r(i) = A(i,i);
        d(i) = NaN;
    end
    
end