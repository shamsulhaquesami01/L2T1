import sys

def solve():
    # Arguments: 1=InputFile, 2=UserOutputFile, 3=ExpectedOutputFile
    input_file = sys.argv[1]
    user_file = sys.argv[2]
    expected_file = sys.argv[3]

    # --- 1. Read Input Data ---
    valid_edges = set()
    try:
        with open(input_file, 'r') as f:
            tokens = f.read().split()
            if not tokens: return
            iterator = iter(tokens)
            
            N = int(next(iterator))
            K = int(next(iterator))
            M = int(next(iterator))
            
            for _ in range(M):
                u = int(next(iterator))
                v = int(next(iterator))
                valid_edges.add((u, v))
                
    except StopIteration:
        print("❌ Input file format error")
        sys.exit(1)

    # --- 2. Read Expected Count ---
    try:
        with open(expected_file, 'r') as f:
            expected_count = int(f.read().split()[0])
    except:
        print("⚠️  Could not read expected output file")
        sys.exit(1)

    # --- 3. Read User Output ---
    try:
        with open(user_file, 'r') as f:
            lines = f.readlines()
            if not lines:
                print("❌ Your output is empty!")
                sys.exit(1)
                
            user_count_line = lines[0].strip()
            if not user_count_line:
                print("❌ First line (Max Matching) is missing")
                sys.exit(1)
                
            user_count = int(user_count_line)
            user_pairs = []
            
            for line in lines[1:]:
                parts = line.split()
                if len(parts) >= 2:
                    user_pairs.append((int(parts[0]), int(parts[1])))
                    
    except ValueError:
        print("❌ Output format error (First line must be integer)")
        sys.exit(1)

    # --- 4. VERIFICATION LOGIC ---

    # Check A: Does the count match the optimal answer?
    if user_count != expected_count:
        print(f"❌ Wrong Matching Count! Expected: {expected_count}, Got: {user_count}")
        sys.exit(1)

    # Check B: Did you print the same number of pairs as you claimed?
    if len(user_pairs) != user_count:
        print(f"❌ Inconsistency: You claimed {user_count} matches but printed {len(user_pairs)} pairs.")
        sys.exit(1)

    # Check C: Validity of Pairs
    seen_officers = set()
    seen_agents = set()

    for u, v in user_pairs:
        # 1. Does this edge exist in input?
        if (u, v) not in valid_edges:
            print(f"❌ Invalid Pair: ({u}, {v}) is not compatible according to input!")
            sys.exit(1)
        
        # 2. Is the officer already matched?
        if u in seen_officers:
            print(f"❌ Duplicate Assignment: Officer {u} is assigned to multiple stations!")
            sys.exit(1)
            
        # 3. Is the agent already matched?
        if v in seen_agents:
            print(f"❌ Duplicate Assignment: Agent {v} is assigned to multiple stations!")
            sys.exit(1)

        seen_officers.add(u)
        seen_agents.add(v)

    print("✅ CORRECT (Valid Matching)")
    sys.exit(0)

if __name__ == "__main__":
    solve()