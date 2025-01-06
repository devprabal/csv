#include <string.h>
#include <stdio.h>

#include "ehs_serde_conversion_functions.h"

static bool convert_day(char day_in[4], char day_out[4], uint8_t out_buf_size)
{
    if(day_out && out_buf_size >= 4)
    {
        memcpy(day_out, day_in, 4);
        return true;
    }
    return false;
}

static bool convert_zone(char zone_in[3], char zone_out[3], uint8_t out_buf_size)
{
    if(zone_out && out_buf_size >= 3)
    {
        memcpy(zone_out, zone_in, 3);
        return true;
    }
    return false;
}

static bool convert_start_time(struct tm start_time_in, char start_time_out[6], uint8_t out_buf_size)
{
    if(start_time_out && out_buf_size >= 6)
    {
        strftime(start_time_out, out_buf_size, "%H:%M", &start_time_in);
        return true;
    }
    return false;
}

static bool convert_end_time(struct tm end_time_in, char end_time_out[6], uint8_t out_buf_size)
{
    if(end_time_out && out_buf_size >= 6)
    {
        strftime(end_time_out, out_buf_size, "%H:%M", &end_time_in);
        return true;
    }
    return false;
}

static bool convert_worked_flag(bool worked_flag_in, char worked_flag_out[2], uint8_t out_buf_size)
{
    if(worked_flag_out && out_buf_size >= 2)
    {
        snprintf(worked_flag_out, out_buf_size, "%d", worked_flag_in);
        return true;
    }
    return false;
}
 
static bool convert_enabled_flag(bool enabled_flag_in, char enabled_flag_out[2], uint8_t out_buf_size)
{
    if(enabled_flag_out && out_buf_size >= 2)
    {
        snprintf(enabled_flag_out, out_buf_size, "%d", enabled_flag_in);
        return true;
    }
    return false;
}
 
static bool convert_actuator_detail(Ehs_Sched_Actuator_Detail actuator_detail_in, char actuator_detail_out[11], uint8_t out_buf_size)
{
    if(actuator_detail_out && out_buf_size >= 11)
    {
        snprintf(actuator_detail_out, out_buf_size, "0x%02x%02x%04x", 
            actuator_detail_in.mode, actuator_detail_in.power, actuator_detail_in.temperature);
        return true;
    }
    return false;
}

static Ehs_Sched_Serde_Conversion_Funcs default_conversion_funcs = {
    .day_converter = convert_day,
    .zone_converter = convert_zone,
    .start_time_converter = convert_start_time,
    .end_time_converter = convert_end_time,
    .worked_flag_converter = convert_worked_flag,
    .enabled_flag_converter = convert_enabled_flag,
    .actuator_detail_converter = convert_actuator_detail,
};


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
