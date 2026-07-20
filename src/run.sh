#!/bin/bash

CLEAN=0
PROGRAM_ARGS=()

# 1. Parse Arguments (Extracts --clean/-c and groups the rest)
for arg in "$@"; do
    if [[ "$arg" == "--clean" || "$arg" == "-c" ]]; then
        CLEAN=1
    else
        PROGRAM_ARGS+=("$arg")
    fi
done

# Check if a source file was provided
if [[ ${#PROGRAM_ARGS[@]} -eq 0 ]]; then
    echo "Usage: $0 <Source.cpp> [--clean] [ProgramArgs...]"
    exit 1
fi

# The first non-flag argument is the Source
SOURCE="${PROGRAM_ARGS[0]}"

# Remove the Source from the array to leave only ProgramArgs
PROGRAM_ARGS=("${PROGRAM_ARGS[@]:1}")

# 2. Setup Variables
# Strip the extension from the source file for the executable name (e.g., 401-500/451.cpp -> 401-500/451)
EXE="${SOURCE%.*}"

# Compile Flags
VERSION_FLAG="-std=c++17"
WARNING_FLAGS=("-Wall" "-Wextra" "-Wshadow")
OPTIMIZATION_FLAGS=("-O3" "-march=native" "-flto")
LINKER_FLAGS=("-lgmpxx" "-lgmp" "-fopenmp")
ARCHITECTURE_FLAGS=("-m64" "-mcmodel=large")

# 3. Compile Step
# \e[36m is Cyan
echo -e "\e[36mCompiling $SOURCE...\e[0m"

g++ "$VERSION_FLAG" "${WARNING_FLAGS[@]}" "${OPTIMIZATION_FLAGS[@]}" "${ARCHITECTURE_FLAGS[@]}" "$SOURCE" -o "$EXE" "${LINKER_FLAGS[@]}"
COMPILE_STATUS=$?

if [[ $COMPILE_STATUS -ne 0 ]]; then
    exit $COMPILE_STATUS
fi

# Ensure executable is called correctly whether it's an absolute path or relative
if [[ "$EXE" == /* ]]; then
    EXE_PATH="$EXE"
else
    EXE_PATH="./$EXE"
fi

# 4. Execution & Timing
# Grab start time in seconds + nanoseconds
START_TIME=$(date +%s.%N)

# Execute the program, passing any remaining arguments cleanly
"$EXE_PATH" "${PROGRAM_ARGS[@]}"
EXIT_CODE=$?

# Grab end time
END_TIME=$(date +%s.%N)

# Cleanup helper function
cleanup() {
    if [[ $CLEAN -eq 1 && -f "$EXE_PATH" ]]; then
        rm -f "$EXE_PATH"
        # \e[90m is Dark Gray
        echo -e "\e[90mCleaned: $EXE_PATH\e[0m"
    fi
}

# If the C++ program failed/segfaulted, exit early
if [[ $EXIT_CODE -ne 0 ]]; then
    cleanup
    exit $EXIT_CODE
fi

# Bash only supports integer math natively, so we use awk for floating-point arithmetic
ELAPSED=$(awk -v start="$START_TIME" -v end="$END_TIME" 'BEGIN { printf "%.6f", end - start }')

# \e[32m is Green
echo -e "\e[32mExecution time: $ELAPSED seconds\e[0m"

# 5. Final Cleanup
cleanup