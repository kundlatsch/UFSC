-- 1)
SELECT nome FROM Funcionario WHERE nome LIKE 'João';

-- 2)
SELECT nome FROM Funcionario WHERE nome NOT LIKE 'João';

-- 3)
SELECT * FROM Medico WHERE nome LIKE '%Silva%';

-- 4)
SELECT * FROM Medico WHERE nome LIKE '_____%Silva%';

-- 5)
SELECT * FROM Paciente WHERE nome LIKE 'C%';

-- 6)
SELECT nome, idade, salario,
CASE
	WHEN salario < 600 THEN 'baixo'
	WHEN salario <= 1200 THEN 'mediano'
	ELSE 'alto'
END
AS faixasalarial
FROM Funcionario;

-- 7)
SELECT andar, sum(capacidade) FROM Ambulatorio 
GROUP BY andar HAVING sum(capacidade) > 100 
ORDER BY andar;

-- 8)
SELECT count(*) as nconsultas, CRM 
FROM consulta 
WHERE EXTRACT(month from data) = 8 GROUP BY CRM;

SELECT count(*), CRM 
FROM consulta 
WHERE data >= date('2015-05-01') AND data <= date('2015-05-31')
GROUP BY CRM;

-- 9)
SELECT (max(data) - min(data)) as intervalo, rg
FROM Consulta
GROUP BY rg
ORDER BY intervalo;

-- 10)
SELECT (max(data) - min(data)) as intervalo, rg
FROM Consulta
GROUP BY rg HAVING (max(data) - min(data)) > 365
ORDER BY intervalo;

-- 11)
SELECT crm, count(*)
FROM Consulta
WHERE DATE_PART('month', data) = DATE_PART('month', now()) - 1
GROUP BY crm HAVING count(*) > 2;
