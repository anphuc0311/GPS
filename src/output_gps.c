#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // [MỚI] Dùng cho hàm sleep() để tạo độ trễ
#include <curl/curl.h> 

#define MAX_DATA_SIZE 128
#define SERVER_URL "http://localhost:3000/api/update-location"
#define SEND_INTERVAL_SEC 5 // Gửi dữ liệu mỗi 5 giây

/**
 * @brief Hàm mô phỏng việc đọc dữ liệu GPS và tạo chuỗi POST.
 * * Sử dụng time(NULL) làm seed cho rand()
 */
void generate_gps_data(char *buffer) {
    // Giá trị cơ sở gần POI D3 Bách Khoa
    double base_lat = 21.0069;
    double base_lon = 105.8437;

    // Tạo giá trị ngẫu nhiên nhỏ để mô phỏng di chuyển (trong vòng 100 mét)
    srand(time(NULL));
    double random_offset_lat = ((double)rand() / RAND_MAX * 0.0005) - 0.00025; 
    double random_offset_lon = ((double)rand() / RAND_MAX * 0.0005) - 0.00025;

    double current_lat = base_lat + random_offset_lat;
    double current_lon = base_lon + random_offset_lon;
    
    const char *device_id = "Device_LIB_C_001";

    // Tạo chuỗi dữ liệu URL-ENCODED
    snprintf(buffer, MAX_DATA_SIZE, 
             "lat=%.6f&lon=%.6f&userId=%s", 
             current_lat, current_lon, device_id);
}

/**
 * @brief Hàm chính để gửi dữ liệu lên server bằng libcurl.
 */
int send_data_to_server(const char *post_data) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        printf("-> [%ld] Đang gửi dữ liệu: %s\n", (long)time(NULL), post_data); 
        
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
    
    printf("--- BẮT ĐẦU GỬI DỮ LIỆU GPS LIÊN TỤC (Mỗi %d giây) ---\n", SEND_INTERVAL_SEC);

    // Vòng lặp vô hạn để mô phỏng truyền tải liên tục
    while(1) {
        // 1. Khởi tạo dữ liệu GPS (Lấy tọa độ mới)
        generate_gps_data(gps_data_buffer);

        // 2. Gửi dữ liệu trực tiếp lên server
        if (send_data_to_server(gps_data_buffer) == 0) {
            // Gửi thành công
        } else {
            // Gửi thất bại
        }

        // 3. Tạm dừng 5 giây trước khi gửi tiếp
        printf("... Tạm dừng %d giây...\n", SEND_INTERVAL_SEC);
        sleep(SEND_INTERVAL_SEC);
    }

    return 0;
}