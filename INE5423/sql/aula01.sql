CREATE TABLE Medicos (
	codm 		INT,
	nome 		varchar(40) 	NOT NULL,
	idade		smallint 	NOT NULL,
	especialidade	char(20),
	CPF		numeric(11)	UNIQUE,
	cidade		varchar(30),
	nroa		int,
	PRIMARY KEY (codm),
	FOREIGN KEY (nroa) REFERENCES Ambulatorio
	ON DELETE restrict
	ON UPDATE restrict
)


CREATE TABLE Ambulatorio (
	nroa		int,
	andar		numeric(3) NOT NULL,
	capacidade 	smallint,
	PRIMARY KEY	(nroa)
)

-- ALTER TABLE Medicos ADD FOREIGN KEY (nroa) REFERENCES Ambulatorio; 

CREATE TABLE Funcionario (
	codf		int,
	nome		varchar(40)	NOT NULL,
	idade		smallint	NOT NULL,
	cidade		varchar(30),
	CPF		numeric(11)	UNIQUE,
	salario		numeric(10),
	cargo		varchar(20),
	PRIMARY KEY 	(codf)
)

CREATE TABLE Paciente (
	codp		int,
	nome		varchar(40)	NOT NULL,
	idade		smallint	NOT NULL,
	cidade		varchar(30),
	CPF		numeric(11)	UNIQUE,
	doenca		varchar(20),
	PRIMARY KEY 	(codp)
)

CREATE TABLE Consultas (
	codm	int,
	codp	int,
	data	date,
	hora	time,
	PRIMARY KEY (codm, data, hora),
	FOREIGN KEY (codm) REFERENCES Medicos,
	FOREIGN KEY (codp) REFERENCES Paciente
)
