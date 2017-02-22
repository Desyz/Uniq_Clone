/*
    author: Javier Laspina

    This program is a basic clone of uniq

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 128

/*
    Reads from the file and returns a substring or a full buffer if no newline
    has been found.
*/
int readLine(int fd, char *buf, int nbytes);

int main(int argc, char const *argv[]) {
    int fd;
    // If no filepath is specifiy open STDIN
    if (argc == 1) {
        fd = STDIN_FILENO;
    } else {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            printf("%s: %s: %s\n", argv[0], argv[1], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    char prevLine[BUFFER_SIZE];
    char currLine[BUFFER_SIZE];
    while ( readLine(fd, currLine, BUFFER_SIZE) > 0) {
      if (strcmp(prevLine, currLine) != 0) {
        printf("%s", currLine);
      }
      strcpy(prevLine, currLine);
    }
    close(fd);
    return 0;
}

int readLine(int fd, char *buf, int nbytes) {
   int numread = 0;
   int returnval;

   while (numread < nbytes - 2) {
      returnval = read(fd, buf + numread, 1);
      if ((returnval == -1) && (errno == EINTR))
         continue;
      if ( (returnval == 0) && (numread == 0) )
         return 0;
      if (returnval == 0)
         break;
      if (returnval == -1)
         return -1;
      numread++;
      if (buf[numread-1] == '\n') {
         buf[numread] = '\0';
         return numread;
      }
   }
   // Buffer full case
   if (errno == 0) {
     buf[nbytes - 1] = '\0';
     return numread;
   }
   errno = EINVAL;
   return -1;
}
