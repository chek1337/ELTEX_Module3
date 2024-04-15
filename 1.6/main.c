#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <bits/stat.h>
#define S_IFMT __S_IFMT     /* These bits determine file type.  */
#define	S_IFDIR __S_IFDIR	/* Directory.  */
#define	S_IFREG	__S_IFREG	/* Regular file.  */

int DirCheck(char *pathname)
{
    DIR *d;
    struct dirent *dir; 
    struct stat statbuf; 
    char pathbuf[4096], cwd[4096];

    if (pathname == NULL) //проверка существование входного каталога
    {
        fprintf(stderr, "ERR pathname: The directory path is undefined\n");
        exit(EXIT_FAILURE);
    }

    d = opendir(pathname);

    if (!d)
    {
        perror("ERR opendir");
        exit(EXIT_FAILURE);
    }

    if(chdir(pathname) == -1) // Смена рабочего каталога
    {
        perror("ERR chdir");
        exit(EXIT_FAILURE);
    };

    if (getcwd(cwd, sizeof(cwd)) == NULL) // Получение полного пути каталога
    {
        perror("ERR getcwd");
        exit(EXIT_FAILURE);
    }
    
    printf("Folder: %s\n", pathname);
    // Подсчет количества дочерних каталогов для оптимального выделения памяти
    for(; (dir = readdir(d)) != NULL; )
    {
        snprintf(pathbuf, 4096, "%s/%s", cwd, dir->d_name);
        stat(pathbuf, &statbuf);
        if ((statbuf.st_mode & S_IFMT) == S_IFDIR) // Наложение битовой маски на st_mode и отсеивание не каталогов
            printf("Dir: %s\n", dir->d_name);
        else if((statbuf.st_mode & S_IFMT) == S_IFREG && dir->d_name[0] != '.')
            printf("File: %s\n", dir->d_name);
        else
            printf("Spec file: %s\n", dir->d_name);
    }

    closedir(d);

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Wrong number of args!\n");
        exit(EXIT_FAILURE);
    }
    return DirCheck(argv[1]);
}