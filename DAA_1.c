#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
void delay(int ms) { 
    usleep(ms * 1000); 
}

void clear_screen() { 
    system("clear"); 
}

void print_table(int** dp, int n, int W, int* weights, int current_item) {
    clear_screen();
    printf("--- Processing Item %d (Weight: %d) ---\n", current_item, current_item > 0 ? weights[current_item - 1] : 0);
    
    printf("Cap:   |");
    for (int w = 0; w <= W; w++) {
        printf("%4d", w);
    }
    printf("\n");
    for (int w = 0; w <= W + 2; w++) printf("----");
    printf("\n");
    
    // Print rows one by one
    for (int i = 0; i <= n; i++) {
        printf("Item %d: |", i);
        for (int w = 0; w <= W; w++) {
            printf("%4d", dp[i][w]);
        }
        if (i == current_item) {
            printf(" <-- Active Row");
        }
        printf("\n");
    }
    printf("\n========================================\n\n");
    delay(300); 
}

int solve_knapsack_live(int n, int W, int* weights, int* values) {
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((W + 1) * sizeof(int));
        for (int w = 0; w <= W; w++) dp[i][w] = 0;
    }
    print_table(dp, n, W, weights, 0);

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (weights[i - 1] <= w) {
                int inclusion = values[i - 1] + dp[i - 1][w - weights[i - 1]];
                int exclusion = dp[i - 1][w];
                dp[i][w] = (inclusion > exclusion) ? inclusion : exclusion;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    
        print_table(dp, n, W, weights, i);
    }

    int final_result = dp[n][W];

    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);

    return final_result;
}

int main() 
{
    FILE* outfile = fopen("result.txt", "a");
    if (!outfile) {
        printf("Error: Could not create or open 'result.txt'\n");
        return 1;
    }

    int n, W;
    printf("Enter number of items (n) and capacity (W): ");
    if (scanf("%d %d", &n, &W) != 2) {
        fclose(outfile);
        return 0;
    }

    int* weights = (int*)malloc(n * sizeof(int));
    int* values = (int*)malloc(n * sizeof(int));

    printf("Enter weights and values for %d items:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &weights[i], &values[i]);
    }

    int max_value = solve_knapsack_live(n, W, weights, values);

    printf("Final Maximum Value: %d\n", max_value);

    fprintf(outfile, "#Test case input (%d %d)  %d\n", n, W, max_value);

    free(weights);
    free(values);
    fclose(outfile);

    printf("\nResult saved to 'result.txt' successfully!\n");
    return 0;
}
