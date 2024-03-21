#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>

#define N 100
#define BIG_N 4000
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

void quickSort(int*** matrices, int *dets, int *sizes, int first, int last, int is_matrix) 
{
    if (first >= last) {
        return;
    }
    srand(time(NULL));
    int i, j, c;
    c = first + rand()%(last - first);
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
            if (is_matrix) {
                int m = max(sizes[i], sizes[j]);
                for (int k = 0; k < m; k++) {
                    for (int l = 0; l < m; l++) {
                        swap(&matrices[i][k][l], &matrices[j][k][l]);
                    }
                }
                swap(&sizes[i], &sizes[j]);
            }
        }
    }
    if (c != j) {
        swap(&dets[c], &dets[j]);
        if (is_matrix) {
            int m = max(sizes[c], sizes[j]);
            for (int k = 0; k < m; k++) {
                for (int l = 0; l < m; l++) {
                    swap(&matrices[c][k][l], &matrices[j][k][l]);
                }
            }
            swap(&sizes[c], &sizes[j]);
        }
    }
    quickSort(matrices, dets, sizes, first, j - 1, is_matrix);
    quickSort(matrices, dets, sizes, j + 1, last, is_matrix);
}

void bubbleSort(int*** arr, int *dets, int *sizes, int _none_, int n, int is_matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (dets[j] > dets[j+1]) {
                swap(dets+j+1, dets+j);
                if (is_matrix) {
                    int m = max(sizes[j + 1], sizes[j]);
                    for (int k = 0; k < m; k++) {
                        for (int l = 0; l < m; l++) {
                            swap(&arr[j+1][k][l], &arr[j][k][l]);
                        }
                    }
                    swap(&sizes[j+1], &sizes[j]);
                }
            }
        }
    }
}

void algorithm_testing(void (*algorithm)(int ***, int*, int*, int, int, int)) {
    int dets[BIG_N];
    int dets_add[BIG_N];
    srand(time(NULL));
    for (int i = 0; i < BIG_N; i++) {
        dets[i] = dets_add[i] = rand() % BIG_N;
    }
    double times[N];
    double mean_times = 0;
    double min_times = DBL_MAX;
    double max_times = -1;
    for (int j = 0; j < N; j++) {
        clock_t begin = clock();
        algorithm(NULL, dets_add, NULL, 0, BIG_N - 1, 0);
        mean_times += times[j] = ((double) (clock() - begin)) / CLOCKS_PER_SEC;
        if (times[j] < min_times) {
            min_times = times[j];
        }
        if (times[j] > max_times) {
            max_times = times[j];
        }
        for (int k = 0; k < BIG_N; k++) {
            dets_add[k] = dets[k];
        }
    }
    mean_times /= N;
    double std_dvt = 0;
    for (int j = 0; j < N; j++) {
        std_dvt += (times[j] - mean_times) * (times[j] - mean_times);
    }
    std_dvt = sqrt(std_dvt / N);
    printf("mean time = %f\nstandard deviation = %f\n", mean_times, std_dvt);
    printf("max time = %f\nmin time = %f\n", max_times, min_times);
}

int main() {
    int choice0;
    printf("1.Test\n2.Normal\n>");
    scanf("%d", &choice0);
    if (choice0 == 1) {
        printf("Quick sort:\n");
        algorithm_testing(quickSort);
        printf("Bubble sort:\n");
        algorithm_testing(bubbleSort);
        return 0;
    }

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
    quickSort(matrices, determinants, sizes, 0, n - 1, 1);
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

