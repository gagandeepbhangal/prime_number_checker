#include <unistd.h>
#include <stdlib.h>
#include "eratosthenes.h"

int num_filters = 0;

pid_t make_stage(int m, int read_fd, int **fds) {
    int *actual_fds = *fds;
    if (pipe(actual_fds) == -1) {
      exit(255);
    }
    int p = fork();
    if (p > 0){
      if ((close(actual_fds[0])) == -1) {
          exit(255);
      }

      filter(m, read_fd, actual_fds[1]);

      if ((close(read_fd)) == -1) {
          exit(255);
      }
      if ((close(actual_fds[1])) == -1) {
          exit(255);
      }
      return p;
    }
    else if (p == 0){
      if ((close(actual_fds[1])) == -1) {
          exit(255);
      }
      return 0;
    }
    else{
      exit(255);
    }
}
