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
#include "symbol_table.h"
#include "assert.h"
#include "stdio.h"
void symbol_table_init(symbol_table_t* symbol_table, arena_t* arena) {
  symbol_table->arena = arena;
  symbol_table->map = map_new(arena);
}

void symbol_table_register_identifier(symbol_table_t* symbol_table, token_t* token) {
  assert(symbol_table && "symbol_table_add_production: symbol_table not provided");
  assert(token && "symbol_table_add_production: token not provided");

  char local_identifier[token_value_size(token)];
  token_get_value(token, local_identifier);

  if (map_get(symbol_table->map, local_identifier)) {
    return;
  }

  symbol_table_item_t* item =
    arena_alloc(symbol_table->arena, sizeof(symbol_table_item_t));

  item->productions = NULL;
  item->identifier = arena_alloc(symbol_table->arena, token_value_size(token));

  token_get_value(token, item->identifier);

  map_put(symbol_table->map, item->identifier, item);
}

void symbol_table_add_production(symbol_table_t* symbol_table, ast_production_t* production) {
  assert(symbol_table && "symbol_table_add_production: symbol_table not provided");
  assert(production && "symbol_table_add_production: production not provided");

  char identifier[token_value_size(&production->head.token)];
  token_get_value(&production->head.token, identifier);

  symbol_table_item_t* item = map_get(symbol_table->map, identifier);

  assert(item && "symbol_table_add_production: symbol was not previously registered");

  if (!item->productions) {
    item->productions = arena_alloc(symbol_table->arena, sizeof(list_t));
    *item->productions = (list_t){0};
    item->productions->arena = symbol_table->arena;
  }

  list_append(item->productions, production);
}
