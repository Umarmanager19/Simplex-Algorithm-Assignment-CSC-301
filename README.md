# Simplex-Algorithm-Assignment-CSC-


Explanation of the C++ Code

This program implements the Simplex Algorithm, which is used to solve Linear Programming (LP) problems in standard form (maximization problems with constraints).

1. Header Files

#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

	•	iostream → Handles input and output.
	•	vector → Used for dynamic arrays (Simplex tableau).
	•	iomanip → For formatted output (e.g., precision control).
	•	limits → Used to define infinity for numerical operations.

2. Simplex Class Definition

class Simplex {
private:
    int m, n;  
    vector<vector<double>> tableau;

	•	m → Number of constraints.
	•	n → Number of decision variables.
	•	tableau → A 2D vector representing the Simplex tableau, which stores the linear equations.

3. Constructor

Simplex(int num_variables, int num_constraints) {
    n = num_variables;
    m = num_constraints;
    tableau.resize(m + 1, vector<double>(n + m + 1, 0));
}

	•	Initializes the tableau matrix, which has (m+1) rows and (n+m+1) columns.
	•	Extra columns for slack variables and the right-hand side (RHS).

4. Input Function

void input() {
    cout << "Enter the coefficients of the objective function (c1, c2, ..., cn): ";
    for (int i = 0; i < n; i++) {
        cin >> tableau[0][i];
        tableau[0][i] = -tableau[0][i]; 
    }

    cout << "Enter the coefficients of the constraint equations:\n";
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> tableau[i][j]; 
        }
        cout << "Enter the right-hand side (b" << i << "): ";
        cin >> tableau[i][n + m]; 

        // Add slack variables
        tableau[i][n + i - 1] = 1;
    }
}

	•	Inputs the objective function and constraints.
	•	Converts maximization to a negative format for Simplex.
	•	Appends slack variables to convert inequalities to equalities.

5. Displaying the Tableau

void displayTableau() {
    cout << "\nCurrent Tableau:\n";
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n + m; j++) {
            cout << setw(10) << fixed << setprecision(2) << tableau[i][j] << " ";
        }
        cout << endl;
    }
}

	•	Prints the Simplex tableau for debugging.

6. Identifying Pivot Column (Entering Variable)

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

	•	Pivot column is the column with the most negative value in row 0 (objective function row).
	•	If no negative values exist, the optimal solution is found.

7. Identifying Pivot Row (Leaving Variable)

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

	•	Pivot row is chosen using the minimum ratio test:
￼
	•	If no valid row is found, the problem is unbounded.

8. Performing Pivoting (Row Operations)

void pivot(int pivot_row, int pivot_col) {
    double pivot_element = tableau[pivot_row][pivot_col];

    // Normalize pivot row
    for (int j = 0; j <= n + m; j++) {
        tableau[pivot_row][j] /= pivot_element;
    }

    // Make other elements in pivot column zero
    for (int i = 0; i <= m; i++) {
        if (i != pivot_row) {
            double factor = tableau[i][pivot_col];
            for (int j = 0; j <= n + m; j++) {
                tableau[i][j] -= factor * tableau[pivot_row][j];
            }
        }
    }
}

	•	Normalizes the pivot row by dividing it by the pivot element.
	•	Eliminates other values in the pivot column to create a unit column.

9. Simplex Algorithm Execution

void solve() {
    while (true) {
        displayTableau();

        int pivot_col = getPivotColumn();
        if (pivot_col == -1) {
            cout << "Optimal solution found.\n";
            break;
        }

        int pivot_row = getPivotRow(pivot_col);
        if (pivot_row == -1) {
            cout << "The problem is unbounded.\n";
            return;
        }

        pivot(pivot_row, pivot_col);
    }
}

	•	Iterates through Simplex steps until an optimal solution is reached.
	•	If no valid pivot column, the solution is optimal.
	•	If no valid pivot row, the problem is unbounded.

10. Extracting the Solution

void getSolution() {
    vector<double> solution(n, 0);

    for (int j = 0; j < n; j++) {
        int basis_row = -1;
        bool is_basic = true;

        for (int i = 1; i <= m; i++) {
            if (tableau[i][j] == 1) {
                if (basis_row == -1)
                    basis_row = i;
                else
                    is_basic = false;
            } else if (tableau[i][j] != 0) {
                is_basic = false;
            }
        }

        if (is_basic && basis_row != -1) {
            solution[j] = tableau[basis_row][n + m];
        }
    }

    cout << "\nOptimal Solution:\n";
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << solution[i] << endl;
    }
    cout << "Maximum Value of Objective Function: " << tableau[0][n + m] << endl;
}

	•	Extracts the optimal values of decision variables.
	•	Prints the maximum value of the objective function.

11. Main Function

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

	•	Takes user input for variables and constraints.
	•	Executes Simplex method to find the optimal solution.

Conclusion

This C++ program implements the Simplex algorithm to solve linear programming problems. It:
	1.	Initializes the Simplex tableau from user input.
	2.	Iteratively finds pivot elements to transform the tableau.
	3.	Identifies an optimal solution or detects unboundedness.
	4.	Extracts and displays the solution.
