#include <stdio.h>
#include <stdbool.h>
#define N 255

int main() {
    int n, m;
    scanf("%d %d", &m, &n);
    int a[N][N];
    int b[N][N];
    int saddle_count = 0;
    int top_sp = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)  {
            scanf("%d", &a[i][j]);
            b[i][j] = a[i][j];
        }
    }
    if (!n || !m) {
        printf("%d\n", 0);
        return 0;
    }

    int queue[N];
    unsigned char top = 0;
    for (int i = 0; i < n; i++) {
        int minimal = a[i][0];
        for (int j = 0; j < m; j++) {
            if (a[i][j] == minimal) {
                queue[top++] = j;
            } else if (a[i][j] < minimal) {
                queue[0] = j;
                top = 1;
                minimal = a[i][j];
            }
        }
        bool stop = false;
        for (int j = 0; j < top; j++) {
            for (int k = 0; k < n && !stop; k++) {
                if (a[k][queue[j]] > a[i][queue[j]]) {
                    stop = true;
                }
            }
            if (!stop) {
                saddle_count++;
                top_sp++;
                int new_saddle = 0;
                for (int p = 0; p < n; p++) {
                    new_saddle += a[p][queue[j]];
                }
                new_saddle = new_saddle / n;
                b[i][queue[j]] = new_saddle;
            }
        }
        top = 0;
    }

    printf("%d\n", saddle_count);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

    return 0;
}
