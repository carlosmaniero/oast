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
#include "arena.h"
#include "lexer.h"
#include "ast.h"
#ifndef PARSER_H
#define PARSER_H
struct parser {
  struct arena* arena;
};

void parser_init(struct parser* parser, struct arena* arena);

struct ast* parser_parse(struct parser* parser, struct lexer* lexer);

#endif
