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
#ifndef EMITTER_H
#define EMITTER_H

#include "ref.h"
#include "ast.h"
#include "arena.h"

typedef struct emitter {
  walker_t walker;
  ast_t* ast;
  arena_t* arena;
} emitter_t;

typedef struct emitter_stack {
  char* production_head;
  struct emitter_stack* previous; 
} emitter_stack_t;

typedef struct emitter_production {
  ref_t ref;
  char* value;
  emitter_stack_t* stack;

  // TODO: think a better approach for error handling
  int has_error;
} emitter_production_t;

void emitter_init(emitter_t* emitter, ast_t* ast, ref_source_t source, arena_t* arena);

emitter_production_t* emitter_emit(emitter_t* emitter); 

#endif
