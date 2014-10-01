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

  char *history[10] = { 0 } ;

  pid_t pid;

  bool backg = false; //everytime we read in a command, start by assuming it's not in the background
  int i = 0;
  int j = 0;
  int k = 0;

  while(should_run)//ADD EXITING
    {
      i = 0;
      j = 0;
      k = 0;
      backg = false;

      printf("\nosh> ");
      
      fflush(stdout);

      fgets(blank,MAX_LINE,stdin);
      
      do //if the history buffer still has blank spaces, fill them in.
	{
	  
	  if (history[j] == NULL)
	    {
	      history[j] = blank;
	    }

	  j++;

	} while (j < 10);

      if (history[9] != NULL) //if the top slot is full
	{
	  history[0] = NULL; //Null out the bottom slot
	  for (j = 0; j < 10; j++) history [j] = history[j+1];
	  history[j] = blank;
	}

      do
	{
	    
	  if (i == 0) 
	    {
	      args[i] = strtok(blank," \n"); // on the first entry, different call to strtok
	      if (args[i] != NULL) 
		{
		  if (strncmp(args[i], "exit",4) == 0)should_run = false; //on exit, turn off loop condition
		}
	    }
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

      if (args[0] != NULL) 
		{
		  if ((strncmp(args[0], "exit",4) != 0) || (strncmp(args[0], "history",7) != 0 )) //on exit or history, don't create processes      
		  {
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
			    fflush(stdout);
			  }
		      }
		  
		  }

		  if (strncmp(args[0], "history",7) == 0) 
		    {
		      for (k=0; k < 10; k++) printf ("%d %s \n", k , history[k]);  //print out history
		    }
	    
		}

	}
    }

