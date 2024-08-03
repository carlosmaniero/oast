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
#include "list.h"
#ifndef AST_H
#define AST_H
typedef struct ast {
  // List of ast_productions
  list_t productions;
} ast_t;

typedef struct ast_production_head {
  token_t token;
} ast_production_head_t;

typedef enum ast_production_body_kind {
  AST_PRODUCTION_BODY_KIND_LITERAL
} ast_production_body_kind_t;

typedef struct ast_production_body_literal {
  token_t token;
} ast_production_body_literal_t;

typedef union ast_production_body_data {
  ast_production_body_literal_t literal;
} ast_production_body_data_t;

typedef struct ast_production_body {
  ast_production_body_kind_t kind;
  ast_production_body_data_t data;
} ast_production_body_t;

typedef struct ast_production {
  ast_production_head_t head;
  ast_production_body_t body;
} ast_production_t;
#endif
