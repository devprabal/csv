#include "ehs_sched_serde.h"
#include "ehs_serde_conversion_functions.h"
#include <string.h> //// strncat
size_t EXPECTED_CSV_COLS = 1;
size_t EXPECTED_LEN_ONE_CSV_ROW = 40; //// bytes
CSV_Col** one_csv_row = 0;

typedef struct Ehs_Sched_Internal_CSV_Rep Ehs_Sched_Internal_CSV_Rep;
struct Ehs_Sched_Internal_CSV_Rep {
    char day[4];
    char zone[3];
    char start_time[6];
    char end_time[6];
    char worked_flag[2];
    char enabled_flag[2];
    char actuator_detail[11];
};


#define EHS_SCHED_SERDE_MAX_CSV_ITEMS (211)
static Ehs_Sched_Internal_CSV_Rep csv_items[EHS_SCHED_SERDE_MAX_CSV_ITEMS];

// static Ehs_Sched_Serde_Conversion_Funcs conversion_funcs = {0, };



// void Ehs_Sched_Serde_to_CSV(Ehs_Sched_Serde_Rep *serde_rep, Ehs_Sched_CSV_Rep *csv_rep)
// {
//     conversion_funcs = ehs_serde_get_conversion_functions();
    
// }

#define EHS_SCHED_CSV_FILE_ORG "reserv.csv"
#define EHS_SCHED_CSV_FILE_NEW "reserv.csv.new"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// typedef struct Ehs_Sched_CSV_Rep Ehs_Sched_CSV_Rep;
// struct Ehs_Sched_CSV_Rep {
//     char* day;
//     char* zone;
//     char* start_time;
//     char* end_time;
//     char* worked_flag;
//     char* enabled_flag;
//     char* actuator_detail;
// };

static char ehs_sched_csv_delimiter = ',';
static void ehs_sched_set_csv_delimiter(char delim)
{
    ehs_sched_csv_delimiter = delim;
}
static char ehs_sched_get_csv_delimiter(void)
{
    return ehs_sched_csv_delimiter;
}

void print_all_columns_to_file(FILE* fptr, char* format, Ehs_Sched_CSV_Rep* data)
{
    fprintf(fptr, format, 
        data->day, data->zone, data->start_time, data->end_time, 
        data->worked_flag, data->enabled_flag, data->actuator_detail);
}

#define EHS_SCHED_CSV_DELIMITER ehs_sched_get_csv_delimiter()

#define EHS_SCHED_CALLOC_TEMP_OUT_BUF(member, size) \
do { member = calloc(1, size); } while (false);

#define EHS_SCHED_FREE_TEMP_OUT_BUF(member, size) \
do { free(member); member = NULL; } while (false);


// bool Ehs_Sched_serialize_to_csv_file(Ehs_Sched_CSV_Rep* data, uint32_t n_rows, uint32_t n_columns)
// {
//     if(!data || 0 == n_rows || 0 == n_columns) return false;

//     FILE* fptr = NULL;

//     uint32_t format_buf_size = (3 * n_columns) + 1; //// '%s,'
//     char format[format_buf_size];
//     memset(format, 0, sizeof(format));

//     for(uint32_t cols = 0, idx = 0; cols < n_columns; cols++, idx += 3)
//     {
//         format[idx] = '%';
//         format[idx+1] = 's';
//         format[idx+2] = EHS_SCHED_CSV_DELIMITER;
//     }
//     format[format_buf_size - 1] = '\n';

//     fptr = fopen(EHS_SCHED_CSV_FILE_NEW, "w");
//     if(fptr)
//     {
//         for(uint32_t row = 0; row < n_rows; row++)
//         {
//             print_all_columns_to_file(fptr, format, data);
//             data++;
//         }
//         fclose(fptr);
//         return true;
//     }
//     return false;
// }

Ehs_Sched_CSV_Rep lib_data_one_row = {0, };

void calloc_one_csv_rep_obj(Ehs_Sched_CSV_Rep* lib_data)
{
    Ehs_Sched_Serde_Conversion_Funcs conversion_funcs = ehs_serde_get_conversion_functions();

    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->day, conversion_funcs.day_buf_size);
    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->zone, conversion_funcs.zone_buf_size);
    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->start_time, conversion_funcs.start_time_buf_size);
    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->end_time, conversion_funcs.end_time_buf_size);
    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->worked_flag, conversion_funcs.worked_flag_buf_size);
    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->enabled_flag, conversion_funcs.enabled_flag_buf_size);
    EHS_SCHED_CALLOC_TEMP_OUT_BUF(lib_data->actuator_detail, conversion_funcs.actuator_detail_buf_size);
}

void free_one_csv_rep_obj(Ehs_Sched_CSV_Rep* lib_data)
{
    Ehs_Sched_Serde_Conversion_Funcs conversion_funcs = ehs_serde_get_conversion_functions();

    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->day, conversion_funcs.day_buf_size);
    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->zone, conversion_funcs.zone_buf_size);
    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->start_time, conversion_funcs.start_time_buf_size);
    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->end_time, conversion_funcs.end_time_buf_size);
    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->worked_flag, conversion_funcs.worked_flag_buf_size);
    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->enabled_flag, conversion_funcs.enabled_flag_buf_size);
    EHS_SCHED_FREE_TEMP_OUT_BUF(lib_data->actuator_detail, conversion_funcs.actuator_detail_buf_size);
}

void usr_to_lib(Ehs_Sched_Serde_Rep* user_data, Ehs_Sched_CSV_Rep* lib_data)
{
    Ehs_Sched_Serde_Conversion_Funcs conversion_funcs = ehs_serde_get_conversion_functions();

    if(conversion_funcs.day_converter && lib_data->day)
    {
        conversion_funcs.day_converter(user_data->day, lib_data->day, conversion_funcs.day_buf_size);
    }
    //TODO: srid

}

bool Ehs_Sched_serialize_to_csv_file(Ehs_Sched_Serde_Rep* data, uint32_t n_rows, uint32_t n_columns)
{
    if(!data || 0 == n_rows || 0 == n_columns) return false;

    FILE* fptr = NULL;

    uint32_t format_buf_size = (3 * n_columns) + 1; //// '%s,'
    char format[format_buf_size];
    memset(format, 0, sizeof(format));

    for(uint32_t cols = 0, idx = 0; cols < n_columns; cols++, idx += 3)
    {
        format[idx] = '%';
        format[idx+1] = 's';
        format[idx+2] = EHS_SCHED_CSV_DELIMITER;
    }
    format[format_buf_size - 1] = '\n';

    fptr = fopen(EHS_SCHED_CSV_FILE_NEW, "w");
    if(fptr)
    {
        calloc_one_csv_rep_obj(&lib_data_one_row);
        for(uint32_t row = 0; row < n_rows; row++)
        {
            usr_to_lib(data, &lib_data_one_row);
            print_all_columns_to_file(fptr, format, &lib_data_one_row);
            data++;
        }
        fclose(fptr);
        return true;
    }
    return false;
}

/*
void test_v1(void)
{
    snprintf(csv_items[0].day, 4, "%s", "Mon");
    snprintf(csv_items[0].zone, 3, "%s", "z1");
    snprintf(csv_items[0].start_time, 6, "%s", "12:01");
    snprintf(csv_items[0].end_time, 6, "%s", "14:07");
    snprintf(csv_items[0].worked_flag, 2, "%s", "1");
    snprintf(csv_items[0].enabled_flag, 2, "%s", "0");
    snprintf(csv_items[0].actuator_detail, 11, "%s", "0x42072C0F");

    snprintf(csv_items[1].day, 4, "%s", "Tue");
    snprintf(csv_items[1].zone, 3, "%s", "z2");
    snprintf(csv_items[1].start_time, 6, "%s", "13:01");
    snprintf(csv_items[1].end_time, 6, "%s", "14:07");
    snprintf(csv_items[1].worked_flag, 2, "%s", "0");
    snprintf(csv_items[1].enabled_flag, 2, "%s", "1");
    snprintf(csv_items[1].actuator_detail, 11, "%s", "0x42072CF0");

    snprintf(csv_items[2].day, 4, "%s", "Sat");
    snprintf(csv_items[2].zone, 3, "%s", "z1");
    snprintf(csv_items[2].start_time, 6, "%s", "13:02");
    snprintf(csv_items[2].end_time, 6, "%s", "14:09");
    snprintf(csv_items[2].worked_flag, 2, "%s", "0");
    snprintf(csv_items[2].enabled_flag, 2, "%s", "1");
    snprintf(csv_items[2].actuator_detail, 11, "%s", "0x42072DF0");

    Ehs_Sched_serialize_to_csv_file(csv_items, 3, 7);
    free_one_csv_rep_obj(&lib_data_one_row);
}
*/

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

void* get_field_from_user_item(void* obj, size_t member_index)
{
    return return_field_data_from_rep(obj, member_index);
}

bool Ehs_Sched_serialize_to_csv_file_v2(uint32_t n_rows, uint32_t n_columns)
{
    if(0 == n_rows || 0 == n_columns) return false;

    FILE* fptr = NULL;
    fptr = fopen(EHS_SCHED_CSV_FILE_NEW, "w");

    uint32_t built_string_size = (3 * get_expected_len_one_csv_row()) + 1; //// '%s,'
    char built_string[built_string_size];
    uint32_t bstr_idx = 0;
    built_string[bstr_idx] = '\0';

    for(uint32_t row = 0; row < n_rows; row++)
    {
        for(uint32_t col = 0; col < n_columns; col++)
        {
            void* usr_field_data = get_field_from_user_item(get_item_from_user_rep(row), col);
            uint32_t tsize;
            char out[tsize];
            one_csv_row[row]->conversion_func(usr_field_data, out, tsize);
            //// TODO: srid replace strncat with snprintf
            strncat(built_string, out, tsize);
            bstr_idx += tsize;
            strncat(built_string, ", ", 2);
            bstr_idx += 2;
        }
        fptr = fopen(EHS_SCHED_CSV_FILE_NEW, "w");
        if(fptr)
        {
            fprintf(fptr, "%s\n", built_string);
        }
        bstr_idx = 0;
        built_string[bstr_idx] = '\0';
    }
    fclose(fptr);
    
    return true;
}

void test_v2(void)
{
    set_expected_len_one_csv_row(40);
    set_expected_csv_cols(7);
    set_all_user_defined_conversion_funcs_and_buf_sizes();
    Ehs_Sched_serialize_to_csv_file_v2(3,7);
}

int main(void)
{
    // test_v1();
    test_v2();
    return 0;
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

size_t get_expected_len_one_csv_row(void)
{
    return EXPECTED_LEN_ONE_CSV_ROW;
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
size_t get_expected_csv_cols(void)
{
    return EXPECTED_CSV_COLS;
}

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