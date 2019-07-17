function [a b]=fCalculaCoefPade(npade,mpade)
    c = fCoefMaclaurin(npade+mpade);
    
%valido para n=n ou n=m+1
%calcular os b´s via sistema de eqs.
    k = npade - mpade;
    for i = 1:mpade
    	for j = 1:i
    		A(i,j)=c(k+i+j);
    		A(j,i)=A(i,j);
    	end
    	B(i)=-c(npade+i+1);
    end
%determinante=det(A)
%[A transpose(B)]
%if  (abs(A(1,1))>1e-14)aux=fLUCholesky(mpade,A,transpose(B)); %para casos onde A(1,1)!=0
%else  aux=fgausspivtotal(mpade,[A transpose(B)]); end %para casos onde A(1,1)=0 (com pivotacao total)
%rmax=fresiduos(mpade, [A transpose(B)],aux)
    aux= A \ transpose(B);
    b=fliplr(transpose(aux)); %%b começa de 1, igual no texto, e fliplr() inverte os indices do vetor 
%completandoo vetor b:
    b(mpade+1:npade)=0;
    b=[1 b]; %incluindo o 1º b, unitario
%calcular os As
    a(1)=c(1);
    for i=2:npade+1
    	S = c(i);
    	for j = 1:i-1
    		S = S + b(j+1)*c(i-j);
    	end
    	a(i) = S;
    end
    b=b(1:mpade+1);
end	 	  	 	      	     		     	 	     	     	 	
