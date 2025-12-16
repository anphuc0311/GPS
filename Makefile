# =========================================================
# Makefile cho GPS Sender (Sử dụng libcurl)
# File nguồn C: gps_sender_libcurl.c
# =========================================================

# Tên file nguồn C
# [ĐÃ SỬA] Cập nhật đường dẫn tương đối đến file C (ví dụ: nằm trong thư mục 'src/')
SRC = src/output_gps.c

# Tên file thực thi (output)
TARGET = output_gps

# Cờ biên dịch:
# -Wall: Bật tất cả cảnh báo
# -std=c99: Sử dụng tiêu chuẩn C99 (hoặc c11)
CFLAGS = -Wall -std=c99 -g

# Tự động tìm cờ biên dịch và liên kết thư viện libcurl
# Nếu pkg-config không có, bạn cần cài đặt: sudo apt install libcurl4-openssl-dev pkg-config
CURL_LIBS = $(shell pkg-config --cflags --libs libcurl)

# Thư viện toán học
MATH_LIB = -lm

# Mục tiêu mặc định: Biên dịch
all: $(TARGET)

# Quy tắc biên dịch: Tạo file thực thi từ file nguồn
$(TARGET): $(SRC)
	@echo "🔥 Bắt đầu biên dịch ứng dụng GPS Sender..."
	# Lệnh biên dịch: gcc [Cờ biên dịch] [File nguồn] [Cờ libcurl] [Cờ thư viện toán học] -o [Tên file thực thi]
	gcc $(CFLAGS) $(SRC) $(CURL_LIBS) $(MATH_LIB) -o $(TARGET)
	@echo "✅ Biên dịch thành công. File thực thi: ./"$(TARGET)

# Quy tắc dọn dẹp (Xóa file thực thi và object files nếu có)
clean:
	@echo "🗑️ Dọn dẹp các file đã biên dịch..."
	rm -f $(TARGET) *.o
	@echo "✅ Hoàn tất dọn dẹp."

# Quy tắc chạy chương trình
run: $(TARGET)
	@echo "🚀 Chạy chương trình..."
	./$(TARGET)

.PHONY: all clean run