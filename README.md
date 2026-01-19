# HỆ THỐNG THUYẾT MINH ĐỊA ĐIỂM DU LỊCH TỰ ĐỘNG

## 1. Giới thiệu dự án
Dự án này xây dựng một hệ thống thuyết minh du lịch thông minh, tự động nhận diện vị trí của người dùng thông qua tín hiệu GPS để phát âm thanh thuyết minh tương ứng. Hệ thống được thử nghiệm thực tế tại khuôn viên Đại học Bách Khoa Hà Nội với các địa điểm như Tòa nhà D3, Thư viện Tạ Quang Bửu, Cổng Parabol...

## 2. Thành phần hệ thống
Hệ thống là sự kết hợp giữa phần cứng (Hardware), Máy chủ (Backend Server) và Ứng dụng di động (Mobile App):

* Thiết bị đầu cuối: Raspberry Pi 4B kết hợp với module GPS.
* Backend Server: Node.js, Express Framework.
* Cơ sở dữ liệu: MongoDB (lưu trữ tọa độ các điểm và thông tin thuyết minh).
* Mobile App: React Native (nhận tín hiệu và phát âm thanh thuyết minh).

## 3. Luồng hoạt động của hệ thống
1. Raspberry Pi 4B đọc dữ liệu tọa độ (Kinh độ, Vĩ độ) từ module GPS theo thời gian thực.
2. Thiết bị xử lý logic so sánh tọa độ hiện tại với các vùng kích hoạt (Trigger zones) đã được thiết lập sẵn trong code C.
3. Khi người dùng di chuyển vào vùng xác định (Ví dụ: khu vực tòa nhà D3), Raspberry Pi gửi một yêu cầu HTTP Request lên Server.
4. Server nhận yêu cầu, truy vấn dữ liệu từ MongoDB và gửi tín hiệu kích hoạt đến ứng dụng di động.
5. Ứng dụng React Native nhận được lệnh và tự động phát file âm thanh (Audio) thuyết minh về địa điểm đó.

## 4. Công nghệ sử dụng
* Ngôn ngữ lập trình: C (xử lý trên Raspberry Pi), JavaScript (Node.js, React Native).
* Phần cứng: Raspberry Pi 4B, Module GPS (Neo-6M hoặc tương đương).
* Framework: Express.js, React Native.
* Cơ sở dữ liệu: MongoDB.
* Giao thức: HTTP, RESTful API.

## 5. Cấu trúc thư mục
* /hardware: Mã nguồn C đọc dữ liệu GPS và xử lý logic trigger trên Raspberry Pi.
* /backend: Mã nguồn Node.js server, quản lý API và kết nối database.
* /mobile-app: Mã nguồn ứng dụng React Native cho điện thoại.
* /assets: Chứa các file âm thanh thuyết minh và hình ảnh minh họa địa điểm.

## 6. Hướng dẫn cài đặt
### Đối với Backend:
1. Di chuyển vào thư mục backend: cd backend
2. Cài đặt thư viện: npm install
3. Cấu hình chuỗi kết nối MongoDB trong file .env
4. Khởi chạy server: npm start

### Đối với Mobile App:
1. Di chuyển vào thư mục mobile-app: cd mobile-app
2. Cài đặt thư viện: npm install
3. Chạy ứng dụng: npx react-native run-android (hoặc run-ios)

### Đối với Raspberry Pi:
1. Biên dịch mã nguồn C: gcc main.c -o tourguide -lm
2. Kết nối module GPS vào cổng Serial.
3. Chạy chương trình: ./tourguide

## 7. Các điểm demo tại Bách Khoa
* Tòa nhà D3: Thuyết minh về lịch sử và các khoa đào tạo tại đây.
* Thư viện Tạ Quang Bửu: Thuyết minh về quy mô, chức năng và các phòng đọc.
* Cổng Parabol: Thuyết minh về ý nghĩa biểu tượng của trường Đại học Bách Khoa.