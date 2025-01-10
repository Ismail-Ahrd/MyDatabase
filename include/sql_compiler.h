#ifndef SQL_COMPILER_H
#define SQL_COMPILER_H

#include <stdint.h>
#include "input_buffer.h"

typedef enum { 
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_SYNTAX_ERROR, 
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_STRING_TOO_LONG
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
  StatementType type;
  Row row_to_insert; //only used by insert statement
} Statement;

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

#endif