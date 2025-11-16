#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Global variable definitions */
position curr;
station stations[10];
int station_count = 7;

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

int checkStation(position *position_to_check, station *station_to_check) {
  float cross_product1;
  float cross_product2;
  float cross_product3;
  float cross_product4;
  cross_product1 = cross_product((station_to_check->positionB.latitude - station_to_check->positionA.latitude), 
  (station_to_check->positionB.longitude - station_to_check->positionA.longitude), 
  (position_to_check->latitude - station_to_check->positionA.latitude), 
  (position_to_check->longitude - station_to_check->positionA.longitude));
  cross_product2 = cross_product((station_to_check->positionC.latitude - station_to_check->positionB.latitude), 
  (station_to_check->positionC.longitude - station_to_check->positionB.longitude), 
  (position_to_check->latitude - station_to_check->positionB.latitude), 
  (position_to_check->longitude - station_to_check->positionB.longitude));
  cross_product3 = cross_product((station_to_check->positionD.latitude - station_to_check->positionC.latitude), 
  (station_to_check->positionD.longitude - station_to_check->positionC.longitude), 
  (position_to_check->latitude - station_to_check->positionC.latitude), 
  (position_to_check->longitude - station_to_check->positionC.longitude));
  cross_product4 = cross_product((station_to_check->positionA.latitude - station_to_check->positionD.latitude), 
  (station_to_check->positionA.longitude - station_to_check->positionD.longitude), 
  (position_to_check->latitude - station_to_check->positionD.latitude), 
  (position_to_check->longitude - station_to_check->positionD.longitude));
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