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

#ifndef LEXER_H
#define LEXER_H

#define FMT_TOKEN_VALUE_FORMATER "%.*s"
#define FMT_TOKEN_VALUE(sv) (int)(sv).length, (sv).ref.source.contents + (sv).ref.cursor.offset

struct lexer
{
  struct walker walker;
};

enum token_kind
{
    TOKEN_UNKNOWN,

    TOKEN_EOF
};

struct token
{
    enum token_kind kind;
    size_t length;
    struct ref ref;
};

void lexer_init(struct lexer* lexer, struct ref_source source);

void lexer_next_token(struct lexer* lexer, struct token* token);

char* lexer_token_kind_to_str(enum token_kind kind);

#endif /* LEXER_H */
