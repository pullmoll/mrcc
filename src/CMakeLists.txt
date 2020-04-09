cmake_minimum_required(VERSION 3.8)

###############################################################################
# Follow ISO C99 standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)          # Use GNU extensions and POSIX standard

###############################################################################
# Check for a variety of include files
include(CheckIncludeFile)
check_include_file (dlfcn.h HAVE_DLFCN_H)
check_include_file (execinfo.h HAVE_EXECINFO_H)
check_include_file (glob.h HAVE_GLOB_H)
check_include_file (inttypes.h HAVE_INTTYPES_H)
check_include_file (memory.h HAVE_MEMORY_H)
check_include_file (pwd.h HAVE_PWD_H)
check_include_file (stdarg.h HAVE_STDARG_H)
check_include_file (stdint.h HAVE_STDINT_H)
check_include_file (stdlib.h HAVE_STDLIB_H)
check_include_file (string.h HAVE_STRING_H)
check_include_file (strings.h HAVE_STRINGS_H)
check_include_file (sys/stat.h HAVE_SYS_STAT_H)
check_include_file (sys/syscall.h HAVE_SYS_SYSCALL_H)
check_include_file (sys/time.h HAVE_SYS_TIME_H)
check_include_file (sys/types.h HAVE_SYS_TYPES_H)
check_include_file (sys/utsname.h HAVE_SYS_UTSNAME_H)
check_include_file (sys/wait.h HAVE_SYS_WAIT_H)
check_include_file (sys/poll.h HAVE_SYS_POLL_H)
check_include_file (syscall.h HAVE_SYSCALL_H)
check_include_file (syslog.h HAVE_SYSLOG_H)
check_include_file (ucontext.h HAVE_UCONTEXT_H)
check_include_file (unistd.h HAVE_UNISTD_H)
check_include_file (pwd.h HAVE_PWD_H)

configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(mrcclib
        args.c cleanup.c compile.c exec.c files.c
        io.c mrutils.c netfsutils.c remote.c safeguard.c
        stringutils.c tempfile.c trace.c traceenv.c utils.c)
target_include_directories(mrcclib PUBLIC "${PROJECT_BINARY_DIR}/src")

add_executable(mrcc mrcc.c)
target_link_libraries(mrcc mrcclib)

add_executable(mrcc-map mrcc-map.c)
target_link_libraries(mrcc-map mrcclib)
