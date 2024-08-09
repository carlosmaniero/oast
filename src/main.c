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
#include "parser.h"
#include "ref.h"
#include "emitter.h"

void print_help(void);
void print_unknown();
void dump_tokens(char* grammar_path);
void dump_ast(char* grammar_path);
void check(char* grammar_path, char* source_path);
char* load_file(arena_t* arena, char* file_path);

int main(int args, char** argsv) {
  if (args < 2 || strcmp(argsv[1], "help") == 0) {
    print_help();
    return 0;
  }

  if (args > 2 && strcmp(argsv[1], "dump-tokens") == 0) {
    dump_tokens(argsv[2]);
    return 0;
  }

  if (args > 2 && strcmp(argsv[1], "dump-ast") == 0) {
    dump_ast(argsv[2]);
    return 0;
  }

  if (args > 3 && strcmp(argsv[1], "check") == 0) {
    check(argsv[2], argsv[3]);
    return 0;
  }

  print_unknown();
  return 1;
}

void dump_ast(char* grammar_path) {
  arena_t arena = arena_new(1024 * 1024 * 4);

  ref_source_t source = {
    .path = grammar_path,
    .contents = load_file(&arena, grammar_path),
  };

  lexer_t lexer;

  symbol_table_t symbol_table;
  symbol_table_init(&symbol_table, &arena);

  lexer_init(&lexer, &symbol_table, source);

  parser_t parser;

  parser_init(&parser, &symbol_table, &arena);

  ast_t* ast = parser_parse(&parser, &lexer);

  printf("AST");

  list_item_t* item = list_head(&ast->productions);

  while (item) {
    ast_production_t* production = item->value;
    printf("\n- "FMT_TOKEN_VALUE_FORMATER, FMT_TOKEN_VALUE(production->head.token));

    list_item_t* body_item = list_head(&production->body);

    while (body_item) {
      ast_production_body_t* body = body_item->value;
      // TODO: evaluate the production type other than assuming it is a literal
      printf("\n  - LITERAL:"FMT_TOKEN_VALUE_FORMATER, FMT_TOKEN_VALUE(body->data.literal.token));
      body_item = list_next(body_item);
    }

    printf("\n");
    item = list_next(item);
  }
}

void dump_tokens(char* grammar_path) {
  arena_t arena = arena_new(1024 * 1024 * 4);

  ref_source_t source = {
    .path = grammar_path,
    .contents = load_file(&arena, grammar_path),
  };

  lexer_t lexer;

  symbol_table_t symbol_table;
  symbol_table_init(&symbol_table, &arena);

  lexer_init(&lexer, &symbol_table, source);

  token_t token = {0};

  while (token.kind != TOKEN_EOF) {
    lexer_next_token(&lexer, &token);

    printf(
        "%s:%ld:%ld:%s:" FMT_TOKEN_VALUE_FORMATER "\n",
        token.ref.source.path,
        token.ref.cursor.row + 1,
        token.ref.cursor.col + 1,
        token_kind_to_str(token.kind),
        FMT_TOKEN_VALUE(token));
  }
}

void check(char* grammar_path, char* source_path) {
  arena_t arena = arena_new(1024 * 1024 * 4);

  ref_source_t grammar_source = {
    .path = grammar_path,
    .contents = load_file(&arena, grammar_path),
  };

  lexer_t lexer;

  symbol_table_t symbol_table;
  symbol_table_init(&symbol_table, &arena);

  lexer_init(&lexer, &symbol_table, grammar_source);

  parser_t parser;

  parser_init(&parser, &symbol_table, &arena);

  ast_t* ast = parser_parse(&parser, &lexer);

  ref_source_t source = {
    .path = grammar_path,
    .contents = load_file(&arena, source_path),
  };

  emitter_t emitter;

  emitter_init(&emitter, ast, source, &arena);

  emitter_production_t* production;

  while((production = emitter_emit(&emitter))) {
    if (production->has_error) {
      puts("invalid syntax");
      exit(1);
    }

    printf("%s:%s\n", production->stack->production_head, production->value);
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
    "  dump-ast    Dump AST from an OAST grammar file\n"
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
