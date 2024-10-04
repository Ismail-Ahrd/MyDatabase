#include <stdio.h>      // For printf, getline
#include <stdlib.h>     // For malloc, realloc, free, exit, EXIT_SUCCESS
#include <string.h>     // For strcmp, strncmp
#include <stdbool.h>    // For the bool type and true/false macros
#include <stdint.h>
#include <unistd.h>

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { 
    PREPARE_SUCCESS, 
    PREPARE_SYNTAX_ERROR, 
    PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct {
  StatementType type;
  Row row_to_insert; //only used by insert statement
} Statement;

//define a macro that gets the size of an attribute in a struct
//(Struct*)0 cast the value 0 to a pointer of type Struct*, the pointer point to a memory address of 0
//but no memory access will actually happen (it is used just to have an imaginary Struct in the address
//0 to access the Attribute and gets his size).
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
//Offset refers to the byte position of a specific attribute within a row. 
//It tells you where each attribute starts relative to the beginning of the row.
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
  uint32_t num_rows;
  void* pages[TABLE_MAX_PAGES];
} Table;


void print_row(Row* row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

//Convert a row into a flat byte representation suitable for storage.
void serialize_row(Row* source, void* destination) {
  memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

//Converts the flat byte representation back into a Row structure for use within the program.
void deserialize_row(void *source, Row* destination) {
  memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

//retreive a pointer to specific row.
void* row_slot(Table* table, uint32_t row_num) {
  //calcluate the page number.
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  //retreive the page.
  void *page = table->pages[page_num];
  if (page == NULL) {
    // Allocate memory only when we try to access page.
    //If the page is not already allocated (i.e., page is NULL), 
    //allocate memory for that page using malloc(PAGE_SIZE) and update the pages array.
    page = table->pages[page_num] = malloc(PAGE_SIZE);
  }
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}

Table* new_table() {
  Table* table = (Table*)malloc(sizeof(Table));
  table->num_rows = 0;
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    table->pages[i] = NULL;
  }
  return table;
}

void free_table(Table* table) {
  for (int i = 0; table->pages[i]; i++) {
    free(table->pages[i]);
  }
  free(table);
}

InputBuffer* new_input_buffer() {
  //Allocate the input buffer dynamicly in the memory
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));

  // Check if memory allocation was successful
  if (input_buffer == NULL) {
    fprintf(stderr, "Unable to allocate memory for InputBuffer.\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

//ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void read_input(InputBuffer* input_buffer) {
  // getline will allocate memory for the buffer if it's NULL  
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  // Replace the \n charachter at the end of the buffer with a null terminator '\0'
  // This converts the buffer into a valid null-terminated string that can be safely used in C
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

//Non-SQL statements like .exit are called “meta-commands”. 
//They all start with a dot, so we check for them and handle them in a separate function.
MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table *table) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    free_table(table);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

//This is the SQL compiler
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    int args_assigned = sscanf(
        input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
        statement->row_to_insert.username, statement->row_to_insert.email);
    if (args_assigned < 3) {
      return PREPARE_SYNTAX_ERROR;
    }
    return PREPARE_SUCCESS;
  }

  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement* statement, Table* table) {
  if (table->num_rows >= TABLE_MAX_ROWS) {
     return EXECUTE_TABLE_FULL;
  }

  Row* row_to_insert = &(statement->row_to_insert);

  serialize_row(row_to_insert, row_slot(table, table->num_rows));
  table->num_rows += 1;

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table* table) {
  Row row;
  for (uint32_t i = 0; i < table->num_rows; i++) {
     deserialize_row(row_slot(table, i), &row);
     print_row(&row);
  }
  return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      return execute_insert(statement, table);
    case STATEMENT_SELECT:
      return execute_select(statement, table);
  }
}

int main(int argc, char* argv[]) {
  Table* table = new_table();
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer, table)) {
        case META_COMMAND_SUCCESS:
          continue;
        case META_COMMAND_UNRECOGNIZED_COMMAND:
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case PREPARE_SUCCESS:
        break;
      case (PREPARE_SYNTAX_ERROR):
        printf("Syntax error. Could not parse statement.\n");
        continue;
      case PREPARE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
        continue;
    }
    switch (execute_statement(&statement, table)) {
      case (EXECUTE_SUCCESS):
        printf("Executed.\n");
        break;
      case (EXECUTE_TABLE_FULL):
        printf("Error: Table full.\n");
        break;
    }
  }
  return 0;
}

// Where are we righ now?
// First, we create a new input buffer to hold user input, the input buffer is a struct of: 
// buffer which is a pointer to the buffer, an buffer_length and input_length
// Then we create a new Table, it is a struct of num_rows and pages: a pointer to pages where we will store the rows 
// Then, we read the user input an put it in the buffer.
// Then, we check if the user input is meta command (starts with .) if yes we execute the command
// If no we call the prepare statement function (can be considered as the compiler)
// based on user input we create a statement (insert or select), a statement is a struct composed of:
// type(StatementType) and rows_to_insert(only used by insert statement). The function returns prepare result
// if it is success we call the execute statement function, this function executes the statement based on
// it's type (insert or select)
// For now we just have a single table with a defined row, and the data is only stored in the memory.