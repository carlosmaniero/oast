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
#include "lexer.h"
#include "assert.h"
#include "ctype.h"

int lexer_not_space_predicate(struct ref* ref);
int lexer_not_lf(struct ref* ref);
int lexer_isspace_predicate(struct ref* ref);
int lexer_isalnum(struct ref* ref);

void lexer_set_token_length(struct lexer* lexer, struct token* token);

void lexer_init(struct lexer* lexer, struct ref_source source) {
  walker_init(&lexer->walker, source);
}

int lexer_not_space_predicate(struct ref* ref) {
  return !isspace(ref_char(ref));
}

int lexer_isspace_predicate(struct ref* ref) {
  return isspace(ref_char(ref));
}

int lexer_not_lf(struct ref* ref) {
  return ref_char(ref) != '\n';
}

int lexer_isalnum(struct ref* ref) {
  return isalnum(ref_char(ref));
}

void lexer_set_token_length(struct lexer* lexer, struct token* token) {
  token->length = lexer->walker.cursor.offset - token->ref.cursor.offset;
}

void lexer_next_token(struct lexer* lexer, struct token* token) {
  token->kind = TOKEN_UNKNOWN;

  walker_walk_while(&lexer->walker, &lexer_isspace_predicate);
  walker_walk(&lexer->walker, &token->ref);

  char token_char = ref_char(&token->ref);

  if (token_char == 0) {
    token->kind = TOKEN_EOF;
  };

  if (token_char == '%') {
    walker_walk_while(&lexer->walker, &lexer_not_lf);
    lexer_next_token(lexer, token);
    return;
  }

  if (isalpha(token_char)) {
    walker_walk_while(&lexer->walker, &lexer_isalnum);

    token->kind = TOKEN_IDENTIFIER;
    lexer_set_token_length(lexer, token);
    return;
  }

  walker_walk_while(&lexer->walker, &lexer_not_space_predicate);
  lexer_set_token_length(lexer, token);
}

char* lexer_token_kind_to_str(enum token_kind kind) {
  switch (kind) {
    case TOKEN_UNKNOWN: 
      return "TOKEN_UNKNOWN";
    case TOKEN_IDENTIFIER:
      return "TOKEN_IDENTIFIER";
    case TOKEN_EOF:
      return "TOKEN_EOF";   
  }
  assert(0 && "lexer_token_kind_to_str: undefined token kind str");
}
