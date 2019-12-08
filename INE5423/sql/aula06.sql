-- 1)
CREATE VIEW vConsulta AS
	SELECT m.nome as medico, p.nome as paciente, c.data
	FROM Medico m, Paciente p, Consulta c
	WHERE c.crm = m.crm AND c.rg = p.rg;

-- 2)
CREATE VIEW vAmbulatoriosSemMedico AS
	SELECT numeroa
	FROM Ambulatorio
	WHERE numeroa NOT IN (SELECT numeroa FROM Medico);

-- 3)
CREATE VIEW vFuncionariosSemConsulta AS
	SELECT nome
	FROM Funcionario
	WHERE rg NOT IN (SELECT rg FROM Consulta);

-- 3.5)
CREATE USER dba;
ALTER USER dba PASSWORD 'senha';
CREATE USER medico;
CREATE USER estagiario;
CREATE USER secretaria;

-- 4)
GRANT SELECT ON vConsulta TO estagiario;

-- 5)
GRANT INSERT ON Funcionario TO estagiario;

-- 6)
GRANT SELECT, INSERT, UPDATE ON Paciente TO medico;

--7)
GRANT SELECT ON Funcionario TO dba;
GRANT UPDATE (salario) ON Funcionario TO dba;

-- 8)
CREATE VIEW vMedicos4Andar as
	SELECT m.*
	FROM Medico m, Ambulatorio a
	WHERE m.numeroa = a.numeroa AND a.andar = 4;

GRANT SELECT ON vMedicos4Andar TO secretaria;

-- 9)
CREATE ROLE consultas;
GRANT SELECT ON Paciente, Medico TO consultas;
CREATE USER u1 WITH ROLE consultas;
CREATE USER u2 WITH ROLE consultas;
