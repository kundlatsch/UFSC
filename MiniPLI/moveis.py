# Questão 3 - Fabricante de Móveis
from pymprog import model

lucro = [5,4,3]
horas = [[4, 1, 2],[3, 4, 2]]
limiteHoras = [110, 80]

tabuas = [2, 3, 1]
limiteTabuas = 55

q3 = model('basic')  

x = q3.var('x', 3, int) 
# função objetivo
q3.maximize(sum(lucro[i]*x[i] for i in range(3)))

# horas de estofamento e acabamento
for i in range(2):
  sum(horas[i][j]*x[j] for j in range(3)) <= limiteHoras[i] 

# tabuas
sum(tabuas[j]*x[j] for j in range(3)) <= limiteTabuas 
 

  
q3.solve()

print("###>Objective value: %f"%q3.vobj())

labels = ["sofás", "mesas", "cadeiras"]
for i in range(3):
  print("Qtd. de "+str(labels[i])+": "+str(x[i].primal))

q3.end()
