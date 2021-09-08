#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>   
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include "trace.h"

#ifdef __cplusplus
extern "C"{ 
#endif

static char  LOG_FILE_PATH[128] = "/home/log";
static int grobal_Trace_Leval = ERROR_TRACE; // 理论来说多线程的话需要初始化个锁来动态调整
static char nowLogName[128] = {0};
static pthread_mutex_t log_mutex; // 写日志锁
static int maxLogNum = 6; // 最多写6个日志文件  la; 0603
static int maxLogSize  = 8; // 最大10M
static  int logfd;

void openOneLogFile()
{
    struct stat buf;
    char tmpName[128] = {0};
    int i = 1;
    pthread_mutex_lock(&log_mutex);

    for (i = 1; i <= maxLogNum; i++) {
        memset(tmpName, 0, sizeof(tmpName));
        stat(tmpName, &buf);
        sprintf(tmpName, "%s%d.txt", LOG_FILE_PATH, i);

        
        if ((buf.st_size / (1024 * 1024)) <  maxLogSize) {
            logfd = open(tmpName, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG);
            printf("====%s\n", tmpName);
            memset(nowLogName, 0, sizeof(nowLogName));

            memcpy(nowLogName, tmpName, sizeof(tmpName) - 1);

            break;
        }

        if (i == maxLogNum) { // 最大值了，就循环写第六个吧不优化了 // 对第六个日志清零，有数据丢失风险 21.6.3
            logfd = open(tmpName, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG);
            memset(nowLogName, 0, sizeof(nowLogName));
            strcpy(nowLogName, tmpName);
            break;
        }
    }
    pthread_mutex_unlock(&log_mutex);
    return;
}

int SysLogInit()
{
    pthread_mutex_init(&log_mutex, NULL);
    openOneLogFile();
    return 0;
}

void Change_Grobal_Trace(const int leval)
{
    grobal_Trace_Leval = leval;
    return;
}

int Get_Grobal_Trace()
{
    return grobal_Trace_Leval;
}

int GetLocalTime(MY_TIME* sTime)
{
    time_t tmpTime;
    struct tm *localTime;

    time(&tmpTime);
    localTime = localtime(&tmpTime);
    sTime->mYear = (int)localTime->tm_year + 1900;
    sTime->mMonth = (int)localTime->tm_mon + 1;
    sTime->mDay = (int)localTime->tm_mday;
    sTime->mHour = (int)localTime->tm_hour;
    sTime->mMin = (int)localTime->tm_min;
    sTime->mSecond = (int)localTime->tm_sec;
    return 0;
}

int sys_trace(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    char buffer[256] = {0}; // 打印最长256
    vsprintf(buffer, format, arg);
    printf("%s", buffer);
    va_end(arg);
    return 0;
}

int sys_log_write(const char *format, ...)
{
    pthread_mutex_lock(&log_mutex);
    va_list arg;
    va_start(arg, format);
    char buffer[256] = {0}; // 打印最长256
    vsprintf(buffer, format, arg);
    write(logfd, buffer, sizeof(buffer));
    va_end(arg);
    pthread_mutex_unlock(&log_mutex);
    struct stat buf;
    stat(nowLogName, &buf);
    if (buf.st_size / (1024 * 1024) >  maxLogSize) {
        openOneLogFile();
    }
    return 0;
}

// int main()
// {
//     int i = 100;
//     if (SysLogInit() != 0) {
//         SYS_ERROR_TRACE("init log err");
//     }
//     int fd = open("/home/log.txt", O_WRONLY|O_CREAT);
//     write(fd, "it is", 10);
//     SYS_ERROR_TRACE("it is a err trace , %d, name = %s", i, "liangan");
//     SYS_INFO_TRACE("it is a info trace , %d, name = %s", i, "liangan");
//     SYS_WARN_TRACE("it is a warning trace, %d, name = %s", i, "liangan");
//     SYS_DEBUG_TRACE("it is a debug trace , %d, name = %s", i,"liangan");
//     SYS_ERROR_LOG("it is a log %s, %s,%s, %d\n", "liangan", "liangan", "liangan", 12312);
//     SYS_ERROR_LOG("it is a log %s, %s,%s, %d\n", "liangan", "liangan", "liangan", 12312);
//     return 0;
// }

#ifdef __cplusplus  
}
#endif