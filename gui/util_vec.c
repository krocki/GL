#include <stdio.h>
#include <stdlib.h>
#include "vec.h"

void read(const char *filename) {

  FILE *f = fopen(filename, "r");
  if (!f) fprintf(stderr, "couldn't read %s\n", filename);

  int ch, nlines = 0;

  do {
    ch = fgetc(f);
    if (ch == '\n') nlines++;
  } while (ch != EOF);

  rewind(f);

  vec v = {0};
  init(&v);
  print_vec(&v);

  char *lines[nlines];
  size_t len = 0;
  size_t header = 1;

  char buf[1024];
  while (fgets(buf, 1024, f)) {
    printf("%s\n", buf);
  }

  //for (int i = 0 ; i < nlines; i++) {
  //  lines[i] = NULL;
  //  len = 0;
  //  getline(&lines[i], &len, f);
  //  if (i<header) continue;
  //  push_back(&v, &lines[i]);
  //  //int field_no = 0;
  //  //char *field = strtok(lines[i], ",");
  //  //while (field) {
  //  //  if (field_no < 3) {
  //  //    printf("%d: '%s'\t", field_no, field);
  //  //    field_no++;
  //  //    field = strtok(NULL, ",");
  //  //  }
  //  //}
  //  //printf("\n");
  //}

  fclose(f);

  //for (int i = header; i < 10; i++) {
  //  printf("i=%d, %s", i, lines[i]);
  //}

  //for (int i = nlines-1; i >= nlines-10; i--) {
  //  printf("i=%d, %s", i, lines[i]);
  //}

  //printf("nlines = %d\n", nlines);

  print_vec(&v);
  release(&v);

}

int main(int argc, char **argv) {

  if (argc < 2) return 0;
  else read(argv[1]);
  return 1;

}
