#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_FILES 16
#define D_SIZE 1024
#define BUFFER_SIZE 2048


void cat_command_normal(char paths[MAX_FILES][D_SIZE]);
void cat_command_number(char paths[MAX_FILES][D_SIZE]);
void cat_command_number_non_blank(char paths[MAX_FILES][D_SIZE]);


void get_paths_with_spaces(char paths[MAX_FILES][D_SIZE], char ** args, int arg_num);
void init_paths(char paths[MAX_FILES][D_SIZE]);
void get_proper_path(char path[D_SIZE], char arg[D_SIZE]);


void init_argument(char argument[D_SIZE])
{
    for(int i=0; i<D_SIZE; i++)
    {
        argument[i] = '\0';
    }
}

void get_paths_with_spaces(char paths[MAX_FILES][D_SIZE], char ** args, int arg_num)
{
    init_paths(paths);
    char argument[D_SIZE];
    int paths_index=0;

    strcpy(argument, *args); //prep for loop


    for(int i=1; i<arg_num+1; i++)
    {
        int arg_length = strlen(argument);

        if(argument[arg_length - 1] == '\\' && i<arg_num)
        {
            argument[arg_length-1] = ' ';
            argument[arg_length] = '\0';
            strcat(argument, *(args+i));
        }
        else
        {

            get_proper_path(paths[paths_index++], argument); // proper path and put into paths
            if(i<arg_num)
            {
                strcpy(argument, *(args+i)); // copy next argument
            }
            

        }
        

    }
}

void init_paths(char paths[MAX_FILES][D_SIZE])
{
    for(int i=0; i<MAX_FILES; i++)
    {
        for(int j=0; j<D_SIZE; j++)
        {
            paths[i][j] = '\0';
        }
    }
}

void get_proper_path(char path[D_SIZE], char arg[D_SIZE])
{
    int got_error=0; //error code for cd

    char path_with_spaces[D_SIZE]; // space for path

    int dir_index=0;

    int i=1;

    strcpy(path_with_spaces, arg);

    // Entire path in path_with_spaces

    if(arg[0]=='/') // absolute path
    {
        strcpy(path, path_with_spaces);
    }
    else if(arg[0]=='~')
    {
        char absolute_dir[D_SIZE];
        strcpy(absolute_dir, getenv("HOME")); //get home directory

        int home_length = strlen(absolute_dir); // length of home path
        
        strcat(absolute_dir, path_with_spaces+1); // skipping ~

        strcpy(path, absolute_dir);
    }
    else
    {
        strcpy(path, path_with_spaces);
    }
}

void red()
{
    printf("\033[1;31m"); // red color
}
void normal()
{
    printf("\033[0m");
}
void cyan()
{
    printf("\033[1;36m"); // red color
}


int main(int argc, char **argv)
{
    char paths[MAX_FILES][D_SIZE];
    int got_error=0;

    if(argc==1)
    {
        red();
        puts("No Argument provided!");
        normal();
        return 0;
    }
    else if(**(argv+1) == '-')
    {
        if(argc == 2)
        {
            red();
            puts("No Argument provided!");
            normal();
            return 0;
        }
        else
        {
            //get path from argv+2
            get_paths_with_spaces(paths, argv+2, argc-2);
        }
    }
    else
    {
        //get path from argv+1
        get_paths_with_spaces(paths, argv+1, argc-1);
        
    }
    

    // call functions

    if(!strcmp(*(argv+1), "-b\0")) // cat -b
    {
        cat_command_number_non_blank(paths);
    }
    else if(!strcmp(*(argv+1), "-n\0")) // cat -n
    {
        cat_command_number(paths);
    }
    else if(**(argv+1) == '-')
    {
        red();
        puts("Invalid Option!\0");
        normal();
    }
    else // ls [dir]
    {
        cat_command_normal(paths);
    }
}

void cat_command_normal(char paths[MAX_FILES][D_SIZE]) 
{
    FILE *f;
    char buffer[BUFFER_SIZE];
    int path_index=0;
    while(paths[path_index][0]!='\0')
    {
        int is_dir=0;
        struct stat file_info;

        if(!stat(paths[path_index], &file_info)) // proper functioning
        {
            if(S_ISDIR(file_info.st_mode))
            {
                red();
                printf("%s is a directory!\n", paths[path_index]);
                normal();
                is_dir=1;
            }
        }

        if(!is_dir) // if not a directory
        {
            f = fopen(paths[path_index], "r");
            if(f!=NULL)
            {
                while(fgets(buffer, BUFFER_SIZE, f)!=NULL)
                {
                    printf("%s", buffer);
                }
            }
            else // file could not be opened
            {
                red();
                switch (errno)
                {
                case EACCES:
                    printf("Caannot acces file: %s\nAccess denied!\n", paths[path_index]);
                    break;
                case ENOENT:
                    printf("File does not exist: %s\n", paths[path_index]);
                    break;
                case EISDIR:
                    printf("%s is a directory!\n", paths[path_index]);
                    break;
                default:
                    printf("Error opening file: %s\n", paths[path_index]);
                }
                normal();
            }
            fclose(f);
        }
        path_index++;
    }

}
void cat_command_number(char paths[MAX_FILES][D_SIZE])
{
    int line_number=1;
    FILE *f;
    char buffer[BUFFER_SIZE];
    int path_index=0;
    while(paths[path_index][0]!='\0')
    {
        int is_dir=0;
        struct stat file_info;

        if(!stat(paths[path_index], &file_info)) // proper functioning
        {
            if(S_ISDIR(file_info.st_mode))
            {
                red();
                printf("%s is a directory!\n", paths[path_index]);
                normal();
                is_dir=1;
            }
        }

        if(!is_dir) // if not a directory
        {
            f = fopen(paths[path_index], "r");
            if(f!=NULL)
            {
                while(fgets(buffer, BUFFER_SIZE, f)!=NULL)
                {
                    printf("%d\t", line_number++);
                    printf("%s", buffer);
                }
            }
            else // file could not be opened
            {
                red();
                switch (errno)
                {
                case EACCES:
                    printf("Caannot acces file: %s\nAccess denied!\n", paths[path_index]);
                    break;
                case ENOENT:
                    printf("File does not exist: %s\n", paths[path_index]);
                    break;
                case EISDIR:
                    printf("%s is a directory!\n", paths[path_index]);
                    break;
                default:
                    printf("Error opening file: %s\n", paths[path_index]);
                }
                normal();
            }
            fclose(f);
        }
        path_index++;
    }

}
void cat_command_number_non_blank(char paths[MAX_FILES][D_SIZE]) // TODO check for whitespaces
{
    int line_number=1;
    FILE *f;
    char buffer[BUFFER_SIZE];
    int path_index=0;
    while(paths[path_index][0]!='\0')
    {
        int is_dir=0;
        struct stat file_info;

        if(!stat(paths[path_index], &file_info)) // proper functioning
        {
            if(S_ISDIR(file_info.st_mode))
            {
                red();
                printf("%s is a directory!\n", paths[path_index]);
                normal();
                is_dir=1;
            }
        }

        if(!is_dir) // if not a directory
        {
            f = fopen(paths[path_index], "r");
            if(f!=NULL)
            {
                while(fgets(buffer, BUFFER_SIZE, f)!=NULL)
                {
                    if(strcmp(buffer, "\n\0")) // if not equal
                    {
                        printf("%d\t", line_number++);
                    }
                    
                    printf("%s", buffer);
                }
            }
            else // file could not be opened
            {
                red();
                switch (errno)
                {
                case EACCES:
                    printf("Caannot acces file: %s\nAccess denied!\n", paths[path_index]);
                    break;
                case ENOENT:
                    printf("File does not exist: %s\n", paths[path_index]);
                    break;
                case EISDIR:
                    printf("%s is a directory!\n", paths[path_index]);
                    break;
                default:
                    printf("Error opening file: %s\n", paths[path_index]);
                }
                normal();
            }
            fclose(f);
        }
        path_index++;
    }

}
