	CREATE TABLE Cidade (
		codigocidade INTEGER, 
		nome VARCHAR(50),

		PRIMARY KEY (codigocidade)
	);

	CREATE TABLE Empregado (
		codEmp INTEGER,
		nome VARCHAR(40) NOT NULL,
		RG numeric(10) NOT NULL unique,
		idade INTEGER 
			CHECK (idade BETWEEN 16 AND 90),
		estadoCivil char(10) 
			CHECK (estadoCivil IN ('solteiro', 'casado', 'viuvo', 'desquitado', 'divorciado')),
		salario FLOAT  
			CHECK (salario > 0),
		codigocidade INTEGER,
		tempoServico INTEGER,

		PRIMARY KEY (codEmp),
		FOREIGN KEY (codigoCidade) 
			REFERENCES Cidade (codigocidade) 
			ON DELETE CASCADE
	);

	CREATE TABLE cliente (
		codigoCliente INTEGER NOT NULL,
		nome VARCHAR(40) NOT NULL,
		codigoCidade INTEGER,

		PRIMARY KEY (codigoCliente),
		FOREIGN KEY (codigoCidade) 
			REFERENCES Cidade (codigoCidade) 
			ON DELETE RESTRICT
	);

	CREATE TABLE ContaCorrente (
		numeroConta INTEGER,
		saldo FLOAT, 
		codigocliente INTEGER,

		PRIMARY KEY (numeroConta),
		FOREIGN KEY (codigocliente) 
			REFERENCES Cliente (codigocliente) 
			ON DELETE restrict
	);

	CREATE TABLE Emprestimo (
		numero INTEGER, 
		valor FLOAT , 
		codigocliente INTEGER,

		PRIMARY KEY (numero),
		FOREIGN KEY (codigocliente) 
			REFERENCES Cliente (codigocliente) 
			ON DELETE restrict
	);

	-- 1)
	INSERT INTO Empregado VALUES (10, 'Joao', '1002002312', '15', 'solteiro', 1200, 1, 1);
	-- new row for relation "empregado" violates check constraint "empregado_idade_check"
	-- Como a idade é menor que 16, da erro

	INSERT INTO Empregado VALUES (9, 'Joao', '1002032312', '26', 'teste', 1200, 1, 1);
	-- new row for relation "empregado" violates check constraint "empregado_estadocivil_check"
	-- Como 'teste' não está na lista de estados civis aceitos, da erro

	-- 2)
	INSERT INTO Empregado VALUES (18, 'Ninguem', NULL, '26', 'teste', 1200, 1, 1);
	-- null value in column "rg" violates not-null constraint
	-- Erro pois foi definido que RG não pode ser nulo

	-- 3)
	INSERT INTO Cidade VALUES (1, 'Biguacu');
	INSERT INTO Cidade VALUES (2, 'Florianopolis');

	-- 4)
	INSERT INTO Empregado VALUES (1, 'Pedro', '1202032312', '26', 'casado', 1200, 99, 2);
	-- insert or update on table "empregado" violates foreign key constraint "empregado_codigocidade_fkey"
	-- Chave estrangeira de cidade inválida. Versão correta:
	INSERT INTO Empregado VALUES (1, 'Pedro', '1202032312', '26', 'casado', 1200, 2, 2);

	-- 5)
	INSERT INTO Empregado VALUES (2, 'Mario', '1232032312', '26', 'solteiro', 1200, 2, 2);

	-- 6)
	SELECT * FROM Empregado;

	-- 7)
	DELETE FROM Cidade WHERE nome='Florianopolis';
	-- Query returned successfully in 66 msec.
	DELETE FROM Cidade WHERE nome='Biguacu';
	-- Query returned successfully in 66 msec.
	SELECT * FROM Empregado;
	-- Tabela Vazia
	SELECT * FROM Cidade;
	-- Tabela Vazia

	-- 8)
	INSERT INTO Cliente VALUES (1, 'Airton', 1);

	-- 9)
	DELETE FROM Cidade WHERE nome='Biguacu';
	-- update or delete on table "cidade" violates foreign key constraint "cliente_codigocidade_fkey" on table "cliente"
	-- A constrain não permite deletar a cidade, pois é chave estrangeira do cliente cadastrado anteriormente
	-- Como Florianópolis era chave estrangeira de um funcionário, que tinha a constrain de ON DELETE Cascade, Florianópolis foi deletada

	-- 10)
	INSERT INTO Contacorrente VALUES (1, 100, 1);

	-- 11)
	CREATE OR REPLACE FUNCTION emprestimo() RETURNS TRIGGER AS $emprestimo$
	BEGIN
		IF NEW.saldo < 0 THEN
			INSERT INTO Emprestimo VALUES (NEW.numeroconta, -NEW.saldo, NEW.codigocliente);
			UPDATE ContaCorrente SET saldo = 0 WHERE numeroconta = NEW.numeroconta;
		END IF;
		RETURN NEW;
	END $emprestimo$
	LANGUAGE plpgsql;

	CREATE TRIGGER autoEmprestimo
	AFTER UPDATE ON ContaCorrente FOR EACH ROW EXECUTE PROCEDURE emprestimo();

	-- 12)
	UPDATE ContaCorrente SET saldo = -1000 WHERE numeroconta=1;

	  
