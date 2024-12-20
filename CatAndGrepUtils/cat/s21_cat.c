#include "s21_cat.h"

int main(int argc, char **argv) {
  const flags flags = reader(argc, argv);
  output(argv, argc, flags);
  return 0;
}

flags reader(int argc, char **argv) {
  if (argc == 1) {
    cat_no_args();
  }
  const struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                        {"number", 0, NULL, 'n'},
                                        {"squeeze-blank", 0, NULL, 's'},
                                        {NULL, 0, NULL, 0}};
  int current_flag = 0;
  flags flags = {0};
  while ((current_flag =
              getopt_long(argc, argv, "+TtsnEveb", long_options, NULL)) != -1) {
    set_flags(current_flag, &flags);
  }
  return flags;
}

void output(char **argv, int argc, const flags flags) {
  int line_counter = 1;
  int prev_char = '\n';
  int counter_empty_line = 0;
  for (int i = optind; i < argc; ++i) {
    print_file(argv[i], flags, &line_counter, &counter_empty_line, &prev_char);
  }
}

void set_flags(int current_flag, flags *flags) {
  switch (current_flag) {
    case 'T':
      flags->T = 1;
      break;
    case 't':
      flags->t = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'E':
      flags->E = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'e':
      flags->e = 1;
      break;
    case 'b':
      flags->b = 1;
      break;
    default:
      flags->Error = 1;
  }
}

void cat_no_args() {
  char buffer[BUFSIZ];
  const int file_read = 1;
  int bytes_read = 0;

  bytes_read = read(file_read, buffer, 8192);

  while (bytes_read != -1) {
    printf("%.*s", bytes_read, buffer);
    bytes_read = read(file_read, buffer, 8192);
  }
}

void print_file(const char *filename, const flags flags, int *line_counter,
                int *counter_empty_line, int *prev_char) {
  if (flags.Error) {
  } else {
    FILE *file = fopen(filename, "r");
    if (file) {
      process_flags(file, flags, line_counter, counter_empty_line, prev_char);
      fclose(file);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", filename);
    }
  }
}

void process_flags(FILE *file, const flags flags, int *line_counter,
                   int *counter_empty_line, int *prev_char) {
  int is_simple_symbol = 1;
  int ch = 0;
  while ((ch = fgetc(file)) != EOF) {
    process_flag_n(flags, *prev_char, line_counter);
    process_flag_b(flags, ch, *prev_char, line_counter);
    process_flag_s(flags, ch, *prev_char, counter_empty_line);
    process_flag_v(flags, ch, &is_simple_symbol);
    process_flag_e(flags, ch, &is_simple_symbol);
    process_flag_E(flags, ch);
    process_flag_t(flags, ch, &is_simple_symbol);
    process_flag_T(flags, ch, &is_simple_symbol);
    if (*counter_empty_line <= 1 && is_simple_symbol) {
      printf("%c", ch);
    }
    *prev_char = ch;
  }
}

void print_number_and_tab(const int line_counter) {
  printf("%6d\t", line_counter);
}

void print_flag_v(const int ch, int *is_simple_symbol) {
  if (ch < 32 && ch != '\t' && ch != '\n') {
    printf("^%c", ch + 64);
    *is_simple_symbol = 0;
  } else if (ch == 127) {
    printf("^?");
    *is_simple_symbol = 0;
  } else if (ch > 127 && ch < 160) {
    printf("M-^%c", ch - 64);
    *is_simple_symbol = 0;
  } else if (ch >= 160) {
    if (ch - 128 == 127) {
      printf("M-^?");
    } else {
      printf("M-%c", ch - 128);
    }
    *is_simple_symbol = 0;
  } else {
    *is_simple_symbol = 1;
  }
}

void process_flag_n(const flags flags, const int prev_char, int *line_counter) {
  if (flags.n && flags.b == 0 && prev_char == '\n') {
    print_and_increase_counter(line_counter);
  }
}

void process_flag_b(const flags flags, const int ch, const int prev_char,
                    int *line_counter) {
  if (flags.b && prev_char == '\n' && ch != '\n') {
    print_and_increase_counter(line_counter);
  }
}

void process_flag_s(const flags flags, const int ch, const int prev_char,
                    int *counter_empty_line) {
  if (flags.s) {
    if (prev_char == '\n' && ch == '\n') {
      (*counter_empty_line)++;
    } else {
      *counter_empty_line = 0;
    }
  }
}

void process_flag_v(const flags flags, const int ch, int *is_simple_symbol) {
  if (flags.v) {
    print_flag_v(ch, is_simple_symbol);
  }
}

void process_flag_e(const flags flags, const int ch, int *is_simple_symbol) {
  if (flags.e) {
    if (ch == '\n') {
      printf("$");
    }
    print_flag_v(ch, is_simple_symbol);
  }
}

void process_flag_E(const flags flags, const int ch) {
  if (flags.E && ch == '\n') {
    printf("$");
  }
}

void process_flag_t(const flags flags, const int ch, int *is_simple_symbol) {
  if (flags.t) {
    if (ch == '\t') {
      printf("^I");
      *is_simple_symbol = 0;
    } else {
      print_flag_v(ch, is_simple_symbol);
    }
  }
}

void process_flag_T(const flags flags, const int ch, int *is_simple_symbol) {
  if (flags.T) {
    if (ch == '\t') {
      printf("^I");
      *is_simple_symbol = 0;
    } else {
      *is_simple_symbol = 1;
    }
  }
}

void print_and_increase_counter(int *line_counter) {
  print_number_and_tab(*line_counter);
  (*line_counter)++;
}