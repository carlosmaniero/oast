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
#include "ref.h"
#include "walker.h"
#include "token.h"
#include "symbol_table.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct lexer
{
  symbol_table_t* symbol_table;
  walker_t walker;
} lexer_t;

void lexer_init(lexer_t* lexer, symbol_table_t* symbol_table, ref_source_t source);

void lexer_next_token(lexer_t* lexer, token_t* token);

#endif /* LEXER_H */
