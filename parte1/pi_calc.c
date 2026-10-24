#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_TERMS 1000000000L // 1 bilhão de termos
#define ITERATIONS 5

double *results;

typedef struct
{
    int thread_id;
    long start_term;
    long end_term;
} ThreadArg;

// Função executada por cada thread (Cálculo de Pi)
void *calculate_pi(void *arg)
{
    ThreadArg *my_data = (ThreadArg *)arg;
    int id = my_data->thread_id;
    long start = my_data->start_term;
    long end = my_data->end_term;

    double local_sum = 0.0;

    for (long n = start; n < end; n++)
    {
        double term = 1.0 / (2.0 * n + 1.0);
        if (n % 2 != 0)
        {
            term = -term;
        }
        local_sum += term;
    }

    results[id] = local_sum;
    pthread_exit(NULL);
}

// Função que executa 1 rodada do cálculo e retorna o tempo gasto em segundos
double run_single_test(int num_threads)
{
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArg *thread_args = malloc(num_threads * sizeof(ThreadArg));
    results = malloc(num_threads * sizeof(double));

    long terms_per_thread = NUM_TERMS / num_threads;
    long remainder = NUM_TERMS % num_threads;
    long current_start = 0;

    // Marca o tempo de início
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Cria as threads
    for (int i = 0; i < num_threads; i++)
    {
        thread_args[i].thread_id = i;
        thread_args[i].start_term = current_start;
        long terms_for_this_thread = terms_per_thread + (i < remainder ? 1 : 0);
        thread_args[i].end_term = current_start + terms_for_this_thread;
        current_start = thread_args[i].end_term;

        pthread_create(&threads[i], NULL, calculate_pi, (void *)&thread_args[i]);
    }

    double final_pi_quarter = 0.0;
    // Aguarda as threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        final_pi_quarter += results[i];
    }

    // Marca o tempo de fim
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // double final_pi = final_pi_quarter * 4.0; // Opcional: imprimir o Pi para debug

    free(threads);
    free(thread_args);
    free(results);

    // Calcula o tempo decorrido em segundos
    double elapsed = (end_time.tv_sec - start_time.tv_sec);
    elapsed += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

    return elapsed;
}

int main()
{
    int thread_counts[] = {1, 2, 4, 8, 16, 32};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Resultados dos Testes de Desempenho (%.0e termos, %d iteracoes)\n", (double)NUM_TERMS, ITERATIONS);
    printf("--------------------------------------------------------------------------\n");
    printf("%-10s | %-12s | %-15s | %-10s | %-10s\n", "Threads", "Media (s)", "Desvio Padrao", "CV (%)", "Status CV");
    printf("--------------------------------------------------------------------------\n");

    for (int c = 0; c < num_configs; c++)
    {
        int t = thread_counts[c];
        double tempos[ITERATIONS];
        double sum = 0.0;

        // Executa as 5 iterações para a quantidade de threads atual
        for (int i = 0; i < ITERATIONS; i++)
        {
            tempos[i] = run_single_test(t);
            sum += tempos[i];
        }

        // Cálculos estatísticos
        double mean = sum / ITERATIONS;

        double sq_sum = 0.0;
        for (int i = 0; i < ITERATIONS; i++)
        {
            sq_sum += pow(tempos[i] - mean, 2);
        }

        double variance = sq_sum / ITERATIONS;
        double stddev = sqrt(variance);
        double cv = (mean > 0) ? (stddev / mean) * 100.0 : 0.0;

        // Formata a linha da tabela
        printf("%-10d | %-12.4f | %-15.4f | %-10.2f | %-10s\n",
               t, mean, stddev, cv, (cv <= 5.0) ? "OK" : "REFAZER");
    }
    printf("--------------------------------------------------------------------------\n");

    return 0;
}