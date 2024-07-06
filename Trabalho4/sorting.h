#ifndef SORTING_H

#define SORTING_H


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "types.h"

/* 
 -f, --ignore-case fold lower case to upper case characters
 -r, --reverse reverse the result of comparisons
 -c, --check, --check=diagnose-first check for sorted input; do not sort
 -m, --merge, merge already sorted files; do not sort
 -b, --ignore-leading-blanks ignore leading blanks
*/
int compare_string(const char *left, const char *right )
{
  if(globalFlags.ignoreCase) return strcasecmp(left, right);
  
  return strcmp(left, right);
}

// TODO: Implementar quick sort
void sort_lines(char **lines, int amount, int line_lenth) {
  for(int i = 0; i < amount; i++) {
    for(int j = i+1; j < amount; j++) {
      if(compare_string(lines[i], lines[j]) <= 0) continue;
      char* tmp = lines[i];
      lines[i] = lines[j];
      lines[j] = tmp;
    }
  }
}

FileData merge_sorted_files(FileData *files, int files_count) {
  int min_len = 0, total_lines = 0;
  for(int i = 0; i < files_count; i++) {
    if(files[i].min_buffer > min_len) min_len = files[i].min_buffer;
    total_lines += files[i].lines_count;
  }


  char **file_lines;

  file_lines = malloc(total_lines * sizeof(char*));
  for(int i = 0; i < total_lines; i++) {
    file_lines[i] = calloc(min_len, sizeof(char));
  }

  int currentIdx = 0;
  int *pointers = calloc(files_count, sizeof(int));

  while(1) {
    char* menor = NULL;
    int idx_menor = -1;

    for(int i = 0; i < files_count; i++) {
      if(pointers[i] == files[i].lines_count) continue;

      if(idx_menor == -1 || compare_string(menor, files[i].file_lines[pointers[i]]) > 0) {
        menor = files[i].file_lines[pointers[i]];
        idx_menor = i;
      }
    }

    if(idx_menor == -1) break;

    file_lines[currentIdx++] = menor;
    pointers[idx_menor]++;
  }

  FileData result;
  result.file_lines = file_lines;
  result.lines_count = total_lines;
  result.min_buffer = min_len;
  return result;
}

#endif