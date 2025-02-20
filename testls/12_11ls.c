#define _DEFAULT_SOURCE
#define S_ISREG
#define link_num_width 3
#define size_width 10
#define blocks_size_width 5
#define ino_width 7
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
#include <libgen.h>
const char *path = ".";
int c, n, m, a_flag = 0, l_flag = 0, t_flag = 0, r_flag = 0, i_flag = 0, s_flag = 0, R_flag = 0, flag = 0, argcv = 0, flagg = 0;
void coloer(char result[], const char *name)
{
    struct stat st;
    if (stat(result, &st) != -1)
    {
        if (S_ISDIR(st.st_mode))
        {
            printf("\033[1;34m"
                   "%s\n\033[0m",
                   name);
        }
        else if (S_ISLNK(st.st_mode))
        {
            printf("\033[1;36m"
                   "%s\n\033[0m",
                   name);
        }
        else if (st.st_mode & S_IXUSR) // write权限
        {
            printf("\033[1;32m"
                   "%s\n\033[0m",
                   name);
        }
        else if (strstr(name, ".png") != NULL || strstr(name, ".jpg") != NULL || strstr(name, ".gif") != NULL)
        {
            printf("\033[1;35m"
                   "%s\n\033[0m",
                   name);
        }
        else if (strstr(name, ".zip") != NULL || strstr(name, ".tar") != NULL || strstr(name, ".gz") != NULL)
        {
            printf("\033[1;31m"
                   "%s\n\033[0m",
                   name);
        }
        else
        {
            printf("%s\n", name);
        }
    }
}
int filesort(const struct dirent **a, const struct dirent **b)
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
    if (r_flag == 1)
    {
        return strcasecmp(bw, aw);
    }
    return strcasecmp(aw, bw);
}
int timesort(const struct dirent **a, const struct dirent **b)
{
    struct stat st_a, st_b;
    char result_a[1024];
    char result_b[1024];
    sprintf(result_a, "%s/%s", path, (*a)->d_name);
    sprintf(result_b, "%s/%s", path, (*b)->d_name);
    if (stat(result_a, &st_a) != -1 && stat(result_b, &st_b) != -1)
    {
        memset(result_a, '\0', strlen(result_a));
        memset(result_b, '\0', strlen(result_b));
        if (r_flag == 1)
        {
            return st_a.st_mtime - st_b.st_mtime;
        }
        return st_b.st_mtime - st_a.st_mtime;
    }
    else
    {
        perror("stat");
        return 0;
    }
}
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
void myls_l_s(const char *myd_name, const char *name)
{
    struct stat st;
    {
        if (stat(myd_name, &st) != -1)
        {
            if (l_flag == 0)
            {
                if (s_flag)
                {
                    if (a_flag == 0)
                    {
                        if (strcmp(name, "..") != 0 && strncmp(name, ".", 1) != 0)
                        {
                            printf("%*ld ", blocks_size_width, st.st_blocks / 2);
                        }
                    }
                    else
                    {
                        printf("%*ld ", blocks_size_width, st.st_blocks / 2);
                    }
                }
            }
            else if (l_flag == 1)
            {
                if (s_flag)
                {
                    if (a_flag == 0)
                    {
                        if (strcmp(name, "..") != 0 && strncmp(name, ".", 1) != 0)
                        {
                            printf("%*ld ", blocks_size_width, st.st_blocks / 2);
                        }
                    }
                    else
                    {
                        printf("%*ld ", blocks_size_width, st.st_blocks / 2);
                    }
                }
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

                printf(" %*lu", link_num_width, st.st_nlink);
                printf(" %s", getpwuid(st.st_uid)->pw_name);
                printf(" %s", getgrgid(st.st_gid)->gr_name);
                printf(" %*lu", size_width, st.st_size);

                char time_str[20];
                struct tm *time = localtime(&st.st_mtime);
                strftime(time_str, sizeof(time_str), "%m月 %d %H:%M", time);
                printf(" %s ", time_str);
            }
        }
    }
}
int myls_R(const char*path)
{

}
int parameter[16];
int indextime;
int main(int argc, char *argv[])
{
    while ((c = getopt(argc, argv, "altrisR")) != -1)
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
            break;
        case 'r':
            r_flag = 1;
            break;
        case 'i':
            i_flag = 1;
            break;
        case 's':
            s_flag = 1;
            break;
        case 'R':
            R_flag = 1;
        }
    }
    int (*sort)(const struct dirent **a, const struct dirent **b);
    sort = filesort;
    struct dirent **file;
    char result[1024];
    struct stat st;
    int notflag = 0;
    if (t_flag)
    {
        sort = timesort;
    }
    if (argc == 1)
    {
        indextime = 1;
        notflag = 1;
    }
    else
    {
        int j = 1, count = 0;
        while (j <= argc - 1)
        {
            if (argv[j][0] != '-')
            {
                count++, argcv = 1;
                parameter[indextime++] = j;
            }
            j++;
        }
        if (argcv == 0)
        {
            indextime = 1;
            notflag = 1;
        }
    }
    for (int time_i = 0; time_i < indextime; time_i++)
    {
        if (notflag == 0)
        {
            path = argv[parameter[time_i]];
            if (judge_file_or_directory(path))
            {
                printf("%s", path);
                return 0;
            }
            if (indextime > 1)
                printf("%s:\n", path);
        }
        n = scandir(path, &file, NULL, sort);
        if (n == -1)
        {
            perror("scandir");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < n; i++)
        {
            memset(result, '\0', strlen(result));
            sprintf(result, "%s/%s", path, file[i]->d_name);
            if ((stat(path, &st) != -1) && i == 0)
            {
                if (s_flag || l_flag)
                {
                    printf("总计 %ld\n", st.st_blocks);
                }
            }
            if (i_flag)
            {
                flag = 1;
                if (a_flag == 0)
                {
                    printf("%*ld ", ino_width, file[i]->d_ino);
                    if (strcmp(file[i]->d_name, "..") != 0 && strncmp(file[i]->d_name, ".", 1) != 0)
                    {
                        coloer(result, file[i]->d_name);
                    }
                }
                else
                {
                    printf("%ld ", file[i]->d_ino);
                }
            }
            if (l_flag)
            {
                flag = 1;
                if (a_flag == 1)
                {
                    myls_l_s(result, file[i]->d_name);
                }
                else if (a_flag == 0)
                {
                    if (strcmp(file[i]->d_name, "..") != 0 && strncmp(file[i]->d_name, ".", 1) != 0)
                    {
                        myls_l_s(result, file[i]->d_name);
                        if (r_flag == 0)
                            coloer(result, file[i]->d_name);
                    }
                }
                flagg = 1;
            }
            if (s_flag)
            {
                flag = 1;
                if (flagg == 0)
                {
                    myls_l_s(result, file[i]->d_name);
                }
            }
            if (a_flag)
            {
                flag = 1;
                coloer(result, file[i]->d_name);
            }
            if (r_flag)
            {
                flag = 1;
                if (a_flag == 0)
                {
                    if (strcmp(file[i]->d_name, "..") != 0 && strncmp(file[i]->d_name, ".", 1) != 0)
                    {
                        coloer(result, file[i]->d_name);
                    }
                }
            }
            if(R_flag)
            {
                flag = 1;
                if(stat(result,&st) != -1)
                {
                    if(S_ISDIR(st.st_mode))
                    {
                        
                    }
                }
            }
            if (flag == 0)
            {
                if (strcmp(file[i]->d_name, "..") != 0 && strncmp(file[i]->d_name, ".", 1) != 0)
                {
                    coloer(result, file[i]->d_name);
                }
            }
            free(file[i]);
        }
        free(file);
        if (time_i < indextime - 1)
        {
            printf("\n");
        }
    }
    return 0;
}