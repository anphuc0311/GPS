const express = require('express');
const route = express.Router();
const {getHomePage, test} = require('../controllers/homeController');


route.get('/', getHomePage);
route.get('/test', test);

module.exports = route; //export default