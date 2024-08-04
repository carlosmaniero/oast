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
#include <aio.h>
#include <string.h>
#include "ref.h"

#ifndef WALKER_H
#define WALKER_H

typedef struct walker {
  ref_t ref;
} walker_t;

void walker_init(walker_t* walker, ref_source_t source);

void walker_walk(walker_t* walker, ref_t* ref);

typedef int (*walker_predicate_t)(ref_t* ref);
typedef int (*walker_ctx_predicate_t)(ref_t* ref, void* ctx);

void walker_walk_while(walker_t* walker, walker_predicate_t predicate);

void walker_ctx_walk_while(walker_t* walker, walker_ctx_predicate_t predicate, void* ctx);

void walker_next_cursor(walker_t* walker);

void walker_next_ncursor(walker_t* walker, size_t n);

int walker_done(walker_t* walker);

#endif
