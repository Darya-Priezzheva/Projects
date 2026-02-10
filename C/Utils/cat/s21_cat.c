#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct opt {
  int n;
  int b;
  int e;
  int s;
  int t;
  int v;
} OPT;

void print_file(char *argv, OPT *options);
int parser(int argc, char **argv, OPT *options);
void print_symbol(int c, int *prev, OPT *options, int *line_number,
                  int *empty_line_printed);

int main(int argc, char **argv) {
  OPT options = {0};

  if (parser(argc, argv, &options) != 0) {
    return EXIT_FAILURE;
  }

  for (int i = optind; i < argc; i++) {
    print_file(argv[i], &options);
  }

  return 0;
}

void print_file(char *argv, OPT *options) {
  FILE *file = fopen(argv, "r");
  if (!file) {
    fprintf(stderr, "%s: %s\n", argv, strerror(errno));
    return;
  }

  int line_number = 1;
  int empty_line_printed = 0;
  int prev = '\n';
  int c = fgetc(file);
  while (c != EOF) {
    print_symbol(c, &prev, options, &line_number, &empty_line_printed);
    c = fgetc(file);
  }

  fclose(file);
}

int parser(int argc, char **argv, OPT *options) {
  static struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "+nbestvTE", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'n':
        options->n = 1;
        break;
      case 'b':
        options->b = 1;
        options->n = 0;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case '?':
        fprintf(stderr, "Unknown option: -%c\n", opt);
        return EXIT_FAILURE;
      default:
        break;
    }
  }
  return 0;
}

void print_symbol(int c, int *prev, OPT *options, int *line_number,
                  int *empty_line_printed) {
  if (!(options->s == 1 && c == '\n' && *prev == '\n' && *empty_line_printed)) {
    if (c == '\n' && *prev == '\n') {
      *empty_line_printed = 1;
    } else {
      *empty_line_printed = 0;
    }

    if (*prev == '\n') {
      if (options->b == 1 && c != '\n') {
        printf("%6d\t", (*line_number)++);
      } else if (options->n == 1 && options->b != 1) {
        printf("%6d\t", (*line_number)++);
      }
    }

    if ((options->e == 1) && c == '\n') {
      printf("$");
    }

    if (options->t == 1 && c == '\t') {
      printf("^");
      c = 'I';
    }
    if (options->v == 1 && c >= 0 && c <= 31 && c != '\n' && c != '\t') {
      printf("^");
      c = c + 64;
    }

    fputc(c, stdout);
  }

  *prev = c;
}
