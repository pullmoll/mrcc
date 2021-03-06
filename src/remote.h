// mrcc - A C Compiler system on MapReduce
// Zhiqiang Ma, https://www.ericzma.com
#pragma once

int compile_remote(char **argv,
                       char *input_fname,
                       char *cpp_fname,
                       char **files,
                       char *output_fname,
                       char *deps_fname,
                       char *server_stderr_fname,
                       pid_t cpp_pid,
                       int local_cpu_lock_fd,
                       struct hostdef *host,
                       int *status);

int put_cpp_fs(char* cpp_fname);
int put_config_fs(char** argv,
        const char* input_fname,
        const char* cpp_fname,
        const char* output_fname);

/* we cleanup them at atexit
//int clean_up_config_fs(char* cpp_fname);
//int clean_up_dir_fs(char* local_fname);
//int clean_up_file_fs(char* local_fname);
//int clean_up_outdir_fs(char* cpp_fname);
*/

