#include <stdio.h>
#include <string.h>

#define MATRIX_LIMIT 1000

void add(int columns, int target_r, int source_r, float factor, float matrix[][columns]);
void multi(int columns, int row, float factor, float matrix[][columns]);
void swap(int columns, int row1, int row2, float matrix[][columns]);
int check_gauss(int rows, int columns, float matrix[][columns]);
int check_jordan(int rows, int columns, float matrix[][columns]);
void print_matrix(int rows, int columns, float matrix[][columns]);



int main (void) 
{
    printf("WELCOME TO KANE's EXPERIMENTAL GAUSS-JORDAN MATRIX CALCULATOR v1.0\n\n");
    
    int rows = 0, columns = 0;

    while(rows < 2 || columns < 1 || rows > MATRIX_LIMIT || columns > MATRIX_LIMIT) {
        printf("Enter Matrix Dimensions(int x int): ");
        if(scanf("%i x %i", &rows, &columns) == 2 && rows >= 2 && columns >=3)
        {
            break;
        } else {
            printf("Invalid input!");
            return 1;
        }
    }

   /*while(rows < 2 || rows > MATRIX_LIMIT) {
        printf("Matrix's Number of Rows(2 minimum): ");
        scanf("%i", &rows);
        columns = rows + 1;
    } */

    float matrix[rows][columns];
    memset(matrix, 0, sizeof(matrix));

    for(int x = 0; x < rows; x++) 
    {
        for (int y = 0; y < columns; y++) 
        {
            printf("Populate Matrix, Enter Value for Matrix Index[%i][%i]: ", x, y);
            if(scanf("%f", &matrix[x][y]) != 1) {
                printf("Invalid Input!");
                return 1;
            }
        }
    }

    printf("\nMatrix:");
    print_matrix(rows, columns, matrix);

    check_gauss(rows, columns, matrix);
    check_jordan(rows, columns, matrix);

    printf("\nPress any key to exit...");
    getchar();
    getch();

    return 0;
}


void add(int columns, int target_r, int source_r, float factor, float matrix[][columns]) 
{
    for(int j = 0; j < columns; j++)
    {
        matrix[target_r][j] = matrix[target_r][j] + (matrix[source_r][j] * factor);
    }

    printf("\nR%i + (%.2f)R%i\n", target_r + 1, factor, source_r + 1);

}

void multi(int columns, int row, float factor, float matrix[][columns]) 
{
    for(int j = 0; j < columns; j++) 
    {
        matrix[row][j] = matrix[row][j] * factor;
    }

    printf("\n(%.2f)R%i\n", factor, row + 1);
    
}

void swap(int columns, int row1, int row2, float matrix[][columns]) 
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

int check_gauss(int rows, int columns, float matrix[][columns]) 
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
                    swap(columns, x, i, matrix);
                    print_matrix(rows, columns, matrix);
                    break;
                }
            }
        }

        //check if diag is still 0
        if(matrix[x][diag_col] == 0) 
        {
            return 2;
        }

        //turn pivot in this row into 1
        if(matrix[x][diag_col] != 1)
        {
            //find inverse of diagonal, then  perform operation
            float factor = 1.0 / matrix[x][diag_col];
            multi(columns, x, factor, matrix);
            print_matrix(rows, columns, matrix);
        }

        //clear out column below pivot
        for(int i = x + 1; i < rows; i++)
        {
            if(matrix[i][diag_col] != 0 && matrix[x][diag_col] != 0)
            {
                //find the f in (t + (s * f)), then perform operation (s is always 1)
                float factor = -matrix[i][diag_col];
                add(columns, i, x, factor, matrix);
                print_matrix(rows, columns, matrix);
            }
        }

    diag_col++;
    }

    return 0;
}

int check_jordan(int rows, int columns, float matrix[][columns]) 
{
    for (int diag_col = 0; diag_col < columns - 1; diag_col++) 
    {
        // clear out column above pivot
        for (int i = 0; i < diag_col; i++) 
        {
            if (matrix[i][diag_col] != 0 && matrix[diag_col][diag_col] != 0) 
            {
                //find the f in (t + (s * f)), then perform operation (s is always 1)
                float factor = -matrix[i][diag_col];
                add(columns, i, diag_col, factor, matrix);
                print_matrix(rows, columns, matrix);
            }
        }
    }

    return 0;
}

void print_matrix(int rows, int columns, float matrix[][columns]) 
{
    printf("\n");

    for (int x = 0; x < rows; x++) 
    {
        for (int y = 0; y < columns; y++) 
        {
            if(matrix[x][y] == 0)
            {
                printf("0.00 ");

            } else {

                printf("%.2f ", matrix[x][y]);

            }

            if(y == columns - 2)
            {
                printf("| ");
            }

        }
        printf("\n");
    }
}