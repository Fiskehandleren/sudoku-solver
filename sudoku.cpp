#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <random> 
#include <unordered_set>

using namespace std;
std::random_device rd;
std::default_random_engine generator(rd());

class Sudoku {
public:
    int** mat;
    int N;
   
    // number of columns/rows.
    int SRN;
   
    // No. Of missing digits
    int K;

    // Map to store the initial numbers in the board
    std::unordered_map<int, int> initialNumbers;
 
    // Constructor
    Sudoku(int N, int K)
    {
        this->N = N;
        this->K = K;
       
        // Compute square root of N
        double SRNd = sqrt(N);
        SRN = (int)SRNd;
        mat = new int*[N];
 
        // Create a row for every pointer
        for (int i = 0; i < N; i++) 
        {
           
            // Note : Rows may not be contiguous
            mat[i] = new int[N];
 
            // Initialize all entries as false to indicate
            // that there are no edges initially
            memset(mat[i], 0, N * sizeof(int));
        }
    }
    
    // Store the initial numbers in the board in a map, in order not to change them
    // when creating candidates
    void storeInitialNumbers() {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; ++col) {
                cout << row*N + col << endl;
                if (mat[row][col] != 0) {
                    initialNumbers.insert({row*N + col, mat[row][col]});
                }
            }
        }
    }
   
    // Sudoku Generator
    void createProblem()
    {
       
        // Fill the diagonal of SRN x SRN matrices
        fillDiagonal();
       
        // Fill remaining blocks
        fillRemaining(0, SRN);
       
        // Remove Randomly K digits to make game
        removeKDigits();

        // Store the initial numbers in the board in order not to change them
        storeInitialNumbers();
    }
   
    // Fill the diagonal SRN number of SRN x SRN matrices
    void fillDiagonal()
    {
        for (int i = 0; i < N; i = i + SRN)
        {
           
            // for diagonal box, start coordinates->i==j
            fillBox(i, i);
        }
    }
    // Returns false if given 3 x 3 block contains num.
    bool unUsedInBox(int rowStart, int colStart, int num)
    {
        for (int i = 0; i < SRN; i++) {
            for (int j = 0; j < SRN; j++) {
                if (mat[rowStart + i][colStart + j]
                    == num) {
                    return false;
                }
            }
        }
        return true;
    }
    // Fill a 3 x 3 matrix.
    void fillBox(int row, int col)
    {
        int num;
        for (int i = 0; i < SRN; i++) {
            for (int j = 0; j < SRN; j++) {
                do {
                    num = randomGenerator(N);
                } while (!unUsedInBox(row, col, num));
                mat[row + i][col + j] = num;
            }
        }
    }
    // Random generator
    int randomGenerator(int upper_bound) {
        std::uniform_int_distribution<int> distribution(1, upper_bound);
        return distribution(generator);
    }

    // Check if safe to put in cell
    bool checkIfSafe(int i, int j, int num)
    {
        return (
            unUsedInRow(i, num) && unUsedInCol(j, num)
            && unUsedInBox(i - i % SRN, j - j % SRN, num));
    }
    // check in the row for existence
    bool unUsedInRow(int i, int num)
    {
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == num) {
                return false;
            }
        }
        return true;
    }
    // check in the row for existence
    bool unUsedInCol(int j, int num)
    {
        for (int i = 0; i < N; i++) {
            if (mat[i][j] == num) {
                return false;
            }
        }
        return true;
    }
    // A recursive function to fill remaining
    // matrix. This should probably be remade
    bool fillRemaining(int i, int j)
    {
        // System.out.println(i+" "+j);
        if (j >= N && i < N - 1) {
            i = i + 1;
            j = 0;
        }
        if (i >= N && j >= N) {
            return true;
        }
        if (i < SRN) {
            if (j < SRN) {
                j = SRN;
            }
        }
        else if (i < N - SRN) {
            if (j == (int)(i / SRN) * SRN) {
                j = j + SRN;
            }
        }
        else {
            if (j == N - SRN) {
                i = i + 1;
                j = 0;
                if (i >= N) {
                    return true;
                }
            }
        }
        for (int num = 1; num <= N; num++) {
            if (checkIfSafe(i, j, num)) {
                mat[i][j] = num;
                if (fillRemaining(i, j + 1)) {
                    return true;
                }
                mat[i][j] = 0;
            }
        }
        return false;
    }
    // Remove the K no. of digits to
    // complete game
    void removeKDigits()
    {
        int count = K;
        while (count != 0) {
            int cellId = randomGenerator(N * N) - 1;
            // extract coordinates i and j
            int i = (cellId / N);
            int j = cellId % N;
            if (j != 0) {
                j = j - 1;
            }
            if (mat[i][j] != 0) {
                count--;
                mat[i][j] = 0;
            }
        }
    }
    // Print sudoku
    void printSudoku(int** board, int row1 = -1, int col1 = -1, int row2 = -1, int col2 = -1) {
        for (int i = 0; i < N; i++) {
            if (i > 0 && i % 3 == 0) {
                for (int k = 0; k <= N; k++) {
                    cout << "- ";
                }
                cout << endl;
            }
            for (int j = 0; j < N; j++) {
                if (j > 0 && j % 3 == 0) {
                    cout << "| ";
                }
                
                if ((i == row1 && j == col1) || (i == row2 && j == col2)) {
                    if (row1 >= 0 && col1 >= 0 && row2 >= 0 && col2 >= 0) {
                        cout << "\033[31m" << to_string(board[i][j]) << " \033[0m";  // ANSI code for red
                    } else {
                        cout << to_string(board[i][j]) << " ";
                    }
                } else {
                    cout << to_string(board[i][j]) << " ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    // Fill every 0 entries in a block, with remaining available digits
    void makeInitialGuess(){
        for (int blockRow = 0; blockRow < N; blockRow += SRN) {
            for (int blockCol = 0; blockCol < N; blockCol += SRN) {
                // Collect digits already present in the block
                std::vector<bool> usedDigits(N + 1, false);
                for (int row = blockRow; row < blockRow + SRN; row++) {
                    for (int col = blockCol; col < blockCol + SRN; col++) {
                        usedDigits[mat[row][col]] = true;
                    }
                }
                // Generate the list of remaining digits
                std::vector<int> remainingDigits;
                for (int digit = 1; digit <= N; ++digit) {
                    if (!usedDigits[digit]) {
                        remainingDigits.push_back(digit);
                    }
                }
                // Shuffle the remaining digits
                for (int i = remainingDigits.size() - 1; i > 0; --i) {
                    int j = randomGenerator(i);
                    std::swap(remainingDigits[i], remainingDigits[j]);
                }
                // Fill in the remaining digits
                int remainingCount = 0;
                for (int row = blockRow; row < blockRow + SRN; row++) {
                    for (int col = blockCol; col < blockCol + SRN; col++) {
                        if (mat[row][col] == 0) {
                            mat[row][col] = remainingDigits[remainingCount++];
                        }
                    }
                }
            }
        }
    }

    // Calculate the error for a given candidate in the Sudoku board. This is the number of 
    // repeated digits in each row and column
    int calculateError(int** candidate) {
        int errorCols = 0;
        std::unordered_set<int> usedDigitsCol; 

        for (int col = 0; col < N; col++) {
            usedDigitsCol.clear();
            for (int row = 0; row < N; row++) {
                if (usedDigitsCol.find(candidate[row][col]) == usedDigitsCol.end()) {
                    usedDigitsCol.insert(candidate[row][col]);
                } else {
                    errorCols++;
                }
            } 
        }

        int errorRows = 0;
        std::unordered_set<int> usedDigitsRow; 
        for (int row = 0; row < N; row++) {
            usedDigitsRow.clear();
            for (int col = 0; col < N; col++) {
                if (usedDigitsRow.find(candidate[row][col]) == usedDigitsRow.end()) {
                    usedDigitsRow.insert(candidate[row][col]);
                } else {
                    errorRows++;
                }
            } 
        }
    
        return -(errorCols + errorRows);
    }


    int** makeCandidate() {
        // Copy mat 
        int **copyMat = new int*[N];  // Allocate new rows
        for (int i = 0; i < N; ++i) {
            copyMat[i] = new int[N];  // Allocate new columns for each row
            for (int j = 0; j < N; ++j) {
                copyMat[i][j] = mat[i][j];  // Copy each element
            }
        }
        auto success = swapDigits(copyMat);
        while (!success) {
            success = swapDigits(copyMat);
        }
        return copyMat;
    }


    // Inside a square, swap two of the numbers randomly
    int** swapDigits(int** board) {
        // Get position of block in which we will be swapping two digits
        auto randomRow = randomGenerator(SRN) - 1;
        auto randomCol = randomGenerator(SRN) - 1;
        // cout << randomRow << ", " << randomCol << endl;

        // Find first digit to swap
        auto randomRowDigit1 = randomGenerator(SRN) - 1;
        auto randomColDigit1 = randomGenerator(SRN) - 1;

        // Find second digit to swap
        auto randomRowDigit2 = randomGenerator(SRN) - 1;
        auto randomColDigit2 = randomGenerator(SRN) - 1;

        // Check if the digits were already filled in the initial board
        auto firstDigit = initialNumbers.find((randomRow*SRN + randomRowDigit1)*N + randomCol*SRN + randomColDigit1);
        auto secondDigit = initialNumbers.find((randomRow*SRN + randomRowDigit2)*N + randomCol*SRN + randomColDigit2);

        if (firstDigit != initialNumbers.end() || secondDigit != initialNumbers.end()) {
            return nullptr;
        }
        // Check if the digits to swap are the same
        else if (randomColDigit1 == randomColDigit2 && randomRowDigit1 == randomRowDigit2) {
            return nullptr;
        } else {
            printSudoku(board, randomRow*SRN + randomRowDigit1, randomCol*SRN + randomColDigit1, randomRow*SRN + randomRowDigit2, randomCol*SRN + randomColDigit2);
            cout << "Swap [" << randomRow*SRN + randomRowDigit1 << ", " << randomCol*SRN + randomColDigit1 << "] = " << board[randomRow*SRN + randomRowDigit1][randomCol*SRN + randomColDigit1] << " with [" << randomRow*SRN + randomRowDigit2 << ", " << randomCol*SRN + randomColDigit2 << "] = " << board[randomRow*SRN + randomRowDigit2][randomCol*SRN + randomColDigit2] << endl;
            auto first = board[randomRow*SRN + randomRowDigit1][randomCol*SRN + randomColDigit1];
            board[randomRow*SRN + randomRowDigit1][randomCol*SRN + randomColDigit1] = board[randomRow*SRN + randomRowDigit2][randomCol*SRN + randomColDigit2];
            board[randomRow*SRN + randomRowDigit2][randomCol*SRN + randomColDigit2] = first;
            printSudoku(board, randomRow*SRN + randomRowDigit1, randomCol*SRN + randomColDigit1, randomRow*SRN + randomRowDigit2, randomCol*SRN + randomColDigit2);
            return board;
        }
    }
};
int main()
{
    srand(time(0));
    int N = 9;
    int K = 20;
    Sudoku* sudoku = new Sudoku(N, K);
    sudoku->createProblem();
    cout << "Initial board with " << K << " elements removed:" << endl;
    sudoku->printSudoku(sudoku->mat);
    cout << "\t" << endl;
    cout << "Filled out non-solved board:" << endl;
    sudoku->makeInitialGuess();
    // Make initial guess 
    sudoku->printSudoku(sudoku->mat);
    // Calculate error
    auto best_score = sudoku->calculateError(sudoku->mat);
    auto iteration = 0;
    auto temp = 0.5;
    while (iteration <= 500 && best_score != 0) {
        cout << "Iteration: " << iteration << endl;
        auto candidate = sudoku->makeCandidate();
        auto candidate_score = sudoku->calculateError(candidate);
        cout << "Best score: " << best_score << " Candidate score: " << candidate_score << endl;
        auto delta = candidate_score - best_score;
        cout << "Delta: " << delta << endl;
        if (delta > 0 || exp(delta/temp) > static_cast<double>(rand()) / RAND_MAX) {
            cout << "Accepted" << endl;
            sudoku->mat = candidate;
            best_score = candidate_score;
        }
        iteration++;
        temp = 0.99 * temp;
    }
    cout << "Final board:" << endl;
    sudoku->printSudoku(sudoku->mat);
    cout << "Final error: " << best_score << endl;
    return 0;
}