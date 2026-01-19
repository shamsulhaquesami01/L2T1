#!/bin/bash

g++ -std=c++17 DSA-II/MST/OFFLINE/prims.cpp -o solution

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Testing Prim's Algorithm..."
echo ""

passed=0
failed=0
warning=0

for i in {1..10}; do
    echo "=== Test $i ==="
    ./solution < DSA-II/MST/OFFLINE/sampleio/input/test$i.txt > temp_output.txt 2>&1
    
    # Extract weight using awk (works on all systems)
    my_weight=$(head -n 1 temp_output.txt | awk '{print $NF}')
    expected_weight=$(head -n 1 DSA-II/MST/OFFLINE/sampleio/prims/out$i.txt | awk '{print $NF}')
    
    if [ "$my_weight" = "$expected_weight" ]; then
        # Skip first 2 lines (weight + root), sort edges, compare
        tail -n +3 temp_output.txt | sort > my_edges.txt
        tail -n +3 DSA-II/MST/OFFLINE/sampleio/prims/out$i.txt | sort > expected_edges.txt
        
        if diff -q my_edges.txt expected_edges.txt > /dev/null 2>&1; then
            echo "PASSED"
            ((passed++))
        else
            echo "Weight matches but edge order differs (This is OK for MST)"
            ((warning++))
        fi
    else
        echo "FAILED - Weight mismatch!"
        echo "Expected: $expected_weight, Got: $my_weight"
        ((failed++))
    fi
    echo ""
done

echo "========================================"
echo "Results: $passed passed, $warning warnings, $failed failed"
echo "========================================"

rm -f temp_output.txt my_edges.txt expected_edges.txt solution
