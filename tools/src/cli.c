#include "../libs/cli.h"
#include "../libs/cli_error.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PTR_ADD(ptr, offset) ((void *)((size_t)ptr + offset))

typedef unsigned char byte;

size_t cli_evaluate_mem_size(const __CLI_ARGS_DESC_LIST l_desc) {
  size_t size = 0;
  for (size_t i = 0; i < l_desc.size; i++) {
    __CLI_ARGS_DESC *desc = &l_desc.desc[i];
    switch (desc->type) {
    case __CLI_STRING:
      size += sizeof(char**); // String is at least a '\0'
      break;
    case __CLI_INT:
      size += sizeof(int);
      break;
    case __CLI_FLOAT:
      size += sizeof(float);
      break;
    case __CLI_FLAG:
      size += sizeof(bool);
      break;
    default:
      set_error(__CLI_UNKNOWN_TYPE, desc->name);
      return 0;
    }
  }
  printf("memsize: %lu\n", size);
  return size;
}

const __CLI_ARGS_DESC *cli_find_arg_desc(const __CLI_ARGS_DESC_LIST l_desc,
                                         const char *name, size_t *index) {
  assert(name[0] != 0x00 && name[1] != 0x00);

  if (name[0] != '-' || name[1] != '-') {
    return NULL;
  }

  for (size_t i = 0; i < l_desc.size; i++) {
    __CLI_ARGS_DESC *desc = &l_desc.desc[i];
    if (strcmp(desc->name, name + 2) == 0) {
      *index = i;
      return desc;
    }
  }
  return NULL;
}

char *CLI_TYPES_FORMATED[] = {
    " [string] :",
    " [int]    :",
    " [float]  :",
    " [flag]   :",
};

void cli_display_help_message(const __CLI_ARGS_DESC_LIST l_desc) {
  char *type;
  for (size_t i = 0; i < l_desc.size; i++) {
    __CLI_ARGS_DESC *desc = &l_desc.desc[i];
    type = CLI_TYPES_FORMATED[desc->type];
    printf("- %s %s %s\n", desc->name, type, desc->description);
  }
}

void *cli_parse(const int argc, const char **argv,
                const __CLI_ARGS_DESC_LIST desc) {
  // Ensure the arguments are not null

  size_t size = cli_evaluate_mem_size(desc);
  void *arguments = malloc(size);
  if (arguments == NULL) {
    set_error(__CLI_UNABLE_TO_ALLOCATE_MEMORY);
    return NULL;
  }

  // Fill the arguments structure

  // Create the arguments structure offset
  byte *offset = malloc(sizeof(size_t) * desc.size);
  if (offset == NULL) {
    set_error(__CLI_UNABLE_TO_ALLOCATE_MEMORY);
    return NULL;
  }

  size_t current_offset = 0;
  // for (size_t i = desc.size - 1; i+1 > 0; i--) {
  for (size_t i = 0; i < desc.size; i++) {
    __CLI_ARGS_DESC *arg_desc = &desc.desc[i];
    switch (arg_desc->type) {
    case __CLI_STRING:
      offset[i] = current_offset;
      current_offset += sizeof(char**);
      break;
    case __CLI_INT:
      offset[i] = current_offset;
      current_offset += sizeof(int);
      break;
    case __CLI_FLOAT:
      offset[i] = current_offset;
      current_offset += sizeof(float);
      break;
    case __CLI_FLAG:
      offset[i] = current_offset;
      current_offset += sizeof(bool);
      break;
    default:
      set_error(__CLI_UNKNOWN_TYPE, arg_desc->name);
      goto __CLI_CLEANUP;
    }
  }

  
  int current_end_offset = size;
  // Create the arguments structure
  for (size_t i = 1; i < (size_t)argc; i++) {
    const char *name = argv[i];
    if (strlen(name) < 2 || name[0] != '-') {
      set_error(__CLI_UNKNOWN_ARGUMENT, name);
      return NULL;
    }
    // Check for reserved arguments
    if (strcmp(name, "--help") == 0) {
      cli_display_help_message(desc);
      exit(0);
    } else {
      size_t arg_index;
      size_t current_offset = 0;
      const __CLI_ARGS_DESC *arg_desc =
          cli_find_arg_desc(desc, name, &arg_index);

      printf("offset[arg_index]: %lu\n", offset[arg_index]);

      if (arg_desc == NULL) {
        set_error(__CLI_UNKNOWN_ARGUMENT, name);
        return NULL;
      }
      switch (arg_desc->type) {
      case __CLI_STRING:
        if (i + 1 >= (size_t)argc) {
          set_error(__CLI_MISSING_VALUE, arg_desc->name);
          goto __CLI_CLEANUP;
        }
        size_t str_len = strlen(argv[i + 1]);
        // Reallocate the memory to store the string
        arguments = realloc(arguments, size + str_len);
        printf("memsize: %d\n", size + str_len);
        if (arguments == NULL) {
          set_error(__CLI_UNABLE_TO_ALLOCATE_MEMORY);
          goto __CLI_CLEANUP;
        }
        // Copy the string to reserved memory
        memcpy(PTR_ADD(arguments, current_end_offset), argv[i + 1], str_len);
        // Copy the pointer to the arguments structure
        memcpy(PTR_ADD(arguments, offset[arg_index]), PTR_ADD(arguments, current_end_offset), sizeof(size_t));
        current_end_offset += str_len;
        i++;
        break;
      case __CLI_INT:
        if (i + 1 >= (size_t)argc) {
          set_error(__CLI_MISSING_VALUE, arg_desc->name);
          goto __CLI_CLEANUP;
        }
        int int_value = atoi(argv[i + 1]);
        printf("int_value: %d | offset[arg_index]: %lu\n", int_value,
               offset[arg_index]);
        memcpy(PTR_ADD(arguments, offset[arg_index]), &int_value, sizeof(int));
        i++;
        break;
      case __CLI_FLOAT:
        if (i + 1 >= (size_t)argc) {
          set_error(__CLI_MISSING_VALUE, arg_desc->name);
          goto __CLI_CLEANUP;
        }
        float float_value = (float)atof(argv[i + 1]);
        memcpy(PTR_ADD(arguments, offset[arg_index]), &float_value, sizeof(float));
        i++;
        break;
      case __CLI_FLAG:;
        bool flag_value = true;
        memcpy(PTR_ADD(arguments, offset[arg_index]), &flag_value, sizeof(bool));
        break;
      default:
        set_error(__CLI_UNKNOWN_TYPE, arg_desc->name);
        goto __CLI_CLEANUP;
      }
    }
  }

  return arguments;
__CLI_CLEANUP:
  free(arguments);
  return NULL;
}

void cli_free(void *args) {
  // TODO: Also free the string inside the structure.

  free(args);
}
