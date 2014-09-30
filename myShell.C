#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80

int main(void)
{
  char *args[MAX_LINE/2 + 1];
  bool should_run = true;
  char blank[MAX_LINE] = { 0 }; //initially blank array buffer to be pointed to by args 

  pid_t pid;

  printf("osh> ");  

  while(should_run)//ADD EXITING
    {
      int i = 0;

      bool backg = false; //everytime we read in a command, start by assuming it's not in the background     

      fgets(blank,MAX_LINE,stdin);
      //printf("osh> ");

      do
	{
	  
	  if (i == 0) args[i] = strtok(blank," \n"); // on the first entry, different call to strtok
 
	  else if (i == MAX_LINE/2 - 1)//if we're on the second to last possible entry

	    {
	      args[i] = strtok (NULL," \n");
	      args[i+1] = 0; // be sure to end with null character
	    }
	  
	  else args[i] = strtok (NULL," \n"); 

	  //printf ("%s",args[i]); // checking arguments

	  if (args[i] != NULL) //prevent segmentation fault on strncmp
	    {
	  
	      if (strncmp(args[i], "&",1) == 0) //if we're trying to start a background process
		{
		  //printf ("true"); //confirm we're in the background
		backg = true;
		args[i] = 0; // be sure to end with null character; can't run a command with this sign
	      
	      }
	    }
	  i++;
	  

	} while (args[i-1] != NULL && (i) < MAX_LINE/2 ); //if the last entry is NULL or we're one from the end, leave the final NULL


      pid = fork();

      if (pid < 0) //error
	{
	  fprintf(stderr, "Fork Failed");
	  return 1;
	}

      else if (pid == 0) //child process
	{
	  execvp(args[0], args);
	  //fflush(stdout);
	  //blank[0] = 0; //refresh blank
	}
    
      else //parent process
	{
	  if (!backg)    
	    {
	      wait(NULL);
	      printf("Child Complete \n");
	      printf("osh> ");
	    }

	  backg = false; //reset the flag
	  
	}
    }
}
