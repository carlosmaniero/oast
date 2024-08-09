#!/bin/sh
# Copyright (C) 2024 Carlos Maniero <carlos@maniero.me>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
OAST_PATH="../oast"
TEST_FILE="$1"

TEST_TMP_FILES="$TEST_FILE.test"

cleanup() {
  rm -f "$TEST_TMP_FILES"*
}

# UI
COLOR_RED=1
COLOR_GREEN=2
COLOR_YELLOW=3
COLOR_CYAN=6
COLOR_GRAY=7

colored() {
    text="$1"

    if [ -t 1 ]; then
      if tput setaf 1 > /dev/null 2>&1; then
        color=$(tput setaf "$2")
        reset=$(tput sgr0)

        printf "%s%s%s" "$color" "$text" "$reset"
        return
      fi
    fi

    printf "%s" "$text"
}

# test output
test_header() {
  colored "$1" "$COLOR_GRAY"
  printf "\n"
}

print_test_path() {
  colored "$TEST_FILE: " $COLOR_CYAN
  printf "\n"
}

print_passed() {
  colored "passed" $COLOR_GREEN
  echo
}

print_failed() {
  context="$1"
  printf "%s: " "$context"
  colored "failed" $COLOR_RED
  echo
}

# expectations
expect_output_contains() {
  context="$1"
  actual_file="$2"
  expected_file="$3"

  if [ "$(wc -l < "$expected_file")" = "0" ]; then
    print_skiped "$context"
    return
  fi

  while read -r expected_line ; do
    if [ "$(grep "$(printf "%s" "$expected_line" | sed 's/\\/\\\\/g')" "$actual_file" | wc -c)" = "0" ]; then
      print_failed "$context"
      colored "(not found) $expected_line" $COLOR_YELLOW
      echo ""
      colored "$(awk '{print "(actual) " $0}' "$actual_file")" $COLOR_GRAY
      echo ""
      exit 1
    fi
  done < "$expected_file"
}

diff_output() {
  context="$1"
  actual_file="$2"
  expected_file="$3"

  if [ "$(wc -l < "$expected_file")" = "0" ]; then
    print_skiped "$context"
    return
  fi

  if cmp -s "$expected_file" "$actual_file"; then
    return
  fi

  colored "$context not match:" $COLOR_YELLOW
  echo ""
  diff "$actual_file" "$expected_file" -u --color
  exit 1
}

# test function
spec() {
  test_header spec
  cat "$1"
}

describe() {
  echo
  test_header describe
  cat "$1"
}

ast_equals() {
  actual_path="$1.actual"
  $OAST_PATH dump-ast "$TEST_FILE" > "$actual_path" 2>&1

  diff_output "$TEST_FILE:$2:1:ast_equals" "$actual_path" "$1"

  print_passed
}

expect_contains_tokens() {
  actual_path="$1.actual"
  $OAST_PATH dump-tokens "$TEST_FILE" > "$actual_path" 2>&1

  expect_output_contains "$TEST_FILE:$2:1:expect_contains_tokens" "$actual_path" "$1"

  print_passed
}

check_equals() {
  actual_path="$1.actual"
  $OAST_PATH check "$TEST_FILE" "$3" > "$actual_path" 2>&1

  diff_output "$TEST_FILE:$2:1:check_equals" "$actual_path" "$1"

  print_passed
}

main() {
  print_test_path

  all_test_end="$(grep -n "END" "$TEST_FILE" | awk -F: '{print $1}')"

  grep -n "%TEST:" "$TEST_FILE" | while read -r line ; do
    start_line="$(echo "$line" | awk -F: '{ print $1 }')"
    test_name="$(echo "$line" | awk -F: '{ print $3 }')"
    test_args="$(echo "$line" | awk -F: '{ print $4 }')"
    end_line="$start_line"

    for test_end in $all_test_end; do
      if [ "$test_end" -gt "$start_line" ]; then
        end_line="$test_end"
        break
      fi
    done

    test_contents_path="$TEST_TMP_FILES.$start_line.$end_line.partial"

    awk -v start="$start_line" -v end="$end_line" 'NR > start && NR < end' "$TEST_FILE" | sed 's/^\%//' > "$test_contents_path"

    if type "$test_name" | grep -q 'function'; then
        "$test_name" "$test_contents_path" "$start_line" "$test_args"
    else
        echo
        colored "Test function" $COLOR_YELLOW
        colored " \"$test_name\" " $COLOR_RED
        colored "is not defined" $COLOR_YELLOW
        echo
        exit 1
    fi
  done || exit 1;

  cleanup
}

main
