function c=fCoefTchebyschev(n,a,b)
m=1e2; %m 'nós' de Tchebyschev
k=1:m; t=cos((2.*k.-1).*pi./(2.*m)); 

i=0;%T0=1
%integrais de Gauss-Tchebyshev:
  soma=0;
  for k=1:m
     soma=soma+func(t(k))*(1);
  end
  %c(1)=(1/pi)*(pi/m*soma);
  c(i+1)=1/m*soma;

i=1;%T1=t
  soma=0;
  for k=1:m
     T(i,k)=t(k); %T1=t
     soma=soma+func(t(k))*T(i,k);
  end
  %c(2)=(2/pi)*(pi/m*soma);
  c(i+1)=2/m*soma;

i=2;%T2=(2t^2-1) 
  soma=0;
  for k=1:m
     T(i,k)=2*t(k)^2-1; %T2=2t^2-1 
     soma=soma+func(t(k))*T(i,k);
  end
  c(i+1)=2/m*soma;

%de T3=(4t^3-3t) em diante poderemos usar a recorrencia p/ i=3:n:
%T(i)=2*t*T(i-1)-T(i-2)
for i=3:n
  soma=0;
  for k=1:m
     T(i,k)=2*t(k)*T(i-1,k)-T(i-2,k); %T(i)=2xT(i-1)-T(i-2) 
     soma=soma+func(t(k))*T(i,k);
  end
  c(i+1)=2/m*soma;
end
%retorna os n+1 coeficientes do aproximandor de grau n
end	 	  	 	      	    	      	   	    		  	 	
