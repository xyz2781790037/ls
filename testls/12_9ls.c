#define _DEFAULT_SOURCE
#define S_ISREG
#define link_num_width 2
#define size_width 5
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
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
int judge_file_or_directory(const char *c)
{
    struct stat st;
    if (stat(c, &st) == -1)
    {
        perror("文件或目录不存在或无法访问");
        exit(1);
    }
    if (S_ISREG(st.st_mode))
    {
        return 1;
    }
    return 0;
}
int filesort_a(const struct dirent **a, const struct dirent **b)
{
    char aw[256], bw[256];
    if ((*a)->d_name[0] == '.' && strcmp((*a)->d_name, ".") != 0 && strcmp((*a)->d_name, "..") != 0)
    {
        strncpy(aw, (*a)->d_name + 1, strlen((*a)->d_name) - 1);
        aw[strlen((*a)->d_name) - 1] = '\0';
    }
    else
    {
        strncpy(aw, (*a)->d_name, strlen((*a)->d_name));
        aw[strlen((*a)->d_name)] = '\0';
    }
    if ((*b)->d_name[0] == '.' && strcmp((*b)->d_name, ".") != 0 && strcmp((*b)->d_name, "..") != 0)
    {
        strncpy(bw, (*b)->d_name + 1, strlen((*b)->d_name) - 1);
        bw[strlen((*b)->d_name) - 1] = '\0';
    }
    else
    {
        strncpy(bw, (*b)->d_name, strlen((*b)->d_name));
        bw[strlen((*b)->d_name)] = '\0';
    }
    return strcasecmp(aw, bw);
}
int filesort(const struct dirent **a, const struct dirent **b)
{
    return strcasecmp((*a)->d_name, (*b)->d_name);
}
void mylsbase(struct dirent **fina, const char *c)
{
    int n;
    if (judge_file_or_directory(c))
    {
        printf("%s", c);
    }
    else
    {
        n = scandir(c, &fina, NULL, filesort);
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
}
void myls_a(struct dirent **fina, const char *c)
{
    int n;
    if (judge_file_or_directory(c))
    {
        printf("%s", c);
    }
    else
    {
        n = scandir(c, &fina, NULL, filesort_a);
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
}
void myls_s(struct dirent **file) {}
void myls_l(struct dirent **file, const char *c)
{
    struct stat st;
    int n;
    n = scandir(c, &file, NULL, filesort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    // printf("总计 %ld\n", st.st_blocks);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(file[i]->d_name, "..") != 0 && strncmp(file[i]->d_name, ".", 1))
        {

            if (stat(file[i]->d_name, &st) != -1)
            {
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

                printf(" %*lu", link_num_width, st.st_mtime);
                printf(" %s", getpwuid(st.st_uid)->pw_name);
                printf(" %s", getgrgid(st.st_gid)->gr_name);
                printf(" %*lu", size_width, st.st_size);

                char time_str[20];
                struct tm *time = localtime(&st.st_mtime);
                strftime(time_str, sizeof(time_str), "%m月 %d %H:%M", time);
                printf(" %s", time_str);
                printf(" %s\n", file[i]->d_name);
            }
            free(file[i]);
        }
    }
    free(file);
}
void myls_t(struct dirent **file) {}
void myls_r(struct dirent **file) {}
void myls_i(struct dirent **file) {}
int main(int argc, char *argv[])
{
    int c, a_flag = 0, l_flag = 0, t_flag = 0, r_flag = 0, i_flag = 0, s_flag = 0;
    char s[10];
    struct dirent **file;
    if (argc == 1)
    {
        mylsbase(file, ".");
    }
    else if (argc == 2 && argv[1][0] != '-')
    {
        mylsbase(file, argv[1]);
    }
    else
    {
        while ((c = getopt(argc, argv, "a::l::t::r::i::s::")) != -1)
        {
            switch (c)
            {
            case 'a':
                a_flag = 1;
                break;
            case 'l':
                l_flag = 1;
                break;
            case 't':
                t_flag = 1;
                myls_t(file);
                break;
            case 'r':
                r_flag = 1;
                myls_r(file);
                break;
            case 'i':
                i_flag = 1;
                myls_i(file);
                break;
            case 's':
                s_flag = 1;
                myls_s(file);
                break;
            }
        }
        if (l_flag)
        {
            if (argc == 3 && a_flag == 0)
            {
                myls_l(file, argv[2]);
            }
            else
            {
                myls_l(file, ".");
            }
        }
        if (a_flag)
        {
            if (argc == 3 && l_flag == 0)
            {
                myls_a(file, argv[2]);
            }
            else
            {
                myls_a(file, ".");
            }
        }
        if (t_flag)
        {
            myls_t(file);
        }

        if (r_flag)
        {
            myls_r(file);
        }

        if (i_flag)
        {
            myls_i(file);
        }

        if (s_flag)
        {
            myls_s(file);
        }
    }
    return 0;
}