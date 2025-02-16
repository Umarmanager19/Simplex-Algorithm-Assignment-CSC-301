#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

class Simplex {
private:
    int m, n;  // m = number of constraints, n = number of variables
    vector<vector<double>> tableau;

public:
    // Constructor to initialize the tableau
    Simplex(int num_variables, int num_constraints) {
        n = num_variables;
        m = num_constraints;
        tableau.resize(m + 1, vector<double>(n + m + 1, 0)); // 1 extra row for objective function

        // For simplicity, we initialize everything to zero in the tableau
    }

    // Function to input the constraints and objective function
    void input() {
        cout << "Enter the coefficients of the objective function (c1, c2, ..., cn): ";
        for (int i = 0; i < n; i++) {
            cin >> tableau[0][i];
        }

        cout << "Enter the coefficients of the constraint equations:\n";
        for (int i = 1; i <= m; i++) {
            cout << "For constraint " << i << ":\n";
            for (int j = 0; j < n; j++) {
                cin >> tableau[i][j];
            }
            cout << "Enter the right-hand side (b" << i << "): ";
            cin >> tableau[i][n + m];
        }

        // Convert the objective function to the correct sign (if minimizing)
        for (int j = 0; j < n; j++) {
            tableau[0][j] = -tableau[0][j]; // Since Simplex assumes maximization
        }
    }

    // Function to display the current tableau
    void displayTableau() {
        cout << "\nCurrent Tableau:\n";
        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n + m; j++) {
                cout << setw(10) << tableau[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Function to identify the pivot column (most negative value in the objective row)
    int getPivotColumn() {
        int pivot_col = -1;
        double min_value = 0;
        for (int j = 0; j < n + m; j++) {
            if (tableau[0][j] < min_value) {
                min_value = tableau[0][j];
                pivot_col = j;
            }
        }
        return pivot_col;
    }

    // Function to identify the pivot row (minimum ratio of RHS to pivot column)
    int getPivotRow(int pivot_col) {
        int pivot_row = -1;
        double min_ratio = numeric_limits<double>::infinity();
        for (int i = 1; i <= m; i++) {
            if (tableau[i][pivot_col] > 0) {
                double ratio = tableau[i][n + m] / tableau[i][pivot_col];
                if (ratio < min_ratio) {
                    min_ratio = ratio;
                    pivot_row = i;
                }
            }
        }
        return pivot_row;
    }

    // Perform pivoting operation to update the tableau
    void pivot(int pivot_row, int pivot_col) {
        double pivot_element = tableau[pivot_row][pivot_col];
        
        // Normalize the pivot row
        for (int j = 0; j <= n + m; j++) {
            tableau[pivot_row][j] /= pivot_element;
        }

        // Update the other rows
        for (int i = 0; i <= m; i++) {
            if (i != pivot_row) {
                double factor = tableau[i][pivot_col];
                for (int j = 0; j <= n + m; j++) {
                    tableau[i][j] -= factor * tableau[pivot_row][j];
                }
            }
        }
    }

    // Function to run the Simplex Algorithm
    void solve() {
        while (true) {
            displayTableau();
            
            int pivot_col = getPivotColumn();
            if (pivot_col == -1) {
                // If there are no negative values in the objective row, we're done
                cout << "Optimal solution found.\n";
                break;
            }
            
            int pivot_row = getPivotRow(pivot_col);
            if (pivot_row == -1) {
                // If no pivot row is found, this means the problem is unbounded
                cout << "The problem is unbounded.\n";
                break;
            }

            // Perform pivoting
            pivot(pivot_row, pivot_col);
        }
    }

    // Function to get the solution from the tableau
    void getSolution() {
        vector<double> solution(n, 0); // Initialize solution vector with zeros
        for (int i = 1; i <= m; i++) {
            bool is_basic = true;
            for (int j = 0; j < n; j++) {
                if (tableau[i][j] != 0) {
                    is_basic = false;
                    break;
                }
            }

            if (is_basic) {
                solution[i - 1] = tableau[i][n + m];  // Assign value of RHS to solution
            }
        }

        cout << "Solution:\n";
        for (int i = 0; i < n; i++) {
            cout << "x" << i + 1 << " = " << solution[i] << endl;
        }
    }
};

int main() {
    int num_variables, num_constraints;
    cout << "Enter number of variables: ";
    cin >> num_variables;
    cout << "Enter number of constraints: ";
    cin >> num_constraints;

    Simplex simplex(num_variables, num_constraints);
    simplex.input();
    simplex.solve();
    simplex.getSolution();

    return 0;
}
