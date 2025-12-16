#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h> // Thư viện để thực hiện HTTP request

#define MAX_DATA_SIZE 128
#define SERVER_URL "http://localhost:3000/api/update-location"

/**
 * @brief Hàm mô phỏng việc đọc dữ liệu GPS và tạo chuỗi POST.
 * @param buffer Buffer để lưu trữ chuỗi dữ liệu GPS (dạng: lat=X.XXXX&lon=Y.YYYY&userId=Z)
 * @return void 
 */
void generate_gps_data(char *buffer) {
    // Giá trị cơ sở gần POI D3 Bách Khoa
    double base_lat = 21.0069;
    double base_lon = 105.8437;

    // Tạo giá trị ngẫu nhiên nhỏ để mô phỏng di chuyển (trong vòng 100 mét)
    srand(time(NULL));
    // Tạo offset ngẫu nhiên
    double random_offset_lat = ((double)rand() / RAND_MAX * 0.0005) - 0.00025; 
    double random_offset_lon = ((double)rand() / RAND_MAX * 0.0005) - 0.00025;

    double current_lat = base_lat + random_offset_lat;
    double current_lon = base_lon + random_offset_lon;
    
    // Giả lập ID thiết bị 
    const char *device_id = "Device_LIB_C_001";

    // =================================================================
    // PHẦN TẠO CHUỖI DỮ LIỆU ĐÚNG DẠNG URL-ENCODED CHO SERVER NODE.JS
    // Server mong đợi: lat, lon, userId
    // =================================================================
    snprintf(buffer, MAX_DATA_SIZE, 
             "lat=%.6f&lon=%.6f&userId=%s", 
             current_lat, current_lon, device_id);
}

/**
 * @brief Hàm chính để gửi dữ liệu lên server bằng libcurl.
 * @param post_data Chuỗi dữ liệu POST đã được format (e.g., "lat=...&lon=...")
 * @return int 0 nếu thành công, khác 0 nếu lỗi.
 */
int send_data_to_server(const char *post_data) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        // Thiết lập URL đích
        curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
        
        // Thiết lập phương thức POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        
        // Thiết lập dữ liệu POST
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        
        // Thiết lập header: Báo cho server đây là dạng URL-encoded
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        printf("-> Đang gửi dữ liệu: %s\n", post_data);
        
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() thất bại: %s\n", curl_easy_strerror(res));
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            return 1;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        printf("<- Phản hồi HTTP Code: %ld\n", http_code);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return 0;
}

int main() {
    char gps_data_buffer[MAX_DATA_SIZE];
    
    // 1. Khởi tạo dữ liệu GPS
    generate_gps_data(gps_data_buffer);

    // 2. Gửi dữ liệu trực tiếp lên server
    if (send_data_to_server(gps_data_buffer) == 0) {
        printf("Gửi dữ liệu GPS hoàn tất.\n");
    } else {
        printf("Gửi dữ liệu GPS thất bại! (Kiểm tra Server Node.js)\n");
    }

    return 0;
}