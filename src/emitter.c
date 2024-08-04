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
#include "emitter.h"
#include <assert.h>
#include <stdio.h>

void emitter_init(emitter_t* emitter, ast_t* ast, ref_source_t source, arena_t* arena) {
  emitter->arena = arena;
  emitter->ast = ast;

  walker_init(&emitter->walker, source);
}

emitter_production_t* emitter_emit(emitter_t* emitter) {
  if (walker_done(&emitter->walker)) {
    return NULL;
  }
  list_item_t* item = list_head(&emitter->ast->productions);

  assert(item && "emitter_emit: empty ast");

  ast_production_t* ast_production = item->value;

  assert(item && "emitter_emit: empty ast production");

  emitter_production_t* result = arena_alloc(emitter->arena, sizeof(emitter_production_t));

  switch(ast_production->body.kind) {
    case AST_PRODUCTION_BODY_KIND_LITERAL: {
      if(strncmp(
          ast_production->body.data.literal.value,
          ref_source_contents_at_cursor(&emitter->walker.ref),
          strlen(ast_production->body.data.literal.value)) == 0) {
        result->value = arena_alloc(emitter->arena, ast_production->body.data.literal.token.length);
        ref_source_to_str(&emitter->walker.ref, result->value, ast_production->body.data.literal.token.length);

        walker_next_ncursor(&emitter->walker, ast_production->body.data.literal.token.length);
      } else {
        result->has_error = 1;
      }
      break;
    }
    default:
      assert(0 && "emitter_emit: unknown production kind");
  }
  
  return result;
}
