const express = require('express');
require('dotenv').config();
const path = require('path')
const configViewEngine = require('./config/viewEngine');
const webRoutes = require('./routes/web');

const app = express();
const port = process.env.PORT;
const hostname = process.env.HOST_NAME;

console.log('Hostname from .env:', port);

// Configure the view engine
configViewEngine(app);

// Routes
app.use('/', webRoutes);
// Start the server
app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`);
});