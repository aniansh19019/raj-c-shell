#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

#define MAX_FILES 16
#define D_SIZE 1024

int rm_command_normal(char paths[MAX_FILES][D_SIZE]); // rm
int rm_command_prompt(char paths[MAX_FILES][D_SIZE]); // rm -i
int rm_command_verbose(char paths[MAX_FILES][D_SIZE]); // rm -v

void get_paths_with_spaces(char paths[MAX_FILES][D_SIZE], char ** args, int arg_num);
void init_paths(char paths[MAX_FILES][D_SIZE]);
void get_proper_path(char path[D_SIZE], char arg[D_SIZE]);

void cyan()
{
    printf("\033[1;36m"); // red color
}

void red()
{
    printf("\033[1;31m"); // red color
}
void normal()
{
    printf("\033[0m");
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

    if(!strcmp(*(argv+1), "-i\0")) // rm -i
    {
        got_error = rm_command_prompt(paths);
    }
    else if(!strcmp(*(argv+1), "-v\0")) // rm -v
    {
        got_error = rm_command_verbose(paths);
    }
    else if(**(argv+1) == '-')
    {
        red();
        puts("Invalid Option!\0");
        normal();
    }
    else // ls [dir]
    {
        got_error = rm_command_normal(paths);
    }

    //error handling needs to happen per command because of list of arguments
}


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


int rm_command_normal(char paths[MAX_FILES][D_SIZE]) // rm
{
    int index=0;
    int got_error=0;
    while(paths[index][0]!='\0' && index<MAX_FILES)
    {
        if(unlink(paths[index]))
        {
            red();
            switch (errno)
            {
            case EACCES:
                printf("Permission denied!\nCannot access file: %s\n", paths[index]);
                break;
            case ENOENT:
                printf("File does not exist : %s\n", paths[index]);
                break;
            case EISDIR:
                printf("The path provided is a directory: %s\n", paths[index]);
                break;
            default:
                puts("Unexpected error occured!");
                break;
            }
            normal();
        }
        index++;
    }
    return 0;
}
int rm_command_prompt(char paths[MAX_FILES][D_SIZE]) 
{
    int index=0;
    int got_error=0;
    while(paths[index][0]!='\0' && index<MAX_FILES)
    {
        cyan();
        printf("Delete the following file? y/n : %s\n", paths[index]);
        normal();
        char* choice = (char*)malloc(4);
        scanf("%s", choice);//take user input
        if(!strcmp(choice, "y\0") || !strcmp(choice, "Y\0")) // if yes, delete the file
        {
            if(unlink(paths[index]))
            {
                red();
                switch (errno)
                {
                case EACCES:
                    printf("Permission denied!\nCannot access file: %s\n", paths[index]);
                    break;
                case ENOENT:
                    printf("File does not exist: %s\n", paths[index]);
                    break;
                case EISDIR:
                    printf("The path provided is a directory: %s\n", paths[index]);
                    break;
                default:
                    puts("Unexpected error occured!");
                    break;
                }
                normal();
            }
        }
        else
        {
            printf("Skipping... %s\n", paths[index]);
        }
        
        free(choice);

        index++;
    }
    return 0;
}
int rm_command_verbose(char paths[MAX_FILES][D_SIZE]) // rm --verbose
{
    int index=0;
    int got_error=0;
    while(paths[index][0]!='\0' && index<MAX_FILES)
    {
        if(unlink(paths[index]))
        {
            red();
            switch (errno)
            {
            case EACCES:
                printf("Permission denied!\nCannot access file: %s\n", paths[index]);
                break;
            case ENOENT:
                printf("File does not exist: %s\n", paths[index]);
                break;
            case EISDIR:
                printf("The path provided is a directory: %s\n", paths[index]);
                break;
            default:
                puts("Unexpected error occured!");
                break;
            }
            normal();
        }
        else // verbose
        {
            cyan();
            printf("Deleted file successfully: %s\n", paths[index]);
            normal();
        }
        
        index++;
    }
    
    return 0;
}
