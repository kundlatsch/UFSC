function YTC=fTchebyschev(n,b,t)
    nt=length(t);
%Serie de Tchebychev em função dos polinomios de Tchebychev Ti(x):
    i=1;%T1=t
    
    for k=1:nt
        T(i,k)=t(k); %T1=t
    end
    
    i=2;%T2=(2t^2-1) 
    
    for k=1:nt
        T(i,k)=2*t(k)^2-1; %T2=2t^2-1 
    end
    
%de T3=(4t^3-3t) em diante poderemos usar a recorrencia p/ i=3:n:
%T(i)=2 t T(i-1)-T(i-2)

    for i=3:n %calcula até o grau n desejado
      for k=1:nt
         T(i,k)=2*t(k)*T(i-1,k)-T(i-2,k); %T(i)=2xT(i-1)-T(i-2) 
       end
    end
    
    for k=1:nt
      YTC(k)=b(1).*1;
      for i=1:n
        YTC(k)=YTC(k).+b(i+1).*T(i,k);
      end
    end
end 	 	  	 	      	    	      	   	    		  	 	
