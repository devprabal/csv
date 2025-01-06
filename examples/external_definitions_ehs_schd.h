#pragma once

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "adapter.h"

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

typedef enum CONVERSION_FUNCS_BUF_SIZES {
    DAY_BUF_SIZE = 4,
    ZONE_BUF_SIZE = 3,
    START_TIME_BUF_SIZE = 6,
    END_TIME_BUF_SIZE = 6,
    WORKED_FLAG_BUF_SIZE = 2,
    ENABLED_FLAG_BUF_SIZE = 2,
    ACTUATOR_DETAIL_BUF_SIZE = 11,
} CONVERSION_FUNCS_BUF_SIZES;

uint8_t get_number_of_fields(void);
Field_Information* get_all_field_information(void);
void test_ehs_schd_internal_to_csv_conversion(void);
