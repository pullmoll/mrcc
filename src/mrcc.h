// mrcc - A C Compiler system on MapReduce
// Zhiqiang Ma, https://www.ericzma.com
#pragma once

/**
 * Name of the program, to be included in log messages.
 *
 * @note This must be defined exactly once in each program that links to
 * trace.c
 */
extern const char *rs_program_name;

int main(int argc, char* argv[]);

