
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define EHS_SCHED_CSV_FILE_NEW "reserv.csv.new"
size_t EXPECTED_LEN_ONE_CSV_ROW = 40; //// bytes
size_t EXPECTED_CSV_COLS = 1;
typedef bool(*generic_conversion_func_t)(void* in, char* out, uint8_t out_buf_size);
typedef struct CSV_Col CSV_Col;
struct CSV_Col {
    generic_conversion_func_t conversion_func;
    uint32_t out_buf_size;
};

CSV_Col** one_csv_row = 0;

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

Ehs_Sched_Serde_Rep obj[3] = {
    [0] = {.day = "Sat", .zone = "z1", .start_time = {.tm_hour = 13, .tm_min = 2}, .end_time = {.tm_hour = 14, .tm_min = 9}, .worked_flag = 0, .enabled_flag = 1, .actuator_detail = {.mode = 0x42, .power = 0xF0, .temperature = 0x072D}},
    [1] = {.day = "Wed", .zone = "z1", .start_time = {.tm_hour = 3, .tm_min = 23}, .end_time = {.tm_hour = 11, .tm_min = 3}, .worked_flag = 1, .enabled_flag = 1, .actuator_detail = {.mode = 0x32, .power = 0x0F, .temperature = 0x081D}},
    [2] = {.day = "Mon", .zone = "z2", .start_time = {.tm_hour = 21, .tm_min = 2}, .end_time = {.tm_hour = 23, .tm_min = 9}, .worked_flag = 0, .enabled_flag = 1, .actuator_detail = {.mode = 0x22, .power = 0xF0, .temperature = 0x062D}},
};

void* get_item_from_user_rep(size_t index)
{
    if(index < 3) return &(obj[index]);
    return 0;
}

void* return_field_data_from_rep(void* obj_in, FIELD_NAMES field_name)
{
    Ehs_Sched_Serde_Rep* obj_cast = (Ehs_Sched_Serde_Rep*)obj_in;
    if(field_name <= ACTUATOR_DETAIL && field_name >= DAY && obj_cast)
    {
        switch (field_name)
        {
        case DAY:
            return obj_cast->day;
            break;
        case ZONE:
            return obj_cast->zone;
            break;
        case START_TIME:
            return &(obj_cast->start_time);
            break;
        case END_TIME:
            return &(obj_cast->end_time);
            break;
        case WORKED_FLAG:
            return &(obj_cast->worked_flag);
            break;
        case ENABLED_FLAG:
            return &(obj_cast->enabled_flag);
            break;
        case ACTUATOR_DETAIL:
            return &(obj_cast->actuator_detail);
            break;
        default:
            break;
        }
    }
    return 0;
}

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

uint32_t field_sizes[] = {
    [DAY] = DAY_BUF_SIZE,
    [ZONE] = ZONE_BUF_SIZE,
    [START_TIME] = START_TIME_BUF_SIZE,
    [END_TIME] = END_TIME_BUF_SIZE,
    [WORKED_FLAG] = WORKED_FLAG_BUF_SIZE,
    [ENABLED_FLAG] = ENABLED_FLAG_BUF_SIZE,
    [ACTUATOR_DETAIL] = ACTUATOR_DETAIL_BUF_SIZE,
};

CONVERSION_FUNCS_BUF_SIZES return_field_data_size(FIELD_NAMES field_name)
{
    if(field_name <= ACTUATOR_DETAIL && field_name >= DAY)
    {
        return field_sizes[field_name];
    }
    return INVALID_BUF_SIZE;
}

size_t sum_all_field_buf_sizes(void)
{
    size_t sum = 0;
    uint32_t fields[] = {
        DAY,
        ZONE,
        START_TIME,
        END_TIME,
        WORKED_FLAG,
        ENABLED_FLAG,
        ACTUATOR_DETAIL,
    };

    size_t n_fields = sizeof(fields)/sizeof(fields[0]);
    for(size_t i = 0; i < n_fields; i++)
    {
        sum += return_field_data_size(fields[i]);
    }
    return sum;
}

size_t get_number_of_fields(void)
{
    //// TODO: make fields common for this and sum_all_field_buf_sizes() functions
    uint32_t fields[] = {
        DAY,
        ZONE,
        START_TIME,
        END_TIME,
        WORKED_FLAG,
        ENABLED_FLAG,
        ACTUATOR_DETAIL,
    };

    size_t n_fields = sizeof(fields)/sizeof(fields[0]);
    return n_fields;
}

size_t get_expected_len_one_csv_row(void)
{
    return EXPECTED_LEN_ONE_CSV_ROW;
}
void* get_field_from_user_item(void* obj, size_t member_index)
{
    return return_field_data_from_rep(obj, member_index);
}

bool Ehs_Sched_serialize_to_csv_file_v2(uint32_t n_rows, uint32_t n_columns)
{
    if(0 == n_rows || 0 == n_columns) return false;

    FILE* fptr = 0;
    fptr = fopen(EHS_SCHED_CSV_FILE_NEW, "w");
    // fptr = fopen(EHS_SCHED_CSV_FILE_NEW, "a");

    uint32_t built_string_size = (3 * get_expected_len_one_csv_row()) + 1; //// '%s,'
    char built_string[built_string_size];
    uint32_t bstr_idx = 0;
    built_string[bstr_idx] = '\0';

    for(uint32_t row = 0; row < n_rows; row++)
    {
        for(uint32_t col = 0; col < n_columns; col++)
        {
            void* usr_field_data = get_field_from_user_item(get_item_from_user_rep(row), col);
            uint32_t tsize = return_field_data_size(col);
            if(INVALID_BUF_SIZE != tsize)
            {
                char out[tsize];
                one_csv_row[col]->conversion_func(usr_field_data, out, tsize);
                //// TODO: srid replace strncat with snprintf
                strncat(built_string, out, tsize);
                bstr_idx += tsize;
                strncat(built_string, ", ", 3); //// TODO: srid use user defined delimiter macro
                bstr_idx += 3;
            }
        }
        
        if(fptr)
        {
            //// TODO: srid do this outside all for-loops
            //// allocate built_string for all rows together, instead of writing one row at a time.
            fprintf(fptr, "%s\n", built_string);
        }
        bstr_idx = 0;
        built_string[bstr_idx] = '\0';
    }
    fclose(fptr);
    
    return true;
}

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

static bool convert_start_time(struct tm* start_time_in, char start_time_out[START_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(start_time_out && out_buf_size >= START_TIME_BUF_SIZE)
    {
        strftime(start_time_out, out_buf_size, "%H:%M", start_time_in);
        return true;
    }
    return false;
}

static bool convert_end_time(struct tm* end_time_in, char end_time_out[END_TIME_BUF_SIZE], uint8_t out_buf_size)
{
    if(end_time_out && out_buf_size >= END_TIME_BUF_SIZE)
    {
        strftime(end_time_out, out_buf_size, "%H:%M", end_time_in);
        return true;
    }
    return false;
}

static bool convert_worked_flag(bool* worked_flag_in, char worked_flag_out[WORKED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(worked_flag_out && out_buf_size >= WORKED_FLAG_BUF_SIZE)
    {
        snprintf(worked_flag_out, out_buf_size, "%d", *worked_flag_in);
        return true;
    }
    return false;
}
 
static bool convert_enabled_flag(bool* enabled_flag_in, char enabled_flag_out[ENABLED_FLAG_BUF_SIZE], uint8_t out_buf_size)
{
    if(enabled_flag_out && out_buf_size >= ENABLED_FLAG_BUF_SIZE)
    {
        snprintf(enabled_flag_out, out_buf_size, "%d", *enabled_flag_in);
        return true;
    }
    return false;
}
 
static bool convert_actuator_detail(Ehs_Sched_Actuator_Detail* actuator_detail_in, char actuator_detail_out[ACTUATOR_DETAIL_BUF_SIZE], uint8_t out_buf_size)
{
    if(actuator_detail_out && out_buf_size >= ACTUATOR_DETAIL_BUF_SIZE)
    {
        snprintf(actuator_detail_out, out_buf_size, "0x%02x%02x%04x", 
            actuator_detail_in->mode, actuator_detail_in->power, actuator_detail_in->temperature);
        return true;
    }
    return false;
}

generic_conversion_func_t field_conversion_funcs[] = {
    [DAY] = convert_day,
    [ZONE] = convert_zone,
    [START_TIME] = convert_start_time,
    [END_TIME] = convert_end_time,
    [WORKED_FLAG] = convert_worked_flag,
    [ENABLED_FLAG] = convert_enabled_flag,
    [ACTUATOR_DETAIL] = convert_actuator_detail,
};

bool init_one_csv_row(size_t n_cols)
{
    one_csv_row = calloc(n_cols, sizeof(CSV_Col*));
    for(size_t i = 0; i < n_cols; i++) {
        one_csv_row[i] = calloc(1, sizeof(CSV_Col));
    }
    return true;
}

bool set_all_conversion_funcs(size_t n_funcs, generic_conversion_func_t conversion_funcs[n_funcs], uint32_t out_buf_sizes[n_funcs])
{
    if(!one_csv_row) {
        init_one_csv_row(n_funcs);
    }
    for(size_t i = 0; i < n_funcs; i++) {
        if(one_csv_row[i]) {
            one_csv_row[i]->conversion_func = conversion_funcs[i];
            one_csv_row[i]->out_buf_size = out_buf_sizes[i];
        }
    }
    return true;
}

void set_all_user_defined_conversion_funcs_and_buf_sizes(void)
{
    size_t n_funcs = sizeof(field_conversion_funcs) / sizeof (field_conversion_funcs [0]);
    set_all_conversion_funcs(n_funcs, field_conversion_funcs, field_sizes);
}

bool set_expected_len_one_csv_row(size_t bytes)
{
    //// 120 lines with 200 bytes per line ~23.5 kB file size
    //// Assumption: keep csv file size within 25 kB
    if(bytes <= 200) {
        EXPECTED_LEN_ONE_CSV_ROW = bytes;
        return true;
    } else {
        printf("\nOh you must be joking! Reduce line len to <= 200 bytes.\n");
        return false;
    }
}
bool set_expected_csv_cols(size_t n_cols)
{
    //// 120 lines with 200 bytes per line ~23.5 kB file size
    //// Assumption: keep csv file size within 25 kB
    //// Assumption: csv line format: `item1,item2,item3,`
    //// There can be 100 columns of 1 byte each to make a max of 200 bytes per line
    if(n_cols <= 100) {
        EXPECTED_CSV_COLS = n_cols;
        return true;
    } else {
        printf("\nOh you must be joking! Reduce number of columns to <= 100.\n");
        return false;
    }
}

void test_v2(void)
{
    // set_expected_len_one_csv_row(40);
    set_expected_len_one_csv_row(sum_all_field_buf_sizes());
    // set_expected_csv_cols(7);
    set_expected_csv_cols(get_number_of_fields());
    set_all_user_defined_conversion_funcs_and_buf_sizes();
    Ehs_Sched_serialize_to_csv_file_v2(3,get_number_of_fields());
}

int main(void)
{
    test_v2();
    return 0;
}
