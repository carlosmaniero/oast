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

#ifndef REF_H
#define REF_H

struct ref_source {
  char* path;
  char* contents;
};

struct ref_cursor
{
    size_t offset;
    size_t row;
    size_t col;
};

struct ref {
  struct ref_source source;
  struct ref_cursor cursor;
};

char ref_char(struct ref* ref);

int ref_distance(struct ref* ref, struct ref* other);

#endif
