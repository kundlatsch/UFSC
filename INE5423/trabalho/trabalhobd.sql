/* Lógico_1: */

CREATE TABLE Guia (
    indicacao_clinica varchar(280),
    qtde_diarias_solicitadas int,
    regime_internacao char(1),
    carater_internacao char(1),
    cod_internacao int PRIMARY KEY,
    indicacao_acidente char(1),
    tempo_doenca_valor int,
    tipo_doenca char(1),
    data_admissao date,
    qtde_diarias_autorizadas int,
    observacao_autorizacao varchar(280),
    ass_data_medico date,
    ass_medico varchar(30),
    ass_data_beneficiario date,
    ass_beneficiario varchar(30),
    ass_data_responsavel date,
    ass_responsavel varchar(30),
    tempo_doenca_tipo char(1),
    senha varchar(10),
    validade_senha date,
    data_emissao_guia date,
    data_autorizacao date,
    fk_Beneficiario_numero_carteira int,
    fk_UnidadeMedica_CNPJ int,
    fk_Profissional_CPF int,
    fk_CID10_cid varchar(6),
    fk_Acomodacao_tipo_acomodacao int,
    fk_Tipo_codigo int
);

CREATE TABLE Beneficiario (
    numero_carteira int PRIMARY KEY,
    validade date,
    nome varchar(30),
    nro_cns int,
    fk_Plano_codigo_plano int
);

CREATE TABLE Plano (
    codigo_plano int PRIMARY KEY,
    nome varchar(30)
);

CREATE TABLE UnidadeMedica (
    CNPJ int PRIMARY KEY,
    nome varchar(30)
);

CREATE TABLE CID10 (
    nome_doenca varchar(30),
    cid varchar(6) PRIMARY KEY
);

CREATE TABLE Procedimento (
    tabela char(2),
    descricao varchar(144),
    codigo_procedimento char(10) PRIMARY KEY
);

CREATE TABLE OPM (
    tabela char(2),
    codigo_opm char(10) PRIMARY KEY,
    descricao char(144),
    valor_unitario int,
    FK_Fabricante_cod_fabricante int
);

CREATE TABLE Prorrogacao (
    data date,
    senha int,
    qtde_autorizada int,
    codigo_prorrogacao int PRIMARY KEY,
    fk_Guia_cod_internacao int,
    fk_Responsavel_codigo_responsavel int,
    fk_Acomodacao_tipo_acomodacao int
);

CREATE TABLE Profissional (
    CPF int PRIMARY KEY,
    nome varchar(30),
    conselho_profissional varchar(30),
    numero_conselho int,
    UF char(2)
);

CREATE TABLE CNES (
    cod_cnes int PRIMARY KEY,
    nome varchar(30)
);

CREATE TABLE CBO (
    cod_cbo int PRIMARY KEY,
    nome varchar(30)
);

CREATE TABLE Fabricante (
    cod_fabricante int PRIMARY KEY,
    nome varchar(30)
);

CREATE TABLE Responsavel (
    codigo_responsavel int PRIMARY KEY,
    nome varchar(30)
);

CREATE TABLE Acomodacao (
    tipo_acomodacao int PRIMARY KEY,
    descricao varchar(30)
);

CREATE TABLE Tipo (
    nome varchar(30),
    codigo int PRIMARY KEY
);

CREATE TABLE CNES_UM (
    FK_UnidadeMedica_CNPJ int,
    FK_CNES_cod_cnes int
);

CREATE TABLE CBO_PR (
    FK_Profissional_CPF int,
    FK_CBO_cod_cbo int
);

CREATE TABLE UMSolicitante (
    fk_Guia_cod_internacao int,
    fk_UnidadeMedica_CNPJ int
);

CREATE TABLE CID10_Secundario (
    fk_CID10_cid varchar(6),
    fk_Guia_cod_internacao int
);

CREATE TABLE ProcedInternacao (
    fk_Procedimento_codigo_procedimento char(10),
    fk_Guia_cod_internacao int,
    qtde_solicitada int,
    qtde_autorizada int
);

CREATE TABLE OPMInternacao (
    fk_OPM_codigo_opm char(10),
    fk_Guia_cod_internacao int,
    quantidade int
);

CREATE TABLE OPMProrrogacao (
    fk_Prorrogacao_codigo_prorrogacao int,
    fk_OPM_codigo_opm char(10),
    quantidade int
);

CREATE TABLE ProcedimentoProrrogacao (
    fk_Procedimento_codigo_procedimento char(10),
    fk_Prorrogacao_codigo_prorrogacao int,
    qtde_solicitada int,
    qtde_autorizada int
);
 
ALTER TABLE Guia ADD CONSTRAINT FK_Guia_2
    FOREIGN KEY (fk_Beneficiario_numero_carteira)
    REFERENCES Beneficiario (numero_carteira)
    ON DELETE CASCADE;
 
ALTER TABLE Guia ADD CONSTRAINT FK_Guia_3
    FOREIGN KEY (fk_UnidadeMedica_CNPJ)
    REFERENCES UnidadeMedica (CNPJ)
    ON DELETE CASCADE;
 
ALTER TABLE Guia ADD CONSTRAINT FK_Guia_4
    FOREIGN KEY (fk_Profissional_CPF)
    REFERENCES Profissional (CPF)
    ON DELETE CASCADE;
 
ALTER TABLE Guia ADD CONSTRAINT FK_Guia_5
    FOREIGN KEY (fk_CID10_cid)
    REFERENCES CID10 (cid)
    ON DELETE CASCADE;
 
ALTER TABLE Guia ADD CONSTRAINT FK_Guia_6
    FOREIGN KEY (fk_Acomodacao_tipo_acomodacao)
    REFERENCES Acomodacao (tipo_acomodacao)
    ON DELETE CASCADE;
 
ALTER TABLE Guia ADD CONSTRAINT FK_Guia_7
    FOREIGN KEY (fk_Tipo_codigo)
    REFERENCES Tipo (codigo)
    ON DELETE CASCADE;
 
ALTER TABLE Beneficiario ADD CONSTRAINT FK_Beneficiario_2
    FOREIGN KEY (fk_Plano_codigo_plano)
    REFERENCES Plano (codigo_plano)
    ON DELETE CASCADE;
 
ALTER TABLE OPM ADD CONSTRAINT FK_OPM_2
    FOREIGN KEY (FK_Fabricante_cod_fabricante)
    REFERENCES Fabricante (cod_fabricante)
    ON DELETE CASCADE;
 
ALTER TABLE Prorrogacao ADD CONSTRAINT FK_Prorrogacao_2
    FOREIGN KEY (fk_Guia_cod_internacao)
    REFERENCES Guia (cod_internacao)
    ON DELETE CASCADE;
 
ALTER TABLE Prorrogacao ADD CONSTRAINT FK_Prorrogacao_3
    FOREIGN KEY (fk_Responsavel_codigo_responsavel)
    REFERENCES Responsavel (codigo_responsavel)
    ON DELETE CASCADE;
 
ALTER TABLE Prorrogacao ADD CONSTRAINT FK_Prorrogacao_4
    FOREIGN KEY (fk_Acomodacao_tipo_acomodacao)
    REFERENCES Acomodacao (tipo_acomodacao)
    ON DELETE CASCADE;
  
ALTER TABLE UMSolicitante ADD CONSTRAINT FK_UMSolicitante_1
    FOREIGN KEY (FK_UnidadeMedica_CNPJ)
    REFERENCES UnidadeMedica (CNPJ)
    ON DELETE SET NULL;
 
ALTER TABLE UMSolicitante ADD CONSTRAINT FK_UMSolicitante_2
    FOREIGN KEY (FK_Guia_cod_internacao)
    REFERENCES Guia (cod_internacao)
    ON DELETE SET NULL;

ALTER TABLE CNES_UM ADD CONSTRAINT FK_CNES_UM_1
    FOREIGN KEY (FK_UnidadeMedica_CNPJ)
    REFERENCES UnidadeMedica (CNPJ)
    ON DELETE SET NULL;
 
ALTER TABLE CNES_UM ADD CONSTRAINT FK_CNES_UM_2
    FOREIGN KEY (FK_CNES_cod_cnes)
    REFERENCES CNES (cod_cnes)
    ON DELETE SET NULL;
 
ALTER TABLE CBO_PR ADD CONSTRAINT FK_CBO_PR_1
    FOREIGN KEY (FK_Profissional_CPF)
    REFERENCES Profissional (CPF)
    ON DELETE SET NULL;
 
ALTER TABLE CBO_PR ADD CONSTRAINT FK_CBO_PR_2
    FOREIGN KEY (FK_CBO_cod_cbo)
    REFERENCES CBO (cod_cbo)
    ON DELETE SET NULL;
 
ALTER TABLE CID10_Secundario ADD CONSTRAINT FK_CID10_Secundario_1
    FOREIGN KEY (fk_CID10_cid)
    REFERENCES CID10 (cid)
    ON DELETE SET NULL;
 
ALTER TABLE CID10_Secundario ADD CONSTRAINT FK_CID10_Secundario_2
    FOREIGN KEY (fk_Guia_cod_internacao)
    REFERENCES Guia (cod_internacao)
    ON DELETE SET NULL;
 
ALTER TABLE ProcedInternacao ADD CONSTRAINT FK_ProcedInternacao_1
    FOREIGN KEY (fk_Procedimento_codigo_procedimento)
    REFERENCES Procedimento (codigo_procedimento)
    ON DELETE RESTRICT;
 
ALTER TABLE ProcedInternacao ADD CONSTRAINT FK_ProcedInternacao_2
    FOREIGN KEY (fk_Guia_cod_internacao)
    REFERENCES Guia (cod_internacao)
    ON DELETE SET NULL;
 
ALTER TABLE OPMInternacao ADD CONSTRAINT FK_OPMInternacao_1
    FOREIGN KEY (fk_OPM_codigo_opm)
    REFERENCES OPM (codigo_opm)
    ON DELETE SET NULL;
 
ALTER TABLE OPMInternacao ADD CONSTRAINT FK_OPMInternacao_2
    FOREIGN KEY (fk_Guia_cod_internacao)
    REFERENCES Guia (cod_internacao)
    ON DELETE SET NULL;
 
ALTER TABLE OPMProrrogacao ADD CONSTRAINT FK_OPMProrrogacao_1
    FOREIGN KEY (fk_Prorrogacao_codigo_prorrogacao)
    REFERENCES Prorrogacao (codigo_prorrogacao)
    ON DELETE SET NULL;
 
ALTER TABLE OPMProrrogacao ADD CONSTRAINT FK_OPMProrrogacao_2
    FOREIGN KEY (fk_OPM_codigo_opm)
    REFERENCES OPM (codigo_opm)
    ON DELETE SET NULL;
 
ALTER TABLE ProcedimentoProrrogacao ADD CONSTRAINT FK_ProcedimentoProrrogacao_1
    FOREIGN KEY (fk_Procedimento_codigo_procedimento)
    REFERENCES Procedimento (codigo_procedimento)
    ON DELETE RESTRICT;
 
ALTER TABLE ProcedimentoProrrogacao ADD CONSTRAINT FK_ProcedimentoProrrogacao_2
    FOREIGN KEY (fk_Prorrogacao_codigo_prorrogacao)
    REFERENCES Prorrogacao (codigo_prorrogacao)
    ON DELETE SET NULL;





SELECT nome FROM Plano WHERE codigo_plano IN (SELECT codigo_plano FROM Beneficiario);
SELECT B.nome FROM Beneficiario B, Guia G WHERE B.numero_carteira = G.fk_Beneficiario_numero_carteira AND G.fk_Tipo_codigo = 4;
SELECT O.descricao FROM OPM O, Guia G, Profissional P, OPMInternacao OI WHERE P.CPF = G.fk_Profissional_CPF AND P.conselho_profissional='10' AND OI.fk_Guia_cod_internacao = G.cod_internacao AND O.codigo_opm = OI.fk_OPM_codigo_opm;

