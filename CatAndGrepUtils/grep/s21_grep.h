#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int count_e;
  int count_f;
  int multi_file;
  int line_counter;
  char *file_name;
  char *file_name_f;
  char pattern[8192];
} params_t;

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
  int count_line;
  int error;
} flags;

int check_args(int argc, char **argv);

void init();

void output(flags *flags, int argc, char **argv, params_t *params);

flags reader(const int argc, char **argv, params_t *params);

void process_e_flag(flags *flags, params_t *params);
void process_f_flag(flags *flags, int *line_counter_for_f_flag,
                    params_t *params);
void set_flag(flags *flags, int flag);
void search(flags flags, char **argv, int which_number_file, params_t *params);
void print_file_name(int is_flag_h, const params_t *params);
void search_in_file(flags *flags, regex_t re, FILE *file, params_t *params);
void remove_invisible_characters(char *line);
void concatenate_string(int *line_counter_for_f_flag, const char *line,
                        params_t *params);
void set_if_multifile(int argc, int is_flag_e, int is_flag_f, params_t *params);
int process_status(flags flags, regex_t *re, params_t *params);
void check_errors(const FILE *file, int is_flag_s, regex_t re, int status,
                  const params_t *params);
char *add_string_end(ssize_t length_tmp_line, char *tmp_line);
void process_output_o_flag(char *line, regex_t *re, const int is_flag_h,
                           const params_t *params);
void print_line(char *line, ssize_t length_line, const int result_regexec,
                regex_t *re, flags *flags, params_t *params);
void process_output(flags *flags, const char *line, const params_t *params);
