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

char ref_char(ref_t* ref) {
  return *(ref->source.contents + ref->cursor.offset);
}

char* ref_source_contents_at_cursor(ref_t* ref) {
  return ref->source.contents + ref->cursor.offset;
}

void ref_source_to_str(ref_t* ref, char* dest, size_t length) {
  memcpy(dest, ref->source.contents + ref->cursor.offset, length);
}

int ref_distance(ref_t* ref, ref_t* other) {
  return ref->cursor.offset - other->cursor.offset;
}
