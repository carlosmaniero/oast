/*
 * Copyright (C) 2024 Carlos Maniero <carlos@maniero.me>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arena.h"
#include "lexer.h"
#include "ref.h"

void print_help(void);
void print_unknown();
void dump_tokens(char* grammar_path);
char* load_file(arena_t* arena, char* file_path);

int main(int args, char** argsv) {
  if (args < 2 || strcmp(argsv[1], "help") == 0) {
    print_help();
    return 0;
  }

  if (args > 2 || strcmp(argsv[1], "dump-tokens") == 0) {
    dump_tokens(argsv[2]);
    return 0;
  }

  print_unknown();
  return 1;
}

void dump_tokens(char* grammar_path) {
  arena_t arena = arena_new(1024 * 1024 * 4);

  struct ref_source source = {
    .path = grammar_path,
    .contents = load_file(&arena, grammar_path),
  };

  struct lexer lexer;

  lexer_init(&lexer, source);

  struct token token = {0};

  while (token.kind != TOKEN_EOF) {
    lexer_next_token(&lexer, &token);

    printf(
        "%s:%ld:%ld:%s: " FMT_TOKEN_VALUE_FORMATER "\n",
        token.ref.source.path,
        token.ref.cursor.row + 1,
        token.ref.cursor.col + 1,
        lexer_token_kind_to_str(token.kind),
        FMT_TOKEN_VALUE(token));
  }
}

void print_help() {
  const char* oast_help = 
    "oast - olang AST utility\n"
    "\n"
    "Usage:\n"
    "  oast <command> [options] <file>\n"
    "\n"
    "Commands:\n"
    "  help        Display this help message\n"
    "  dump-tokens Dump tokens from an OAST grammar file\n"
    "  check       Test an OAST grammar file against a source\n"
    "\n"
    "Examples:\n"
    "  oast help\n"
    "  oast dump-tokens grammar.oast\n"
    "  oast check grammar.oast source-file";
  puts(oast_help);
}

void print_unknown() {
  puts("error: invalid invocation!\n");

  print_help();
}

char* load_file(arena_t* arena, char* file_path) {
  FILE* file = fopen(file_path, "r");

  if (!file) {
      perror("Error opening file");
      exit(1);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = arena_alloc(arena, length + 1);

  if (!buffer) {
      fclose(file);
      perror("Memory allocation failed");
      exit(1);
  }

  size_t bytesRead = fread(buffer, 1, length, file);
  fclose(file);

  buffer[bytesRead] = '\0';

  return buffer;
}
