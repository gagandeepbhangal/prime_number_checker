#include <stdio.h>

#include "eratosthenes.h"

int filter(int m, int readfd, int writefd) {
  int num;
  while (read(readfd, &num, sizeof(int)) > 0){
    if (num % m != 0){
      if (write(writefd, &num, sizeof(int)) == -1){
        return 1;
      }
    }
  }
  return 0;
}
