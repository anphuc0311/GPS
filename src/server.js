
const express = require('express');
const bodyParser = require('body-parser'); 
const mongoose = require('mongoose');

// Import Controller và Router
const gpsRoutes = require('./routes/gps_routes'); // Chúng ta sẽ tạo file này sau

const app = express();
const PORT = 3000;

// Cho phép Node.js đọc dữ liệu gửi từ ứng dụng C (dạng URL-encoded)
app.use(bodyParser.urlencoded({ extended: true })); 

// Cho phép Node.js đọc dữ liệu dạng JSON 
app.use(bodyParser.json()); 

// CORS: Cho phép các nguồn khác kết nối
app.use((req, res, next) => {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

// 1. DATABASE (Kết nối MongoDB)

const MONGO_URI = 'mongodb://localhost:27017/my_gps_app'; 

mongoose.connect(MONGO_URI)
    .then(() => console.log(' Đã kết nối thành công tới MongoDB'))
    .catch(err => console.error(' Lỗi kết nối MongoDB:', err));


// 2. ROUTES (Gắn Định tuyến)
// Gắn router vào app
app.use('/api', gpsRoutes);

// START SERVER
app.listen(PORT, () => {
    console.log(`------------------------------------------------`);
    console.log(` Server Node.js (MVC) đang chạy tại: http://localhost:${PORT}`);
    console.log(` Sẵn sàng nhận dữ liệu POST từ ứng dụng C`);
    console.log(`------------------------------------------------`);
});