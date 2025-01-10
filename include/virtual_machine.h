#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "input_buffer.h"
#include "table.h"

typedef enum { 
  EXECUTE_SUCCESS, 
  EXECUTE_TABLE_FULL, 
  EXECUTE_DUPLICATE_KEY 
} ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

//MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table *table);

#endif