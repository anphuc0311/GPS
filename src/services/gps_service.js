const geometryService = {
    // Công thức Haversine để tính khoảng cách giữa 2 tọa độ GPS (đơn vị: km)
    //lat1, lon1: tọa độ điểm cố định
    //lat2, lon2: tọa độ điểm GPS
    //Điểm thuyết minh (Point of Interest)
    POI: [
        { 
            name: "Hà Nội", 
            lat: 21.0285, 
            lon: 105.8542,
            bk_km: 100 },
        { 
            name: "Hồ Chí Minh", 
            lat: 10.8231, 
            lon: 106.6297,
            bk_km: 100 
        },
        { 
            name: "Đà Nẵng", 
            lat: 16.0544, 
            lon: 108.2022,
            bk_km: 100 
        }
    ],
    calculateDistance: (lat1, lon1, lat2, lon2) => {
        const R = 6371; 
        const dLat = (lat2 - lat1) * (Math.PI / 180);
        const dLon = (lon2 - lon1) * (Math.PI / 180);
        const a = 
            Math.sin(dLat / 2) * Math.sin(dLat / 2) +
            Math.cos(lat1 * (Math.PI / 180)) * Math.cos(lat2 * (Math.PI / 180)) * Math.sin(dLon / 2) * Math.sin(dLon / 2);
        const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        return parseFloat((R * c).toFixed(2));
    },

    // Kiểm tra xem tọa độ có nằm trong vùng an toàn (bán kính 10km quanh Hà Nội) không
    checkSafetyZone: (currentLat, currentLon) => {
        const CENTER_HANOI = { lat: 21.0285, lon: 105.8542 }; 
        const MAX_SAFETY_DIST = 10; // 10km

        const distance = geometryService.calculateDistance(
            CENTER_HANOI.lat, CENTER_HANOI.lon,
            currentLat, currentLon
        );

        return {
            distanceFromCenter: distance,
            isSafe: distance < MAX_SAFETY_DIST,
            message: distance < MAX_SAFETY_DIST 
                ? "An toàn (Trong bán kính 10km)" 
                : `CẢNH BÁO: Đã ra khỏi vùng an toàn! (${distance} km)`
        };
    },
    //Kiểm tra xem tọa độ có gần điểm thuyết minh (POI) nào không
    IsnearPOI: (currentLat, currentLon) => {
        for (const poi of geometryService.POI) {
            const distance = geometryService.calculateDistance(
                poi.lat, poi.lon,
                currentLat, currentLon
            );
            if (distance < poi.bk_km) {
                return poi; 
            }
        }
        return null;            
    }
};

module.exports = geometryService;