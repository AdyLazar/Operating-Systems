#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>

int main (int argc , char *argv[])
{
    char * fifo1 = "C:\\Users\\adria\\OneDrive\\Desktop\\year 2\\Computer science\\Operating Systems\\laboratory\\lab11\\fifo1";
    char * fifo2 = "C:\\Users\\adria\\OneDrive\\Desktop\\year 2\\Computer science\\Operating Systems\\laboratory\\lab11\\fifo2";

    if(mkfifo("fifo1",S_IFIFO|0666,0) == -1 && errno != EEXIST) // create a fifo
    {
        printf("An error has occured while creating fifo1 %s", strerror(errno));
        return 11;    
    }
    if(mkfifo("fifo2",S_IFIFO|0666,0) == -1 && errno != EEXIST) // create a fifo
    {
        printf("An error has occured while creating fifo2");
        return 13;    
    } 
    int id = fork(); // fork the execution line
    if(id == -1)
    {
        printf("An error occured with the fork");
        return 0;
    }
    if(id == 0) // child 1
    {
        char x[256];
        printf("Input something here: ");
        fgets(x,sizeof(x),stdin);
        printf("Opening...");
        int fd = open("fifo1",O_WRONLY);
        printf("Opened");
        printf("Writing...");
        if (write(fd,&x,sizeof(x) == -1))
        {
             return 12;
        }
        printf("Written");
        close(fd);
        exit(0);
    } else{
        int fd;
        int id2 = fork(); // fork the execution line again
        if (id2 == 0) // child2
        {
            char strFromParent[256];
            int fd = open("fifo2",O_WRONLY);
            read(fd,&strFromParent, sizeof(strFromParent));
            close(fd);
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
        char y[256];

        int fd1 = open("fifo1",O_WRONLY);
        read(fd, &y, sizeof(y)); // read from child1;

        int fd2 = open("fifo2",O_WRONLY);
        write(fd2,&y,sizeof(y)); // write to child2;

        close(fd1); // close the read channel for the first pipe
        close(fd2); // close the write channel for the seconf pipe
        wait(NULL);
        wait(NULL);
        printf("The initial string was: %s",y);
        exit(0);
        }

    }
    return 0;

}