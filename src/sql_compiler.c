#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "sql_compiler.h"

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement) {
  statement->type = STATEMENT_INSERT;

  char* keyword = strtok(input_buffer->buffer, " ");
  char* id_string = strtok(NULL, " ");
  char* username = strtok(NULL, " ");
  char* email = strtok(NULL, " ");

  if (id_string == NULL || username == NULL || email == NULL) {
    return PREPARE_SYNTAX_ERROR;
  }
  
  int id = atoi(id_string);
  if (id < 0) {
    return PREPARE_NEGATIVE_ID;
  }

  if (strlen(username) > COLUMN_USERNAME_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }
  if (strlen(email) > COLUMN_EMAIL_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }

  statement->row_to_insert.id = id;
  strcpy(statement->row_to_insert.username, username);
  strcpy(statement->row_to_insert.email, email);

  return PREPARE_SUCCESS;
}

PrepareResult prepare_select_one(InputBuffer* input_buffer, Statement* statement) {
  statement->type = STATEMENT_SELECT_ONE;

  // Check if the query matches the expected format
  char* keyword = strtok(input_buffer->buffer, " ");
  char* where_clause = strtok(NULL, " ");
  char* id_clause = strtok(NULL, "=");

  if (where_clause == NULL || id_clause == NULL || strcmp(where_clause, "where") != 0 || strcmp(id_clause, "id") != 0) {
    return PREPARE_SYNTAX_ERROR;
  }

  char* id_string = strtok(NULL, " ");
  if (id_string == NULL) {
    return PREPARE_SYNTAX_ERROR;
  }

  int id = atoi(id_string);
  if (id < 0) {
    return PREPARE_NEGATIVE_ID;
  }

  statement->id_to_select = id;
  return PREPARE_SUCCESS;
}

//This is the SQL compiler
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    return prepare_insert(input_buffer, statement);
  }
  // select where id = <value>
  if (strncmp(input_buffer->buffer, "select where", 12) == 0) {
    return prepare_select_one(input_buffer, statement);
  }

  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}