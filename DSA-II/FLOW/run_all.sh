#!/bin/bash

# ================= COLORS =================
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# ================= HELPER FUNCTION =================
run_tests() {
    PROB_NAME="$1"
    SOURCE="$2"
    EXE="$3"
    IN_DIR="$4"
    OUT_DIR="$5"
    CHECKER="$6" # Optional python checker

    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}       TESTING: $PROB_NAME             ${NC}"
    echo -e "${BLUE}========================================${NC}"

    # 1. Compile
    if [ ! -f "$SOURCE" ]; then
        echo -e "${RED}❌ Source file $SOURCE not found!${NC}"
        return
    fi

    echo "Compiling $SOURCE..."
    g++ -std=c++17 -o "$EXE" "$SOURCE"

    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Compilation Failed!${NC}"
        return
    fi
    echo -e "${GREEN}✅ Compilation Successful!${NC}"
    echo "----------------------------------------"

    # 2. Run Test Cases
    # Check if directory exists
    if [ ! -d "$IN_DIR" ]; then
         echo -e "${YELLOW}⚠️  Input directory $IN_DIR does not exist. Skipping.${NC}"
         return
    fi

    for input_file in "$IN_DIR"/*.txt; do
        # Handle case where no files exist
        [ -e "$input_file" ] || continue

        filename=$(basename "$input_file")
        expected_file="$OUT_DIR/$filename"

        if [ ! -f "$expected_file" ]; then
            echo -e "⚠️  Skipping $filename (No expected output)"
            continue
        fi

        # Run Program
        ./"$EXE" < "$input_file" > my_output.txt

        # VERDICT LOGIC
        # 1. Try Exact Match First (Fastest & Safest for Problem 2)
        if diff -w my_output.txt "$expected_file" > /dev/null; then
            echo -e "Test Case $filename: ${GREEN}PASSED (Exact Match)${NC}"
        else
            # 2. If Exact Match fails AND we have a checker (Problem 1), use it
            if [ ! -z "$CHECKER" ] && [ -f "$CHECKER" ]; then
                check_output=$(python3 "$CHECKER" "$input_file" my_output.txt "$expected_file" 2>&1)
                
                if [[ $check_output == *"CORRECT"* ]]; then
                    echo -e "Test Case $filename: ${GREEN}PASSED (Valid Logic)${NC}"
                else
                    echo -e "Test Case $filename: ${RED}FAILED${NC}"
                    echo "   [Checker Output]:"
                    echo "$check_output" | head -n 5
                fi
            else
                # 3. Standard Failure (for Problem 2 or missing checker)
                echo -e "Test Case $filename: ${RED}FAILED${NC}"
                echo "   Mismatch details (First 5 lines):"
                diff -w -y --suppress-common-lines my_output.txt "$expected_file" | head -n 5
            fi
        fi
    done

    # Cleanup for this problem
    rm -f "$EXE" my_output.txt
    echo ""
}

# ================= MAIN EXECUTION =================

# --- PROBLEM 1 CONFIGURATION ---
# Uses check_flow.py because flows can vary
run_tests "Problem 1 (Max Flow)" \
          "PRBLM1.cpp" \
          "prblm1" \
          "Test_Cases/Problem 1/Inputs" \
          "Test_Cases/Problem 1/Outputs" \
          "check_flow.py"

# --- PROBLEM 2 CONFIGURATION ---
# Uses standard diff because Bipartite Matching pairs usually 
# expect specific output or exact counts.
run_tests "Problem 2 (Bipartite)" \
          "PRBLM2.cpp" \
          "prblm2" \
          "Test_Cases/Problem 2/Inputs" \
          "Test_Cases/Problem 2/Outputs" \
          "" # No python checker for Problem 2

echo -e "${BLUE}All tests finished.${NC}"