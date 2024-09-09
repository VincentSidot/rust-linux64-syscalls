#include "libs/cli.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  char **foo;
  int bar;
  float baz;
  bool qux;
  char __reserved__[];
} __attribute__((packed)) ARGS;

int main(const int argc, const char *argv[]) {

  printf("sizeof(ARGS): %lu\n", sizeof(ARGS));

  // Create cli description
  __CLI_ARGS_DESC desc[] = {
      {"foo", __CLI_STRING, false,
       "This is foo.\n And to describe it, we need a multiline description.\n "
       "So here it is."},
      {"bar", __CLI_INT, false, "This is bar"},
      {"baz", __CLI_FLOAT, false, "This is baz"},
      {"qux", __CLI_FLAG, false, "This is qux"},
  };
  __CLI_ARGS_DESC_LIST my_desc = {sizeof(desc) / sizeof(__CLI_ARGS_DESC), desc};

  ARGS *my_args = cli_parse(argc, argv, my_desc);

  if (my_args == 0) {
    cli_print_error();
    return 1;
  }

  printf("bar: %i\n", my_args->bar);
  printf("baz: %f\n", my_args->baz);
  printf("qux: %d\n", my_args->qux);
  printf("foo: %s\n", *my_args->foo);

  cli_free(my_args);

  return 0;
}
