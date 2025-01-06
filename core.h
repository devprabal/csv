#pragma once

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "adapter.h"

typedef struct Csv_Config Csv_Config;

bool csv_init(Csv_Config config);
bool v3(Csv_Config config, int n_rows, int n_columns);
