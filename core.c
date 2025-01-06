#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "adapter.h"

typedef struct Csv_Internal_Config Csv_Internal_Config;
struct Csv_Internal_Config
{
    size_t expected_len_one_row;
    char file_name[UINT8_MAX];
    size_t n_fields;
    get_col_data_func_t get_col_data;
    Field_Information field_info[];
};

#define MAX_N_CSV_INITIALIZERS (3)
Csv_Internal_Config* csv_initializer[MAX_N_CSV_INITIALIZERS]; //// corresponding to N csv files, should be dynamically allocated
static int csv_initializer_free_idx = 0;

//// TODO: srid write to new file, delete old file, rename new file to old file

void set_csv_internal_config(int initlzr_idx, Csv_Config config)
{
    snprintf(csv_initializer[initlzr_idx]->file_name, strlen(config.file_name) + 1, "%s", config.file_name);
    csv_initializer[initlzr_idx]->n_fields = config.n_fields;
    csv_initializer[initlzr_idx]->get_col_data = config.get_col_data;

    size_t sum_field_buf_size = 0;
    for(int i = 0; i < config.n_fields; i++)
    {
        (csv_initializer[initlzr_idx]->field_info[i]).index = config.field_info[i].index;
        (csv_initializer[initlzr_idx]->field_info[i]).buf_size = config.field_info[i].buf_size;
        (csv_initializer[initlzr_idx]->field_info[i]).converter = config.field_info[i].converter;

        sum_field_buf_size += config.field_info[i].buf_size;
    }

    //// 120 lines with 200 bytes per line ~23.5 kB file size
    //// Assumption: keep csv file size within 25 kB
    //// see max number of columns assumption
    if(sum_field_buf_size <= 200)
    {
        csv_initializer[initlzr_idx]->expected_len_one_row = sum_field_buf_size;
    }
    else
    {
        printf("\nOh you must be joking! Reduce line len to <= 200 bytes.\n");
    }
}

static int get_csv_initlzr_idx_by_file_name(char file_name[UINT8_MAX])
{
    int idx = -1;
    for(int i = 0; i < csv_initializer_free_idx; i++)
    {
        if(strncmp(csv_initializer[i]->file_name, file_name, strlen(file_name)) == 0)
        {
            idx = i;
            break;
        }
    }
    return idx;
}

bool is_valid_filename(char file_name[UINT8_MAX])
{
    //// filename length should not exceed UINT8_MAX
    if(file_name[0] == '\0') return false;

    for(int i = 0; i < UINT8_MAX; i++)
    {
        //// checks for properly terminated string
        if(file_name[i] == '\0') return true;
    }

    return false;
}

bool is_valid_initializer(Csv_Config config)
{
    if(is_valid_filename(config.file_name) == false)
    {
        printf("\nProvide proper null terminated file name of length < %d\n", UINT8_MAX);
        return false;
    }
    if(get_csv_initlzr_idx_by_file_name(config.file_name) != -1)
    {
        printf("\nA csv initializer with file name %s exists, please use different file name\n", config.file_name);
        return false;
    }
    if(config.n_fields == 0)
    {
        printf("\nProvide number of fields\n");
        return false;
    }
    if(config.field_info == NULL)
    {
        printf("\nProvide fields_info address of a valid Field_Information array\n");
        return false;
    }
    if(config.get_col_data == NULL)
    {
        printf("\nProvide function ptr to get field data from [row,col] of the structure to serialize\n");
        return false;
    }
    return true;
}

bool csv_init(Csv_Config config)
{
    //// Each csv initializer is uniquely identified by the file_name
    //// If two different modules use the same file_name, the behaviour is according to module which calls csv_init() first
    //// DO NOT CALL csv_init() twice with the same file_name
    if(false == is_valid_initializer(config))
    {
        printf("\nIncorrect Csv_Config for init\n");
        return false;
    }

    int initlzr_idx = csv_initializer_free_idx;
    if(initlzr_idx >= MAX_N_CSV_INITIALIZERS)
    {
        printf("\ncsv initializer found full when trying to init for [%s], increase MAX_N_CSV_INITIALIZERS\n", config.file_name);
        return false;
    }

    csv_initializer[initlzr_idx] = malloc(sizeof(Csv_Internal_Config) + ((sizeof(Field_Information) * config.n_fields)));
    if(NULL == csv_initializer)
    {
        printf("\nWARNING: Could not init csv module for [%s]. Cannot write to file.\n", config.file_name);
        return false;
    }

    csv_initializer_free_idx++;
    set_csv_internal_config(initlzr_idx, config);
    printf("\ncsv init complete for [%s].\n", config.file_name);
    return true;
}

bool v3(Csv_Config config, int n_rows, int n_columns)
{
    ////TODO: srid config is still alive, so we can use it for conversion functions, instead of copying each field_info into Csv_Internal_Config
    if(0 >= n_rows || 0 >= n_columns) return false;
    if(false == is_valid_filename(config.file_name)) return false;
    int initlzr_idx = get_csv_initlzr_idx_by_file_name(config.file_name);
    if(initlzr_idx == -1)
    {
        printf("\ncsv_init() not called for file name %s\n", config.file_name);
        return false;
    }

    FILE* fptr = 0;
    fptr = fopen(csv_initializer[initlzr_idx]->file_name, "w");

    uint32_t bstr_size = (3 * csv_initializer[initlzr_idx]->expected_len_one_row) + 1; //// '%s,'
    char built_string[bstr_size];
    uint32_t bstr_idx = 0;
    built_string[bstr_idx] = '\0';

    for(int row = 0; row < n_rows; row++)
    {
        for(int col = 0; col < n_columns; col++)
        {
            void* usr_field_data = csv_initializer[initlzr_idx]->get_col_data(row, col);
            uint32_t tsize = csv_initializer[initlzr_idx]->field_info[col].buf_size;
            char out[tsize];
            (csv_initializer[initlzr_idx]->field_info[col]).converter(usr_field_data, out, tsize);
            //// TODO: srid replace strncat with snprintf
            strncat(built_string, out, tsize);
            bstr_idx += tsize;
            strncat(built_string, ", ", 3); //// TODO: srid use user defined delimiter macro
            bstr_idx += 3;
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
