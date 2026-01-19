#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  //thao tac voi file
#include <termios.h> // config uart
#include <unistd.h> //thao tac voi cong serial
#include <string.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <time.h>
//-----------------------------------struct & variable------------------------

#define SERVER_URL "http://localhost:3000/api/update-location"
struct position{
  float latitude;
  float longitude;
};
typedef struct position position;

struct station{
  int num;
  char name[100];
  int audio_idx;
  position positionA;
  position positionB;
  position positionC;
  position positionD;
};
typedef struct station station;

position curr;
station stations[10];

//------------------------------------function--------------------------------
float convertToDecimal(float raw, char dir);
void tachGPGLL(char *line);
float cross_product(float x1,float y1,float x2,float y2);
int checkStation(position position_to_check, station station_to_check);
void initStations(void); //----add station----
//void process_gps();
//void filler();

int main() {
    int fd; //= serial_port
    struct termios tty;
    char buf[256];
    ssize_t n;
    initStations();

    fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (fd < 0) {
        perror("not open /dev/ttyACM0");
        return 1;
    }

    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) { //lay cau hinh hien tai cua cong serial
        perror("tcgetattr");
        close(fd);
        return 1;
    }

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    tty.c_iflag = 0;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        close(fd);
        return 1;
    }

    printf("=== Đang đọc dữ liệu NMEA từ VK-162 ===\n");
    printf("Nhấn Ctrl+C để dừng.\n\n");

    while (1) {
        n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("%s", buf);
            tachGPGLL(buf); 
            fflush(stdout);
            for (int i = 1; i< 10; i++) {
              if (checkStation(curr, stations[i]) == 1) {
                printf("\n %s", stations[i].name);
                //process_gps();
                }
            }
        } else {
            usleep(100000); 
        }
    }

    close(fd);
    return 0;
}

//-----------------------------------baby function------------------------------
float convertToDecimal(float raw, char dir) {
    int degrees = (int)(raw / 100);
    float minutes = raw - (degrees * 100);
    float decimal = degrees + (minutes / 60.0);
    if (dir == 'S' || dir == 'W') decimal = -decimal;
    return decimal;
}

void tachGPGLL(char *line) {
    char type[8];
    float lat_raw, lon_raw;
    char lat_dir, lon_dir;
    float latitude, longitude;

    if (strncmp(line, "$GPGLL", 6) == 0) {
        int fields = sscanf(line, "$%[^,],%f,%c,%f,%c", type, &lat_raw, &lat_dir, &lon_raw, &lon_dir);
        if (fields >= 5) {
            latitude = convertToDecimal(lat_raw, lat_dir);
            longitude = convertToDecimal(lon_raw, lon_dir);
            curr.latitude = latitude;
            curr.longitude = longitude;
            printf("\n Vĩ độ: %.6f°, Kinh độ: %.6f°\n", latitude, longitude);
        }
    }
  }

float cross_product(float x1,float y1,float x2,float y2) {
  return x1*y2-y1*x2;
}
// Hàm gửi dữ liệu bằng libcurl (giữ nguyên logic bạn yêu cầu)
int send_data_to_server(const char *post_data) {
    CURL *curl;
    CURLcode res;
    int success = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        printf("-> [%ld] Dang gui du lieu: %s\n", (long)time(NULL), post_data); 
        
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() that bai: %s\n", curl_easy_strerror(res));
        } else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            printf("<- Phan hoi HTTP Code: %ld\n", http_code);
            success = 1;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return success ? 0 : 1;
}

// Hàm xử lý đóng gói dữ liệu khi phát hiện vào một Station
void process_gps(position current_pos, station s) {
    char post_data[512];
    
    // Đóng gói tọa độ và thông tin trạm thành chuỗi URL-encoded
    // userId ở đây có thể dùng làm định danh thiết bị
    snprintf(post_data, sizeof(post_data), 
             "lat=%.6f&lon=%.6f&stationId=%d&stationName=%s&userId=Pi4_BK_01", 
             current_pos.latitude, 
             current_pos.longitude, 
             s.num, 
             s.name);

    // Gọi hàm gửi lên server
    send_data_to_server(post_data);
}
int checkStation(position position_to_check, station station_to_check) {
  float cross_product1;
  float cross_product2;
  float cross_product3;
  float cross_product4;
  cross_product1 = cross_product((station_to_check.positionB.latitude - station_to_check.positionA.latitude), 
  (station_to_check.positionB.longitude - station_to_check.positionA.longitude), 
  (position_to_check.latitude - station_to_check.positionA.latitude), 
  (position_to_check.longitude - station_to_check.positionA.longitude));
  cross_product2 = cross_product((station_to_check.positionC.latitude - station_to_check.positionB.latitude), 
  (station_to_check.positionC.longitude - station_to_check.positionB.longitude), 
  (position_to_check.latitude - station_to_check.positionB.latitude), 
  (position_to_check.longitude - station_to_check.positionB.longitude));
  cross_product3 = cross_product((station_to_check.positionD.latitude - station_to_check.positionC.latitude), 
  (station_to_check.positionD.longitude - station_to_check.positionC.longitude), 
  (position_to_check.latitude - station_to_check.positionC.latitude), 
  (position_to_check.longitude - station_to_check.positionC.longitude));
  cross_product4 = cross_product((station_to_check.positionA.latitude - station_to_check.positionD.latitude), 
  (station_to_check.positionA.longitude - station_to_check.positionD.longitude), 
  (position_to_check.latitude - station_to_check.positionD.latitude), 
  (position_to_check.longitude - station_to_check.positionD.longitude));
  if ((cross_product1 >= 0 && cross_product2 >= 0 && cross_product3 >= 0 && cross_product4 >= 0) || (cross_product1 <= 0 && cross_product2 <= 0 && cross_product3 <= 0 && cross_product4 <= 0)) return 1;
  else return 0;
}

void initStations(void) {
  stations[0].num = 1;
  strcpy(stations[0].name, "cổng Trần Đại Nghĩa");
  stations[0].audio_idx = 1;
  stations[0].positionA = (struct position){21.005243, 105.845490};
  stations[0].positionB = (struct position){21.005163, 105.845779}; 
  stations[0].positionC = (struct position){21.004923, 105.845801};
  stations[0].positionD = (struct position){21.004914, 105.845486};
  
  stations[1].num = 2;
  strcpy(stations[1].name, "tòa C7 và D3");
  stations[1].audio_idx = 2;
  stations[1].positionA = (struct position){21.005267, 105.844686};
  stations[1].positionB = (struct position){21.005243, 105.845490};
  stations[1].positionC = (struct position){21.004914, 105.845486};
  stations[1].positionD = (struct position){21.004861, 105.844646};
  
  stations[2].num = 3;
  strcpy(stations[2].name, "thư viện Tạ Quang Bửu");
  stations[2].audio_idx = 3;
  stations[2].positionA = (struct position){21.005259, 105.843734};
  stations[2].positionB = (struct position){21.005267, 105.844686};
  stations[2].positionC = (struct position){21.004861, 105.844646};
  stations[2].positionD = (struct position){21.004709, 105.843721};
  
  stations[3].num = 4;
  strcpy(stations[3].name, "tòa Alumi");
  stations[3].audio_idx = 4;
  stations[3].positionA = (struct position){21.005232, 105.843039};
  stations[3].positionB = (struct position){21.005259, 105.843734};
  stations[3].positionC = (struct position){21.004709, 105.843721};
  stations[3].positionD = (struct position){21.004819, 105.843045};
  
  stations[4].num = 5;
  strcpy(stations[4].name, "canteen Bách Khoa");
  stations[4].audio_idx = 5;
  stations[4].positionA = (struct position){21.005302, 105.842243};
  stations[4].positionB = (struct position){21.005232, 105.843039};
  stations[4].positionC = (struct position){21.004819, 105.843045};
  stations[4].positionD = (struct position){21.004924, 105.842307};
  
  stations[5].num = 6;
  strcpy(stations[5].name, "cổng Parabol");
  stations[5].audio_idx = 6;
  stations[5].positionA = (struct position){21.005360, 105.841371};
  stations[5].positionB = (struct position){21.005302, 105.842243};
  stations[5].positionC = (struct position){21.004924, 105.842307};
  stations[5].positionD = (struct position){21.005017, 105.841366};
  
  stations[6].num = 7;
  strcpy(stations[0].name, "hồ Tiền và tòa D6, D8");
  stations[6].audio_idx = 7;
  stations[6].positionA = (struct position){21.004641, 105.842852};
  stations[6].positionB = (struct position){21.004664, 105.843190};
  stations[6].positionC = (struct position){21.003786, 105.843172};
  stations[6].positionD = (struct position){21.003841, 105.842791};
}

