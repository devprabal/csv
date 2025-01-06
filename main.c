#include "external/ehs_sched/definitions.h"

Ehs_Sched_Serde_Rep obj[3] = {
    [0] = {.day = "Sat", .zone = "z1", .start_time = {.tm_hour = 13, .tm_min = 2}, .end_time = {.tm_hour = 14, .tm_min = 9}, .worked_flag = 0, .enabled_flag = 1, .actuator_detail = {.mode = 0x42, .power = 0xF0, .temperature = 0x072D}},
    [1] = {.day = "Wed", .zone = "z1", .start_time = {.tm_hour = 3, .tm_min = 23}, .end_time = {.tm_hour = 11, .tm_min = 3}, .worked_flag = 1, .enabled_flag = 1, .actuator_detail = {.mode = 0x32, .power = 0x0F, .temperature = 0x081D}},
    [2] = {.day = "Mon", .zone = "z2", .start_time = {.tm_hour = 21, .tm_min = 2}, .end_time = {.tm_hour = 23, .tm_min = 9}, .worked_flag = 0, .enabled_flag = 1, .actuator_detail = {.mode = 0x22, .power = 0xF0, .temperature = 0x062D}},
};

void test_v2(void)
{
    csv_init(set_ehs_sched_field_information);
    Ehs_Sched_serialize_to_csv_file_v2(3,7);
}

int main(void)
{
    test_v2();
    return 0;
}
