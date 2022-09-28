const express = require('express');

const app = express();

app.use('/users', (req, res, next) => {
    res.send('<ul><li>João</li><li>Ana</li><li>Felipe</li></ul>')
});

app.use('/', (req, res, next) => {
    res.send('<h1>Welcome Page</h1>')
});

app.listen(3000);
