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
#include "list.h"
#include "map.h"
#include "token.h"
#include "ast.h"
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol_table_item {
  char* identifier;
  list_t* productions;
} symbol_table_item_t;

typedef struct symbol_table {
  map_t* map;
  arena_t* arena;
} symbol_table_t;

void symbol_table_init(symbol_table_t* symble_table, arena_t* arena);
void symbol_table_register_identifier(symbol_table_t* symble_table, token_t* token);
void symbol_table_add_production(symbol_table_t* symble_table, ast_production_t* production);

#endif
