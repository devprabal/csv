#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "api/api.h"

//// TODO: srid write to new file, delete old file, rename new file to old file
#define EHS_SCHED_CSV_FILE_NEW "reserv.csv.new"

typedef enum FIELD_NAMES {
    //// TODO: srid this should be from 0 to x all continuous values
    //// else the for-loops at many places will behave strangely.
    //// Better not do this then. Or make some rules.
    DAY,
    ZONE,
    START_TIME,
    END_TIME,
    WORKED_FLAG,
    ENABLED_FLAG,
    ACTUATOR_DETAIL,
} FIELD_NAMES;

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

typedef enum CONVERSION_FUNCS_BUF_SIZES {
    DAY_BUF_SIZE = 4,
    ZONE_BUF_SIZE = 3,
    START_TIME_BUF_SIZE = 6,
    END_TIME_BUF_SIZE = 6,
    WORKED_FLAG_BUF_SIZE = 2,
    ENABLED_FLAG_BUF_SIZE = 2,
    ACTUATOR_DETAIL_BUF_SIZE = 11,
    INVALID_BUF_SIZE = 0,
} CONVERSION_FUNCS_BUF_SIZES;



bool convert_day(char day_in[DAY_BUF_SIZE], char day_out[DAY_BUF_SIZE], uint8_t out_buf_size);
bool convert_zone(char zone_in[ZONE_BUF_SIZE], char zone_out[ZONE_BUF_SIZE], uint8_t out_buf_size);
bool convert_start_time(struct tm* start_time_in, char start_time_out[START_TIME_BUF_SIZE], uint8_t out_buf_size);
bool convert_end_time(struct tm* end_time_in, char end_time_out[END_TIME_BUF_SIZE], uint8_t out_buf_size);
bool convert_worked_flag(bool* worked_flag_in, char worked_flag_out[WORKED_FLAG_BUF_SIZE], uint8_t out_buf_size);
bool convert_enabled_flag(bool* enabled_flag_in, char enabled_flag_out[ENABLED_FLAG_BUF_SIZE], uint8_t out_buf_size);
bool convert_actuator_detail(Ehs_Sched_Actuator_Detail* actuator_detail_in, char actuator_detail_out[ACTUATOR_DETAIL_BUF_SIZE], uint8_t out_buf_size);

void set_ehs_sched_field_information(void);
