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

void walker_next_cursor(struct walker* walker);
void walker_cursor_to_ref(struct walker* walker, struct ref* ref);

void walker_init(struct walker* walker, struct ref_source source) {
  walker->ref.source = source;

  walker->ref.cursor.offset = 0;
  walker->ref.cursor.row = 0;
  walker->ref.cursor.col = 0;
}

void walker_walk(struct walker* walker, struct ref* ref) {
  walker_cursor_to_ref(walker, ref);
  walker_next_cursor(walker);
}

void walker_walk_while(struct walker* walker, walker_predicate_t predicate) {
  struct ref ref;

  while(1) {
    walker_cursor_to_ref(walker, &ref);

    if (!predicate(&ref) || ref_char(&ref) == 0) {
      break;
    }

    walker_next_cursor(walker);
  }
}

void walker_next_cursor(struct walker* walker) {
  struct ref ref;
  walker_cursor_to_ref(walker, &ref);

  if (ref_char(&ref) == '\n') {
    walker->ref.cursor.row++;
    walker->ref.cursor.col = 0;
  } else {
    walker->ref.cursor.col++;
  }

  walker->ref.cursor.offset++;
}

void walker_cursor_to_ref(struct walker* walker, struct ref* ref) {
  *ref = walker->ref;
}
