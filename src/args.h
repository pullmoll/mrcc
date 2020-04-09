// mrcc - A C Compiler system on MapReduce
// Zhiqiang Ma, https://www.ericzma.com
#pragma once

unsigned int argv_len(char* argv[]);

int copy_argv(char *from[], char **out[], int delta);

int find_compiler(char **argv, char ***out_argv);

int argv_append(char **argv, char *toadd);
int scan_args(char *argv[], char **input_file, char **output_file, char ***ret_newargv);
int expand_preprocessor_options(char ***argv_ptr);
void free_argv(char **argv);

int strip_dasho(char **from, char ***out_argv);

int strip_local_args(char **from, char ***out_argv);


char *argv_tostr(char* argv[]);

int set_action_opt(char **a, const char *new_c);
