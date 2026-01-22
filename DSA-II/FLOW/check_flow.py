import sys

def solve():
    # Arguments: 1=InputFile, 2=UserOutputFile, 3=ExpectedOutputFile
    input_file = sys.argv[1]
    user_file = sys.argv[2]
    expected_file = sys.argv[3]

    # 1. Read Input Data to build the graph structure
    with open(input_file, 'r') as f:
        try:
            line1 = f.read().split()
            if not line1: return # Empty file
            iterator = iter(line1)
            N = int(next(iterator))
            M = int(next(iterator))
            
            edges = [] # Stores (u, v, capacity)
            for _ in range(M):
                u = int(next(iterator))
                v = int(next(iterator))
                c = int(next(iterator))
                edges.append((u, v, c))
            
            s = int(next(iterator))
            t = int(next(iterator))
        except StopIteration:
            print("❌ Input file format error")
            sys.exit(1)

    # 2. Read Expected Max Flow (Just the first line)
    try:
        with open(expected_file, 'r') as f:
            expected_max_flow = int(f.read().split()[0])
    except:
        print("⚠️  Could not read expected output file")
        expected_max_flow = None

    # 3. Read User Output
    with open(user_file, 'r') as f:
        lines = f.readlines()
    
    if not lines:
        print("❌ Your output is empty!")
        sys.exit(1)

    try:
        user_max_flow = int(lines[0].strip())
    except ValueError:
        print("❌ First line of output is not an integer")
        sys.exit(1)

    # 4. Verify Max Flow Value
    if expected_max_flow is not None and user_max_flow != expected_max_flow:
        print(f"❌ Wrong Max Flow! Expected: {expected_max_flow}, Got: {user_max_flow}")
        sys.exit(1)

    # 5. Verify Constraints & Conservation
    # We need to parse the "u v flow/cap" lines
    # Map edges to flow: (u, v, original_index) -> flow
    # Since multigraphs exist, we strictly follow the line order
    
    node_balance = [0] * (N + 200) # Buffer for safety
    
    # Skip the first line (max flow value)
    user_edge_lines = lines[1:]
    
    if len(user_edge_lines) != M:
        print(f"❌ Output line count mismatch. Expected {M} edge lines, got {len(user_edge_lines)}.")
        sys.exit(1)

    for i, line in enumerate(user_edge_lines):
        parts = line.replace('/', ' ').split()
        # Expected format: u v flow capacity
        if len(parts) != 4:
            print(f"❌ Line {i+2} format error: '{line.strip()}'")
            sys.exit(1)
            
        u_out = int(parts[0])
        v_out = int(parts[1])
        f_out = int(parts[2])
        c_out = int(parts[3])
        
        # Match with input
        u_in, v_in, c_in = edges[i]
        
        if u_out != u_in or v_out != v_in:
            print(f"❌ Edge mismatch at line {i+2}. Input has ({u_in}->{v_in}), you printed ({u_out}->{v_out})")
            sys.exit(1)
            
        if c_out != c_in:
            print(f"❌ Capacity mismatch at line {i+2}. Input: {c_in}, You printed: {c_out}")
            sys.exit(1)

        if f_out < 0 or f_out > c_in:
            print(f"❌ Invalid flow on edge {u_in}->{v_in}: {f_out}/{c_in} (Capacity violated)")
            sys.exit(1)
            
        node_balance[u_in] -= f_out
        node_balance[v_in] += f_out

    # Check Conservation (In == Out) for all except s and t
    for i in range(N):
        if i == s or i == t: continue
        
        if node_balance[i] != 0:
            print(f"❌ Flow conservation violated at Node {i}. Net Flow: {node_balance[i]}")
            sys.exit(1)

    # Check Source Outflow matches Total Flow
    # Balance at S should be negative of total flow (since we subtracted outgoing)
    if abs(node_balance[s]) != user_max_flow:
        print(f"❌ Source flow inconsistency. Sum of outgoing edges from source: {abs(node_balance[s])}, but you claimed: {user_max_flow}")
        sys.exit(1)

    print("✅ CORRECT (Valid Flow)")
    sys.exit(0)

if __name__ == "__main__":
    solve()