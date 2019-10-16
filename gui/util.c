#include <stdio.h>
#include <stdlib.h>

void read(const char *filename) {

  FILE *f = fopen(filename, "r");

  if (!f) fprintf(stderr, "couldn't read %s\n", filename);

  int ch, nlines = 0;

  do {
    ch = fgetc(f);
    if (ch == '\n') nlines++;
  } while (ch != EOF);

  rewind(f);

  char *lines[nlines];
  size_t len = 0;
  for (int i = 0 ; i < nlines; i++) {
    lines[i] = NULL;
    len = 0;
    getline(&lines[i], &len, f);
  }

  fclose(f);

  for (int i = 0; i < 10; i++) {
    printf("i=%d, %s", i, lines[i]);
  }

  for (int i = nlines-1; i >= nlines-10; i--) {
    printf("i=%d, %s", i, lines[i]);
  }

  printf("nlines = %d\n", nlines);

}

int main(int argc, char **argv) {

  if (argc < 2) return 0;
  else read(argv[1]);
  return 1;

}
