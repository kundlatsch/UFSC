function x=fCholesky(n,a,b) 
% Algoritmo de Cholesky
%Matriz de coeficientes simÃ©trica, positiva definida, de ordem n x n
	k=1;
	L(1,1)=sqrt(a(1,1));
	for i=2:n
		L(i,1)=a(i,1)/L(1,1);
	endfor
	for k=2:n-1
		soma=0;
		for r=1:k-1
			soma=soma+L(k,r)^2;
		endfor %r
		L(k,k)=sqrt(a(k,k)-soma);
		for i=k+1:n
			soma=0;
			for r=1:k-1
				soma=soma+L(i,r)*L(k,r);
			endfor %r
			L(i,k)=(a(i,k)-soma)/L(k,k);
		endfor %i
	endfor %k
	k=n;
		soma=0;
		for r=1:k-1
			soma=soma+L(k,r)^2;
		endfor %r
		L(k,k)=sqrt(a(k,k)-soma);
% Resolvendo o sistema para o termo independente b
%L.C=b
c(1)=b(1)/L(1,1);
for i=2:n
soma=0;
	for j=1:i-1
	soma=soma+L(i,j)*c(j);
	end
    c(i)=(b(i)-soma)/L(i,i);
end
%U*X=C ->LT*X=C
x(n)=c(n)/L(n,n);
for i=n-1:-1:1
soma=0;
	for j=i+1:n
	soma=soma+L(j,i)*x(j);%Note o uso da matriz U=transposta de L
	end
    x(i)=(c(i)-soma)/L(i,i);
end
end	 	  	  	  		     	   		  	 	
