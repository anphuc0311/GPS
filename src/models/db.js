const mongoose = require('mongoose');

const locationSchema = new mongoose.Schema({
    userId: { type: String, required: true }, // ID thiết bị (VD: Device_01)
    lat: { type: Number, required: true },    // Vĩ độ
    lon: { type: Number, required: true },    // Kinh độ
    timestamp: { type: Date, default: Date.now }, // Thời gian nhận (tự động)
    analysis: { 
        distanceFromCenter: Number, // Khoảng cách tới trung tâm
        isSafe: Boolean,            // Có an toàn không?
        message: String             // Thông báo
    }
});

const Location = mongoose.model('Location', locationSchema);
module.exports = Location;