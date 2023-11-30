#pragma once

#define MIN(a, b) (a > b ? b : a)
#define MAX(a, b) (a < b ? b : a)
#define FLOAT_IS_GREATER(a, b, epsilon) ((a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon))
#define FLOAT_IS_LESS(a, b, epsilon) ((b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon))
