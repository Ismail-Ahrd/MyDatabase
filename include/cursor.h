#ifndef CURSOR_H
#define CURSOR_H
 
#include "table.h"
#include "b+tree.h"


typedef struct {
  Table* table;
  //uint32_t row_num;  When our table was a simple array of rows, we could access a row given just the row number
  //Now that it’s a tree, we identify a position by the page number of the node, and the cell number within that node.
  uint32_t page_num;
  uint32_t cell_num;
  bool end_of_table; // Indicates a position one past the last element (which is somewhere we may want to insert a row)
} Cursor;

Cursor* table_start(Table* table);
Cursor* leaf_node_find(Table* table, uint32_t page_num, uint32_t key);
Cursor* internal_node_find(Table* table, uint32_t page_num, uint32_t key);
Cursor* table_find(Table* table, uint32_t key);

void* cursor_value(Cursor* cursor);
void cursor_advance(Cursor* cursor);

#endif