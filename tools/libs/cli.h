// This header define macros use to parse cli arguments.
// Author: Vincent S. <v.sidot@aniah.fr>
// Date: 2024-09-03
#ifndef CLI_H
#define CLI_H

#include <stddef.h>
#include <stdbool.h>


typedef enum {
    __CLI_STRING,
    __CLI_INT,
    __CLI_FLOAT,
    __CLI_FLAG,
} __CLI_TYPES;

typedef struct {
    char* name;
    __CLI_TYPES type;
    bool required;
    char* description;
} __CLI_ARGS_DESC ;

typedef struct {
    size_t size;
    __CLI_ARGS_DESC* desc;
} __CLI_ARGS_DESC_LIST;

/**
 * @brief Evaluate the memory size needed for the arguments structure
 * 
 * @param desc const __CLI_ARGS_DESC_LIST; The arguments description list
 * @return int The size of the memory needed
 */
size_t cli_evaluate_mem_size(const __CLI_ARGS_DESC_LIST desc);

/**
 * @brief Find the argument description by its name
 * 
 * @param desc[] const __CLI_ARGS_DESC_LIST; The arguments description list
 * @param name const char*; The name of the argument
 */
const __CLI_ARGS_DESC* cli_find_arg_desc(const __CLI_ARGS_DESC_LIST desc, const char* name, size_t *index);

/**
 * @brief Parse the command line arguments. 
 * 
 * @param argc const int; The number of arguments (from main)
 * @param argv const char**; The arguments (from main)
 * @param desc const __CLI_ARGS_DESC_LIST; The arguments description list
 * @return void* Pointer to the arguments structure (NULL if an error occured)
 */
void* cli_parse(const int argc, const char** argv, const __CLI_ARGS_DESC_LIST desc);

/**
 * @brief Free the memory allocated by the cli_parse function
 * 
 * @param args void*; The arguments structure
 * @return void
 */
void cli_free(void* args);

/**
 * @brief Print the last error message
 * @return void;
 */
void cli_print_error();

#endif
