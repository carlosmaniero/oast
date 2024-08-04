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
#include "parser.h"
#include <assert.h>

static void parse_set_literal_value(parser_t* parser, ast_production_body_literal_t* literal);

void parser_init(parser_t* parser, symbol_table_t* symbol_table, arena_t* arena) {
  parser->arena = arena;
  parser->symbol_table = symbol_table;
}

ast_t* parser_parse(parser_t* parser, lexer_t* lexer) {
  ast_t* root = arena_alloc(parser->arena, sizeof(ast_t));

  root->productions = (list_t){0};
  root->productions.arena = parser->arena;

  ast_production_t* production = arena_alloc(parser->arena, sizeof(ast_production_t));

  lexer_next_token(lexer, &production->head.token);
  production->body.kind = AST_PRODUCTION_BODY_KIND_LITERAL;

  // TODO: ensure there is an = token in between head and body
  // At this point we are only consuming it
  lexer_next_token(lexer, &production->body.data.literal.token);

  lexer_next_token(lexer, &production->body.data.literal.token);
  parse_set_literal_value(parser, &production->body.data.literal);

  symbol_table_add_production(parser->symbol_table, production);

  list_append(&root->productions, production);

  return root;
}

static void parse_set_literal_value(parser_t* parser, ast_production_body_literal_t* literal) {
  char value[token_value_size(&literal->token)];
  char* pvalue = value;

  token_get_value(&literal->token, value);

  char* actual_value = literal->value = arena_alloc(parser->arena, token_value_size(&literal->token));

  assert(*pvalue == '"' && "parse_set_literal_value: invalid token literal");

  pvalue++;

  while(*pvalue != '"') {
    // TODO: allow escaped strings
    *actual_value = *pvalue;
    actual_value++;

     pvalue++;
  }
}
