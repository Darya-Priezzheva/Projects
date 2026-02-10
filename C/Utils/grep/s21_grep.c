#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char **patterns;
  int count_pat;
  int cap_pat;
  int e;  // несколько шаблонов
  int i;  // игнор регистра
  int v;  // инверсия - несовпадающие строки
  int c;  // кол-во совпадений
  int l;  // имя файла
  int n;  // нумерация строк
  int h;  // не указывать имя файла если их больше одного
  int s;  // не выводить ошибки связанные с файлами
  int o;  // вывод только совпадающей части шаблона
  int f;  // чтение шаблонов из файла
} OPT;

int parser(int argc, char **argv, OPT *options);
void grep_file(FILE *file, const char *filename, OPT *options, int mult_files);
void free_memory(OPT *options);
char *combine_patterns(OPT *options);
char *my_strdup(const char *str);
void add_patterns_from_file(OPT *options, const char *filename);
void print_match_part(const char *line, regex_t *regex);

int main(int argc, char **argv) {
  OPT options = {0};
  options.cap_pat = 10;
  options.patterns = malloc(options.cap_pat * sizeof(char *));
  if (!options.patterns) {
    fprintf(stderr, "Memory allocation fail.\n");
    return EXIT_FAILURE;
  }
  if (parser(argc, argv, &options) != 0) {
    free_memory(&options);
    return EXIT_FAILURE;
  }

  if (options.count_pat == 0) {
    fprintf(stderr, "Missing pattern.\n");
    free_memory(&options);
    return EXIT_FAILURE;
  }

  int mult_files = (argc - optind > 1);

  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      if (!options.s) {
        fprintf(stderr, "grep: %s: %s\n", argv[i], strerror(errno));
      }

      continue;
    }
    grep_file(file, argv[i], &options, mult_files);
    fclose(file);
  }
  free_memory(&options);
  return 0;
}

int parser(int argc, char **argv, OPT *options) {
  int c;
  options->cap_pat = 10;
  options->count_pat = 0;
  while ((c = getopt(argc, argv, "e:f:ivclnhso")) != -1) {
    switch (c) {
      case 'e':
        options->e = 1;
        if (options->count_pat >= options->cap_pat) {
          options->cap_pat *= 2;
          options->patterns =
              realloc(options->patterns, options->cap_pat * sizeof(char *));
        }
        options->patterns[options->count_pat] = my_strdup(optarg);
        options->count_pat++;
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        add_patterns_from_file(options, optarg);
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        break;
    }
  }
  if (options->count_pat == 0 && optind < argc) {
    options->patterns[options->count_pat] = my_strdup(argv[optind]);
    options->count_pat++;
    optind++;
  }
  return 0;
}

void grep_file(FILE *file, const char *filename, OPT *options, int mult_files) {
  regex_t regex;
  char line[1000];
  int line_number = 0;
  int match_count = 0;

  int cflags = REG_EXTENDED | (options->i == 1 ? REG_ICASE : 0);

  char *big_pattern = combine_patterns(options);
  if (!big_pattern) {
    if (!options->s) {
      fprintf(stderr, "Faild to combine patterns.\n");
    }

    return;
  }

  if (regcomp(&regex, big_pattern, cflags) != 0) {
    if (!options->s) {
      fprintf(stderr, "grep: invalid regcomp().\n");
    }
    free(big_pattern);
    return;
  }

  while (fgets(line, sizeof(line), file)) {
    line_number++;
    int match = (regexec(&regex, line, 0, NULL, 0) == 0);
    if (options->v == 1) {
      match = !match;
    }
    if (match) {
      match_count++;

      if (options->l) {
        printf("%s\n", filename);
        break;
      }

      if (!options->c) {
        if (mult_files && !options->h) {
          printf("%s:", filename);
        }
        if (options->n) {
          printf("%d:", line_number);
        }
        if (options->o) {
          print_match_part(line, &regex);
        } else {
          printf("%s", line);
          if (line[strlen(line) - 1] != '\n') {
            printf("\n");
          }
        }
      }
    }
  }
  if (options->c) {
    if (mult_files && !options->h) {
      printf("%s:", filename);
    }
    printf("%d\n", match_count);
  }
  regfree(&regex);
  free(big_pattern);
}

char *combine_patterns(OPT *options) {
  size_t len = 3;
  for (int i = 0; i < options->count_pat; i++) {
    len += strlen(options->patterns[i]) + 1;
  }
  char *big_pattern = malloc(len * sizeof(char));
  if (!big_pattern) {
    fprintf(stderr, "Memory allocation fail.\n");
    return NULL;
  }
  strcpy(big_pattern, "(");
  for (int i = 0; i < options->count_pat; i++) {
    if (i > 0) {
      strcat(big_pattern, "|");
    }
    strcat(big_pattern, options->patterns[i]);
  }
  strcat(big_pattern, ")");

  return big_pattern;
}

char *my_strdup(const char *str) {
  size_t len = strlen(str) + 1;
  char *dup = malloc(len);
  if (!dup) {
    return NULL;
  }
  memcpy(dup, str, len);
  return dup;
}

void add_patterns_from_file(OPT *options, const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    if (!options->s) {
      fprintf(stderr, "grep: %s: %s\n", filename, strerror(errno));
    }
    return;
  }
  char pattern[1000];

  while (fgets(pattern, sizeof(pattern), f)) {
    size_t len = strlen(pattern);
    if (pattern[len - 1] == '\n') {
      pattern[len - 1] = '\0';
    }
    if (options->count_pat >= options->cap_pat) {
      options->cap_pat *= 2;
      options->patterns =
          realloc(options->patterns, options->cap_pat * sizeof(char *));
    }
    options->patterns[options->count_pat++] = my_strdup(pattern);
  }
  fclose(f);
}

void print_match_part(const char *line, regex_t *regex) {
  regmatch_t match;
  const char *p_line = line;
  while (regexec(regex, p_line, 1, &match, 0) == 0) {
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so), p_line + match.rm_so);
    p_line += match.rm_eo;
  }
}

void free_memory(OPT *options) {
  for (int i = 0; i < options->count_pat; i++) {
    free(options->patterns[i]);
  }
  free(options->patterns);
}
