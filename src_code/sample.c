#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char* argv[]) {
    // open file
    FILE *fp;
    size_t len;
    char line[255];
    char filename[255], p_filename[255];
    int errflag, content_flag;
    strcpy(filename,argv[1]);
    strncpy(p_filename, filename, strlen(filename)-7);
    strcat(p_filename, ".txt");
    while (1)
    {
    //system("clear");
    fp = fopen(argv[1], "r");
    len = 255;
    // need malloc memory for line, if not, segmentation fault error will occurred.
    //line = malloc(sizeof(char) * len);
    // check if file exist (and you can open it) or not
    if (fp == NULL) 
    {
        continue;
    }
    else
    {
        errflag=1;
        content_flag = 0;
        while(fgets(line, len, fp) != NULL) 
        {
            content_flag = 1;
            //printf("%s\n", line);
            if(strcmp(line,"RET")==0)
            {
                printf("NO ERROR IN FILE %s\n",p_filename );
                errflag =0;
                break;
            }
            
        }
            fclose(fp);
            if (errflag==1 && content_flag == 1)
                printf("ERROR IN FILE %s\n",p_filename );


    }
    //free(line); 
    sleep(4);    
    }
}