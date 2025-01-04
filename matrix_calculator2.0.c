#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MATRIX_LIMIT 1000
#define OPERATIONS 9
#define HISTORY_FILE "history.log"

void add(int columns, int target_r, int source_r, double factor, double matrix[][columns]);
void multi(int columns, int row, double factor, double matrix[][columns], int type);
void swap(int columns, int row1, int row2, double matrix[][columns]);
void print_matrix(int rows, int columns, double matrix[][columns], int type);
int populate_matrix(int rows, int columns, double matrix[][columns], int type);
int matrix_dimensions(int *rows, int *columns, int type);

int gauss_jordan(void);
void check_gauss(int rows, int columns, double matrix[][columns], int type, double *det_address);
void check_jordan(int rows, int columns, double matrix[][columns], int type);
int add_matrix(void);
int scalar_multi(void);
int multi_matrix(void);
int transpose(void);
int trace(void);
int determinant(void);
int inverse(void);
int view_history(void);
void save_to_history(const char *operation, const char *result);


struct operation {
    int id;
    char *name;
};

struct operation op[OPERATIONS];

int main(void) {
    op[0].name = "Gauss-Jordan";
    op[1].name = "Add";
    op[2].name = "Scalar-Multi";
    op[3].name = "Matrix-Multi";
    op[4].name = "Transpose";
    op[5].name = "Trace";
    op[6].name = "Determinant";
    op[7].name = "Inverse";
    op[8].name = "View History";
    int selected = 0;
    int return_value = 0;

    while (1) {
        system("cls");
        printf("WELCOME TO NEO's EXPERIMENTAL GENERAL PURPOSE MATRIX CALCULATOR \033[33mv2.0\033[0m\n\n");
        printf("Matrix Operations: \n");

        for (int i = 0; i < OPERATIONS; i++) {
            op[i].id = i + 1;
            printf("%i. %s\n", op[i].id, op[i].name);
        }

        printf("\nSelect Number (0 to exit): ");
        scanf("%i", &selected);
        if (selected == 0) {
            system("cls");
            printf("Exiting Program...");
            getchar();
            getch();
            return 0;
        }

        if (selected < 1 || selected > OPERATIONS) {
            printf("\nInvalid Input...");
            getch();
            continue;
        }

        system("cls");

        printf("Selected Operation: %s\n\n", op[selected - 1].name);

        switch (selected) {
            case 1:
                return_value = gauss_jordan();
                break;
            case 2:
                return_value = add_matrix();
                break;
            case 3:
                return_value = scalar_multi();
                break;
            case 4:
                return_value = multi_matrix();
                break;
            case 5:
                return_value = transpose();
                break;
            case 6:
                return_value = trace();
                break;
            case 7:
                return_value = determinant();
                break;
            case 8:
                return_value = inverse();
                break;
            case 9:
                return_value = view_history();
                break;
            default:
                continue;
        }

        printf("\nReturn Value = %d\nPress Enter to Continue...", return_value);
        getchar();
        getchar();
        continue;
    }
}

void save_to_history(const char *operation, const char *result) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (file == NULL) {
        perror("Error opening history file");
        return;
    }
    fprintf(file, "%s:\n\n%s\n", operation, result);
    fprintf(file, "==========================\n\n");
    fclose(file);
}

int view_history(void) {
    int selected = 0;

    printf("1. View All\n");
    for (int i = 0; i < OPERATIONS - 1; i++) {
        op[i].id = i + 2;
        printf("%i. %s\n", op[i].id, op[i].name);
    }

    printf("\nSelect Number: ");
    scanf("%d", &selected);

    system("cls");

    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        perror("Error opening history file");
        return 1;
    }

    char line[512];
    int count = 1;
    int found = 0, alpha_find = 0;

    printf("\nOperation History:\n==========================\n\n");
    switch (selected) {
            case 1:
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
                alpha_find = 1;
            case 2:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Gauss-Jordan") != NULL) {
                        alpha_find = 1;
                        found = 1; // Mark that we have found "Gauss-Jordan"
                    }

                    // If we've found "Gauss-Jordan", start printing lines
                    if (found) {
                        printf("%s", line);
                    }

                    // If the line contains "=", stop printing and reset found variable
                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            // Same principle for the rest
            case 3:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Addition") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }

                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            case 4:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Scalar") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }

                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            case 5:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Matrix") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }

                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            case 6:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Transpose") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }


                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            case 7:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Trace") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }

                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            case 8:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Determinant") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }

                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            case 9:
                found = 0;
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Inverse") != NULL) {
                        alpha_find = 1;
                        found = 1;
                    }

                    if (found) {
                        printf("%s", line);
                    }

                    if (strstr(line, "=") != NULL) {
                        found = 0;
                    }
                }
                break;
            default:
                system("cls");
                fclose(file);
                return 1;
        }
    
    if (alpha_find == 0) {
        system("cls");
        printf("None Found\n");
    }

    fclose(file);
    return 0;
}

int gauss_jordan(void) {   
    int rows = 0, columns = 0, type = 0;

    // Get matrix dimensions
    if (matrix_dimensions(&rows, &columns, type) != 0) {
        return 2;
    }

    // Initialize the matrix
    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    // Populate the matrix with user input
    if (populate_matrix(rows, columns, matrix, type) != 0) {
        return 3;
    }

    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    // Save the initial matrix to the log
    char operation[256], result[1024] = {0};
    snprintf(operation, sizeof(operation), "Gauss-Jordan Elimination");
    snprintf(result, sizeof(result), "Original Matrix:\n");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Perform Gauss and Jordan eliminations
    check_gauss(rows, columns, matrix, type, NULL);
    check_jordan(rows, columns, matrix, type);

    // Append the resulting matrix to the log
    strcat(result, "\nReduced Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            if(matrix[i][j] > -0.00001 && matrix[i][j] < 0.00001) {
                snprintf(value, sizeof(value), "0.00 ", matrix[i][j]);
            } else {
                snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            }
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Save the operation and result to the history log
    save_to_history(operation, result);

    return 0;
}


int add_matrix(void)
{
    int rows = 0, columns = 0, type = 1;

    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));
    double matrix2[rows][columns];
    memset(matrix2, 0, sizeof(matrix2));

    printf("1st Matrix:\n");
    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    printf("2nd Matrix:\n");
    if(populate_matrix(rows, columns, matrix2, type) != 0)
    {
        return 3;
    }

    printf("Matrices:");
    print_matrix(rows, columns, matrix, type);
    printf("(+)");
    print_matrix(rows, columns, matrix2, type);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            matrix[i][j] = matrix[i][j] + matrix2[i][j];
        }
    }

    print_matrix(rows, columns, matrix, type);
    
    char operation[256], result[1024] = {0};
    snprintf(operation, sizeof(operation), "Addition");
    snprintf(result, sizeof(result), "Matrix 1:\n");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j] - matrix2[i][j]); // Original value
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    strcat(result, "\nMatrix 2:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix2[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    strcat(result, "\nResult:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            if(matrix[i][j] > -0.00001 && matrix[i][j] < 0.00001) {
                snprintf(value, sizeof(value), "0.00 ", matrix[i][j]);
            } else {
                snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            }
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    save_to_history(operation, result);

    return 0;
}
    
    
int scalar_multi(void) {
    int rows = 0, columns = 0, type = 2;
    double factor = 0.0;

    // Get matrix dimensions
    if (matrix_dimensions(&rows, &columns, type) != 0) {
        return 2;
    }

    // Initialize the matrix
    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    // Get the scalar factor from the user
    printf("Input Factor: ");
    scanf("%lf", &factor);
    printf("\n");

    // Populate the matrix with user input
    if (populate_matrix(rows, columns, matrix, type) != 0) {
        return 3;
    }

    // Display the original matrix
    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    // Prepare the history log entry
    char operation[256], result[1024] = {0};
    snprintf(operation, sizeof(operation), "Scalar Multiplication");
    snprintf(result, sizeof(result), "Original Matrix:\n");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Add the scalar factor to the log
    char factorString[64];
    snprintf(factorString, sizeof(factorString), "\nScalar Factor: %.2lf\n", factor);
    strcat(result, factorString);

    // Multiply the entire matrix by the scalar factor
    for (int i = 0; i < rows; i++) {
        multi(columns, i, factor, matrix, type);
    }
    printf("x\033[33m%.2lf\033[0m\n", factor);

    // Display the resulting matrix
    print_matrix(rows, columns, matrix, type);

    // Append the resulting matrix to the log
    strcat(result, "\nResulting Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            if(matrix[i][j] > -0.00001 && matrix[i][j] < 0.00001) {
                snprintf(value, sizeof(value), "0.00 ", matrix[i][j]);
            } else {
                snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            }
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Save the operation and results to the history log
    save_to_history(operation, result);

    return 0;
}

int multi_matrix(void) {
    int rows = 0, columns = 0, rows2 = 0, columns2 = 0, type = 3;

    printf("1st Matrix:\n");
    if (matrix_dimensions(&rows, &columns, type) != 0) {
        return 2;
    }

    printf("2nd Matrix:\n");
    if (matrix_dimensions(&rows2, &columns2, type) != 0) {
        return 2;
    }

    if (columns != rows2) {
        printf("Incompatible Matrix Dimensions!");
        return 4;
    }

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));
    double matrix2[rows2][columns2];
    memset(matrix2, 0, sizeof(matrix2));
    double matrix3[rows][columns2];
    memset(matrix3, 0, sizeof(matrix3));

    printf("1st Matrix:\n");
    if (populate_matrix(rows, columns, matrix, type) != 0) {
        return 3;
    }

    printf("2nd Matrix:\n");
    if (populate_matrix(rows2, columns2, matrix2, type) != 0) {
        return 3;
    }

    printf("Matrices:");
    print_matrix(rows, columns, matrix, type);
    printf("(x)");
    print_matrix(rows2, columns2, matrix2, type);
    printf("\n");

    // Prepare log entry
    char operation[256], result[2048] = {0};
    snprintf(operation, sizeof(operation), "Matrix Multiplication");
    snprintf(result, sizeof(result), "1st Matrix:\n");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    strcat(result, "\n2nd Matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < columns2; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix2[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Matrix multiplication algorithm
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns2; j++) {
            double sum = 0;
            for (int k = 0; k < columns; k++) {
                sum += matrix[i][k] * matrix2[k][j];
            }
            matrix3[i][j] = sum;
        }
    }

    // Append resulting matrix to the log
    strcat(result, "\nResulting Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns2; j++) {
            char value[32];
            if(matrix[i][j] > -0.00001 && matrix[i][j] < 0.00001) {
                snprintf(value, sizeof(value), "0.00 ", matrix[i][j]);
            } else {
                snprintf(value, sizeof(value), "%.2lf ", matrix3[i][j]);
            }
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Display the resulting matrix
    print_matrix(rows, columns2, matrix3, type);

    // Save to history
    save_to_history(operation, result);

    return 0;
}

int transpose(void) {
    int rows = 0, columns = 0, type = 4;

    // Get matrix dimensions
    if (matrix_dimensions(&rows, &columns, type) != 0) {
        return 2;
    }

    // Initialize matrices
    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));
    double matrix2[columns][rows];
    memset(matrix2, 0, sizeof(matrix2));

    // Populate the original matrix with user input
    if (populate_matrix(rows, columns, matrix, type) != 0) {
        return 3;
    }

    // Display the original matrix
    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    // Transpose the matrix by switching rows and columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix2[j][i] = matrix[i][j];
        }
    }

    // Display the transposed matrix
    print_matrix(columns, rows, matrix2, type);

    // Prepare the history log entry
    char operation[256], result[2048] = {0};
    snprintf(operation, sizeof(operation), "Transpose");

    // Append the original matrix to the log
    snprintf(result, sizeof(result), "Original Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Append the transposed matrix to the log
    strcat(result, "\nTransposed Matrix:\n");
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix2[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Save the operation and results to the history log
    save_to_history(operation, result);

    return 0;
}

int trace(void) {
    int rows = 0, columns = 0, type = 5;

    // Get matrix dimensions
    if (matrix_dimensions(&rows, &columns, type) != 0) {
        return 2;
    }

    // Initialize the matrix
    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    // Populate the matrix with user input
    if (populate_matrix(rows, columns, matrix, type) != 0) {
        return 3;
    }

    // Display the matrix
    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    // Compute the trace of the matrix (sum of diagonal elements)
    double trace = 0;
    for (int i = 0; i < rows; i++) {
        trace += matrix[i][i];
    }
    
    printf("\nThe trace of the matrix is %.2lf\n", trace);

    // Prepare the history log entry
    char operation[256], result[1024] = {0};
    snprintf(operation, sizeof(operation), "Trace");

    // Append the original matrix to the log
    snprintf(result, sizeof(result), "Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Append the trace result to the log
    char traceResult[64];
    snprintf(traceResult, sizeof(traceResult), "\nTrace: %.2lf\n", trace);
    strcat(result, traceResult);

    // Save the operation and result to the history log
    save_to_history(operation, result);

    return 0;
}


int determinant(void) {
    int rows = 0, columns = 0, type = 6;

    // Get matrix dimensions
    if (matrix_dimensions(&rows, &columns, type) != 0) {
        return 2;
    }

    // Initialize the matrix
    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    // Populate the matrix with user input
    if (populate_matrix(rows, columns, matrix, type) != 0) {
        return 3;
    }

    // Display the original matrix
    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    // Initialize determinant calculation
    double det = 1;
    double *det_address = &det;

    // Perform Gaussian elimination to simplify determinant calculation
    check_gauss(rows, columns, matrix, type, det_address);

    // Check for zero values in the diagonal to determine if det = 0
    for (int i = 1; i < rows; i++) {
        if (matrix[i][i] == 0) {
            det = 0;
            break;
        }
    }

    // Display the determinant result
    printf("\nThe determinant of the matrix is %.2lf\n", det);

    // Prepare the history log entry
    char operation[256], result[2048] = {0};
    snprintf(operation, sizeof(operation), "Determinant");

    // Append the original matrix to the log
    snprintf(result, sizeof(result), "Original Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Append determinant result to the log
    char determinantResult[64];
    snprintf(determinantResult, sizeof(determinantResult), "\nDeterminant: %.2lf\n", det);
    strcat(result, determinantResult);

    // Save the operation and results to the history log
    save_to_history(operation, result);

    return 0;
}


int inverse(void) {
    int rows = 0, columns = 0, type = 7;

    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    columns *= 2;

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    for (int i = 0; i < rows; i++) {
        matrix[i][i + (columns / 2)] = 1;
    }

    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    check_gauss(rows, columns, matrix, type, NULL);

    for (int i = 1; i < rows; i++)
    {
        if (matrix[i][i] == 0) {
            printf("\n\033[031mInverse does not exist\033[0m\n");
            return 0;
        }
    }
    
    check_jordan(rows, columns, matrix, type);

    // Log the result
    char operation[256], result[2048] = {0};
    snprintf(operation, sizeof(operation), "Inverse Matrix");

    // Log the original matrix
    snprintf(result, sizeof(result), "Original Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns / 2; j++) {
            char value[32];
            snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    // Log the inverse matrix
    strcat(result, "\nInverse Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = columns / 2; j < columns; j++) {
            char value[32];
            if(matrix[i][j] > -0.00001 && matrix[i][j] < 0.00001) {
                snprintf(value, sizeof(value), "0.00 ", matrix[i][j]);
            } else {
                snprintf(value, sizeof(value), "%.2lf ", matrix[i][j]);
            }
            strcat(result, value);
        }
        strcat(result, "\n");
    }

    save_to_history(operation, result);

    return 0;
}

int matrix_dimensions(int *rows, int *columns, int type)
{
    while(*rows < 2 || *columns < 1 || *rows > MATRIX_LIMIT || *columns > MATRIX_LIMIT) 
    {
        if(type == 0) 
        {
            printf("Enter Matrix Dimensions (int x int): ");
            if(scanf("%i x %i", rows, columns) == 2 && *rows >= 2 && *columns >=3)
            {
                printf("\n");
                break;
            } else {
                printf("\nInvalid input...\n");\
                return 2;
            }
        } else if (type == 5 || type == 6 || type == 7) { // requires square matrix
            printf("Enter Matrix Size (int): ");
            if(scanf("%i", rows) == 1 && *rows >= 2)
            {
                *columns = *rows;
                printf("\n");
                break;
            } else {
                printf("\nInvalid input...\n");
                return 2;
            }
        } else {
            printf("Enter Matrix Dimensions (int x int): ");
            if(scanf("%i x %i", rows, columns) == 2 && *rows >= 2 && *columns >= 2)
            {
                printf("\n");
                break;
            } else {
                printf("\nInvalid input...\n");
                return 2;
            }
        }
    }
    return 0;
}

int populate_matrix(int rows, int columns, double matrix[][columns], int type)
{
    if (type == 7) {
        columns /= 2;
    }

    for(int x = 0; x < rows; x++) 
    {
        for (int y = 0; y < columns; y++) 
        {
            printf("Populate Matrix, Enter Value for Matrix Index[\033[33m%i\033[0m][\033[33m%i\033[0m]: ", x, y);
            if(scanf("%lf", &matrix[x][y]) != 1) {
                printf("\nInvalid Input...\n");
                return 3;
            }
        }
    }
    printf("\n");

    return 0;
}

void print_matrix(int rows, int columns, double matrix[][columns], int type) 
{
    printf("\n");

    for (int x = 0; x < rows; x++) 
    {
        for (int y = 0; y < columns; y++) 
        {
            if(matrix[x][y] > -0.0001 && matrix[x][y] < 0.0001)
            {
                printf("0.00 ");
            } else {
                 printf("%.2lf ", matrix[x][y]);
            }

            if(y == columns - 2 && type == 0)
            {
                printf("| ");
            }

            if (y == columns / 3 && type == 7)
            {
                printf("| ");
            }

        }
            printf("\n");
    }
}


void add(int columns, int target_r, int source_r, double factor, double matrix[][columns]) 
{
    for(int j = 0; j < columns; j++)
    {
        matrix[target_r][j] = matrix[target_r][j] + (matrix[source_r][j] * factor);
    }

    printf("\nR%i + (\033[33m%.2lf\033[0m)R%i\n", target_r + 1, factor, source_r + 1);

}

void multi(int columns, int row, double factor, double matrix[][columns], int type) 
{
    for(int j = 0; j < columns; j++) 
    {
        matrix[row][j] = matrix[row][j] * factor;
    }

    if(type == 0 || type == 6 || type == 7)
    {
    printf("\n(\033[33m%.2lf\033[0m)R%i\n", factor, row + 1);
    }
    
}

void swap(int columns, int row1, int row2, double matrix[][columns]) 
{

    float temp;

    for(int y = 0; y < columns; y++) 
    {
        temp = matrix[row1][y];
        matrix[row1][y] = matrix[row2][y];
        matrix[row2][y] = temp;
    }

    printf("\nR%i <-> R%i\n", row1 + 1, row2 + 1);

}

void check_gauss(int rows, int columns, double matrix[][columns], int type, double *det_address) 
{

    int diag_col = 0;

    for(int x = 0; x < rows; x++) 
    {
        //swap current row with next suitable row
        if(matrix[x][diag_col] == 0)
        {
            for(int i = x; i < rows; i++) 
            {
                if(matrix[i][diag_col] != 0)
                {
                    if (type == 6) {
                        *det_address *= -1;
                    }
                    swap(columns, x, i, matrix);
                    print_matrix(rows, columns, matrix, type);
                    break;
                }
            }
        }

        //check if diag is still 0
        if(matrix[x][diag_col] == 0) 
        {
            return;
        }

        //turn pivot in this row into 1
        if(matrix[x][diag_col] != 1)
        {
            //find inverse of diagonal, then perform operation
            if (type == 6) {
                *det_address *= matrix[x][diag_col];
            }
            double factor = 1.0 / matrix[x][diag_col];
            multi(columns, x, factor, matrix, type);
            print_matrix(rows, columns, matrix, type);
        }

        //clear out column below pivot
        for(int i = x + 1; i < rows; i++)
        {
            if(matrix[i][diag_col] != 0 && matrix[x][diag_col] != 0)
            {
                //find the f in (t + (s * f)), then perform operation (s is always 1)
                float factor = -matrix[i][diag_col];
                add(columns, i, x, factor, matrix);
                print_matrix(rows, columns, matrix, type);
            }
        }

    diag_col++;
    }
}

void check_jordan(int rows, int columns, double matrix[][columns], int type) 
{
    for (int diag_col = 0; diag_col < columns - 1; diag_col++) 
    {
        if (type == 7 && diag_col == (columns / 2)) {
            return;
        }

        // clear out column above pivot
        for (int i = 0; i < diag_col; i++) 
        {
            if (matrix[i][diag_col] != 0 && matrix[diag_col][diag_col] != 0) 
            {
                //find the f in (t + (s * f)), then perform operation (s is always 1)
                float factor = -matrix[i][diag_col];
                add(columns, i, diag_col, factor, matrix);
                print_matrix(rows, columns, matrix, type);
            }
        }
    }

}