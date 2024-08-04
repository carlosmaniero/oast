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
#include "token.h"
#include "assert.h"

char* token_kind_to_str(enum token_kind kind) {
  switch (kind) {
    case TOKEN_UNKNOWN: 
      return "TOKEN_UNKNOWN";
    case TOKEN_IDENTIFIER:
      return "TOKEN_IDENTIFIER";
    case TOKEN_EQUAL:
      return "TOKEN_EQUAL";
    case TOKEN_STRING:
      return "TOKEN_STRING";
    case TOKEN_EOF:
      return "TOKEN_EOF";   
  }
  assert(0 && "lexer_token_kind_to_str: undefined token kind str");
}

int token_value_size(token_t* token) {
  return token->length + 1;
}

void token_get_value(token_t* token, char* dest) {
  dest[token->length] = 0;
  ref_source_to_str(&token->ref, dest, token->length);
}
