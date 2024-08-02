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
struct ast {
  // List of ast_productions
  list_t productions;
};

struct ast_production_head {
  struct token token;
};

enum ast_production_body_kind {
  AST_PRODUCTION_BODY_KIND_LITERAL
};

struct ast_production_body_literal {
  struct token token;
};

union ast_production_body_data {
  struct ast_production_body_literal literal;
};

struct ast_production_body {
  enum ast_production_body_kind kind;
  union ast_production_body_data data;
};

struct ast_production {
  struct ast_production_head head;
  struct ast_production_body body;
};
#endif
