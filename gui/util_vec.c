#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "vec.h"

regex_t re(const char *str_regex) {
  regex_t re;
  if (regcomp (&re, str_regex, REG_EXTENDED)) {
    fprintf(stderr, "could not compile regex: %s\n", str_regex);
  }
  return re;
}

vec *read(const char *filename, const char *symbol) {

  FILE *f = fopen(filename, "r");
  if (!f) fprintf(stderr, "couldn't read %s\n", filename);

  int ch, nlines = 0;

  do {
    ch = fgetc(f);
    if (ch == '\n') nlines++;
  } while (ch != EOF);

  rewind(f);

  vec *v = calloc(1, sizeof(vec));
  init(v);
  print_vec(v);

  char *lines[nlines];
  size_t len = 0;
  size_t header = 1;
  size_t row = 0;
  int match_idx = -1;
  char *delim = ",";
  char buf[1024];
  printf("expr = '%s'\n", symbol);
  regex_t expr = re(symbol);
  int tok_match;
  size_t max_groups = 3;
  regmatch_t group_arr[max_groups];

  while (fgets(buf, 1024, f)) {
    int field_no = 0;
    char *field = strtok(buf, delim);
    while (field) {
      if (row == 0) {
        printf("field no [%d]:'%s'\n", field_no, field);
        tok_match = regexec(&expr, field, max_groups, group_arr, 0);
        if (!tok_match) {
          //matched++;
          puts("[match]");
          match_idx = field_no;
        }
      } else {
        if (field_no == match_idx) {
          float *f = malloc(sizeof(float));
          *f = atof(field);
          printf("%p\n", f);
          push_back(v, f);
        }
      }

      field_no++;
      field = strtok(NULL, delim);
    }
    row++;
  }

  regfree(&expr);

  fclose(f);

  return v;
}

int main(int argc, char **argv) {

  if (argc < 2) return 0;
  else {
    vec *v = read(argv[1], argv[2]);
    print_vec(v);
    for (int i=0; i<v->len; i++) {
      printf("%d: %f\n", i, *(float*)(v->arr[i].p));
    }
    release(v);
    free(v);
  }
  return 1;

}
