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

bool set_expected_len_one_csv_row(size_t bytes);
bool set_expected_csv_cols(size_t n_cols);
size_t get_expected_len_one_csv_row(void);
size_t get_expected_csv_cols(void);
typedef bool(*generic_conversion_func_t)(void* in, char* out, uint8_t out_buf_size);

typedef struct CSV_Col CSV_Col;
struct CSV_Col {
    generic_conversion_func_t conversion_func;
    uint32_t out_buf_size;
};

bool init_one_csv_row(size_t n_cols);
bool set_all_conversion_funcs(size_t n_funcs, generic_conversion_func_t conversion_funcs[n_funcs], uint32_t out_buf_sizes[n_funcs]);
