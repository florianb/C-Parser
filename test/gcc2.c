/* Compiler driver program that can handle many languages.
   Copyright (C) 1987, 1989, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
   2010, 2011
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This program is the user interface to the C compiler and possibly to
other compilers.  It is used because compilation is a complicated procedure
which involves running several programs and passing temporary files between
them, forwarding the users switches to those programs selectively,
and deleting the temporary files at the end.

CC recognizes how to compile each input file by suffixes in the file names.
Once it knows which kind of compilation to perform, the procedure for
compilation is specified by a string called a "spec".  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "multilib.h" /* before tm.h */
#include "tm.h"
#include "xregex.h"
#include "obstack.h"
#include "intl.h"
#include "prefix.h"
#include "gcc.h"
#include "diagnostic.h"
#include "flags.h"
#include "opts.h"
#include "vec.h"

/* By default there is no special suffix for target executables.  */
/* FIXME: when autoconf is fixed, remove the host check - dj */
#if defined(TARGET_EXECUTABLE_SUFFIX) && defined(HOST_EXECUTABLE_SUFFIX)
#define HAVE_TARGET_EXECUTABLE_SUFFIX
#endif

/* By default there is no special suffix for host executables.  */
#ifdef HOST_EXECUTABLE_SUFFIX
#define HAVE_HOST_EXECUTABLE_SUFFIX
#else
#define HOST_EXECUTABLE_SUFFIX ""
#endif

/* By default, the suffix for target object files is ".o".  */
#ifdef TARGET_OBJECT_SUFFIX
#define HAVE_TARGET_OBJECT_SUFFIX
#else
#define TARGET_OBJECT_SUFFIX ".o"
#endif

static const char dir_separator_str[] = { DIR_SEPARATOR, 0 };

/* Most every one is fine with LIBRARY_PATH.  For some, it conflicts.  */
#ifndef LIBRARY_PATH_ENV
#define LIBRARY_PATH_ENV "LIBRARY_PATH"
#endif

/* If a stage of compilation returns an exit status >= 1,
   compilation of that file ceases.  */

#define MIN_FATAL_STATUS 1

/* Flag set by cppspec.c to 1.  */
int is_cpp_driver;

/* Flag set to nonzero if an @file argument has been supplied to gcc.  */
static bool at_file_supplied;

/* Definition of string containing the arguments given to configure.  */
#include "configargs.h"

/* Flag saying to print the command line options understood by gcc and its
   sub-processes.  */

static int print_help_list;

/* Flag saying to print the version of gcc and its sub-processes.  */

static int print_version;

/* Flag indicating whether we should ONLY print the command and
   arguments (like verbose_flag) without executing the command.
   Displayed arguments are quoted so that the generated command
   line is suitable for execution.  This is intended for use in
   shell scripts to capture the driver-generated command line.  */
static int verbose_only_flag;

/* Flag indicating how to print command line options of sub-processes.  */

static int print_subprocess_help;

/* Whether we should report subprocess execution times to a file.  */

FILE *report_times_to_file = NULL;

/* Nonzero means place this string before uses of /, so that include
   and library files can be found in an alternate location.  */

#ifdef TARGET_SYSTEM_ROOT
static const char *target_system_root = TARGET_SYSTEM_ROOT;
#else
static const char *target_system_root = 0;
#endif

/* Nonzero means pass the updated target_system_root to the compiler.  */

static int target_system_root_changed;

/* Nonzero means append this string to target_system_root.  */

static const char *target_sysroot_suffix = 0;

/* Nonzero means append this string to target_system_root for headers.  */

static const char *target_sysroot_hdrs_suffix = 0;

/* Nonzero means write "temp" files in source directory
   and use the source file's name in them, and don't delete them.  */

static enum save_temps {
  SAVE_TEMPS_NONE,		/* no -save-temps */
  SAVE_TEMPS_CWD,		/* -save-temps in current directory */
  SAVE_TEMPS_OBJ		/* -save-temps in object directory */
} save_temps_flag;

/* Output file to use to get the object directory for -save-temps=obj  */
static char *save_temps_prefix = 0;
static size_t save_temps_length = 0;

/* The compiler version.  */

static const char *compiler_version;

/* The target version.  */

static const char *const spec_version = DEFAULT_TARGET_VERSION;

/* The target machine.  */

static const char *spec_machine = DEFAULT_TARGET_MACHINE;

/* Nonzero if cross-compiling.
   When -b is used, the value comes from the `specs' file.  */

#ifdef CROSS_DIRECTORY_STRUCTURE
static const char *cross_compile = "1";
#else
static const char *cross_compile = "0";
#endif

/* Greatest exit code of sub-processes that has been encountered up to
   now.  */
static int greatest_status = 1;

/* This is the obstack which we use to allocate many strings.  */

static struct obstack obstack;

/* This is the obstack to build an environment variable to pass to
   collect2 that describes all of the relevant switches of what to
   pass the compiler in building the list of pointers to constructors
   and destructors.  */

static struct obstack collect_obstack;

/* Forward declaration for prototypes.  */
struct path_prefix;
struct prefix_list;

static void init_spec (void);
static void store_arg (const char *, int, int);
static void insert_wrapper (const char *);
static char *load_specs (const char *);
static void read_specs (const char *, int);
static void set_spec (const char *, const char *);
static struct compiler *lookup_compiler (const char *, size_t, const char *);
static char *build_search_list (const struct path_prefix *, const char *,
				bool, bool);
static void xputenv (const char *);
static void putenv_from_prefixes (const struct path_prefix *, const char *,
				  bool);
static int access_check (const char *, int);
static char *find_a_file (const struct path_prefix *, const char *, int, bool);
static void add_prefix (struct path_prefix *, const char *, const char *,
			int, int, int);
static void add_sysrooted_prefix (struct path_prefix *, const char *,
				  const char *, int, int, int);
static char *skip_whitespace (char *);
static void delete_if_ordinary (const char *);
static void delete_temp_files (void);
static void delete_failure_queue (void);
static void clear_failure_queue (void);
static int check_live_switch (int, int);
static const char *handle_braces (const char *);
static inline bool input_suffix_matches (const char *, const char *);
static inline bool switch_matches (const char *, const char *, int);
static inline void mark_matching_switches (const char *, const char *, int);
static inline void process_marked_switches (void);
static const char *process_brace_body (const char *, const char *, const char *, int, int);
static const struct spec_function *lookup_spec_function (const char *);
static const char *eval_spec_function (const char *, const char *);
static const char *handle_spec_function (const char *);
static char *save_string (const char *, int);
static void set_collect_gcc_options (void);
static int do_spec_1 (const char *, int, const char *);
static int do_spec_2 (const char *);
static void do_option_spec (const char *, const char *);
static void do_self_spec (const char *);
static const char *find_file (const char *);
static int is_directory (const char *, bool);
static const char *validate_switches (const char *);
static void validate_all_switches (void);
static inline void validate_switches_from_spec (const char *);
static void give_switch (int, int);
static int used_arg (const char *, int);
static int default_arg (const char *, int);
static void set_multilib_dir (void);
static void print_multilib_info (void);
static void perror_with_name (const char *);
static void display_help (void);
static void add_preprocessor_option (const char *, int);
static void add_assembler_option (const char *, int);
static void add_linker_option (const char *, int);
static void process_command (unsigned int, struct cl_decoded_option *);
static int execute (void);
static void alloc_args (void);
static void clear_args (void);
static void fatal_signal (int);
#if defined(ENABLE_SHARED_LIBGCC) && !defined(REAL_LIBGCC_SPEC)
static void init_gcc_specs (struct obstack *, const char *, const char *,
			    const char *);
#endif
#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
static const char *convert_filename (const char *, int, int);
#endif

static const char *getenv_spec_function (int, const char **);
static const char *if_exists_spec_function (int, const char **);
static const char *if_exists_else_spec_function (int, const char **);
static const char *replace_outfile_spec_function (int, const char **);
static const char *remove_outfile_spec_function (int, const char **);
static const char *version_compare_spec_function (int, const char **);
static const char *include_spec_function (int, const char **);
static const char *find_file_spec_function (int, const char **);
static const char *find_plugindir_spec_function (int, const char **);
static const char *print_asm_header_spec_function (int, const char **);
static const char *compare_debug_dump_opt_spec_function (int, const char **);
static const char *compare_debug_self_opt_spec_function (int, const char **);
static const char *compare_debug_auxbase_opt_spec_function (int, const char **);
static const char *pass_through_libs_spec_func (int, const char **);