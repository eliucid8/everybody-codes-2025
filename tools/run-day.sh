#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./tools/run_day.sh <day> [--test] [--preset PRESET] [--build-dir BUILD_DIR] [--no-configure] [--] [program args...]
#
# Examples:
#   ./tools/run_day.sh 1                    # build/debug/preset "debug", run days/day01/day01 with input.txt
#   ./tools/run_day.sh 3 --test             # run with test_input.txt
#   ./tools/run_day.sh 10 --preset debug -- --flag foo
#   ./tools/run_day.sh 7 --build-dir build-release -- arg1 arg2
#   ./tools/run_day.sh 4 --no-configure     # try building without reconfiguring (may fail if target unknown)

# Defaults - change if you prefer other names
DEFAULT_PRESET="debug"
DEFAULT_BUILD_DIR="build-debug"
DAYS_SOURCE_DIR="days"

# Parse positional + options
if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <day-number> [--test] [--preset PRESET] [--build-dir BUILD_DIR] [--no-configure] [--] [program args...]"
  exit 2
fi

day_raw="$1"
shift

use_test_input=false
preset="${DEFAULT_PRESET}"
build_dir="${DEFAULT_BUILD_DIR}"
no_configure=false
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
    --no-configure)
      no_configure=true
      shift
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

# ---------- helper: configure build dir or preset ----------
ensure_configured() {
  # Arguments:
  #   $1 = preset (may be empty)
  #   $2 = build_dir
  local preset="$1"
  local build_dir="$2"

  if [[ "${no_configure}" == "true" ]]; then
    echo "Auto-configure disabled (--no-configure); skipping configure step."
    return 0
  fi

  if [[ -n "${preset}" ]] && cmake --help 2>/dev/null | grep -q -- --preset; then
    echo "Configuring via preset: ${preset}"
    cmake --preset "${preset}"
  else
    echo "Configuring via cmake -S . -B ${build_dir} -DCMAKE_BUILD_TYPE=Debug"
    cmake -S . -B "${build_dir}" -DCMAKE_BUILD_TYPE=Debug
  fi

  # Optional: keep a convenient compile_commands.json in repo root for clangd/Neovim
  if [[ -f "${build_dir}/compile_commands.json" ]]; then
    ln -sf "${build_dir}/compile_commands.json" compile_commands.json
  fi
}

# ---------- build attempt with auto-configure ----------
build_target_with_possible_reconfigure() {
  local preset="$1"
  local build_dir="$2"
  local target="$3"

  # Prefer preset-aware build if supported
  if cmake --build --help 2>/dev/null | grep -q -- --preset; then
    if cmake --build --preset "${preset}" --target "${target}"; then
      return 0
    else
      # If auto-configure is disabled, don't attempt to reconfigure
      if [[ "${no_configure}" == "true" ]]; then
        echo "Preset-aware build failed and auto-configure disabled. Aborting."
        return 1
      fi
      echo "Preset-aware build failed (maybe stale configuration). Will reconfigure and retry."
      ensure_configured "${preset}" "${build_dir}"
      if cmake --build --preset "${preset}" --target "${target}"; then
        return 0
      else
        echo "Build still failed after reconfigure."
        return 1
      fi
    fi
  fi

  # Fallback flow for cmake versions without --build --preset
  if cmake --build "${build_dir}" --target "${target}"; then
    return 0
  else
    if [[ "${no_configure}" == "true" ]]; then
      echo "Build failed (target probably unknown) and auto-configure disabled. Aborting."
      return 1
    fi
    echo "Build failed (target probably unknown). Reconfiguring ${build_dir} and retrying..."
    ensure_configured "" "${build_dir}"
    if cmake --build "${build_dir}" --target "${target}"; then
      return 0
    else
      echo "Build failed after reconfigure."
      return 1
    fi
  fi
}

# Build: prefer cmake --build --preset <preset> if supported, otherwise fallback to cmake --build <build_dir>
echo "Building target '${target_name}' (preset='${preset}', build-dir='${build_dir}')..."

if ! build_target_with_possible_reconfigure "${preset}" "${build_dir}" "${target_name}"; then
  echo "ERROR: Unable to build target ${target_name}."
  exit 5
fi

# After build, check executable exists (adjust for generator specifics if necessary)
if [[ ! -x "${exe_path}" ]]; then
  # try to locate target if ninja or different layout created it elsewhere
  alt_path="$(find "${build_dir}" -type f -name "${target_name}" -perm /u=x,g=x,o=x | head -n1 || true)"
  if [[ -n "${alt_path}" ]]; then
    echo "Found executable at ${alt_path}; using that."
    exe_path="${alt_path}"
  else
    echo "Executable not found at expected path: ${exe_path}"
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
fi

echo "Running ${exe_path} with input ${input_file} ..."
# If input file exists, run with it as stdin; handle piping correctly:
if [[ -f "${input_file}" ]]; then
  # Use exec so signals propagate and exit code visible
  exec "${exe_path}" "${prog_args[@]:-}" < "${input_file}"
else
  exec "${exe_path}" "${prog_args[@]:-}"
fi

