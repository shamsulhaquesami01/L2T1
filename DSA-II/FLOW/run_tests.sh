#!/bin/bash

# ================= CONFIGURATION =================
SOURCE_FILE="PRBLM1.cpp"
EXECUTABLE="prblm1"
INPUT_DIR="Test_Cases/Problem 1/Inputs"
EXPECTED_DIR="Test_Cases/Problem 1/Outputs"
CHECKER_SCRIPT="check_flow.py"
# =================================================

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' 

echo "Compiling $SOURCE_FILE..."
g++ -std=c++17 -o "$EXECUTABLE" "$SOURCE_FILE"

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation Failed!${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Compilation Successful!${NC}"
echo "----------------------------------------"

for input_file in "$INPUT_DIR"/*.txt; do
    filename=$(basename "$input_file")
    expected_file="$EXPECTED_DIR/$filename"

    if [ ! -f "$expected_file" ]; then
        echo "⚠️  Skipping $filename (No expected output)"
        continue
    fi

    # Run User Program
    ./"$EXECUTABLE" < "$input_file" > my_output.txt

    # 1. Try Exact Match First (Fastest)
    if diff -w my_output.txt "$expected_file" > /dev/null; then
        echo -e "Test Case $filename: ${GREEN}PASSED (Exact Match)${NC}"
    else
        # 2. If Diff fails, use Python Validator
        # python3 check_flow.py [Input] [UserOutput] [ExpectedOutput]
        check_output=$(python3 "$CHECKER_SCRIPT" "$input_file" my_output.txt "$expected_file" 2>&1)
        
        if [[ $check_output == *"CORRECT"* ]]; then
            echo -e "Test Case $filename: ${GREEN}PASSED (Valid Flow)${NC}"
        else
            echo -e "Test Case $filename: ${RED}FAILED${NC}"
            echo "$check_output"
            echo "----------------------------------------"
        fi
    fi
done

rm "$EXECUTABLE" my_output.txt
echo "----------------------------------------"
echo "Done."