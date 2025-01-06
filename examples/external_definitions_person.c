#include "external_definitions_person.h"
#include "adapter.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum COUNTRY {
    COUNTRY_USA,
    COUNTRY_KOR,
    COUNTRY_BRA,
    COUNTRY_CHI,
    COUNTRY_PAN,
    COUNTRY_COL,
    COUNTRY_PER,
    COUNTRY_ARG,
    COUNTRY_URU,
    COUNTRY_ECU,
    COUNTRY_SUR,
    COUNTRY_IND,
    COUNTRY_MAX
} COUNTRY;

typedef struct Address_Internal_Rep Address_Internal_Rep;
struct Address_Internal_Rep {
    int flat_no;
    COUNTRY country;
    char city[20];
};

typedef struct Person_Internal_Rep Person_Internal_Rep;
struct Person_Internal_Rep {
    char name[NAME_BUF_SIZE];
    char pronouns[PRONOUNS_BUF_SIZE];
    uint8_t age;
    Address_Internal_Rep address;
    Person_Internal_Rep* next;
};

static bool convert_name(void* name_in, char name_out[NAME_BUF_SIZE], uint8_t out_buf_size);
static bool convert_pronouns(void* pronouns_in, char pronouns_out[PRONOUNS_BUF_SIZE], uint8_t out_buf_size);
static bool convert_age(void* age_in, char age_out[AGE_BUF_SIZE], uint8_t out_buf_size);
static bool convert_address(void* address_in, char address_out[ADDRESS_BUF_SIZE], uint8_t out_buf_size);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                         conversion functions start                         //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

static bool convert_name(void* day_in, char day_out[NAME_BUF_SIZE], uint8_t out_buf_size)
{
    if(day_out && out_buf_size >= NAME_BUF_SIZE)
    {
        memcpy(day_out, day_in, NAME_BUF_SIZE);
        return true;
    }
    return false;
}

static bool convert_pronouns(void* pronouns_in, char pronouns_out[PRONOUNS_BUF_SIZE], uint8_t out_buf_size)
{
    if(pronouns_out && out_buf_size >= PRONOUNS_BUF_SIZE)
    {
        memcpy(pronouns_out, pronouns_in, PRONOUNS_BUF_SIZE);
        return true;
    }
    return false;
}

static bool convert_age(void* age_in, char age_out[AGE_BUF_SIZE], uint8_t out_buf_size)
{
    if(age_out && out_buf_size >= AGE_BUF_SIZE)
    {
        snprintf(age_out, out_buf_size, "%d", *((uint8_t*)age_in));
        return true;
    }
    return false;
}

static bool convert_address(void* address_in, char address_out[ADDRESS_BUF_SIZE], uint8_t out_buf_size)
{
    if(address_out && out_buf_size >= ADDRESS_BUF_SIZE)
    {
        int idx = snprintf(address_out, out_buf_size, "flat_no. %d, ", ((Address_Internal_Rep*)address_in)->flat_no);
        idx += snprintf(address_out + idx, out_buf_size, "%s, ", ((Address_Internal_Rep*)address_in)->city);
        if(((Address_Internal_Rep*)address_in)->country == COUNTRY_KOR)
        {
            idx += snprintf(address_out + idx, out_buf_size, "KOR");
        }
        else if(((Address_Internal_Rep*)address_in)->country == COUNTRY_IND)
        {
            idx += snprintf(address_out + idx, out_buf_size, "IND");
        }
        else
        {
            idx += snprintf(address_out + idx, out_buf_size, "%d", ((Address_Internal_Rep*)address_in)->country);
        }
        return true;
    }
    return false;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                          conversion functions end                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

static Field_Information person_fields_info[] = {
    {.index = NAME, .buf_size = NAME_BUF_SIZE, .converter = convert_name},
    {.index = PRONOUNS, .buf_size = PRONOUNS_BUF_SIZE, .converter = convert_pronouns},
    {.index = AGE, .buf_size = AGE_BUF_SIZE, .converter = convert_age},
    {.index = ADDRESS, .buf_size = ADDRESS_BUF_SIZE, .converter = convert_address},
};

uint8_t get_person_number_of_fields(void)
{
    return sizeof(person_fields_info)/sizeof(Field_Information);
}

Field_Information* get_person_all_field_information(void)
{
    return person_fields_info;
}

static Person_Internal_Rep people[] = {
    {.name = "prabal", .pronouns = "he/him", .age = 25, .address = {.flat_no = 204, .country = COUNTRY_IND, .city = "Noida"}},
    {.name = "gopal", .pronouns = "he/him", .age = 23, .address = {.flat_no = 101, .country = COUNTRY_IND, .city = "Delhi"}},
    {.name = "shin dongjoon", .pronouns = "he/him", .age = 29, .address = {.flat_no = 305, .country = COUNTRY_KOR, .city = "Gyeonggi-do"}},
    {.name = "woo soyeon", .pronouns = "she/her", .age = 26, .address = {.flat_no = 204, .country = COUNTRY_KOR, .city = "Busan"}},
};

static Person_Internal_Rep* people_ll = NULL;

static Person_Internal_Rep* make_new_person(Person_Internal_Rep person)
{
    Person_Internal_Rep* new_person = calloc(1, sizeof(Person_Internal_Rep));
    if(!new_person)
    {
        printf("\nFailed to create person representation\n");
        return NULL;
    }
    else
    {
        strncpy(new_person->name, person.name, strlen(person.name));
        strncpy(new_person->pronouns, person.pronouns, strlen(person.pronouns));
        new_person->age = person.age;
        new_person->address = person.address;
        people->next = NULL;
    }
    return new_person;
}

static void add_person_to_list(Person_Internal_Rep person)
{
    if(people_ll == NULL)
    {
        people_ll = make_new_person(person);
    }
    else
    {
        Person_Internal_Rep* itr = people_ll;
        while(itr->next)
        {
            itr = itr->next;
        }
        itr->next = make_new_person(person);
    }
}

void* get_person_field_data_from_array(int row_idx, int field_idx)
{
    int n_rows = sizeof(people)/sizeof(Person_Internal_Rep);
    if(row_idx >= n_rows) return NULL;

    Person_Internal_Rep* row = &(people[row_idx]);
    if(field_idx <= ADDRESS && field_idx >= NAME && row)
    {
        switch (field_idx)
        {
        case NAME:
            return row->name;
            break;
        case PRONOUNS:
            return row->pronouns;
            break;
        case AGE:
            return &(row->age);
            break;
        case ADDRESS:
            return &(row->address);
            break;
        default:
            break;
        }
    }
    return NULL;
}

static int get_person_number_from_heap(void)
{
    int n_rows = 0;
    Person_Internal_Rep* itr = people_ll;
    while(itr)
    {
        n_rows++;
        itr = itr->next;
    }
    return n_rows;
}

void* get_person_field_data_from_heap(int row_idx, int field_idx)
{
    if(row_idx >= get_person_number_from_heap()) return NULL;

    int idx = 0;
    Person_Internal_Rep* itr = people_ll;
    itr = people_ll;
    while(itr)
    {
        if(idx == row_idx)
        {
            break;
        }
        idx++;
        itr = itr->next;
    }

    if(field_idx <= ADDRESS && field_idx >= NAME && itr)
    {
        switch (field_idx)
        {
        case NAME:
            return itr->name;
            break;
        case PRONOUNS:
            return itr->pronouns;
            break;
        case AGE:
            return &(itr->age);
            break;
        case ADDRESS:
            return &(itr->address);
            break;
        default:
            break;
        }
    }
    return NULL;
}

void test_person_internal_to_csv_conversion(void)
{
    {
        Csv_Config config = {
            .file_name = "person_array.csv",
            .n_fields = get_person_number_of_fields(),
            .field_info = get_person_all_field_information(),
            .get_col_data = get_person_field_data_from_array,
        };
        if(true == csv_init(config))
        {
            int n_rows = sizeof(people)/sizeof(Person_Internal_Rep);
            v3(config, n_rows, get_person_number_of_fields());
        }
    }

    {
        int n_person_in_array = sizeof(people)/sizeof(Person_Internal_Rep);
        for(int i = 0; i < n_person_in_array; i++)
        {
            add_person_to_list(people[i]);
        }
        Csv_Config config = {
            .file_name = "person_heap.csv",
            .n_fields = get_person_number_of_fields(),
            .field_info = get_person_all_field_information(),
            .get_col_data = get_person_field_data_from_heap,
        };
        if(true == csv_init(config))
        {
            int n_rows = get_person_number_from_heap();
            v3(config, n_rows, get_person_number_of_fields());
        }
    }
    //// output contents of 'person_array.csv' and 'person_heap.csv' both files should be identical
}
