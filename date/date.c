#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATH_SIZE 1024

void date_command_normal();
void date_command_utc();
void date_command_modified(char path[512]);

void red()
{
    printf("\033[1;31m"); // red color
}
void normal()
{
    printf("\033[0m");
}

void get_path_with_spaces(char dir_name[PATH_SIZE], char** args, int args_to_process)
{
    int got_error=0; //error code for cd

    char path_with_spaces[PATH_SIZE]; // space for path

    int dir_index=0;

    int i=1;

    strcpy(path_with_spaces, *args);

    while(i<args_to_process) // dealing with spaces in the path
    {
        strcat(path_with_spaces, " \0");
        strcat(path_with_spaces, *(args+i));
        i++;
    }

    // Entire path in path_with_spaces

    if(**args=='/') // absolute path
    {
        strcpy(dir_name, path_with_spaces);
    }
    else if(**args=='~')
    {
        char absolute_dir[PATH_SIZE];
        strcpy(absolute_dir, getenv("HOME")); //get home directory

        int home_length = strlen(absolute_dir); // length of home path
        
        strcat(absolute_dir, path_with_spaces+1); // skipping ~

        strcpy(dir_name, absolute_dir);
    }
    else
    {
        strcpy(dir_name, path_with_spaces);
    }
}

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        date_command_normal();
    }
    else if(argc==2 && !strcmp(*(argv+1), "-u\0"))
    {
        date_command_utc();
    }
    else if(argc>2 && !strcmp(*(argv+1), "-r\0"))
    {
        char path[PATH_SIZE];
        get_path_with_spaces(path, argv+2, argc-2);
        date_command_modified(path);
    }
    else
    {
        red();
        puts("Invalid Option/Argument!");
        normal();
    }
    
}



void date_command_normal() // date 
{
    time_t now;
    time(&now);
    printf("The current local time is: %s", ctime(&now));
}

void date_command_utc() // date -u
{
    time_t now;
    time(&now);
    struct tm *utc_time = gmtime(&now);
    printf("The current UTC time is: %s", asctime(utc_time));

}

void date_command_modified(char path[PATH_SIZE]) // date -r
{
    struct stat file_info;
    if(!stat(path, &file_info)) // proper functioning
    {
        printf("Last modified time: %s", ctime(&file_info.st_mtime));
    }
    else
    {
        red();
        switch(errno)
        {
            case EACCES:
                puts("Permission denied!");
                break;
            case ENOENT:
                puts("Path does not exist!");
                break;
            case ENOTDIR:
                puts("Part of the path is not a directory!");
                break;
            default:
                puts("Unexpected error occured!");
        }
        normal();
    }
    
}