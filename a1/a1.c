#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void listDir(const char *path, int perm)
{ // printf("Am intrat in ITERATIVA\n");
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            lstat(fullPath, &statbuf);
            if (lstat(fullPath, &statbuf) == 0)
            {

                if (perm != 1 && perm != 0)
                {
                    if (stat(fullPath, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
                    {
                        if (statbuf.st_size < perm)
                        {
                            printf("%s\n", fullPath);
                        }
                    }
                }
                else if (perm == 1)
                {
                    if ((statbuf.st_mode & 0200) != 0)
                    {
                        printf("%s\n", fullPath);
                    }
                }
                else
                    printf("%s\n", fullPath);
            }
        }
    }
    closedir(dir);
}

int verificareParsare(const char *path, int sect, int linie, int ok)
{
    int fd1 = -1;
    char magic[5] = "\0";
    int headerSize = 0;
    int version = 0;
    int nrSections = 0;

    ssize_t flagmagic;
    ssize_t flagheader;
    ssize_t flagversion;
    ssize_t flagnrSections;

    char buffer[2000000] = "";

    struct sectiune
    {
        char sect_name[10];
        int sect_type;
        int sect_offset;
        int sect_size;
    };

    fd1 = open(path, O_RDONLY);
    if (fd1 == -1)
    {
        perror("Could not open input file");
        return 1;
    }
    // magic
    flagmagic = read(fd1, &magic, 4);
    if (flagmagic < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    else if (strncmp(magic, "0ceH", 5) != 0)
    {
        // printf("magicul este:%s",magic);
        // printf("ERROR\nwrong magic");
        // exit(1);
        return -1;
    }

    // headerSeize
    flagheader = read(fd1, &headerSize, 2);
    if (flagheader < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    /// version
    flagversion = read(fd1, &version, 2);
    if (flagversion < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    else if (version < 70 || version > 122)
    {
        // printf("ERROR\nwrong version");
        // exit(1);
        return -1;
    }
    // sectiuni
    flagnrSections = read(fd1, &nrSections, 1);
    if (flagnrSections < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    else if (nrSections < 5 || nrSections > 16)
    {
        // printf("ERROR\nwrong sect_nr");
        // exit(1);
        return -1;
    }
    
    if (ok == 1)
    {
        struct sectiune s1;
        if (sect > nrSections)
        {
            printf("ERROR\nInvalid section");
            return -1;
        }

        for (int i = 0; i < sect; i++)
        {
            read(fd1, &s1.sect_name, 9);

            read(fd1, &s1.sect_type, 4);
            if (!(s1.sect_type == 74 || s1.sect_type == 83 || s1.sect_type == 95 || s1.sect_type == 58 || s1.sect_type == 85))
            {
                return -1;
            }
            read(fd1, &s1.sect_offset, 4);
            read(fd1, &s1.sect_size, 4);
        }

        lseek(fd1, s1.sect_offset, SEEK_SET);
        read(fd1, &buffer, s1.sect_size);
        char *sir = strtok(buffer, "\n");
        int nr = 0;
        while (sir != NULL)
        {
            nr++;
            sir = strtok(NULL, "\n");
        }

        if (linie > nr)
        {
            printf("ERROR\ninvalid line");
            exit(1);
        }

        int nr2 = nr - linie + 1;

        printf("SUCCESS\n");
        lseek(fd1, s1.sect_offset, SEEK_SET);
        read(fd1, &buffer, s1.sect_size);
        sir = strtok(buffer, "\n");
        nr = 0;
        while (sir != NULL)
        {
            nr++;
            if (nr == nr2)
            {
                printf("%s\n", sir);
                break;
            }
            sir = strtok(NULL, "\n");
        }
    }
    else if (ok == 2)
    {   //printf("Nr de sectiuni este:%d\n",nrSections);
        int iteratii=0;
        int sectCu15 = 0;
        struct sectiune s1;
        //int size=0;
        for (int i = 0; i < nrSections; i++)
        {
        iteratii++;
            //sectCu15 = 0;
            read(fd1, &s1.sect_name, 9);
            read(fd1, &s1.sect_type, 4);
            if (!(s1.sect_type == 74 || s1.sect_type == 83 || s1.sect_type == 95 || s1.sect_type == 58 || s1.sect_type == 85))
            {
                return -1;
            }
            read(fd1, &s1.sect_offset, 4);
            read(fd1, &s1.sect_size, 4);
            
            //printf("Offset%d\n",s1.sect_offset);
            lseek(fd1, s1.sect_offset, SEEK_SET);
            read(fd1, &buffer, s1.sect_size);
            //printf("Sectiunea e valida\n");
            char *sir = strtok(buffer, "\n");
            int nr = 0;
            //printf("Bufferul este:%s\n",buffer);
            while (sir != NULL)
            {
                nr++;
                //printf("%s\n",sir);
                sir = strtok(NULL, "\n");
            }
            //nr++;
            //printf("Nr de linii este %d\n",nr);
            if (nr == 15)
                {sectCu15++;
                //printf("Nr de sectiuni este %d\n",nr);
                }
                //printf("Nr de sectiuni este %d\n",sectCu15);
                //printf("Nr de iteratii %d:\n",iteratii);
                //printf("i=%d\n",i);
                
            //printf("\n");
            //size+=s1.sect_size;

            lseek(fd1, 9+21*(i+1), SEEK_SET);
        }
         //printf("Nr de iteratii %d:\n",iteratii);
        if (sectCu15 >= 2)
            return 5;
    }

    return 0;
}
void listRecursiv(const char *path, int perm)
{ // printf("Am intrat in recursiva\n");
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                if (perm == 1)
                {
                    if ((statbuf.st_mode & 0200) != 0)
                    {
                        printf("%s\n", fullPath);
                        if (S_ISDIR(statbuf.st_mode))
                        {
                            listRecursiv(fullPath, perm);
                        }
                    }
                    else if (S_ISDIR(statbuf.st_mode))

                        listRecursiv(fullPath, perm);
                }
                else if (perm == 2)
                {  //printf("Am intrat in listRecursiv\n");

                    int afis = verificareParsare(fullPath, 0, 0, 2);
                    if (afis == 5)
                        printf("%s\n", fullPath);
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        listRecursiv(fullPath, perm);
                    }
                }
                else if (perm != 1 && perm != 0 && perm != 2)
                {
                    if (stat(fullPath, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
                    {
                        if (statbuf.st_size < perm)
                        {

                            printf("%s\n", fullPath);

                            if (S_ISDIR(statbuf.st_mode))

                                listRecursiv(fullPath, perm);
                        }
                    }
                    else if (S_ISDIR(statbuf.st_mode))

                        listRecursiv(fullPath, perm);
                }
                else
                {
                    printf("%s\n", fullPath);
                    if (S_ISDIR(statbuf.st_mode))

                        listRecursiv(fullPath, perm);
                }
            }
        }
    }

    closedir(dir);
}

int parsare(const char *path)
{
    int fd1 = -1;
    char magic[5] = "\0";
    int headerSize = 0;
    int version = 0;
    int nrSections = 0;

    ssize_t flagmagic;
    ssize_t flagheader;
    ssize_t flagversion;
    ssize_t flagnrSections;

    struct sectiune
    {
        char sect_name[10];
        int sect_type;
        int sect_offset;
        int sect_size;
    };

    fd1 = open(path, O_RDONLY);
    if (fd1 == -1)
    {
        perror("Could not open input file");
        return 1;
    }
    // magic
    flagmagic = read(fd1, &magic, 4);
    if (flagmagic < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    else if (strncmp(magic, "0ceH", 5) != 0)
    {
        // printf("magicul este:%s",magic);
        printf("ERROR\nwrong magic");
        exit(1);
    }

    // headerSeize
    flagheader = read(fd1, &headerSize, 2);
    if (flagheader < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    /// version
    flagversion = read(fd1, &version, 2);
    if (flagversion < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    else if (version < 70 || version > 122)
    {
        printf("ERROR\nwrong version");
        exit(1);
    }
    // sectiuni
    flagnrSections = read(fd1, &nrSections, 1);
    if (flagnrSections < 0)
    {

        perror("Could not read from input file");
        exit(1);
    }
    else if (nrSections < 5 || nrSections > 16)
    {
        printf("ERROR\nwrong sect_nr");
        exit(1);
    }

    struct sectiune s1;

    for (int i = 0; i < nrSections; i++)
    {
        read(fd1, &s1.sect_name, 9);

        read(fd1, &s1.sect_type, 4);
        if (!(s1.sect_type == 74 || s1.sect_type == 83 || s1.sect_type == 95 || s1.sect_type == 58 || s1.sect_type == 85))
        {
            printf("ERROR\nwrong sect_types");
            exit(1);
        }
        read(fd1, &s1.sect_offset, 4);
        read(fd1, &s1.sect_size, 4);
    }

    lseek(fd1, 9, SEEK_SET);
    printf("SUCCESS\n");
    printf("version=%d\n", version);
    printf("nr_sections=%d\n", nrSections);

    for (int i = 0; i < nrSections; i++)
    {
        printf("section%d: ", i + 1);
        read(fd1, &s1.sect_name, 9);
        char str[10];
        strncpy(str, s1.sect_name, 9);
        str[9] = '\0';
        printf("%s ", str);

        read(fd1, &s1.sect_type, 4);
        printf("%d ", s1.sect_type);

        read(fd1, &s1.sect_offset, 4);

        read(fd1, &s1.sect_size, 4);
        printf("%d ", s1.sect_size);

        printf("\n");
    }

    return 0;
}
void extract(char *path, int sect, int linie, int ok)
{
    int decizie = verificareParsare(path, sect, linie, ok);
    if (decizie == -1)
    {
        printf("ERROR\ninvalid file");
        exit(1);
    }
}
int main(int argc, char **argv)
{
    char substring[256] = "";
    char nr[10] = "";
    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("72328\n");
        }
        else if (strcmp(argv[1], "list") == 0)
        {
            if ((strcmp(argv[2], "recursive") == 0) && (strncmp(argv[3], "path=", 5)) == 0)
            { // printf("Am intrat in 1\n");
                int perm = 0;
                strcpy(substring, argv[3]);
                printf("SUCCESS\n");
                listRecursiv(substring + 5, perm);
            }
            else if (strncmp(argv[2], "path=", 5) == 0)
            { // printf("Am intrat in 2\n");
                int perm = 0;
                strcpy(substring, argv[2]);
                printf("SUCCESS\n");
                listDir(substring + 5, perm);
            }
            else if ((strcmp(argv[3], "recursive") == 0) && (strcmp(argv[2], "has_perm_write") == 0) && (strncmp(argv[4], "path=", 5)) == 0)
            { // printf("Am intrat in 3\n");
                int perm = 1;
                strcpy(substring, argv[4]);
                printf("SUCCESS\n");
                listRecursiv(substring + 5, perm);
            }
            else if ((strcmp(argv[2], "has_perm_write") == 0) && (strncmp(argv[3], "path=", 5)) == 0)
            { // printf("Am intrat in 4\n");
                int perm = 1;
                strcpy(substring, argv[3]);
                printf("SUCCESS\n");
                listDir(substring + 5, perm);
            }
            else if ((strncmp(argv[2], "size_smaller=", 13) == 0) && (strncmp(argv[3], "path=", 5)) == 0)
            { // printf("Am intrat in 5\n");
                int perm = 0;
                strcpy(nr, argv[2] + 13);
                perm = (int)atoi(nr);
                strcpy(substring, argv[3]);
                printf("SUCCESS\n");
                listDir(substring + 5, perm);
            }
            else if ((strncmp(argv[2], "size_smaller=", 13) == 0) && (strncmp(argv[3], "path=", 5)) == 0)
            { // printf("Am intrat in 6\n");
                int perm = 0;
                strcpy(nr, argv[2] + 13);
                perm = (int)atoi(nr);
                strcpy(substring, argv[3]);
                printf("SUCCESS\n");
                listDir(substring + 5, perm);
            }
            else if ((strncmp(argv[2], "size_smaller=", 13) == 0) && (strcmp(argv[3], "recursive") == 0) && (strncmp(argv[4], "path=", 5)) == 0)
            { // printf("Am intrat in 7\n");
                int perm = 0;
                strcpy(nr, argv[2] + 13);
                perm = (int)atoi(nr);
                strcpy(substring, argv[4]);
                printf("SUCCESS\n");
                listRecursiv(substring + 5, perm);
            }
            else if ((strncmp(argv[3], "size_smaller=", 13) == 0) && (strcmp(argv[2], "recursive") == 0) && (strncmp(argv[4], "path=", 5)) == 0)
            { // printf("Am intrat in 7\n");
                int perm = 0;
                strcpy(nr, argv[3] + 13);
                perm = (int)atoi(nr);
                strcpy(substring, argv[4]);
                printf("SUCCESS\n");
                listRecursiv(substring + 5, perm);
            }
        }
        else if ((strcmp(argv[1], "parse") == 0) && (strncmp(argv[2], "path=", 5)) == 0)
        {
            // printf("Am intrat in 8\n");
            strcpy(substring, argv[2]);

            parsare(substring + 5);
        }

        else if ((strcmp(argv[1], "extract") == 0) && (strncmp(argv[2], "path=", 5)) == 0 && (strncmp(argv[3], "section=", 8)) == 0 && (strncmp(argv[4], "line=", 5)) == 0)
        {
            // printf("Am intrat in 9\n");
            int nrsect = 0;
            int nrlinie = 0;
            char sect[100] = "";

            strcpy(sect, argv[3] + 8);
            nrsect = (int)atoi(sect);
            // printf("Nr sectiunii este %d\n",nrsect);
            strcpy(substring, argv[2]);

            strcpy(sect, argv[4] + 5);
            nrlinie = (int)atoi(sect);
            // printf("Nr liniei este %d\n",nrlinie);

            extract(substring + 5, nrsect, nrlinie, 1);
        }
        else if ((strcmp(argv[1], "findall") == 0) && (strncmp(argv[2], "path=", 5)) == 0)
        {     //printf("Am intrat in 10\n");
            strcpy(substring, argv[2]);
            printf("SUCCESS\n");
            listRecursiv(substring + 5, 2);
        }
    }

    return 0;
}
