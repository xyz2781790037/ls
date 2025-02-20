// #define _GNU_SOURCE /* 使用versionsort需要加上该宏定义 */
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

char mylsmk[7];
int filesort(const struct dirent **a, const struct dirent **b)
{
    
    if ((*a)->d_type == (*b)->d_type)
    {
        return strcoll((*a)->d_name, (*b)->d_name);
    }
    return (*a)->d_type - (*b)->d_type;
}
void mylsbase(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, filesort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        if (strcmp(fina[i]->d_name, "..") != 0 && strncmp(fina[i]->d_name, ".", 1))
        {
            printf("%s\n", fina[i]->d_name);
        }
        free(fina[i]);
    }
    free(fina);
}
void myls_a(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, filesort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", fina[i]->d_name);
        free(fina[i]);
    }
    free(fina);
}
void myls_l()
{
    struct stat st;
    if (S_ISDIR(st.st_mode))
        printf("d");
    else if (S_ISLNK(st.st_mode))
        printf("l");
    else
        printf("-");
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