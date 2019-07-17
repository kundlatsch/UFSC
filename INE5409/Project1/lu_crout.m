function X = lu_crout(A,B)

    
    [A B] = crout_decomp(A,B);
    
    for j = 1 : size(B,2)
        X(:,j) = solve_x_crout(A,B(:,j));
    end
end