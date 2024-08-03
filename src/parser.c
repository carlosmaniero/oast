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

void parser_init(parser_t* parser, arena_t* arena) {
  parser->arena = arena;
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

  list_append(&root->productions, production);

  return root;
}
