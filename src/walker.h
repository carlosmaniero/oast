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

struct walker {
  struct ref ref;
};

void walker_init(struct walker* walker, struct ref_source source);

void walker_walk(struct walker* walker, struct ref* ref);

typedef int (*walker_predicate_t)(struct ref* ref);

void walker_walk_while(struct walker* walker, walker_predicate_t predicate);

void walker_next_cursor(struct walker* walker);

#endif
