# Running sources code and compare the results with the expected answers

import platform
import subprocess
import pandas
import time

# List of programming languages used for this repo and their corresponding file extensions
LANGUAGE_USED = {
    "C": "c",
    "C++": "cpp",
    "Python": "py",
}

COMPILING_LANGUAGES = ["C", "C++"]
INTERPRETED_LANGUAGES = ["Python"]

# For source codes that needed to be compiled before running
# Each language will have its own script file
COMPILING_SCRIPT = {
    "C": "run_c",
    "C++": "run_cpp",
}

# For interpreted languages, we can directly run the source code with the corresponding interpreter
INTERPRETER_COMMAND = {
    "Python": "uv run"
}

TOTAL_PROBLEMS = 900

def run_code(problem_index: int) -> tuple[str, float] | None:
    subfolder_index = (problem_index - 1) // 100
    subfolder_name = f"{100 * subfolder_index + 1}-{100 * (subfolder_index + 1)}"
    
    source_code_file = None
    programming_language = None
    for language, extension in LANGUAGE_USED.items():
        file_name = f"{problem_index}.{extension}"
        file_path = f"{subfolder_name}/{file_name}"
        try:
            with open(file_path, "r") as f:
                source_code_file = file_path
                programming_language = language
                break
        except FileNotFoundError:
            continue

    if source_code_file is None or programming_language is None:
        # print(f"Source code file for problem {problem_index} not found.")
        return None 
    
    if programming_language in INTERPRETED_LANGUAGES:
        command = INTERPRETER_COMMAND[programming_language].split() + [source_code_file]
    elif programming_language in COMPILING_LANGUAGES:
        command = []

    if len(command) == 0:
        # print(f"Compiling and running for language {programming_language} is not implemented yet.")
        return None

    # Run the command and capture the output
    try:
        start_time = time.perf_counter()
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        elapsed_time = time.perf_counter() - start_time
        # Expect the result in a single line and as a single continuous string (without newline characters)
        result = result.stdout.strip()
        if "\n" in result:
            # print(f"Unexpected output format for problem {problem_index}: multiple lines found.")
            return None
        return result, elapsed_time
    
    except subprocess.CalledProcessError as e:
        print(f"Error running the code for problem {problem_index}: {e}")
        return None    

if __name__ == "__main__":
    problem_answer = []
    for problem_index in range(1, TOTAL_PROBLEMS + 1):
        result = run_code(problem_index)
        if result is not None:
            print(f"Problem {problem_index}: {result[0]} (Time: {result[1]:.6f} seconds)")
            problem_answer.append((problem_index, result[0], result[1]))
        else:
            problem_answer.append((problem_index, "N/A", 0.0))

    # Save the results to a CSV file
    df = pandas.DataFrame(problem_answer, columns=["ID", "Answer", "Time"])
    df.to_csv("problem_answers.csv", index=False)