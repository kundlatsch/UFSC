INSERT into Ambulatorio VALUES (4, 0, 4)
INSERT into Ambulatorio VALUES (10, 1, 6)

INSERT into Medico VALUES (1, 'Jorge', 27, 'OTORRINOLARINGOLOGIA', '11423912309', 'Pindamoiangaba', 4)
INSERT into Medico VALUES (2, 'Teo', 27, 'NEUROCIRURGIAO', '12445678125', 'Auto Paraguacu', 4)
INSERT into Medico VALUES (3, 'Chico', 56, 'PEDIATRA', '45446672125', 'Rio de Janeiro', 4)

INSERT into Funcionario VALUES (1, 'Maria', 22, 'Mafra', '92412902112', 1800, 'Secretaria');
INSERT into Funcionario VALUES (2, 'Micao', 25, 'Lages', '15491412980', 3000, 'Zelador');
INSERT into Funcionario VALUES (3, 'Felipe', 40, 'Porto Alegre', '29410458012', 7000, 'Diretor')

UPDATE Ambulatorio SET capacidade = 180 WHERE nroa = 4

DELETE FROM Medico WHERE nroa = 4

INSERT into Medico VALUES (1, 'Jorge', 27, 'OTORRINOLARINGOLOGIA', '11423912309', 'Pindamoiangaba', 10);
INSERT into Medico VALUES (2, 'Teo', 27, 'NEUROCIRURGIAO', '12445678125', 'Auto Paraguacu', 10);
INSERT into Medico VALUES (3, 'Chico', 56, 'PEDIATRA', '45446672125', 'Rio de Janeiro', 10)

SELECT a.nroa
FROM Ambulatorio a, Medico m
WHERE m.nroa <> a.nroa

SELECT a.nroa
FROM Ambulatorio a, Medico m
WHERE a.nroa <> m.nroa

SELECT nroa
FROM Ambulatorio
WHERE nroa NOT IN (SELECT nroa FROM Medico)
