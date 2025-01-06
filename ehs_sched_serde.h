#pragma once

#include <time.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct Ehs_Sched_CSV_Rep Ehs_Sched_CSV_Rep;
struct Ehs_Sched_CSV_Rep {
    char* day;
    char* zone;
    char* start_time;
    char* end_time;
    char* worked_flag;
    char* enabled_flag;
    char* actuator_detail;
};

typedef struct Ehs_Sched_Actuator_Detail Ehs_Sched_Actuator_Detail;
struct Ehs_Sched_Actuator_Detail {
    uint8_t mode;
    uint8_t power;
    int16_t temperature;
};

typedef struct Ehs_Sched_Serde_Rep Ehs_Sched_Serde_Rep;
struct Ehs_Sched_Serde_Rep {
    char day[4];
    char zone[3];
    struct tm start_time;
    struct tm end_time;
    bool worked_flag;
    bool enabled_flag;
    Ehs_Sched_Actuator_Detail actuator_detail;
};
