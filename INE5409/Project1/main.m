format long
n = 50;
# a) armazenar o sistema em forma de matriz completa 
[A B] = generate_matrix(n);

# a1) resolver o sistema por LU Crout
lu_crout = lu_crout(A,B);

# a2) imprimir a primeira e a ultima incognita
primeira_incognita_lu_crout = lu_crout(1)
ultima_incognita_lu_crout = lu_crout(n)
residuo_lu_crout = max(abs(A*lu_crout - B))

# a3) número total teórico de operações em ponto flutuante
operacoes = (4*n^3+15*n^2-7*n-6)/6

# b) Guardar a matriz na forma otimizada: isso é feito dentro da função
#    gauss_scheduling, por optimize_matrix

# b1) resolver o sistema por gauss otimizado
gauss_scheduling = gauss_scheduling(A, B);

# b2) imprimir a primeira e a ultima incognita
primeira_incognita_gauss_otimizado = gauss_scheduling(1)
ultima_incognita_gauss_otimizado = gauss_scheduling(n)
residuo_gauss_otimizado = max(abs(A*gauss_scheduling - B))

# b3) número total teórico de operações em ponto flutuante
operacoes = (4*n^3+9*n^2-n-6)/6

# c) resolver o sistema por gauss seidel

# c1) analise da diagonal dominante

converge = 'sim';
for i=1:n
    k = abs(A(i,i));
    sum = 0;
    for j=1:n
       if i~=j
            sum = sum + abs(A(i,j));
       end
    end
    if k < sum
      converge = 'nao';
    end
end
converge

# c2) fator de relaxamento escolhido: 1.1

# c3) solução do sistema
Xi(1:n) = 0;
tolerance = 10e-4;
gauss_seidel = gauss_seidel(A, B, n, Xi, tolerance);

primeira_incognita_gauss_seidel = gauss_seidel(1)
ultima_incognita_gauss_seidel = gauss_seidel(n)
residuo_gauss_seidel = max(abs(A*gauss_seidel - B))

# c4) número total de operações em ponto flutuante = 4752

# c5) erro de truncamento máximo
Xi(1:n) = 0;
tolerance = 10e-8;
# max_trunc = gauss_seidel(A, B, n, Xi, tolerance);
# erro_de_truncamento_max = max(gauss_seidel .- max_trunc)	 	  	 	      	    	      	   	    		  	 	
