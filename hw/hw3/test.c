#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <dirent.h>
 

 void main(int argc, char *argv[]) {
            char *path = "/";
            struct stat buf;
            if ( stat(path, &buf) == -1 )
                perror(argv[0]), exit(1);
            printf("Links: %d Size: %d\n", buf.st_nlink, buf.st_size);
         }




           
            
          
         
        
       


