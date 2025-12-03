const express = require('express');
const route = express.Router();
const gpsController = require('../controllers/homeController');

route.post('/update-location', gpsController.updateLocation); // Dành cho C gọi vào
route.get('/history', gpsController.getHistory);              // Dành cho trình duyệt xem

module.exports = route; 