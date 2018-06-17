#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

double total = 0, sq1 = 0, sq2 = 0, x = 0;

void *Child_One(void *vargp)
{
    int *n = (int *)vargp;
    sq1 = (double)*n / 3 + 1;
    sq2 = (double)2 * *n / 3;
    x = sq1;

    do
    {
        total += sqrt(x);
        x++;
    } while (x <= sq2);
}

void *Child_Two(void *vargp)
{
    int *n = (int *)vargp;

    sq1 = (double)2 * *n / 3 + 1;
    sq2 = (double)*n;
    x = sq1;
    do
    {
        total += sqrt(x);
        x++;
    } while (x <= sq2);
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    if (argc > 1)
    {
        int n = atoi(argv[1]);
        double mutipleOf_3 = (double)n / 3;

        if (mutipleOf_3 == n / 3)
        {
            pthread_create(&tid, NULL, Child_One, &n);
            pthread_join(tid, NULL);

            pthread_create(&tid, NULL, Child_Two, &n);
            pthread_join(tid, NULL);

            sq1 = (double)1;
            sq2 = (double)n / 3;
            x = sq1;
            do
            {
                total += sqrt(x);
                x++;
            } while (x <= sq2);

            printf("Sum of square roots: %f\n", total);
        }
        else
        {
            printf("Input is not a multiple of 3\n");
        }
    }
    else
    {
        printf("No argument found...\n");
    }

    return (0);
}