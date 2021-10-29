import { createConnection } from 'typeorm';

// Configs from ormconfig.json will be loaded
createConnection().then(() => console.log("Connected with DB!")).catch((err) => console.log(err));