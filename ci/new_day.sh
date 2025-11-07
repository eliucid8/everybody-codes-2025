#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <day-number>"
  exit 1
fi

num="$1"

if ! [[ "$num" =~ ^[0-9]+$ ]]; then
  echo "Error: numeric day required"
  exit 2
fi

if [[ "$num" -lt 10 ]]; then
  day_number="0${num}"
else
  day_number="${num}"
fi

new_directory="days/day${day_number}"

if [[ -d "$new_directory" ]]; then
  echo "Error: $new_directory already exists"
  exit 3
fi

mkdir -p "$new_directory"

# replace 'dayXX' with padded and a more specific placeholder 'dayNUM' if needed
sed "s/dayXX/day${day_number}/g" ci/main.cpp.template > "$new_directory/main.cpp"
# careful: this next replacement replaces literal 'day' occurrences; risky
sed "s/dayXX/day${day_number}/g" ci/cmake_template > "$new_directory/CMakeLists.txt"

: > "$new_directory/input.txt"
: > "$new_directory/test_input.txt"

echo "Created $new_directory"

