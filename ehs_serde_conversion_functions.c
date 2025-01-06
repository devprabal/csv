#include <string.h>
#include <stdio.h>

#include "ehs_serde_conversion_functions.h"

typedef enum CONVERSION_FUNCS_BUF_SIZES {
    DAY_BUF_SIZE = 4,
    ZONE_BUF_SIZE = 3,
    START_TIME_BUF_SIZE = 6,
    END_TIME_BUF_SIZE = 6,
    WORKED_FLAG_BUF_SIZE = 2,
    ENABLED_FLAG_BUF_SIZE = 2,
    ACTUATOR_DETAIL_BUF_SIZE = 11,
};

static bool convert_day(char day_in[DAY_BUF_SIZE], char day_out[DAY_BUF_SIZE], uint8_t out_buf_size)
{
    if(day_out && out_buf_size >= DAY_BUF_SIZE)
    {
        memcpy(day_out, day_in, DAY_BUF_SIZE);
        return true;
    }
    return false;
}

static bool convert_zone(char zone_in[ZONE_BUF_SIZE], char zone_out[ZONE_BUF_SIZE], uint8_t out_buf_size)
{
    if(zone_out && out_buf_size >= ZONE_BUF_SIZE)
    {
        memcpy(zone_out, zone_in, ZONE_BUF_SIZE);
        return true;
    }
    return false;
}

static bool convert_start_time(struct tm start_time_in, char start_time_out[START_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(start_time_out && out_buf_size >= START_TIME_BUF_SIZE)
    {
        strftime(start_time_out, out_buf_size, "%H:%M", &start_time_in);
        return true;
    }
    return false;
}

static bool convert_end_time(struct tm end_time_in, char end_time_out[END_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(end_time_out && out_buf_size >= END_TIME_BUF_SIZE)
    {
        strftime(end_time_out, out_buf_size, "%H:%M", &end_time_in);
        return true;
    }
    return false;
}

static bool convert_worked_flag(bool worked_flag_in, char worked_flag_out[WORKED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(worked_flag_out && out_buf_size >= WORKED_FLAG_BUF_SIZE)
    {
        snprintf(worked_flag_out, out_buf_size, "%d", worked_flag_in);
        return true;
    }
    return false;
}
 
static bool convert_enabled_flag(bool enabled_flag_in, char enabled_flag_out[ENABLED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(enabled_flag_out && out_buf_size >= ENABLED_FLAG_BUF_SIZE)
    {
        snprintf(enabled_flag_out, out_buf_size, "%d", enabled_flag_in);
        return true;
    }
    return false;
}
 
static bool convert_actuator_detail(Ehs_Sched_Actuator_Detail actuator_detail_in, char actuator_detail_out[ACTUATOR_DETAIL_BUF_SIZE], uint8_t out_buf_size)
{
    if(actuator_detail_out && out_buf_size >= ACTUATOR_DETAIL_BUF_SIZE)
    {
        snprintf(actuator_detail_out, out_buf_size, "0x%02x%02x%04x", 
            actuator_detail_in.mode, actuator_detail_in.power, actuator_detail_in.temperature);
        return true;
    }
    return false;
}

static Ehs_Sched_Serde_Conversion_Funcs default_conversion_funcs = {
    .day_converter = convert_day,
    .day_buf_size = DAY_BUF_SIZE,

    .zone_converter = convert_zone,
    .zone_buf_size = ZONE_BUF_SIZE,

    .start_time_converter = convert_start_time,
    .start_time_buf_size = START_TIME_BUF_SIZE,

    .end_time_converter = convert_end_time,
    .end_time_buf_size = END_TIME_BUF_SIZE,

    .worked_flag_converter = convert_worked_flag,
    .worked_flag_buf_size = WORKED_FLAG_BUF_SIZE,

    .enabled_flag_converter = convert_enabled_flag,
    .enabled_flag_buf_size = ENABLED_FLAG_BUF_SIZE,

    .actuator_detail_converter = convert_actuator_detail,
    .actuator_detail_buf_size = ACTUATOR_DETAIL_BUF_SIZE,
};

// static Ehs_Sched_Serde_Conversion_Buf_Sizes default_conversion_buf_sizes = {
//     .day_buf_size = 
// }

#define EHS_SERDE_SET_CONVERSION_MEMBER(custom, default) \
do {  if(custom) default = custom; } while (false);


void ehs_serde_set_conversion_functions(Ehs_Sched_Serde_Conversion_Funcs conversion_funcs)
{
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.day_converter, default_conversion_funcs.day_converter);
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.zone_converter, default_conversion_funcs.zone_converter);
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.start_time_converter, default_conversion_funcs.start_time_converter);
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.end_time_converter, default_conversion_funcs.end_time_converter);
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.worked_flag_converter, default_conversion_funcs.worked_flag_converter);
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.enabled_flag_converter, default_conversion_funcs.enabled_flag_converter);
    EHS_SERDE_SET_CONVERSION_MEMBER(conversion_funcs.actuator_detail_converter, default_conversion_funcs.actuator_detail_converter);
}

Ehs_Sched_Serde_Conversion_Funcs ehs_serde_get_conversion_functions(void)
{
    Ehs_Sched_Serde_Conversion_Funcs conversion_funcs = default_conversion_funcs;
    return conversion_funcs;
}
