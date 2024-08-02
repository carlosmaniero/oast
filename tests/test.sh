#!/bin/sh
OAST_PATH="../oast"
TEST_FILE="$1"

TEST_TMP_FILES="$TEST_FILE.test"

AST_OUTPUT_FILE="$TEST_FILE.test.ast_output"
AST_EXPECT_OUTPUT_FILE="$TEST_FILE.test.expected_ast_output"

TOKENS_OUTPUT_FILE="$TEST_FILE.test.tokens_output"
TOKENS_EXPECT_OUTPUT_FILE="$TEST_FILE.test.expected_tokens_output"

# Misc
extract_comment() {
  tag="$1"
  comment="% $tag %"

  grep "$comment" "$TEST_FILE" | sed -e "s/$comment//"
}

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

print_test_description() {
  colored "$TEST_FILE: " $COLOR_CYAN
  printf "\n"
  colored "$(extract_comment spec)" $COLOR_GRAY
  printf "\n\n"
}

print_passed() {
  context="$1"
  printf "%s: " "$context"
  colored "passed" $COLOR_GREEN
  echo
}

print_failed() {
  context="$1"
  printf "%s: " "$context"
  colored "failed" $COLOR_RED
  echo
}

print_skiped() {
  context="$1"
  printf "%s: " "$context"
  colored "not set" $COLOR_GRAY
  echo
}

expect_output() {
  context="$1"
  actual_file="$2"
  expected_file="$3"

  if [ "$(wc -l < "$expected_file")" = "0" ]; then
    print_skiped "$context"
    return
  fi

  if [ "$(cat "$expected_file")" = "%empty%" ]; then
    if [ "$(wc -c < "$actual_file")" = "0" ]; then
      print_passed "$context"
      return
    fi
  fi

  while read -r expected_line ; do
    if [ "$(grep "$expected_line" "$actual_file" | wc -c)" = "0" ]; then
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

  if [ "$(cat "$expected_file")" = "%empty%" ]; then
    if [ "$(wc -c < "$actual_file")" = "0" ]; then
       return
    fi
  fi

  colored "$context not match:" $COLOR_YELLOW
  echo ""
  diff "$actual_file" "$expected_file" -u --color
  exit 1
}

test_tokens() {
  extract_comment "expect-token" > "$TOKENS_EXPECT_OUTPUT_FILE"

  $OAST_PATH dump-tokens "$TEST_FILE" > "$TOKENS_OUTPUT_FILE" 2>&1

  expect_output "expect-token" "$TOKENS_OUTPUT_FILE" "$TOKENS_EXPECT_OUTPUT_FILE"

  print_passed expect-token
}

test_ast() {
  extract_comment "ast" > "$AST_EXPECT_OUTPUT_FILE"

  $OAST_PATH dump-ast "$TEST_FILE" > "$AST_OUTPUT_FILE" 2>&1

  diff_output "ast" "$AST_OUTPUT_FILE" "$AST_EXPECT_OUTPUT_FILE"

  print_passed ast
}


main() {
  print_test_description

  test_tokens
  test_ast

  cleanup
}

main
