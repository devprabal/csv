#include "core.h"

typedef struct CSV_Col CSV_Col;
struct CSV_Col {
    generic_conversion_func_t conversion_func;
    uint32_t out_buf_size;
};

CSV_Col** one_csv_row = 0;


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

void set_all_user_defined_conversion_funcs_and_buf_sizes(void)
{
    size_t n_funcs = sizeof(field_conversion_funcs) / sizeof (field_conversion_funcs [0]);
    set_all_conversion_funcs(n_funcs, field_conversion_funcs, field_sizes);
}

static csv_init_status = false;

bool is_csv_init_complete(void)
{
    return csv_init_status;
}

void set_csv_init_status(bool is_complete)
{
    csv_init_status = is_complete;
}

typedef void (*field_information_setter_func_t)(void);
typedef size_t (*get_row_length_func_t)(void);
typedef uint8_t (*get_number_of_columns_func_t)(void);

typedef struct Csv_Properties Csv_Properties;
struct Csv_Properties
{
    char file_name[UINT8_MAX];
    field_information_setter_func_t field_info_setter_fptr;
    get_row_length_func_t row_len_fptr;
    get_number_of_columns_func_t n_col_fptr;
};

#define MAX_N_CSV_INITIALIZERS (3)
Csv_Properties csv_initializer[MAX_N_CSV_INITIALIZERS]; //// initializers corresponding to N csv files
static int csv_initializer_free_idx = 0;

int get_csv_initializer_index_from_file_name(char* file_name)
{
    if(!file_name) return -1;

    for(int i = 0; i < MAX_N_CSV_INITIALIZERS; i++)
    {
        if(csv_initializer[i].file_name)
        {
            if(0 == strncmp(csv_initializer[i].file_name, file_name, strlen(csv_initializer[i].file_name)))
            {
                return i;
            }
        }
    }
    printf("\nNot found\n");
    return -1;
}

int get_csv_initializer_free_index(void)
{
    int ret = csv_initializer_free_idx;
    if(ret < MAX_N_CSV_INITIALIZERS)
    {
        csv_initializer_free_idx++;
        return ret;
    }
    return -1;
}

bool is_valid_initializer(Csv_Properties props)
{
    if(props.file_name[0] == '\0')
    {
        printf("\nProvide file name\n");
        return false;
    }
    if(props.field_info_setter_fptr == NULL)
    {
        printf("\nProvide field_information_setter_func_t function pointer\n");
        return false;
    }
    if(props.row_len_fptr == NULL)
    {
        printf("\nProvide get_row_length_func_t function pointer\n");
        return false;
    }
    if(props.n_col_fptr == NULL)
    {
        printf("\nProvide get_number_of_columns_func_t function pointer\n");
        return false;
    }
    return true;
}

bool csv_init(Csv_Properties props)
{
    if(false == is_valid_initializer(props))
    {
        printf("\nIncorrect Csv_Properties for init\n");
        return false;
    }
    int initlzr_idx = get_csv_initializer_free_index();
    if(-1 == initlzr_idx)
    {
        printf("\ncsv initializer full, increase MAX_N_CSV_INITIALIZERS\n");
        return false;
    }
    
    snprintf(csv_initializer[initlzr_idx].file_name, strlen(props.file_name), "%s", props.file_name);

    csv_initializer[initlzr_idx].field_info_setter_fptr();
    uint8_t max_fields = csv_initializer[initlzr_idx].n_col_fptr();
    if(0 == max_fields)
    {
        printf("\nProvide Field_Information. See api.h\n");
        return false;
    }
    // set_expected_len_one_csv_row(40);
    set_expected_len_one_csv_row(csv_initializer[initlzr_idx].row_len_fptr());
    // set_expected_csv_cols(7);
    // set_expected_csv_cols(get_number_of_fields());
    set_expected_csv_cols(max_fields);
    // set_all_user_defined_conversion_funcs_and_buf_sizes();
    set_csv_init_status(true);
    printf("\nSetting of Field_Information complete.\n");
    return true;
}
