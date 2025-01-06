#include "ehs_sched_serde.h"
#include "ehs_serde_conversion_functions.h"

typedef struct Ehs_Sched_Internal_CSV_Rep Ehs_Sched_Internal_CSV_Rep;
struct Ehs_Sched_Internal_CSV_Rep {
    char day[4];
    char zone[3];
    char start_time[6];
    char end_time[6];
    char worked_flag[2];
    char enabled_flag[2];
    char actuator_detail[11];
};


#define EHS_SCHED_SERDE_MAX_CSV_ITEMS (211)
static Ehs_Sched_Internal_CSV_Rep csv_items[EHS_SCHED_SERDE_MAX_CSV_ITEMS];

static Ehs_Sched_Serde_Conversion_Funcs conversion_funcs = {0, };



void Ehs_Sched_Serde_to_CSV(Ehs_Sched_Serde_Rep *serde_rep, Ehs_Sched_CSV_Rep *csv_rep)
{
    conversion_funcs = ehs_serde_get_conversion_functions();
    
}


void Ehs_Sched_Serde_DB_to_CSV_DB()