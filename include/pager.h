#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>

#define TABLE_MAX_PAGES 100
#define PAGE_SIZE 4096

// The Pager accesses the page cache and the file. 
typedef struct {
  int file_descriptor;
  uint32_t file_length;
  uint32_t num_pages;
  void* pages[TABLE_MAX_PAGES];
} Pager;

void pager_flush(Pager* pager, uint32_t page_num);
Pager* pager_open(const char* filename);
void* get_page(Pager* pager, uint32_t page_num);
uint32_t get_unused_page_num(Pager* pager);

#endif