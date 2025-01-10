#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>
#include "sql_compiler.h"
#include "b+tree.h"

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define ID_SIZE size_of_attribute(Row, id)
#define USERNAME_SIZE size_of_attribute(Row, username)
#define EMAIL_SIZE size_of_attribute(Row, email)

#define ID_OFFSET 0
#define USERNAME_OFFSET (ID_OFFSET + ID_SIZE)
#define EMAIL_OFFSET (USERNAME_OFFSET + USERNAME_SIZE)

#define ROW_SIZE (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)

#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS (ROWS_PER_PAGE * TABLE_MAX_PAGES)
#define INVALID_PAGE_NUM UINT32_MAX

// The Table object makes requests for pages through the pager.
typedef struct {
  //uint32_t num_rows;
  uint32_t root_page_num;
  Pager* pager;
} Table;

void db_close(Table* table);
Table* db_open(const char* filename);
void create_new_root(Table* table, uint32_t right_child_page_num);

#endif