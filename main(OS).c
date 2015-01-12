//
//  main.c
//  OS_2
//
//  Created by Nikita Makarov on 15.01.14.
//  Copyright (c) 2014 Nikita Makarov. All rights reserved.
//
//  Окно фильтра имеет размер 3x3.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct
{
    int lines;
    int columns;
    int position;
    long ** matrix;
    pthread_mutex_t MTX;
} matrix_attr;

long SortAndReturnMedian(long * array, int size);

long ** Read(int m, int n);

void Print(long ** matrix, int m, int n);

void CalculateOneLine(matrix_attr * M);

int main()
{
    srand((unsigned int)time(NULL));
    
    matrix_attr M;
    printf("Enter matrix size (minimum is 3x3): ");
    scanf("%d %d", &M.lines , &M.columns);
    
    if (M.lines < 3 || M.columns < 3)
    {
        printf("Wrong matrix size, try again: ");
        scanf("%d %d", &M.lines , &M.columns);
        if (M.lines < 3 || M.columns < 3)
        {
            printf("OMG..\n");
            return 0;
        }
    }
    
    printf("Enter matrix\n");
    M.matrix = Read(M.lines, M.columns);
    M.position = 0;
    
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    M.MTX = mut;
   
    Print(M.matrix, M.lines, M.columns);
    printf("\n");
    
    int thread_max = 0;
    printf("Enter number of max threads: ");
    scanf("%d", &thread_max);
    if (thread_max <= 0)
        return 0;
    
    int MAX_THREADS_NEEDED = M.lines / 2;
    
    int k;
    printf("Enter number of filtrations: ");
    scanf("%d", &k);
    if (k <= 0)
    {
        printf("Wrong k, try again: ");
        scanf("%d", &k);
        if (k <= 0)
        {
            printf("OMG..\n");
            return 0;
        }
    }
    
    pthread_t ThreadStorage[thread_max];
    void * xxx = NULL;
    
    for (int i = 0; i < k; i++)
    {
        if (thread_max >= MAX_THREADS_NEEDED)
        {
            int MagicVariable = 0;
            for (int j = 0; j < MAX_THREADS_NEEDED; j++)
            {
                M.position = MagicVariable * 2;
                int Median = pthread_create(&ThreadStorage[j], NULL, (void*)CalculateOneLine, &M);
                //printf("Thread created with big threads\n");
                if (Median != 0)
                    return 1;
                MagicVariable++;
            }
            for (int j = 0; j < MAX_THREADS_NEEDED; j++)
            {
                pthread_join(ThreadStorage[j], xxx);
            }
        }
        else
        {
            int ThreadsRemained = MAX_THREADS_NEEDED;
            int MagicVariable = 0;
            while (ThreadsRemained > 0) {
                for (int j = 0; j < thread_max; j++)
                {
                    M.position = MagicVariable * 2;
                    int Median = pthread_create(&ThreadStorage[j], NULL, (void*)CalculateOneLine, &M);
                    //printf("Thread created with small threads\n");
                    if (Median != 0)
                        return 1;
                    MagicVariable++;
                }
                for (int j = 0; j < thread_max; j++)
                {
                    pthread_join(ThreadStorage[j], xxx);
                }
                ThreadsRemained -= thread_max;
            }
        }
    }
    
    Print(M.matrix, M.lines, M.columns);
    printf("\n");
    
    return 0;
}

void CalculateOneLine(matrix_attr * M)
{
    pthread_mutex_lock(&M->MTX);
    //printf("Mutex locked\n");
    int pos = M->position;
    //printf("Position viewed\n");
    pthread_mutex_unlock(&M->MTX);
    //printf("Mutex unlocked\n");

    int limit_m = M->lines - 2;
    int limit_n = M->columns - 2;
    
    for (int i = pos; i < limit_m && i < pos + 3; i++)
    {
        for (int j = 0; j < limit_n; j++)
        {
            //printf("Making filtration\n");
            int k = 0;
            long array[9];
            for (int x = 0; x < 3; x++)
                for (int y = 0; y < 3; y++)
                    array[k++] = M->matrix[i + x][j + y];
            M->matrix[i + 1][j + 1] = SortAndReturnMedian(array, 9);
        }
    }
}
long SortAndReturnMedian(long * array, int size)
{
    for(int i = 0; i < size; i++)
        for(int j = size - 1; j > i; j--)
            if (array[j - 1] > array[j])
            {
                array[j - 1] = array[j - 1] ^ array[j];
                array[j] = array[j - 1] ^ array[j];
                array[j - 1] = array[j] ^ array[j - 1];
                
            }
    return size == 9 ? array[4] : array[1];
}

long ** Read(int m, int n)
{
    long ** matrix = (long**)malloc(sizeof(long*) * m);
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (long*)malloc(sizeof(long) * n);
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //scanf("%ld ", &matrix[i][j]);
            matrix[i][j] = rand() % 2;
        }
    }
    return matrix;
}

void Print(long ** matrix, int m, int n)
{
    //char t;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //t = matrix[i][j];
            printf("%ld\t", matrix[i][j]);
        }
        printf("\n");
    }
}