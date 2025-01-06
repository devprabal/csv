#include <stdio.h>
#include "examples/external_definitions_ehs_schd.h"
#include "examples/external_definitions_person.h"
#include "adapter.h"
#include "core.h"

void test_v3(void)
{
    test_ehs_schd_internal_to_csv_conversion();
    test_person_internal_to_csv_conversion();
}

int main(void)
{
    test_v3();
    return 0;
}
