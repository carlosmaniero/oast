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
#include <stdio.h>
#include <string.h>

void print_help(void);
void print_unknown();

int main(int args, char** argsv) {
  if (args < 2 || strcmp(argsv[1], "help") == 0) {
    print_help();
    return 0;
  }

  print_unknown();
  return 1;
}

void print_help() {
  const char* oast_help = 
    "oast - olang AST utility\n"
    "\n"
    "Usage:\n"
    "  oast <command> [options] <file>\n"
    "\n"
    "Commands:\n"
    "  help        Display this help message\n"
    "  dump-tokens Dump tokens from an OAST grammar file\n"
    "  check       Test an OAST grammar file against a source\n"
    "\n"
    "Examples:\n"
    "  oast help\n"
    "  oast dump-tokens grammar.oast\n"
    "  oast check grammar.oast source-file";
  puts(oast_help);
}

void print_unknown() {
  puts("error: invalid invocation!\n");

  print_help();
}
