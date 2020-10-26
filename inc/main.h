#ifndef __main_h__
#define __main_h__
#include "lcd.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <cJSON.h>
void *picture();
void show_picture(char * name);
unsigned long file_size_get(const char *pfile_path);
void lcd_close(void);
int show_video_data(unsigned int x,unsigned int y,char *pjpg_buf,unsigned int jpg_buf_size);  
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path);
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color);
int mmap_lcd(void);
int lcd_open(void);
int chat();
#endif
