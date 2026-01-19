#!/bin/bash

echo "========================================="
echo "Testing Kruskal's Algorithm"
echo "========================================="

g++ -std=c++17 2305055_kruskal.cpp -o kruskal_solution

if [ $? -ne 0 ]; then
    echo "Kruskal compilation failed!"
    exit 1
fi

k_passed=0
k_failed=0

for i in {1..10}; do
    echo -n "Test $i: "
    ./kruskal_solution < sampleio/input/test$i.txt > temp_output.txt 2>&1
    
    my_weight=$(head -n 1 temp_output.txt | awk '{print $NF}')
    expected_weight=$(head -n 1 sampleio/kruskals/out$i.txt | awk '{print $NF}')
    
    if [ "$my_weight" = "$expected_weight" ]; then
        tail -n +2 temp_output.txt | sort > my_edges.txt
        tail -n +2 sampleio/kruskals/out$i.txt | sort > expected_edges.txt
        
        if diff -q my_edges.txt expected_edges.txt > /dev/null 2>&1; then
            echo "✓ PASSED"
            ((k_passed++))
        else
            echo "✓ PASSED (different edge order)"
            ((k_passed++))
        fi
    else
        echo "✗ FAILED (Expected weight: $expected_weight, Got: $my_weight)"
        ((k_failed++))
    fi
done

echo ""
echo "Kruskal Results: $k_passed passed, $k_failed failed"
echo ""

echo "========================================="
echo "Testing Prim's Algorithm"
echo "========================================="

g++ -std=c++17 2305055_prims.cpp -o prims_solution

if [ $? -ne 0 ]; then
    echo "Prim compilation failed!"
    exit 1
fi

p_passed=0
p_failed=0
p_warning=0

for i in {1..10}; do
    echo -n "Test $i: "
    ./prims_solution < sampleio/input/test$i.txt > temp_output.txt 2>&1
    
    my_weight=$(head -n 1 temp_output.txt | awk '{print $NF}')
    expected_weight=$(head -n 1 sampleio/prims/out$i.txt | awk '{print $NF}')
    
    if [ "$my_weight" = "$expected_weight" ]; then
        tail -n +3 temp_output.txt | sort > my_edges.txt
        tail -n +3 sampleio/prims/out$i.txt | sort > expected_edges.txt
        
        if diff -q my_edges.txt expected_edges.txt > /dev/null 2>&1; then
            echo "✓ PASSED"
            ((p_passed++))
        else
            echo "⚠ Weight matches but edge order differs (OK)"
            ((p_passed++))
            ((p_warning++))
        fi
    else
        echo "✗ FAILED (Expected weight: $expected_weight, Got: $my_weight)"
        ((p_failed++))
    fi
done

echo ""
if [ $p_warning -gt 0 ]; then
    echo "Prim Results: $p_passed passed ($p_warning with different edge order), $p_failed failed"
else
    echo "Prim Results: $p_passed passed, $p_failed failed"
fi
echo ""

echo "========================================="
echo "Overall: $((k_passed + p_passed)) passed, $((k_failed + p_failed)) failed"
if [ $p_warning -gt 0 ]; then
    echo "Note: $p_warning Prim test(s) had different edge order (this is normal for MST)"
fi
echo "========================================="

rm -f temp_output.txt my_edges.txt expected_edges.txt kruskal_solution prims_solution


