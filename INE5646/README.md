# INE5646

Este repositório contém o código do projeto Anime tracker, desenvolvido para a disciplina de Programação Web da UFSC.

## Execução local

Para executar localmente, é preciso configurar a base de dados, backend e frontend. A base de dados utilizada no desenvolvimento foi Postgres rodando em um container docker, mas qualquer base suportada pelo TypeORM. Caso outra base seja utilizada, é necessário instalar suas dependências no backend e configurar corretamente o TypeORM no arquivo `ormconfig.json`.

A instalação do projeto pode ser feita seguindo os seguintes passos:

```
git clone https://github.com/kundlatsch/INE5646.git
cd INE5646

# criar o container docker da base postgres
docker run --name postgres-db -e POSTGRES_PASSWORD=docker -p 5432:5432 -d postgres
docker exec postgres-db createdb -U postgres anime

# instalar as dependencias do backend e rodar as migrações da base e o servidor
cd backend
npm install
npm run typeorm migration:run
npm start

# instalar as dependencias do frontend e rodar o servidor
cd ../frontend
npm install
npm start
```

O projeto estará disponível em `http://localhost:3000/`.

## Backend

O backend da aplicação foi desenvolvido em TypeScript utilizando o runtime Node.js. As principais bibliotecas utilizadas foram o `express` para criar o servidor web, o `TypeORM` para os relacionamentos com o banco de dados e `bcryptjs` para criptografar a senha dos usuários. A autenticação foi feita utilizando JWT.

## Frontend

O frontend da aplicação foi desenvolvido com React.js, em conjunto com algumas bibliotecas como `react-router-dom` para o roteamento das páginas, `matherial-ui` para o componente de modal utilizado ao adicionar um anime novo, `formik` para validação de formulários e `react-icons` para utilizar ícones dentro da aplicação.

Nenhuma biblioteca ou framework de CSS foi utilizado para o desenvolvimento do site, e apenas media queries foram usadas para tornar o site responsivo. A versão para smartphones foi testada no layout do Galaxy S9/S9+ e iPhone X/XS através do console de desenvolvedor do Firefox. O suporte mobile é para telas menores que 500px.
