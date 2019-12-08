-- 1)
SELECT p.*
FROM Paciente p, Consulta c
WHERE p.rg = c.rg AND c.coddoenca IN 
	(SELECT codDoenca FROM Doenca WHERE UPPER(descricao) = 'SARAMPO');

-- 2)
SELECT *
FROM Medico
WHERE idade > 40 AND codesp IN
	(SELECT codesp FROM Especialidade WHERE UPPER(nome) = 'ORTOPEDIA');

-- 3)
SELECT *
FROM Consulta
WHERE crm != 49 AND crm != 79;

-- 4)

SELECT *
FROM Ambulatorio
WHERE andar = 4 AND (capacidade = 50 OR numeroa > 10);

-- Query para verificar se a anterior funcionou
SELECT * FROM Ambulatorio ORDER BY andar;

-- 5)
SELECT m.nome, e.nome AS especialidade
FROM Medico m, Especialidade e;

-- 6)
SELECT numeroa AS numero
FROM ambulatorio
WHERE andar = 3;

-- 7)
SELECT crm, data
FROM Consulta
WHERE rg = 122 OR rg = 725;

-- 8)
SELECT numeroa
FROM Ambulatorio
WHERE andar != 2 AND andar != 4 AND capacidade > 50;

-- 9)
SELECT m.nome as medico, c.data AS dataConsulta
FROM Medico m, Consulta c
WHERE m.crm = c.crm
ORDER BY m.nome;

-- 10)
SELECT a.numeroa, a.capacidade, m.nome AS medico
FROM Ambulatorio a, Medico m
WHERE a.andar = 5 AND a.numeroa = m.numeroa;

-- 11)
SELECT m.nome AS medico, p.nome AS paciente, c.data AS dataconsulta
FROM Medico m, Paciente p, Consulta c
WHERE c.crm = m.crm AND c.rg = p.rg
ORDER BY m.nome;

-- 12)
SELECT m.nome AS medico, p.nome AS paciente
FROM Medico m, Paciente p, Consulta c
WHERE c.crm = m.crm AND c.rg = p.rg 
AND c.hora >= 7 AND c.hora <= 12 AND c.data = '2003-04-15' 
AND m.codesp IN 
(SELECT codesp FROM Especialidade WHERE UPPER(nome) = 'ORTOPEDIA');

-- 13)
SELECT distinct p.nome, c.crm
FROM Paciente p, Consulta c, Medico m
WHERE p.rg = c.rg AND c.coddoenca IN 
	(SELECT codDoenca FROM Doenca WHERE UPPER(descricao) = 'PNEUMONIA')
AND c.crm IN
	(SELECT crm FROM Medico WHERE UPPER(nome) = 'JOÃO CARLOS SANTOS' OR UPPER(nome) = 'MARIA SOUZA');

-- 14)
(SELECT nome FROM Paciente) UNION (SELECT nome FROM Medico);

-- 15)
(SELECT nome, idade FROM Paciente WHERE codcidade IN
	(SELECT codcidade FROM Cidade WHERE UPPER(descricao) = 'FLORIANOPOLIS')) UNION 
(SELECT nome, idade FROM Medico WHERE codcidade IN 
	(SELECT codcidade FROM Cidade WHERE UPPER(descricao) = 'FLORIANOPOLIS')) UNION 
(SELECT nome, idade FROM Funcionario WHERE codcidade IN 
	(SELECT codcidade FROM Cidade WHERE UPPER(descricao) = 'FLORIANOPOLIS'))

-- 16)
SELECT distinct f.nome, f.rg
FROM Funcionario f, Paciente p
WHERE f.rg != p.rg AND f.salario < 300;

-- 17)
SELECT distinct numeroa
FROM Ambulatorio
WHERE numeroa NOT IN (SELECT numeroa FROM Medico);

-- 18)
SELECT nome, rg
FROM Funcionario
WHERE rg IN (SELECT rg FROM Paciente);
