
// It is known that fleas in Berland can jump only vertically and horizontally, 
// and the length of the jump is always equal to s centimeters. 
// A flea has found herself at the center of some cell of the checked board of the size n × m centimeters (each cell is 1 × 1 centimeters). 
// She can jump as she wishes for an arbitrary number of times, she can even visit a cell more than once. 
// The only restriction is that she cannot jump out of the board.

// The flea can count the amount of cells that she can reach from the starting position (x, y).
//  Let's denote this amount by dx, y. Your task is to find the number of such starting positions (x, y),
//   which have the maximum possible value of dx, y



#include <iostream>

using namespace std;

int main() {
    // Use long long to prevent overflow during multiplication
    long long n, m, s;
    
    // Read input: n (rows), m (cols), s (jump size)
    if (cin >> n >> m >> s) {
        
        // Calculate optimal starting positions for the row dimension (n)
        long long countX;
        long long remX = n % s;
        long long divX = n / s;
        
        if (remX == 0) {
            // If n is perfectly divisible by s, all positions give the same reach
            countX = n;
        } else {
            // Otherwise, only the positions in the 'remainder' chains give max reach (divX + 1)
            // Number of such positions = (number of chains) * (size of each chain)
            countX = remX * (divX + 1);
        }

        // Calculate optimal starting positions for the column dimension (m)
        long long countY;
        long long remY = m % s;
        long long divY = m / s;
        
        if (remY == 0) {
            countY = m;
        } else {
            countY = remY * (divY + 1);
        }

        // The total number of pairs is the product of the counts
        cout << countX * countY << endl;
    }

    return 0;
}