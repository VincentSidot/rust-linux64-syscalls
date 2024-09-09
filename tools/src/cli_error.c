#include "../libs/cli_error.h"
#include <stdarg.h>
#include <stdio.h>

const char *__CLI_ERROR_MESSAGE[] = {
    "Unknown error\n",
    "Unable to allocate memory, see errno for more information\n",
    "Unknown argument: \"%s\"\n",
    "Unknown type for argument: \"%s\"\n",
    "Missing value for argument: \"%s\"\n",
};
int __CLI_LAST_ERROR = __CLI_UNKNOWN_ERROR;
char __CLI_ERROR_BUFFER[__CLI_ERROR_BUFFER_LEN] = {0};

void cli_print_error() {
  printf("%s", __CLI_ERROR_BUFFER);
  return;
}

char *__cli_ptr_to_buffer() { return __CLI_ERROR_BUFFER; }
char **__cli_ptr_to_error_message() { return (char **)__CLI_ERROR_MESSAGE; }

void set_error(const int error_code, ...) {
  va_list args;
  va_start(args, error_code);
  
  const char *error_message = __CLI_ERROR_MESSAGE[error_code];


}
