#include "definitions.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                         conversion functions start                         //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

bool convert_day(char day_in[DAY_BUF_SIZE], char day_out[DAY_BUF_SIZE], uint8_t out_buf_size)
{
    if(day_out && out_buf_size >= DAY_BUF_SIZE)
    {
        memcpy(day_out, day_in, DAY_BUF_SIZE);
        return true;
    }
    return false;
}

bool convert_zone(char zone_in[ZONE_BUF_SIZE], char zone_out[ZONE_BUF_SIZE], uint8_t out_buf_size)
{
    if(zone_out && out_buf_size >= ZONE_BUF_SIZE)
    {
        memcpy(zone_out, zone_in, ZONE_BUF_SIZE);
        return true;
    }
    return false;
}

bool convert_start_time(struct tm* start_time_in, char start_time_out[START_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(start_time_out && out_buf_size >= START_TIME_BUF_SIZE)
    {
        strftime(start_time_out, out_buf_size, "%H:%M", start_time_in);
        return true;
    }
    return false;
}

bool convert_end_time(struct tm* end_time_in, char end_time_out[END_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(end_time_out && out_buf_size >= END_TIME_BUF_SIZE)
    {
        strftime(end_time_out, out_buf_size, "%H:%M", end_time_in);
        return true;
    }
    return false;
}

bool convert_worked_flag(bool* worked_flag_in, char worked_flag_out[WORKED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(worked_flag_out && out_buf_size >= WORKED_FLAG_BUF_SIZE)
    {
        snprintf(worked_flag_out, out_buf_size, "%d", *worked_flag_in);
        return true;
    }
    return false;
}
 
bool convert_enabled_flag(bool* enabled_flag_in, char enabled_flag_out[ENABLED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(enabled_flag_out && out_buf_size >= ENABLED_FLAG_BUF_SIZE)
    {
        snprintf(enabled_flag_out, out_buf_size, "%d", *enabled_flag_in);
        return true;
    }
    return false;
}
 
bool convert_actuator_detail(Ehs_Sched_Actuator_Detail* actuator_detail_in, char actuator_detail_out[ACTUATOR_DETAIL_BUF_SIZE], uint8_t out_buf_size)
{
    if(actuator_detail_out && out_buf_size >= ACTUATOR_DETAIL_BUF_SIZE)
    {
        snprintf(actuator_detail_out, out_buf_size, "0x%02x%02x%04x", 
            actuator_detail_in->mode, actuator_detail_in->power, actuator_detail_in->temperature);
        return true;
    }
    return false;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                          conversion functions end                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//


// generic_conversion_func_t field_conversion_funcs[] = {
//     [DAY] = convert_day,
//     [ZONE] = convert_zone,
//     [START_TIME] = convert_start_time,
//     [END_TIME] = convert_end_time,
//     [WORKED_FLAG] = convert_worked_flag,
//     [ENABLED_FLAG] = convert_enabled_flag,
//     [ACTUATOR_DETAIL] = convert_actuator_detail,
// };

Field_Information ehs_fields_info[] = {
    {.index = DAY, .buf_size = DAY_BUF_SIZE, .converter = convert_day};
    {.index = ZONE, .buf_size = ZONE_BUF_SIZE, .converter = convert_zone};
    {.index = START_TIME, .buf_size = START_TIME_BUF_SIZE, .converter = convert_start_time};
    {.index = END_TIME, .buf_size = END_TIME_BUF_SIZE, .converter = convert_end_time};
    {.index = WORKED_FLAG, .buf_size = WORKED_FLAG_BUF_SIZE, .converter = convert_worked_flag};
    {.index = ENABLED_FLAG, .buf_size = ENABLED_FLAG_BUF_SIZE, .converter = convert_enabled_flag};
    {.index = ACTUATOR_DETAIL, .buf_size = ACTUATOR_DETAIL_BUF_SIZE, .converter = convert_actuator_detail};
};

void set_ehs_sched_field_information(void)
{
    size_t n_fields = sizeof(ehs_fields_info)/sizeof(Field_Information);
    set_field_information(n_fields, &ehs_fields_info);
}
