function X = solve_x_crout(A,B)
   n = size(A, 1);
   
   % LU*x = B
   
   % L*C = B
   C(1) = B(1)/A(1,1);
   for i=2: n
     somatorio = sum(A(i,1:i-1).*C(1:i-1));
     C(i) = (B(i) - somatorio) / A(i,i);
   end
   
   % U*x = C
   X(n) = C(n);
   for i=n-1:-1:1
     somatorio = sum(A(i,i+1:n).*X(i+1:n));
     X(i) = C(i) - somatorio;
   end
   
end