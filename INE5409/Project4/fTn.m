function Tn=fTn(n,a,b)
  h=(b-a)/n;
  t=a:h:b;
  y=f1(t);
  soma=0;
  for i=2:n
    soma=soma+y(i);
  end %for i
  Tn=0.5*h*(y(1)+2*soma+y(n+1));
end %function