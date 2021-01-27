#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

#define D_SIZE 1024

void* ls_command_normal(char dir_name[D_SIZE]);
void* ls_command_all(char dir_name[D_SIZE]);
void* ls_command_almost_all(char dir_name[D_SIZE]);

void get_path_with_spaces(char dir_name[D_SIZE], char** args, int args_to_process);

//TODO ordering of files
//TODO color



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
    char dir_name[D_SIZE];
    void* got_error=NULL;
    
    if(argc==1)
    {
        strcpy(dir_name, ".\0");
    }
    else if(**(argv+1) == '-')
    {
        if(argc == 2)
        {
            strcpy(dir_name, ".\0");
        }
        else
        {
            //get path from argv+2
            get_path_with_spaces(dir_name, argv+2, argc-2);
        }
    }
    else
    {
        //get path from argv+1
        get_path_with_spaces(dir_name, argv+1, argc-1);
    }


    // call functions

    if(argc == 1) // ls
    {
        got_error = ls_command_normal(dir_name);
    }
    else if(!strcmp(*(argv+1), "-a\0")) // ls -a 
    {
        got_error = ls_command_all(dir_name);
    }
    else if(!strcmp(*(argv+1), "-A\0")) // ls -A
    {
        got_error = ls_command_almost_all(dir_name);
    }
    else if(**(argv+1) == '-')
    {
        puts("Invalid Option!\0");
    }
    else // ls [dir]
    {
        got_error = ls_command_normal(dir_name);
    }

    red();

    if(got_error==NULL) // error handling
    {
        switch(errno)
        {
            case EACCES:
                puts("Access Denied!");
                break;
            case ENOENT:
                puts("Directory does not exist!");
                break;
            case ENOTDIR:
                puts("Path is not a directory!");
                break;
            default:
                puts("Unexpected error occured!");
        }
    }
    normal();

    
    
    

}



void get_path_with_spaces(char dir_name[D_SIZE], char** args, int args_to_process)
{
    int got_error=0; //error code for cd

    char path_with_spaces[D_SIZE]; // space for path

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
        char absolute_dir[D_SIZE];
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




void* ls_command_normal(char dir_name[D_SIZE])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(dir->d_name[0]!='.')
            {
                if(dir->d_type == DT_UNKNOWN)
                {
                    printf("\033[0;31m"); // red color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_REG)
                {
                    printf("\033[1;33m"); // yellow color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_DIR)
                {
                    printf("\033[1;34m"); // blue color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_LNK)
                {
                    printf("\033[1;35m"); // magenta color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else
                {
                    printf("%s\n", dir->d_name);
                }  
            }
        }
        closedir(d);
    }
    
    return d;
}

void* ls_command_all(char dir_name[D_SIZE])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(dir->d_type == DT_UNKNOWN)
                {
                    printf("\033[0;31m"); // red color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_REG)
                {
                    printf("\033[1;33m"); // yellow color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_DIR)
                {
                    printf("\033[1;34m"); // blue color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_LNK)
                {
                    printf("\033[1;35m"); // magenta color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else
                {
                    printf("%s\n", dir->d_name);
                }
        }
        closedir(d);
    }
    return d;
}

void* ls_command_almost_all(char dir_name[D_SIZE])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name, ".\0") && strcmp(dir->d_name, "..\0"))
            {
                if(dir->d_type == DT_UNKNOWN)
                {
                    printf("\033[0;31m"); // red color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_REG)
                {
                    printf("\033[1;33m"); // yellow color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_DIR)
                {
                    printf("\033[1;34m"); // blue color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_LNK)
                {
                    printf("\033[1;35m"); // magenta color
                    printf("%s\n", dir->d_name);
                    printf("\033[0m");
                }
                else
                {
                    printf("%s\n", dir->d_name);
                }
            }
        }
        closedir(d);
    }
    return d;
}




