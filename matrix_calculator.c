#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MATRIX_LIMIT 1000
#define OPERATIONS 7

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
int determinant(void);
int inverse(void);

struct operation {
    int id;
    char *name;
};

struct operation op[OPERATIONS];


int main (void) 
{
    //name the operations
    struct operation op[OPERATIONS];
    op[0].name = "Gauss-Jordan";
    op[1].name = "Add";
    op[2].name = "Scalar-Multi";
    op[3].name = "Matrix-Multi";
    op[4].name = "Transpose";
    op[5].name = "Determinant";
    op[6].name = "Inverse";
    int selected = 0;
    int return_value = 0;

    while (1) {
        system("cls");
        printf("WELCOME TO KANE's EXPERIMENTAL GENERAL PURPOSE MATRIX CALCULATOR \033[33mv1.2\033[0m\n\n");
        printf("Matrix Operations: \n");

        //assign operations with ids and print ids with corresponding name
        for(int i = 0; i < OPERATIONS; i++)
        {
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
        }

        //if selected is invalid, exit
        if(selected < 0 || selected > OPERATIONS) {
            printf("Invalid Input...");
            getch();
            continue;
        }

        system("cls");

        //select operations
        printf("%s SELECTED\n\n", op[selected - 1].name);
        switch(selected)
        {
            default:
                return 1;
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
                return_value = determinant();
                break;
            case 7:
                return_value = inverse();
                break;
        }

        printf("\nReturn Value = %d\nPress Enter to Continue...", return_value);
        getchar();
        getchar();
        continue;
    }
}

int gauss_jordan(void)
{   
    int rows = 0, columns = 0, type = 0;

    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    check_gauss(rows, columns, matrix, type, NULL);
    check_jordan(rows, columns, matrix, type);

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
    return 0;

}

int scalar_multi(void)
{
    int rows = 0, columns = 0, type = 2;
    double factor = 0.0;

    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));
    printf("Input Factor: ");
    scanf("%lf", &factor);
    printf("\n");

    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    printf("Matrix:", factor);
    print_matrix(rows, columns, matrix, type);

    //reuse multi(), multiply whole matrix one row at a time
    for(int i = 0; i < rows; i++)
    {
        multi(columns, i, factor, matrix, type);
    }
    printf("x\033[33m%.2lf\033[0m\n", factor);

    print_matrix(rows, columns, matrix, type);

    return 0;
}

int multi_matrix(void) 
{
    int rows = 0, columns = 0, rows2, columns2, type = 3;

    printf("1st Matrix:\n");
    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    printf("2nd Matrix:\n");
    if(matrix_dimensions(&rows2, &columns2, type) != 0)
    {
        return 2;
    }

    if(columns != rows2) 
    {
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
    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    printf("2nd Matrix:\n");
    if(populate_matrix(rows2, columns2, matrix2, type) != 0)
    {
        return 3;
    }

    printf("Matrices:");
    print_matrix(rows, columns, matrix, type);
    printf("(x)");
    print_matrix(rows2, columns2, matrix2, type);
    printf("\n");

    //matrix multiplication algorithm
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns2; j++)
        {
            int a = 0;
            int b = 0;
            for(int k = 0; k < columns; k++)
            {
                a = matrix[i][k] * matrix2[k][j];
                b = b + a;
                if(k < columns)
                {
                    printf(" (\033[33m%.1lf\033[0m x \033[33m%.1lf\033[0m) ", matrix[i][k], matrix2[k][j]);
                }
                if(k < columns - 1)
                {
                    printf("+");
                }
            }

            matrix3[i][j] = b;
        }
    printf("\n");
    }

    print_matrix(rows, columns2, matrix3, type);
    return 0;

}

int transpose(void)
{
    int rows = 0, columns = 0, type = 4;

    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));
    double matrix2[columns][rows];
    memset(matrix2, 0, sizeof(matrix2));

    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    //switch rows and columns
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            matrix2[j][i] = matrix[i][j];
        }
    }

    print_matrix(columns, rows, matrix2, type);

    return 0;
}

int determinant(void)
{
    int rows = 0, columns = 0, type = 5;

    if(matrix_dimensions(&rows, &columns, type) != 0)
    {
        return 2;
    }

    double matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    if(populate_matrix(rows, columns, matrix, type) != 0)
    {
        return 3;
    }

    printf("Matrix:");
    print_matrix(rows, columns, matrix, type);

    double det = 1;
    double *det_address = &det;

    check_gauss(rows, columns, matrix, type, det_address);

    for (int i = 1; i < rows; i++)
    {
        if (matrix[i][i] == 0) {
            det = 0;
            break;
        }
    }

    printf("\nThe determinant of the matrix is %.2lf\n", det);
    return 0;
}

int inverse(void)
{
    int rows = 0, columns = 0, type = 6;

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

    return 0;
}

int matrix_dimensions(int *rows, int *columns, int type)
{
    while(*rows < 2 || *columns < 1 || *rows > MATRIX_LIMIT || *columns > MATRIX_LIMIT) 
    {
        if(type == 0) 
        {
            printf("Enter Matrix Dimensions(int x int): ");
            if(scanf("%i x %i", rows, columns) == 2 && *rows >= 2 && *columns >=3)
            {
                printf("\n");
                break;
            } else {
                printf("Invalid input...");\
                getch();
                return 2;
            }
        } else if (type == 5 || type == 6) { // requires square matrix
            printf("Enter Matrix Size (int): ");
            if(scanf("%i", rows) == 1 && *rows >= 2)
            {
                *columns = *rows;
                printf("\n");
                break;
            } else {
                printf("Invalid input...");
                getch();
                return 2;
            }
        } else {
            printf("Enter Matrix Dimensions(int x int): ");
            if(scanf("%i x %i", rows, columns) == 2 && *rows >= 2 && *columns >= 2)
            {
                printf("\n");
                break;
            } else {
                printf("Invalid input...");
                getch();
                return 2;
            }
        }
    }
    return 0;
}

int populate_matrix(int rows, int columns, double matrix[][columns], int type)
{
    if (type == 6) {
        columns /= 2;
    }

    for(int x = 0; x < rows; x++) 
    {
        for (int y = 0; y < columns; y++) 
        {
            printf("Populate Matrix, Enter Value for Matrix Index[\033[33m%i\033[0m][\033[33m%i\033[0m]: ", x, y);
            if(scanf("%lf", &matrix[x][y]) != 1) {
                printf("Invalid Input...");
                getch();
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

            if (y == columns / 3 && type == 6)
            {
                printf("|");
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

    if(type == 0 || type == 5 || type == 6)
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
                    if (type == 5) {
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
            if (type == 5) {
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
        if (type == 6 && diag_col == (columns / 2)) {
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