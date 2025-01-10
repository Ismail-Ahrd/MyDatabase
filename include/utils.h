#ifndef UTILS_H
#define UTILS_H

#include "sql_compiler.h"
#include "table.h"

void serialize_row(Row* source, void* destination);
void deserialize_row(void *source, Row* destination);

#endif