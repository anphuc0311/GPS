#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  //thao tac voi file
#include <termios.h> // config uart
#include <unistd.h> //thao tac voi cong serial
#include <string.h>
#include <sys/types.h>
#include "../lib/gps.h"


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
            for (int i = 0; i < station_count; i++) {
              if (checkStation(&curr, &stations[i]) == 1) {
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








//void process_gps() {
//}
