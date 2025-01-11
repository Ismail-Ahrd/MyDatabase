#include <stdio.h>      // For printf, getline
#include <stdlib.h>     // For malloc, realloc, free, exit, EXIT_SUCCESS
#include <string.h>     // For strcmp, strncmp
#include <stdbool.h>    // For the bool type and true/false macros
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>      // For open flags like O_RDWR, O_CREAT
#include <sys/stat.h> // For file permission flags like S_IWUSR, S_IRUSR

#include "input_buffer.h"
#include "sql_compiler.h"
#include "cursor.h"
#include "virtual_machine.h"
#include "b+tree.h"

void print_prompt() { printf("db > "); }

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Must supply a database filename.\n");
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  Table* table = db_open(filename);

  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer, table)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_NEGATIVE_ID):
        printf("ID must be positive.\n");
        continue;
      case (PREPARE_STRING_TOO_LONG):
        printf("String is too long.\n");
        continue;
      case (PREPARE_SYNTAX_ERROR):
        printf("Syntax error. Could not parse statement.\n");
        continue;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }

    switch (execute_statement(&statement, table)) {
      case (EXECUTE_SUCCESS):
        printf("Executed.\n");
        break;
      case (EXECUTE_DUPLICATE_KEY):
        printf("Error: Duplicate key.\n");
        break;
      case (EXECUTE_KEY_NOT_FOUND):
        printf("Error: key not found.\n");
        break;
      case (EXECUTE_TABLE_FULL):
        printf("Error: Table full.\n");
        break;
    }
  }
}
// Where are we righ now?
// we will implement the splitting of a leaf node, and the creation of an internal node to serve as a parent for the two leaf node.