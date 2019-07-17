function x = gauss_scheduling(A, B)
    [n r t d] = optimize_matrix(A, B);
	for i=2:n
		aux=t(i)/r(i-1);
		r(i)=r(i)-aux*d(i-1);
		B(i)=B(i)-aux*B(i-1);
	end
	
	x(n)=B(n)/r(n);
	for i=n-1:-1:1
		x(i)=(B(i)-d(i)*x(i+1))/r(i);
	end
	x = transpose(x);
end