#include "s21_grep.h"

int main(const int argc, char **argv) {
  if (check_args(argc, argv)) {
    return 1;
  }
  params_t params;
  init(&params);
  flags flags = reader(argc, argv, &params);
  output(&flags, argc, argv, &params);

  return 0;
}

int check_args(int argc, char **argv) {
  int error = 0;
  if (argv == NULL) {
    perror("Input arguments");
    error = 1;
  }
  if (argc < 2) {
    fprintf(stderr, "Not enough arguments\n");
    error = 1;
  }
  return error;
}

void init(params_t *params) {
  params->count_e = 0;
  params->multi_file = 0;
  params->file_name = "";
  params->line_counter = 0;
}

flags reader(const int argc, char **argv, params_t *params) {
  int current_flag = 0;
  flags flags = {0};
  int line_counter_for_f_flag = 0;
  while ((current_flag = getopt(argc, argv, "e:ivclnhsof:")) != -1) {
    switch (current_flag) {
      case 'e':
        process_e_flag(&flags, params);
        break;
      case 'f':
        process_f_flag(&flags, &line_counter_for_f_flag, params);
        break;
      default:
        set_flag(&flags, current_flag);
        break;
    }
  }
  if (optind + 1 >= argc) {
    flags.error = 1;
  }
  set_if_multifile(argc, flags.e, flags.f, params);
  return flags;
}

void process_e_flag(flags *flags, params_t *params) {
  flags->e = 1;
  params->count_e++;
  if (params->count_e == 1) {
    strcpy(params->pattern, optarg);
  } else {
    strcat(params->pattern, "|");
    strcat(params->pattern, optarg);
  }
}

void process_f_flag(flags *flags, int *line_counter_for_f_flag,
                    params_t *params) {
  flags->f = 1;
  FILE *f_file = fopen(optarg, "r");
  if (f_file) {
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, f_file) != EOF) {
      remove_invisible_characters(line);
      concatenate_string(line_counter_for_f_flag, line, params);
    }
    free(line);
  }
  fclose(f_file);
}

void concatenate_string(int *line_counter_for_f_flag, const char *line,
                        params_t *params) {
  if (*line_counter_for_f_flag == 0) {
    strcpy(params->pattern, line);
  } else {
    strcat(params->pattern, "\\|");
    strcat(params->pattern, line);
  }
  (*line_counter_for_f_flag)++;
}

void set_flag(flags *flags, int flag) {
  switch (flag) {
    case 'i':
      flags->i = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'o':
      flags->o = 1;
      break;
    default:
      flags->error = 1;
      break;
  }
}

void output(flags *flags, int argc, char **argv, params_t *params) {
  int number_file = flags->e || flags->f ? optind : optind + 1;
  if (params->multi_file == 0) {
    search(*flags, argv, number_file, params);
  } else {
    for (; number_file < argc; number_file++) {
      search(*flags, argv, number_file, params);
    }
  }
}

void set_if_multifile(int argc, int is_flag_e, int is_flag_f,
                      params_t *params) {
  if (argc - optind > 2 && is_flag_e == 0 && is_flag_f == 0) {
    params->multi_file = 1;
  }

  if (argc - optind + 1 > 2 && (is_flag_e == 1 || is_flag_f == 1)) {
    params->multi_file = 1;
  }
}

void remove_invisible_characters(char *line) {
  char *src, *dst;
  for (src = dst = line; *src != '\0'; src++) {
    if (*src != '\n') {
      *dst++ = *src;
    }
  }
  *dst = '\0';
}

void search(flags flags, char **argv, int which_number_file, params_t *params) {
  if (!flags.e && !flags.f) {
    strcpy(params->pattern, argv[optind]);
  }

  params->line_counter = 0;
  regex_t re;
  int status = process_status(flags, &re, params);
  params->file_name = argv[which_number_file];

  FILE *file = fopen(params->file_name, "r");
  check_errors(file, flags.s, re, status, params);
  search_in_file(&flags, re, file, params);

  if (flags.c) {
    print_file_name(flags.h, params);
    printf("%d\n", flags.count_line);
  }
  regfree(&re);
  fclose(file);
}

void print_file_name(int is_flag_h, const params_t *params) {
  if (params->multi_file && !is_flag_h) {
    printf("%s:", params->file_name);
  }
}

void search_in_file(flags *flags, regex_t re, FILE *file, params_t *params) {
  char *tmp_line = NULL;
  size_t len = 0;
  const int result_regexec = flags->v ? REG_NOMATCH : 0;
  ssize_t length_tmp_line = 0;
  int is_print_file = 0;
  while ((length_tmp_line = getline(&tmp_line, &len, file)) != EOF &&
         !is_print_file) {
    if (flags->l) {
      tmp_line = add_string_end(length_tmp_line, tmp_line);
      int success = regexec(&re, tmp_line, 0, NULL, 0);
      if (success == 0) {
        printf("%s\n", params->file_name);
        is_print_file = 1;
      }
    } else {
      print_line(tmp_line, length_tmp_line, result_regexec, &re, flags, params);
    }
  }
  if (tmp_line) free(tmp_line);
}

int process_status(flags flags, regex_t *re, params_t *params) {
  int status = 1;
  if (flags.i) {
    status = regcomp(re, params->pattern, REG_ICASE);
  } else {
    if (flags.e) {
      status = regcomp(re, params->pattern, REG_EXTENDED);
    } else {
      status = regcomp(re, params->pattern, 0);
    }
  }
  return status;
}

void check_errors(const FILE *file, int is_flag_s, regex_t re, int status,
                  const params_t *params) {
  if (file == NULL) {
    if (!is_flag_s) {
      fprintf(stderr, "Unable to open file: %s", params->file_name);
    }
    regfree(&re);
    exit(-1);
  }
  if (status != 0 && !is_flag_s) {
    fprintf(stderr, "Regular Expression Error");
    regfree(&re);
    exit(-1);
  }
}

void print_line(char *line, ssize_t length_line, const int result_regexec,
                regex_t *re, flags *flags, params_t *params) {
  line = add_string_end(length_line, line);
  if (line && length_line != EOF) {
    if (flags->o) {
      process_output_o_flag(line, re, flags->h, params);
    } else {
      int success = 0;
      success = regexec(re, line, 0, NULL, 0);

      if (flags->n) {
        params->line_counter++;
      }

      if (success == result_regexec) {
        process_output(flags, line, params);
      }
    }
  }
}

char *add_string_end(ssize_t length_tmp_line, char *tmp_line) {
  if (length_tmp_line > 0 && tmp_line[length_tmp_line - 1] == '\n') {
    tmp_line[length_tmp_line - 1] = '\0';
  }
  return tmp_line;
}

void process_output_o_flag(char *line, regex_t *re, const int is_flag_h,
                           const params_t *params) {
  regmatch_t pmatch[1];
  char *p = line;

  while (regexec(re, p, 1, pmatch, 0) == 0) {
    print_file_name(is_flag_h, params);
    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           &p[pmatch[0].rm_so]);
    p += pmatch[0].rm_eo;
  }
}

void process_output(flags *flags, const char *line, const params_t *params) {
  if (flags->c) {
    flags->count_line += 1;
  } else {
    print_file_name(flags->h, params);
    if (flags->n) {
      printf("%d:%s\n", params->line_counter, line);
    } else {
      printf("%s\n", line);
    }
  }
}