#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"

int main(int argc, char *argv[]) {
    // Turning off sigpipe
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    if (argc != 2){
      fprintf(stderr, "Usage:\n\tpfact n\n");
      exit(1);
    }
    int n;
    for (int q = 0; q < strlen(argv[1]); q++){
      if (argv[1][q]>= '0' && argv[1][q] <= '9'){
          n = strtol(argv[1], NULL, 10);
      }
      else{
        fprintf(stderr, "Usage:\n\tpfact n\n");
        exit(1);
      }
    }
    if (n < 2){
      fprintf(stderr, "Usage:\n\tpfact n\n");
      exit(1);
    }
    if (n == 2 || n == 3){
      printf("%d is prime\n", n);
      printf("Number of filters = 0\n");
      exit(0);
    }
    int m = 2;
    int square_root = (sqrt(n));
    int fd[n][2];
    int *fds = fd[1];
    int status;
    int factors[n];
    int counter = 0;
    int factor_counter = 0;
    int previous = 0;

    factors[0] = -1000;

    if (pipe(fd[0]) == -1) {
      perror("pipe");
      exit(2);
    }
    int r = fork();
    if (r > 0){
      if ((close(fd[0][0])) == -1) {
          perror("close");
          exit(2);
      }
      //create pipe of 2 to n numbers
      if (counter != -1){
        for (int j = 2; j <= n; j++){
          if (write(fd[0][1], &j, sizeof(int)) == -1){
            exit(2);
          }
        }
      }
      if ((close(fd[0][1])) == -1) {
          perror("close");
          exit(2);
      }
      counter = -1;
      wait(&status);
      printf("Number of filters = %d\n", WEXITSTATUS(status));
      exit(0);
    }
    else if (r == 0){
      pid_t num;
      int stat;
      if ((close(fd[0][1])) == -1) {
          perror("close");
          exit(2);
      }
      for (int l = 1; m <= square_root; l++){
        if (l == 256){
          exit(2);
        }
        previous = m;
        if (read(fd[l-1][0], &m, sizeof(int)) == -1){
          perror("fork");
          exit(2);
        }
        if (n%m == 0){
          if (factors[1] == m){
            printf("%d %d %d\n", n, factors[0], m);
            exit(0);
          }
          if (factor_counter != 0){
            printf("%d is not the product of two primes\n", n);
            exit(0);
          }
          if (m*m == n){
            printf("%d %d %d\n", n, m, m);
            exit(0);
          }
          if ((n/m)%m == 0){
            printf("%d is not the product of two primes\n", n);
            exit(0);
          }
          factors[factor_counter] = m;
          factors[factor_counter+1] = n/m;
          factor_counter += 2;
        }

        num = make_stage(m, fd[l-1][0], &fds);

        if (num == 255){
          return 2;
        }
        fds = fd[l+1];
        if (num > 0){
          wait(&stat);
          if (WIFEXITED(stat)){
            if (stat == 255){
              return 2;
            }
            exit(WEXITSTATUS(stat)+1);
          }
        }
      }
      if (n%previous == 0){
        if (factor_counter != 0){
          if (factors[1] > square_root){
            printf("%d %d %d\n", n, previous, factors[1]);
            exit(-1);
          }
          printf("%d is not the product of two primes\n", n);
          exit(-1);
        }
        else{
          if ((n/previous)%previous == 0){
            printf("%d is not the product of two primes\n", n);
            exit(-1);
          }else{
            printf("%d %d %d\n", n, previous, n/previous);
            exit(-1);
          }
        }
      }
      else{
        if (factors[0] != -1000){
          printf("%d %d %d\n", n, factors[0], factors[1]);
          exit(-1);
        }
        printf("%d is prime\n", n);
        exit(-1);
      }
      exit(0);
    }
    else{
      perror("fork");
      exit(2);
    }
    return 0;
  }
