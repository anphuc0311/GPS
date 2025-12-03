const Location = require('../models/db');
const geometryService = require('../services/gps_service');

const gpsController = {
    // API nhận dữ liệu: POST /api/update-location
    updateLocation: async (req, res) => {
        try {
            // Lấy dữ liệu từ body (do body-parser xử lý)
            const { lat, lon, userId } = req.body; 
            
            // Chuyển đổi sang số 
            const numLat = parseFloat(lat);
            const numLon = parseFloat(lon);

            // Kiểm tra dữ liệu hợp lệ
            if (isNaN(numLat) || isNaN(numLon) || !userId) {
                return res.status(400).json({ error: "Dữ liệu không hợp lệ. Cần lat, lon (số) và userId." });
            }
            
            console.log(`[Dữ liệu mới] Device: ${userId} | Lat: ${numLat} | Lon: ${numLon}`);

            // Tính toán logic
            const safetyStatus = geometryService.checkSafetyZone(numLat, numLon);

            // Tạo bản ghi mới để lưu vào MongoDB
            const newLocation = new Location({
                userId: userId,
                lat: numLat,
                lon: numLon,
                analysis: safetyStatus
            });

            // Lưu xuống Database (Lệnh này chờ MongoDB ghi xong mới chạy tiếp)
            const savedRecord = await newLocation.save();

            // Phản hồi lại cho ứng dụng C biết là đã thành công
            res.json({
                success: true,
                message: "Server đã lưu dữ liệu thành công.",
                data: {
                    id: savedRecord._id,
                    status: safetyStatus.message
                }
            });

        } catch (error) {
            console.error("Lỗi Server:", error);
            res.status(500).json({ error: "Lỗi nội bộ server." });
        }
    },

    // API xem lịch sử: GET /api/history
    getHistory: async (req, res) => {
        try {
            // Lấy 50 bản ghi mới nhất, sắp xếp theo thời gian giảm dần
            const history = await Location.find().sort({ timestamp: -1 }).limit(50);
            res.json(history);
        } catch (error) {
            res.status(500).json({ error: error.message });
        }
    }
};

module.exports = gpsController;