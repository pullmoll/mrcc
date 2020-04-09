// mrcc - A C Compiler system on MapReduce
// Zhiqiang Ma, https://www.ericzma.com
#pragma once

int add_cleanup(const char *filename);

void cleanup_tempfiles_from_signal_handler(void);

void cleanup_tempfiles(void);
