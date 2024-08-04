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
#include "walker.h"

void walker_next_cursor(walker_t* walker);
void walker_cursor_to_ref(walker_t* walker, ref_t* ref);

void walker_init(walker_t* walker, ref_source_t source) {
  walker->ref.source = source;

  walker->ref.cursor.offset = 0;
  walker->ref.cursor.row = 0;
  walker->ref.cursor.col = 0;
}

void walker_walk(walker_t* walker, ref_t* ref) {
  walker_cursor_to_ref(walker, ref);
  walker_next_cursor(walker);
}

void walker_walk_while(walker_t* walker, walker_predicate_t predicate) {
  ref_t ref;

  while(1) {
    walker_cursor_to_ref(walker, &ref);

    if (!predicate(&ref) || ref_char(&ref) == 0) {
      break;
    }

    walker_next_cursor(walker);
  }
}

void walker_ctx_walk_while(walker_t* walker, walker_ctx_predicate_t predicate, void* context) {
  ref_t ref;

  while(1) {
    walker_cursor_to_ref(walker, &ref);

    if (!predicate(&ref, context) || ref_char(&ref) == 0) {
      break;
    }

    walker_next_cursor(walker);
  }
}

void walker_next_cursor(walker_t* walker) {
  ref_t ref;
  walker_cursor_to_ref(walker, &ref);

  if (ref_char(&ref) == '\n') {
    walker->ref.cursor.row++;
    walker->ref.cursor.col = 0;
  } else {
    walker->ref.cursor.col++;
  }

  walker->ref.cursor.offset++;
}

void walker_next_ncursor(walker_t* walker, size_t n) {
  while(n-- > 0) {
    walker_next_cursor(walker);
  }
}

void walker_cursor_to_ref(walker_t* walker, ref_t* ref) {
  *ref = walker->ref;
}

int walker_done(walker_t* walker) {
  return ref_char(&walker->ref) == 0;
}
