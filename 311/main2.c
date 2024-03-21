#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>
#define N 100
#define BIG_N 13000
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

int determinant(int matrix[K][K], int k, int* rows, int depth) {
    int s = 0;
    if (k - depth == 1) {
        int i = 0;
        for (; rows[i] == 0; i++);
        return matrix[depth][i];
    }
    for (int i = 0, real = 0; i < k; i++) {
        if (rows[i] == 0) {
            continue;
        }
        rows[i] = 0;
        if (real % 2 == 0) {
            s += matrix[depth][i] * determinant(matrix, k, rows, depth + 1);
        } else {
            s += -matrix[depth][i] * determinant(matrix, k, rows, depth + 1);
        }
        real++;
        rows[i] = 1;
    }
    return s;
}

void quickSort(int* arr, int* dets, int* sizes, int first, int last, int is_matrix) {
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
            swap(dets + i, dets + j);
            if (is_matrix) {
                int m = max(sizes[i], sizes[j]);
                for (int k = 0; k < m; k++) {
                    for (int l = 0; l < m; l++) {
                        swap(arr + K*K*i + k*K + l,
                             arr + K*K*j + k*K + l);
                    }
                }
                swap(sizes + i, sizes + j);
            }
        }
    }
    if (c != j) {
        swap(dets + c, dets + j);
        if (is_matrix) {
            int m = max(sizes[c], sizes[j]);
            for (int k = 0; k < m; k++) {
                for (int l = 0; l < m; l++) {
                    swap(arr + K*K*c + k*K + l,
                         arr + K*K*j + k*K + l);
                }
            }
            swap(sizes + c, sizes + j);
        }
    }
    quickSort(arr, dets, sizes, first, j - 1, is_matrix);
    quickSort(arr, dets, sizes, j + 1, last, is_matrix);
}

void bubbleSort(int* arr, int n, int* dets, int* sizes, int is_matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (dets[j] > dets[j+1]) {
                swap(dets+j+1, dets+j);
                if (is_matrix) {
                    int m = max(sizes[j + 1], sizes[j]);
                    for (int k = 0; k < m; k++) {
                        for (int l = 0; l < m; l++) {
                            swap(arr + K * K * (j + 1) + k * K + l,
                                 arr + K * K * j + k * K + l);
                        }
                    }
                    swap(sizes+j+1, sizes+j);
                }
            }
        }
    }
}

void check_alg();

int main() {
    int choice0;
    printf("1.Test\n2.Normal\n>");
    scanf("%d", &choice0);
    if (choice0 == 1) {
        check_alg();
        return 0;
    }

    int matrix[N][K][K];
    int sizes[N];
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int k;
        scanf("%d", &k);
        sizes[i] = k;
        for (int p = 0; p < k; p++) {
            for (int l = 0; l < k; l++){
                scanf("%d", &matrix[i][p][l]);
            }
        }
    }
    int detMatr[N];
    for (int i = 0; i < n; i++) {
        int rows[K];
        for (int j = 0; j < sizes[i]; j++) {
            rows[j] = 1;
        }
        detMatr[i] = determinant(matrix[i], sizes[i], rows, 0);
    }
    quickSort((int *) matrix, detMatr, sizes, 0, n - 1, 1);
    for (int i = 0; i < n; i++) {
        int k = sizes[i];
        for (int p = 0; p < k; p++) {
            for (int l = 0; l < k; l++) {
                printf("%d ", matrix[i][p][l]);
            }
            printf("\n");
        }
    }
    return 0;
}

void check_alg() {
    int dets[BIG_N];
    int dets_add[BIG_N];
    srand(time(NULL));
    for (int i = 0; i < BIG_N; i++) {
        dets[i] = dets_add[i] = rand() % BIG_N;
    }
    double times[N];
    double mean_time = 0;
    double min_time = DBL_MAX;
    double max_time = -1;
    for (int i = 0; i < N; i++) {
        clock_t begin = clock();
        quickSort(NULL, dets_add, NULL, 0, BIG_N - 1, 0);
        mean_time += times[i] = ((double)(clock() - begin)) / CLOCKS_PER_SEC;
        if (times[i] < min_time) {
            min_time = times[i];
        }
        if (times[i] > max_time) {
            max_time = times[i];
        }
        for (int j = 0; j < BIG_N; j++) {
            dets_add[j] = dets[j];
        }
    }
    mean_time /= N;
    double std_dvt = 0;
    for (int i = 0; i < N; i++) {
        std_dvt += (times[i] - mean_time)*(times[i] - mean_time);
    }
    std_dvt = sqrt(std_dvt / N);
    printf("quick sort\nmean time = %e\nstandard deviation = %e\n", mean_time, std_dvt);
    printf("max time = %e\nmin time = %e\n", max_time, min_time);

    mean_time = 0;
    std_dvt = 0;
    min_time = DBL_MAX;
    max_time = -1;
    for (int i = 0; i < 20; i++) {
        clock_t begin = clock();
        bubbleSort(NULL, BIG_N, dets_add, NULL, 0);
        mean_time += times[i] = ((double)(clock() - begin)) / CLOCKS_PER_SEC;
        if (times[i] < min_time) {
            min_time = times[i];
        }
        if (times[i] > max_time) {
            max_time = times[i];
        }
        printf(".");
        if (i % 10 == 0 && i > 0) {
            printf("\n");
        }
        for (int j = 0; j < BIG_N; j++) {
            dets_add[j] = dets[j];
        }
    }
    mean_time /= 20;
    for (int i = 0; i < 20; i++) {
        std_dvt += (times[i] - mean_time)*(times[i] - mean_time);
    }
    std_dvt = sqrt(std_dvt / 20);
    printf("\nbubble sort mean time = %e\nstandard deviation = %e\n", mean_time, std_dvt);
    printf("max time = %e\nmin time = %e", max_time, min_time);
}