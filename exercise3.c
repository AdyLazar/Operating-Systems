#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>

int main (int argc , char *argv[])
{
    int fd[2];
    // -f[0] read
    // -f[1] write
    if(pipe(fd) == -1)
    {
        printf("An error has occured while opening the pipe");
        return 0;    
    }
    int id = fork(); // fork the execution line
    if(id == -1)
    {
        printf("An error occured with the fork");
        return 0;
    }
    if(id == 0) // child 1
    {
        close(fd[0]);
        char x[256];
        printf("Input something here: ");
        fgets(x,sizeof(x),stdin);
        write(fd[1],&x,sizeof(x));
        close(fd[1]);
        exit(0);
    } else{
        int fd1[2];
        if(pipe(fd1) == -1)
        {
            printf("An error occured while opening the second pipe");
        }
        int id2 = fork(); // fork the execution line again
        if (id2 == 0) // child2
        {
            close(fd1[1]);
            close(fd[0]);
            close(fd[1]);
            char strFromParent[256];
            read(fd1[0],&strFromParent, sizeof(strFromParent));
            close(fd1[0]);
            int length = sizeof(strFromParent)/sizeof(char);
            char vowels[] = "aeiou";
            for (int i = 0; i<strlen(strFromParent); i++)
            {
                for (int j = 0; j<5;j++)
                {
                    if(vowels[j] == strFromParent[i])
                    {
                        strFromParent[i] = '1'+j;
                    }
                }
            }
            printf("The final resault is: %s", strFromParent);
            exit(0);
          
        }else{  // parent
        close(fd[1]); // close the writing channel for the first pipe
        close(fd1[0]); // close the reading channel for the second pipe
        char y[256];
        read(fd[0], &y, sizeof(y)); // read from child1;
        write(fd1[1],&y,sizeof(y)); // write to child2;
        close(fd[0]); // close the read channel for the first pipe
        close(fd1[1]); // close the write channel for the seconf pipe
        wait(NULL);
        wait(NULL);
        printf("The initial string was: %s",y);
        exit(0);
        }

    }
    return 0;

}