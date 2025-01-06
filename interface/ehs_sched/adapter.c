#include "adapter.h"
#include "api/api.h"

static Field_Information field_information[UINT8_MAX];
static uint8_t field_information_free_idx = 0;

static size_t EXPECTED_LEN_ONE_CSV_ROW = 40; //// bytes
static size_t EXPECTED_CSV_COLS = 1;

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
    #if 0
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
    #endif
    size_t sum = 0;
    uint8_t max_fields = get_field_information_free_idx();
    for(int i = 0; i < max_fields; i++)
    {
        sum += field_information[i].buf_size;
    }
    return sum;
}

size_t get_number_of_fields(void)
{
    #if 0
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
    #endif
    return 0;
}

size_t get_expected_len_one_csv_row(void)
{
    return EXPECTED_LEN_ONE_CSV_ROW;
}
void* get_field_from_user_item(void* obj, size_t member_index)
{
    return return_field_data_from_rep(obj, member_index);
}

uint8_t get_field_information_free_idx(void)
{
    return field_information_free_idx;
}

void increment_field_information_free_idx(void)
{
    field_information_free_idx++;
}

void set_field_information(uint8_t n_fields, Field_Information info[n_fields])
{
    if(is_csv_init_complete())
    {
        printf("\nCall to all set_field_information() should come before the call to csv_init()\n");
        printf("\ncsv_init is now complete, cannot set more field informations.\n");
        return;
    }
    for(int i = 0; i < n_fields; i++)
    {
        uint8_t free_idx = get_field_information_free_idx();
        field_information[i].index = info[free_idx].index;
        field_information[i].buf_size = info[free_idx].buf_size;
        field_information[i].converter = info[free_idx].converter;
        increment_field_information_free_idx();
    }
}

Field_Information get_field_information(size_t index)
{
    uint8_t max_fields = get_field_information_free_idx();
    for(int i = 0; i < max_fields; i++)
    {
        if(field_information[i].index == index)
        {
            return field_information[i];
        }
    }
    printf("\nNot Found\n");
}
