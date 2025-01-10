#include "input_buffer.h"
#include <stdio.h>
#include <stdlib.h> 

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));

  if (input_buffer == NULL) {
    fprintf(stderr, "Unable to allocate memory for InputBuffer.\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

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