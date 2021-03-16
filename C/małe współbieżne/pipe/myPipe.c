#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "err.h"



int main (int argc, char *argv[])
{
  int i = 0;
  int n = argc - 1;
  int* pipe_dsc = malloc(sizeof(int) * n * 2);
  
  while(i < n){

    if (pipe(pipe_dsc + 2*i) == -1) syserr("Error in pipe\n");

    switch (fork()){
      case -1: 
        syserr("Error in fork\n");

      case 0:
        
        //wywołuje program A
        //wczytuje to co poprzedni wypisze
        if(i != 0)
	        if(dup2(pipe_dsc[2*i - 2], 0) == -1) syserr("Error in dup2\n");
        // wypisuje do pipe'a
        if(i != n - 1){
	        if(dup2(pipe_dsc[2*i + 1], 1) == -1) syserr("Error in dup2\n");
        }
        if (close (pipe_dsc [2*i]) == -1) syserr("Error in close\n");
        if (close (pipe_dsc [2*i+1]) == -1) syserr("Error in close\n");
       
	    execlp(argv[i+1], argv[i+1], NULL);
	    syserr("Error in execvp\n");
        exit (0);
     
    default: /* proces macierzysty */
     // if (close (pipe_dsc [2*i]) == -1) syserr("Error in parent, close (pipe_dsc [0])\n");
      if (close (pipe_dsc [2*i+1 ]) == -1) syserr("Error in parent, close (pipe_dsc [1])\n");

    }
    i++;
    
  }
  
    /* czekanie na zakończenie procesow potomnych */
  for (i = 1; i <= n; i++)
    if (wait(0) == -1) 
      syserr("Error in wait\n");
  free(pipe_dsc);
  return 0;
  
}
