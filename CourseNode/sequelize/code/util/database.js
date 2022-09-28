const Sequelize = require('sequelize');

const sequelize = new Sequelize('postgres', 'postgres', 'password', {
  dialect: 'postgres',
  host: 'localhost'
});

module.exports = sequelize;
