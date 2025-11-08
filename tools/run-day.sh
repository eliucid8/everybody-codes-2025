#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./tools/run_day.sh <day> [--test] [--preset debug] [--build-dir build-debug] [--] [program args...]
#
# Examples:
#   ./tools/run_day.sh 1                    # build/debug/preset "debug", run days/day01/day01 with input.txt
#   ./tools/run_day.sh 3 --test             # run with test_input.txt
#   ./tools/run_day.sh 10 --preset debug -- --flag foo
#   ./tools/run_day.sh 7 --build-dir build-release -- arg1 arg2

# Defaults - change if you prefer other names
DEFAULT_PRESET="debug"
DEFAULT_BUILD_DIR="build-debug"
DAYS_SOURCE_DIR="days"

# Parse positional + options
if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <day-number> [--test] [--preset PRESET] [--build-dir BUILD_DIR] [--] [program args...]"
  exit 2
fi

day_raw="$1"
shift

use_test_input=false
preset="${DEFAULT_PRESET}"
build_dir="${DEFAULT_BUILD_DIR}"
# collect program args after `--`
prog_args=()

# parse flags (simple loop)
while [[ $# -gt 0 ]]; do
  case "$1" in
    --test)
      use_test_input=true
      shift
      ;;
    --preset)
      if [[ $# -lt 2 ]]; then echo "Missing value for --preset"; exit 2; fi
      preset="$2"; shift 2
      ;;
    --build-dir)
      if [[ $# -lt 2 ]]; then echo "Missing value for --build-dir"; exit 2; fi
      build_dir="$2"; shift 2
      ;;
    --) shift; prog_args=("$@"); break
      ;;
    -*)
      echo "Unknown option: $1"; exit 2
      ;;
    *)
      # treat as extra positional (unlikely); collect then continue
      prog_args+=("$1"); shift
      ;;
  esac
done

# validate numeric day
if ! [[ "$day_raw" =~ ^[0-9]+$ ]]; then
  echo "Error: day must be a positive integer (got '$day_raw')"
  exit 3
fi

# produce zero-padded day (02 style)
day_num="$day_raw"
day_pad=$(printf "%02d" "$day_num")

target_name="day${day_pad}"
target_dir="${DAYS_SOURCE_DIR}/${target_name}"
exe_path="${build_dir}/${DAYS_SOURCE_DIR}/${target_name}/${target_name}"

# sanity checks
if [[ ! -d "${target_dir}" ]]; then
  echo "Error: day source directory not found: ${target_dir}"
  exit 4
fi

# Build: prefer cmake --build --preset <preset> if supported, otherwise fallback to cmake --build <build_dir>
echo "Building target '${target_name}' (preset='${preset}', build-dir='${build_dir}')..."

build_ok=false
if cmake --help 2>/dev/null | grep -q -- --preset; then
  # cmake supports --build --preset
  if cmake --build --preset "${preset}" --target "${target_name}"; then
    build_ok=true
  else
    echo "Warning: build with preset '${preset}' failed; attempting fallback build-dir '${build_dir}'..."
  fi
else
  echo "Info: this cmake does not support --build --preset; falling back to build-dir approach."
fi

if ! $build_ok ; then
  # fallback: ensure build dir exists (configure if needed)
  if [[ ! -d "${build_dir}" || ! -f "${build_dir}/CMakeCache.txt" ]]; then
    echo "Build directory '${build_dir}' not configured. Configuring it now as Debug build..."
    cmake -S . -B "${build_dir}" -DCMAKE_BUILD_TYPE=Debug
  fi

  cmake --build "${build_dir}" --target "${target_name}"
fi

# After build, check executable exists (adjust for generator specifics if necessary)
if [[ ! -x "${exe_path}" ]]; then
  echo "Executable not found at expected path: ${exe_path}"
  # try to locate target if ninja or different layout created it elsewhere
  alt_path="$(find "${build_dir}" -type f -name "${target_name}" -perm /u=x,g=x,o=x | head -n1 || true)"
  if [[ -n "${alt_path}" ]]; then
    echo "Found executable at ${alt_path}; using that."
    exe_path="${alt_path}"
  else
    echo "Could not find executable for ${target_name}; aborting."
    exit 6
  fi
fi

# choose input file
if $use_test_input; then
  input_file="${target_dir}/test_input.txt"
else
  input_file="${target_dir}/input.txt"
fi

if [[ ! -f "${input_file}" ]]; then
  echo "Warning: input file does not exist: ${input_file} (running without redirected input)."
  run_cmd=( "${exe_path}" "${prog_args[@]}" )
else
  run_cmd=( "${exe_path} ${prog_args[@]} < ${input_file}" )
fi

echo "Running ${exe_path} with input ${input_file} ..."
# If input file exists, run with it as stdin; handle piping correctly:
if [[ -f "${input_file}" ]]; then
  # Use exec so signals propagate and exit code visible
  exec "${exe_path}" "${prog_args[@]}" < "${input_file}"
else
  exec "${exe_path}" "${prog_args[@]}"
fi
