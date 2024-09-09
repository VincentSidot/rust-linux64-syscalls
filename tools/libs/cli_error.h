// This header define macros use to parse cli errors.
// Author: Vincent S. <v.sidot@aniah.fr>
// Date: 2024-09-03
#ifndef CLI_ERROR_H
#define CLI_ERROR_H

#define __CLI_UNKNOWN_ERROR             0
#define __CLI_UNABLE_TO_ALLOCATE_MEMORY 1
#define __CLI_UNKNOWN_ARGUMENT          2
#define __CLI_UNKNOWN_TYPE              3
#define __CLI_MISSING_VALUE             4

#define __CLI_ERROR_BUFFER_LEN 1024

void cli_print_error();
char* __cli_ptr_to_buffer();
char** __cli_ptr_to_error_message();

/**
 * @brief Set the last error message
 * 
 * @param error const int; The error code
 * @param ...; The arguments to format the error message (Error message specific)
*/
void set_error(const int error_code, ...);
#define __inner_m_set_error(error, ...) \
  snprintf(__cli_ptr_to_buffer(), __CLI_ERROR_BUFFER_LEN, __cli_ptr_to_error_message()[error], ##__VA_ARGS__)
#endif
