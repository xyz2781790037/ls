#define _GNU_SOURCE /* 使用versionsort需要加上该宏定义 */
#define _DEFAULT_SOURCE
#define MAX_FILENAME_LENGTH 20
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>

char mylsmk[8];
void mylsbase(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, versionsort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        if (strcmp(fina[i]->d_name, "..") != 0 && strncmp(fina[i]->d_name, ".", 1))
        {
            printf("%s  ", fina[i]->d_name);
            if ((i + 1) % 4 == 0)
            {
                printf("\n");
            }
        }
        free(fina[i]);
    }
    free(fina);
}
void myls_a(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, versionsort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s  ", fina[i]->d_name);
        free(fina[i]);
    }

    free(fina);
}
void myls_l()
{
    struct stat st;
    printf((S_ISDIR(st.st_mode)) ? "d" : "-");
    printf((st.st_mode & S_IRUSR) ? "r" : "-");
    printf((st.st_mode & S_IWUSR) ? "w" : "-");
    printf((st.st_mode & S_IXUSR) ? "x" : "-");
    printf((st.st_mode & S_IRGRP) ? "r" : "-");
    printf((st.st_mode & S_IWGRP) ? "w" : "-");
    printf((st.st_mode & S_IXGRP) ? "x" : "-");
    printf((st.st_mode & S_IROTH) ? "r" : "-");
    printf((st.st_mode & S_IWOTH) ? "w" : "-");
    printf((st.st_mode & S_IXOTH) ? "x" : "-");

    printf("%d", st.st_size);
}
int main(int argc, char *argv[])
{
    int mark_ls;
    // if (argc > 1 && argv[1][0] == '-')
    // {
    //     for (int i = 1; argv[1][i] != '\0'; i++)
    //     {
    //         argv[1][i] = mylsmk[i - 1];
    //         mark_ls = i;
    //     }
    // }
    // printf("%d", argc);
    // printf("%c\n", argv[1][0]);
    if (argc > 1)
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            mark_ls = 1;
        }
    }
    struct dirent **fina; // filename
    if (argc == 1)
    {
        mylsbase(fina);
    }
    else
    {
        for (int i = 0; i < mark_ls; i++)
        {
            if (mark_ls)
            {
                myls_a(fina);
            }
        }
    }
    return 0;
}