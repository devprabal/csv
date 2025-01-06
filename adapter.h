#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "core.h"

typedef bool(*generic_conversion_func_t)(void* in, char* out, uint8_t out_buf_size);

typedef struct Field_Information Field_Information;
struct Field_Information
{
    uint8_t index;
    uint32_t buf_size;
    generic_conversion_func_t converter;
};

typedef void*(*get_col_data_func_t)(int row_idx, int col_idx);

typedef struct Csv_Config Csv_Config;
struct Csv_Config
{
    char file_name[UINT8_MAX];
    uint8_t n_fields;
    Field_Information* field_info;
    get_col_data_func_t get_col_data;
};
