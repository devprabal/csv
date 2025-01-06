#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "ehs_sched_serde.h"

typedef bool(*convert_day_func_t)(char* day_in, char* day_out, uint8_t out_buf_size);
typedef bool(*convert_zone_func_t)(char* zone_in, char* zone_out, uint8_t out_buf_size);
typedef bool(*convert_start_time_func_t)(struct tm start_time_in, char* start_time_out, uint8_t out_buf_size);
typedef bool(*convert_end_time_func_t)(struct tm end_time_in, char* end_time_out, uint8_t out_buf_size);
typedef bool(*convert_worked_flag_func_t)(bool worked_flag_in, char* worked_flag_out, uint8_t out_buf_size);
typedef bool(*convert_enabled_flag_func_t)(bool enabled_flag_in, char* enabled_flag_out, uint8_t out_buf_size);
typedef bool(*convert_actuator_detail_func_t)(Ehs_Sched_Actuator_Detail actuator_detail_in, char* actuator_detail_out, uint8_t out_buf_size);


typedef struct Ehs_Sched_Serde_Conversion_Funcs Ehs_Sched_Serde_Conversion_Funcs;
struct Ehs_Sched_Serde_Conversion_Funcs {
    convert_day_func_t day_converter;
    convert_zone_func_t zone_converter;
    convert_start_time_func_t start_time_converter;
    convert_end_time_func_t end_time_converter;
    convert_worked_flag_func_t worked_flag_converter;
    convert_enabled_flag_func_t enabled_flag_converter;
    convert_actuator_detail_func_t actuator_detail_converter;
};

void ehs_serde_set_conversion_functions(Ehs_Sched_Serde_Conversion_Funcs conversion_funcs);
Ehs_Sched_Serde_Conversion_Funcs ehs_serde_get_conversion_functions(void);
