import express from 'express';
import 'reflect-metadata';
import cors from 'cors';

import routes from './routes';
import './database/connect';

const app = express();

app.use(express.json());
app.use(cors());
app.use(routes);

app.listen(3333, () => console.log("Server started at http://localhost:3333"));
