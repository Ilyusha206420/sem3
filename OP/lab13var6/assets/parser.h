#pragma once

#include <stdio.h>

#define ERROR_CONF_OPEN   -1
#define ERROR_CONF_SYNTAX -2
#define ERROR_CUT_TYPE    -3
#define ERROR_FILE_COUNT  -4
#define ERROR_OPEN_FILE   -5

#define DEFAULT_SEGMENT_COUNT 4
#define DEFAULT_CUT_TYPE      0

#define VERTICAL_CUT   1
#define HORIZONTAL_CUT 2
#define GRID_CUT       3

int parseConfig(char *confName, FILE **fp, int *segments, int *cutType);