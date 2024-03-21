#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 100
#define K 64

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int determinant(int** matrix, int k, int* rows, int depth) {
    int s = 0;
    if (k - depth == 1) {
        int i = 0;
        for (; rows[i] == 0; i++);
        return matrix[depth][i];
    }
    for (int i = 0, sign = 1; i < k; i++) {
        if (rows[i] == 0) {
            continue;
        }
        rows[i] = 0;
        s += sign * matrix[depth][i] * determinant(matrix, k, rows, depth + 1);
        sign = -sign;
        rows[i] = 1;
    }
    return s;
}

void quickSort(int*** matrices, int *dets, int *sizes, int first, int last)
{
    if (first >= last) {
        return;
    }
    srand(time(NULL));
    int i, j, c;
    c = first + rand() % (last - first);
    i = first;
    j = last;
    while (i < j) {
        while (dets[i] <= dets[c] && i < last) {
            i++;
        }
        while (dets[j] > dets[c]) {
            j--;
        }
        if (i < j) {
            swap(&dets[i], &dets[j]);
            int m = max(sizes[i], sizes[j]);
            for (int k = 0; k < m; k++) {
                for (int l = 0; l < m; l++) {
                    swap(&matrices[i][k][l], &matrices[j][k][l]);
                }
            }
            swap(&sizes[i], &sizes[j]);
        }
    }
    if (c != j) {
        swap(&dets[c], &dets[j]);
        int m = max(sizes[c], sizes[j]);
        for (int k = 0; k < m; k++) {
            for (int l = 0; l < m; l++) {
                swap(&matrices[c][k][l], &matrices[j][k][l]);
            }
        }
        swap(&sizes[c], &sizes[j]);
    }
    quickSort(matrices, dets, sizes, first, j - 1);
    quickSort(matrices, dets, sizes, j + 1, last);
}

int main() {
    int*** matrices = (int***)malloc(N * sizeof(int**));

    for (int i = 0; i < N; i++) {
        matrices[i] = (int**)malloc(K * sizeof(int*));
        for (int j = 0; j < K; j++) {
            matrices[i][j] = (int*)malloc(K * sizeof(int));
        }
    }
    int sizes[N];
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int k;
        scanf("%d", &k);
        sizes[i] = k;
        for (int p = 0; p < k; p++) {
            for (int l = 0; l < k; l++) {
                scanf("%d", &matrices[i][p][l]);
            }
        }
    }
    int determinants[N];
    for (int i = 0; i < n; i++) {
        int rows[K];
        for (int j = 0; j < sizes[i]; j++) {
            rows[j] = 1;
        }
        determinants[i] = determinant(matrices[i], sizes[i], rows, 0);
    }
    quickSort(matrices, determinants, sizes, 0, n - 1);
    for (int i = 0; i < n; i++) {
        int k = sizes[i];
        for (int p = 0; p < k; p++) {
            for (int l = 0; l < k; l++) {
                printf("%d ", matrices[i][p][l]);
            }
            printf("\n");
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            free(matrices[i][j]);
        }
        free(matrices[i]);
    }
    free(matrices);
    return 0;
}

