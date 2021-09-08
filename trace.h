#ifndef __MY_TRACE__H__
#define __MY_TRACE__H__


#ifdef __cplusplus
extern "C"{ 
#endif

//#include <sys/time.h>
#include <time.h>
#pragma pack(1) 

typedef enum Trace_Leval {
    CLOSE_TRACE,
    DEBUG_TRACE = 1,
    INFO_TRACE,
    WARN_TRACE,
    ERROR_TRACE
} TRACE_LEVAL;

typedef struct My_Time {
    int mYear;
    int mMonth;
    int mDay;
    int mHour;
    int mMin;
    int mSecond;
} MY_TIME;

int GetLocalTime(MY_TIME* sTime);
int Get_Grobal_Trace();
void Change_Grobal_Trace(const int leval);
int sys_trace(const char *format, ...);

#define SYS_ERROR_TRACE(format, ...) \
{ \
        if (Get_Grobal_Trace() >= ERROR_TRACE) \
        { \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            printf("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            printf("[ERROR] function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_trace(format, ##__VA_ARGS__); \
            printf("\n"); \
        } \
} \

#define SYS_WARN_TRACE(format, ...) \
{ \
        if (Get_Grobal_Trace() >= WARN_TRACE) \
        { \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            printf("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            printf("[WARN]  function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_trace(format, ##__VA_ARGS__); \
            printf("\n"); \
        } \
} \

#define SYS_INFO_TRACE(format, ...) \
{ \
        if (Get_Grobal_Trace() >= INFO_TRACE) \
        { \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            printf("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            printf("[INFO]  function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_trace(format, ##__VA_ARGS__); \
            printf("\n"); \
        } \
} \



#define SYS_DEBUG_TRACE(format, ...) \
{   \
        if (Get_Grobal_Trace() >= DEBUG_TRACE) \
        { \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            printf("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            printf("[DEBUG] function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_trace(format, ##__VA_ARGS__); \
            printf("\n"); \
        } \
} \

/****************************************************** LOG 的宏 ****************************************************/
#define SYS_ERROR_LOG(format, ...) \
{ \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            sys_log_write("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            sys_log_write("[ERROR] function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_log_write(format, ##__VA_ARGS__); \
            sys_log_write("\n"); \
} \

#define SYS_WARN_LOG(format, ...) \
{ \
            MY_TIME tmpTime; \
            sys_log_write(&tmpTime); \
            sys_log_write("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            sys_log_write("[WARN]  function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_log_write(format, ##__VA_ARGS__); \
            sys_log_write("\n"); \
} \

#define SYS_INFO_LOG(format, ...) \
{ \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            sys_log_write("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            sys_log_write("[INFO]  function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_log_write(format, ##__VA_ARGS__); \
            sys_log_write("\n"); \
} \

// 后面的换行符可以优化掉
#define SYS_DEBUG_LOG(format, ...) \
{   \
            MY_TIME tmpTime; \
            GetLocalTime(&tmpTime); \
            sys_log_write("[%2d-%2d %2d:%2d:%2d] ", tmpTime.mMonth, tmpTime.mDay, tmpTime.mHour, tmpTime.mMin, tmpTime.mSecond); \
            sys_log_write("[DEBUG] function:%s line:%d: ", __FUNCTION__, __LINE__); \
            sys_log_write(format, ##__VA_ARGS__); \
            sys_log_write("\n"); \
} \



#ifdef __cplusplus 
}
#endif

#endif