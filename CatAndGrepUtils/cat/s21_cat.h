#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int T;
  int E;
  int Error;
} flags;

flags reader(int argc, char **argv);
void set_flags(int current_flag, flags *flags);

void output(char **argv, int argc, const flags flags);

void cat_no_args();

void print_flag_v(int ch, int *is_simple_symbol);
void print_file(const char *filename, flags option, int *line_counter,
                int *counter_empty_liner, int *prev_char);

void print_number_and_tab(int line_counter);

void process_flags(FILE *file, flags flags, int *line_counter,
                   int *counter_empty_line, int *prev_char);

void process_flag_n(flags flags, int prev_char, int *line_counter);
void process_flag_b(flags flags, int ch, int prev_char, int *line_counter);
void process_flag_s(flags flags, int ch, int prev_char,
                    int *counter_empty_line);
void process_flag_v(flags flags, int ch, int *is_simple_symbol);
void process_flag_e(flags flags, int ch, int *is_simple_symbol);
void process_flag_E(const flags flags, const int ch);
void process_flag_t(flags flags, int ch, int *is_simple_symbol);
void process_flag_T(flags flags, int ch, int *is_simple_symbol);

void print_and_increase_counter(int *line_counter);
