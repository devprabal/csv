#include "external_definitions_ehs_schd.h"
#include "adapter.h"

#include <stdio.h>
#include <string.h>

typedef struct Schd_Internal_Actuator_Rep Schd_Internal_Actuator_Rep;
struct Schd_Internal_Actuator_Rep {
    uint8_t mode;
    uint8_t power;
    int16_t temperature;
};

typedef struct Schd_Internal_Rep Schd_Internal_Rep;
struct Schd_Internal_Rep {
    char day[4];
    char zone[3];
    struct tm start_time;
    struct tm end_time;
    bool worked_flag;
    bool enabled_flag;
    Schd_Internal_Actuator_Rep actuator_detail;
};

static bool convert_day(void* day_in, char day_out[DAY_BUF_SIZE], uint8_t out_buf_size);
static bool convert_zone(void* zone_in, char zone_out[ZONE_BUF_SIZE], uint8_t out_buf_size);
static bool convert_start_time(void* start_time_in, char start_time_out[START_TIME_BUF_SIZE], uint8_t out_buf_size);
static bool convert_end_time(void* end_time_in, char end_time_out[END_TIME_BUF_SIZE], uint8_t out_buf_size);
static bool convert_worked_flag(void* worked_flag_in, char worked_flag_out[WORKED_FLAG_BUF_SIZE], uint8_t out_buf_size);
static bool convert_enabled_flag(void* enabled_flag_in, char enabled_flag_out[ENABLED_FLAG_BUF_SIZE], uint8_t out_buf_size);
static bool convert_actuator_detail(void* actuator_detail_in, char actuator_detail_out[ACTUATOR_DETAIL_BUF_SIZE], uint8_t out_buf_size);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                         conversion functions start                         //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

static bool convert_day(void* day_in, char day_out[DAY_BUF_SIZE], uint8_t out_buf_size)
{
    if(day_out && out_buf_size >= DAY_BUF_SIZE)
    {
        memcpy(day_out, day_in, DAY_BUF_SIZE);
        return true;
    }
    return false;
}

static bool convert_zone(void* zone_in, char zone_out[ZONE_BUF_SIZE], uint8_t out_buf_size)
{
    if(zone_out && out_buf_size >= ZONE_BUF_SIZE)
    {
        memcpy(zone_out, zone_in, ZONE_BUF_SIZE);
        return true;
    }
    return false;
}

static bool convert_start_time(void* start_time_in, char start_time_out[START_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(start_time_out && out_buf_size >= START_TIME_BUF_SIZE)
    {
        strftime(start_time_out, out_buf_size, "%H:%M", start_time_in);
        return true;
    }
    return false;
}

static bool convert_end_time(void* end_time_in, char end_time_out[END_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(end_time_out && out_buf_size >= END_TIME_BUF_SIZE)
    {
        strftime(end_time_out, out_buf_size, "%H:%M", end_time_in);
        return true;
    }
    return false;
}

static bool convert_worked_flag(void* worked_flag_in, char worked_flag_out[WORKED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(worked_flag_out && out_buf_size >= WORKED_FLAG_BUF_SIZE)
    {
        snprintf(worked_flag_out, out_buf_size, "%d", *((bool*)worked_flag_in));
        return true;
    }
    return false;
}

static bool convert_enabled_flag(void* enabled_flag_in, char enabled_flag_out[ENABLED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(enabled_flag_out && out_buf_size >= ENABLED_FLAG_BUF_SIZE)
    {
        snprintf(enabled_flag_out, out_buf_size, "%d", *((bool*)enabled_flag_in));
        return true;
    }
    return false;
}

static bool convert_actuator_detail(void* actuator_detail_in, char actuator_detail_out[ACTUATOR_DETAIL_BUF_SIZE], uint8_t out_buf_size)
{
    if(actuator_detail_out && out_buf_size >= ACTUATOR_DETAIL_BUF_SIZE)
    {
        snprintf(actuator_detail_out, out_buf_size, "0x%02x%02x%04x", 
            ((Schd_Internal_Actuator_Rep*)actuator_detail_in)->mode,
            ((Schd_Internal_Actuator_Rep*)actuator_detail_in)->power,
            ((Schd_Internal_Actuator_Rep*)actuator_detail_in)->temperature);
        return true;
    }
    return false;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                          conversion functions end                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

static Field_Information ehs_schd_fields_info[] = {
    {.index = DAY, .buf_size = DAY_BUF_SIZE, .converter = convert_day},
    {.index = ZONE, .buf_size = ZONE_BUF_SIZE, .converter = convert_zone},
    {.index = START_TIME, .buf_size = START_TIME_BUF_SIZE, .converter = convert_start_time},
    {.index = END_TIME, .buf_size = END_TIME_BUF_SIZE, .converter = convert_end_time},
    {.index = WORKED_FLAG, .buf_size = WORKED_FLAG_BUF_SIZE, .converter = convert_worked_flag},
    {.index = ENABLED_FLAG, .buf_size = ENABLED_FLAG_BUF_SIZE, .converter = convert_enabled_flag},
    {.index = ACTUATOR_DETAIL, .buf_size = ACTUATOR_DETAIL_BUF_SIZE, .converter = convert_actuator_detail},
};

uint8_t get_number_of_fields(void)
{
    return sizeof(ehs_schd_fields_info)/sizeof(Field_Information);
}

Field_Information* get_all_field_information(void)
{
    return ehs_schd_fields_info;
}

Schd_Internal_Rep obj[3] = {
    [0] = {.day = "Sat", .zone = "z1", .start_time = {.tm_hour = 13, .tm_min = 2}, .end_time = {.tm_hour = 14, .tm_min = 9}, .worked_flag = 0, .enabled_flag = 1, .actuator_detail = {.mode = 0x42, .power = 0xF0, .temperature = 0x072D}},
    [1] = {.day = "Wed", .zone = "z1", .start_time = {.tm_hour = 3, .tm_min = 23}, .end_time = {.tm_hour = 11, .tm_min = 3}, .worked_flag = 1, .enabled_flag = 1, .actuator_detail = {.mode = 0x32, .power = 0x0F, .temperature = 0x081D}},
    [2] = {.day = "Mon", .zone = "z2", .start_time = {.tm_hour = 21, .tm_min = 2}, .end_time = {.tm_hour = 23, .tm_min = 9}, .worked_flag = 0, .enabled_flag = 1, .actuator_detail = {.mode = 0x22, .power = 0xF0, .temperature = 0x062D}},
};

void* get_field_data(int row_idx, int field_idx)
{
    int n_rows = sizeof(obj)/sizeof(Schd_Internal_Rep);
    if(row_idx >= n_rows) return NULL;

    Schd_Internal_Rep* row = &(obj[row_idx]);
    if(field_idx <= ACTUATOR_DETAIL && field_idx >= DAY && row)
    {
        ////TODO: srid switch case is easy to forget when extending fields
        ////TODO: srid move it to ehs_schd_fields_info
        switch (field_idx)
        {
        case DAY:
            return row->day;
            break;
        case ZONE:
            return row->zone;
            break;
        case START_TIME:
            return &(row->start_time);
            break;
        case END_TIME:
            return &(row->end_time);
            break;
        case WORKED_FLAG:
            return &(row->worked_flag);
            break;
        case ENABLED_FLAG:
            return &(row->enabled_flag);
            break;
        case ACTUATOR_DETAIL:
            return &(row->actuator_detail);
            break;
        default:
            break;
        }
    }
    return NULL;
}

void test_ehs_schd_internal_to_csv_conversion(void)
{
    Csv_Config config = {
        .file_name = "oct31.csv",
        .n_fields = get_number_of_fields(), ////TODO: srid you need to ensure that n_fields > fields_info[any].index
        .field_info = get_all_field_information(),
        .get_col_data = get_field_data,
    };
    if(true == csv_init(config))
    {
        int n_rows = sizeof(obj)/sizeof(Schd_Internal_Rep);
        v3(config, n_rows, get_number_of_fields());
    }
}
