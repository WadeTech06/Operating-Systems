#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    double total = 0, sq1 = 0, sq2 = 0, x = 0;
    int fd1[2], fd2[2];

    // create pipe descriptors
    pipe(fd1);
    pipe(fd2);

    if (argc > 1)
    {
        int n = atoi(argv[1]);
        double mutipleOf_3 = (double)n / 3;

        if (mutipleOf_3 == n / 3)
        {
            pid_t firstChild, secondChild;
            firstChild = fork();
            if (firstChild > 0)
            {

                //parent
                waitpid(-1, NULL, 0);

                secondChild = fork();
                if (secondChild > 0)
                {
                    //parent
                    waitpid(-1, NULL, 0);

                    close(fd2[1]);
                    read(fd2[0], &total, sizeof(total));
                    close(fd2[0]);

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
                else if (secondChild < 0)
                {
                    perror("fork");
                    exit(1);
                }
                else
                {
                    //secondChild

                    close(fd1[1]);
                    close(fd2[0]);

                    read(fd1[0], &total, sizeof(total));
                    close(fd1[0]);

                    sq1 = (double)2 * n / 3 + 1;
                    sq2 = (double)n;
                    x = sq1;
                    do
                    {
                        total += sqrt(x);
                        x++;
                    } while (x <= sq2);

                    write(fd2[1], &total, sizeof(total));
                }
            }
            else if (firstChild < 0)
            {
                perror("fork");
                exit(1);
            }
            else
            {
                //firstChild

                sq1 = (double)n / 3 + 1;
                sq2 = (double)2 * n / 3;
                x = sq1;

                do
                {
                    total += sqrt(x);
                    x++;
                } while (x <= sq2);

                close(fd1[0]);
                write(fd1[1], &total, sizeof(total));
                close(fd1[1]);
            }
        }
        else
        {
            printf("input is not a multiple of 3\n");
        }
    }
    else
    {
        printf("No argument found...\n");
    }
}