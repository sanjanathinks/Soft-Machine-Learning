#include <stdio.h>
#include <stdlib.h>

//*** PRINT MATRIX OUT
void printMatrix(double** matrix, int row, int col) {
    int i, j;
    
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%.5f, ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


//*** MEMORY
void FreeMatrix(double** matrix, int row, int col) {
    int i;
    for (i = 0; i < row; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

//*** TRANSPOSE MATRIX
double** transposeMatrix(double **givenMatrix, double **TransposedM, int rows, int columns) {
    int i;
    int j;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            TransposedM[i][j] = givenMatrix[j][i];
        }
    }
    return TransposedM;
}

//**** MULTIPLY MATRIX
double** MultiplyMatrix(double** A, int rowA, int colA, double** B, int rowB, int colB, double** result) {
    int i, j, k;
    int rowC = rowA;
    int colC = colB;
    for (i = 0; i < rowC; i++) {
        for (j = 0; j < colC; j++) {
            // Multiply a row i of A with col j of B
            for (k = 0; k < colA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

//**** CREATE IDENTITY MATRIX
double** CreateIdentityMatrix(int row, int col) {
    double** matrix = (double**)malloc(sizeof(double*) * row);
    int i, j;
    for (i = 0; i < row; i++) {
        matrix[i] = (double*)malloc(sizeof(double) * col);
    }
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (i == j) {
                matrix[i][j] = 1;
            }
            else {
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}

//**** GAUSS JORDAN ALGO
double** gaussJordan (double** augmentMatrix, int attributes) {
    double c;
    for (int i = 0; i < attributes; i++) { // here we are iterating through attributes
        c = augmentMatrix[i][i]; // c = diagonal elements
        // printf("%f\n", c);
        for (int j = 0; j < 2*attributes; j++) { // traversing through the augmented matrix
            augmentMatrix[i][j] /= c; // dividing the elements of that row by the pivot element
            // printf("%f\n", augmentMatrix[i][j]); // reducing pivot to 1 by dividing row with diagonal element
        }
        // printMatrix(augmentMatrix, attributes, 2*attributes);

        for (int j = 0; j < attributes; j++) { // going through attributes
            if (i != j) { // when we are not at diagonal
                c = augmentMatrix[j][i]; // setting c to equal all other attributes that are not diagonal
                for (int k = 0; k < 2*attributes; k++) { // traversing through augmented matrix
                    augmentMatrix[j][k] -= c*augmentMatrix[i][k]; 
                    // multiplying from the original matrix to augmented matrix using pivot element
                }
            }
        }
    }
    // printMatrix(augmentMatrix, attributes, 2*attributes);
    return augmentMatrix;
}

//**** SAVING GAUSS JORDAN
double** saveGJM(double** augmentedMatrix, int att, double** savedMatrix) {
    for (int i = 0; i < att; i++) {
        for (int j = att; j < 2*att; j++) {
            savedMatrix[i][j-att] = augmentedMatrix[i][j];
            // printMatrix(savedMatrix, att, att);
        }
    }
    return savedMatrix;
}

//**** AUGMENTED MATRIX
double** augmentMatrix(double** matrix, int dim) {
  double** identityMatrix = CreateIdentityMatrix(dim, dim);

  double** augmentedMatrix = (double**)malloc(sizeof(double*) * dim);
    for (int row = 0; row < dim; row++) {
        augmentedMatrix[row] = (double*)malloc(sizeof(double) * dim * 2);
    } 

  for(int row = 0; row < dim; row++) {
    for(int col = 0; col < dim; col++) {
      augmentedMatrix[row][col] = matrix[row][col];
    } 
    for(int col = dim; col < 2 * dim; col++) {
      augmentedMatrix[row][col] = identityMatrix[row][col - dim];
      }  
  }
  // printMatrix(augmentedMatrix, dim, dim * 2);
  return augmentedMatrix;
}

//******* MAIN
    int main(int argc, char** argv) {
    int attributes, trainingdata, numhouses;

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        return 0;
    }
    fscanf(fp, "%i\n", &attributes);
    // printf("%i\n", attributes);
    fscanf(fp, "%i\n", &trainingdata);
    // printf("%i\n", trainingdata);
    
    double** matrixX = (double**)malloc(sizeof(double*) * trainingdata);
    for (int i = 0; i < trainingdata; i++) {
        matrixX[i] = malloc((attributes+1)*sizeof(double));
    }
    
    double** matrixY = (double**)malloc((trainingdata) * sizeof(double));
    for (int i = 0; i < trainingdata; i++) {
        matrixY[i] = malloc(1 * sizeof(double));
    }

    int reader; 
    for (int i = 0; i < trainingdata; i++) {
        reader = fscanf(fp, " %lf, ", matrixY[i]);
        matrixX[i][0] = 1;
        for (int j = 1; j < attributes+1; j++) {
        reader = fscanf(fp, " %lf, ", &matrixX[i][j]);
        }
    }

    // printMatrix(matrixX, trainingdata, attributes+1); 
    // printMatrix(matrixY, trainingdata, 1);

    
    double** matrixWeights = (double**)malloc((attributes+1)*sizeof(double));
    for (int i = 0; i < attributes+1; i++) {
        matrixWeights[i] = malloc(1 * sizeof(double));
    }
    

   double** matrixXTransposed = (double**)malloc(sizeof(double*) * (attributes+1));
    for (int i = 0; i < (attributes+1); i++) {
        matrixXTransposed[i] = malloc((trainingdata)*sizeof(double));
    }

    double** MatrixXTProduct = (double**)malloc(sizeof(double*) * (trainingdata));
    for (int i = 0; i < (trainingdata); i++) {
        MatrixXTProduct[i] = malloc((trainingdata)*sizeof(double));
    }

    matrixXTransposed = transposeMatrix(matrixX, matrixXTransposed, (attributes+1), trainingdata);
    //printMatrix(matrixXTransposed, attributes+1, trainingdata); 

    MatrixXTProduct = MultiplyMatrix(matrixXTransposed, (attributes+1), (trainingdata), matrixX, (trainingdata), 
    (attributes+1), MatrixXTProduct);
    // printMatrix(MatrixXTProduct, (attributes+1), (attributes+1));
    int a1 = attributes+1;
    
    double** AugmentedMatrixXTP = augmentMatrix(MatrixXTProduct, (a1));

    double** InverseXTP = (double**)malloc(sizeof(double*) * (a1));
    for (int i = 0; i < (a1); i++) {
        InverseXTP[i] = malloc((a1)*sizeof(double));
    }
    InverseXTP = gaussJordan(AugmentedMatrixXTP, a1);
    // printMatrix(InverseXTP, a1, 2*a1);

    double** saveGJ = (double**)malloc(sizeof(double*) * (a1));
    for (int i = 0; i < (a1); i++) {
        saveGJ[i] = malloc((a1)*sizeof(double));
    }

    saveGJ = saveGJM(InverseXTP, attributes+1, saveGJ);
    // printMatrix(saveGJ, a1, a1);

    double** intermediateXTXT = (double**)malloc(sizeof(double*) * (a1));
    for (int i = 0; i < (a1); i++) {
        intermediateXTXT[i] = malloc((trainingdata)*sizeof(double));
    }

    intermediateXTXT = MultiplyMatrix(saveGJ, a1, a1, matrixXTransposed, a1, trainingdata, 
    intermediateXTXT);
    // printMatrix(intermediateXTXT, a1, trainingdata);

    matrixWeights = MultiplyMatrix(intermediateXTXT, a1, trainingdata, matrixY, trainingdata, 1, matrixWeights);
    // printMatrix(matrixWeights, a1, 1);

    fclose(fp);
    
    fp = fopen(argv[2], "r");
    fscanf(fp, "%i\n", &numhouses);
    // printf("%i\n", numhouses);
    double finalvals;
    double store;
    // int counter;
    float storer = 0;

    while (!feof(fp)) {
        for (int i  = 0; i < numhouses; i++) {
            finalvals = matrixWeights[0][0];
            for (int j = 1; j < a1; j++) {
                storer = fscanf(fp, "%lf,", &store);
                finalvals += store*(matrixWeights[j][0]);
                // counter++;
                // printf("%d\n", counter);
                // printf("%0.0f\n", finalvals);
            }
            if (storer == 1) {
                printf("%0.0f\n", finalvals);
            }
        }
    }
    fclose(fp);

    FreeMatrix(matrixX, trainingdata, attributes);
    FreeMatrix(matrixY, trainingdata, 1);
    FreeMatrix(matrixWeights, a1, 1);
    FreeMatrix(matrixXTransposed, a1, a1);
    FreeMatrix(MatrixXTProduct, trainingdata, trainingdata);
    FreeMatrix(InverseXTP, a1, a1);
    FreeMatrix(saveGJ, a1, a1);
    FreeMatrix(intermediateXTXT, a1, trainingdata);
}
