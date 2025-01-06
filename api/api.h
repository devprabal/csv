#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef bool(*generic_conversion_func_t)(void* in, char* out, uint8_t out_buf_size);

typedef struct Field_Information Field_Information;
struct Field_Information
{
    size_t index;
    uint32_t buf_size;
    generic_conversion_func_t converter;
};

void set_field_information(uint8_t n_fields, Field_Information info[n_fields]);
Field_Information get_field_information(uint8_t index);
