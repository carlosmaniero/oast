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
#include "walker.h"

#ifndef TOKEN_H
#define TOKEN_H

#define FMT_TOKEN_VALUE_FORMATER "%.*s"
#define FMT_TOKEN_VALUE(sv) (int)(sv).length, (sv).ref.source.contents + (sv).ref.cursor.offset

typedef enum token_kind
{
    TOKEN_UNKNOWN,

    TOKEN_IDENTIFIER,
    TOKEN_EQUAL,
    TOKEN_STRING,

    TOKEN_EOF
} token_kind_t;

typedef struct token
{
    enum token_kind kind;
    size_t length;
    ref_t ref;
} token_t;

char* token_kind_to_str(token_kind_t kind);

int token_value_size(token_t* token);
void token_get_value(token_t* token, char* dest);

#endif /* LEXER_H */
