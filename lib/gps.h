#ifndef GPS_H
#define GPS_H

struct position {
    float latitude;
    float longitude;
};
typedef struct position position;

struct station {
    int num;
    char name[100];
    int audio_idx;
    struct position positionA;
    struct position positionB;
    struct position positionC;
    struct position positionD;
};
typedef struct station station;

extern position curr;
extern station stations[10];
extern int station_count;

float convertToDecimal(float raw, char dir);
void tachGPGLL(char *line);
float cross_product(float x1,float y1,float x2,float y2);
int checkStation( position *position_to_check, station *station_to_check);
void initStations(void); 

#endif
