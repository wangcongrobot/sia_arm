# include <stdio.h>
# include <fstream>
# include <sys/time.h>
# include <time.h>
#include <ros/ros.h>

struct timeval    tv; 
struct timezone   tz;      
struct tm         *p;
char hhour[2] = { 0,0 }, mmin[2] = { 0,0 }, ssec[2] = { 0,0 };
int  shiw[3] = { 0,0,0 }, gew[3] = { 0,0,0 };

void SD_dataRecord(uint8_t* sd_msg, char j, char b, char c, char d); //记录数据整体 j代表关节 bcd代表时间间隔
void sysUsecTime(); 
char int2char(int num);