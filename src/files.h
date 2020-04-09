// mrcc - A C Compiler system on MapReduce
// Zhiqiang Ma, https://www.ericzma.com
#pragma once

char* get_basename_no_ext(const char* sfile);
const char * find_basename(const char *sfile);
char* find_extension(const char* sfile);
const char * find_extension_const(const char *sfile);
int is_source(const char *sfile);
int is_object(const char *filename);
int is_preprocessed(const char *sfile);
int source_needs_local(const char *filename);
int output_from_source(const char *sfile, const char *out_extn, char **ofile);

const char * preproc_exten(const char *e);
