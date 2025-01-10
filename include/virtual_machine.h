#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "utils.h"
#include "input_buffer.h"
#include "sql_compiler.h"
#include "cursor.h"

typedef enum { 
  EXECUTE_SUCCESS, 
  EXECUTE_TABLE_FULL, 
  EXECUTE_DUPLICATE_KEY 
} ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table *table);
ExecuteResult execute_statement(Statement* statement, Table* table);

#endif