#pragma once

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "adapter.h"

typedef enum PERSON_FIELD_NAMES {
    NAME,
    PRONOUNS,
    AGE,
    ADDRESS,
} PERSON_FIELD_NAMES;

typedef enum PERSON_CONVERSION_FUNCS_BUF_SIZES {
    NAME_BUF_SIZE = 30,
    PRONOUNS_BUF_SIZE = 20,
    AGE_BUF_SIZE = 4,
    ADDRESS_BUF_SIZE = 30,
} PERSON_CONVERSION_FUNCS_BUF_SIZES;

uint8_t get_peson_number_of_fields(void);
Field_Information* get_person_all_field_information(void);
void test_person_internal_to_csv_conversion(void);
