/********************************************************************************
* This program creates multiple Threads, 
* 
* Each Thread is given the task to determine all the prime numbers of a range
* with a maximum which is determined by the user.
* Each Thread creates their own log files in which all the prime numbers are written
* 
********************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#define _REENTRANT
#define DCE_COMPAT

/*----------- Function Prototypes ------------------*/
void* OptimusPrime(void* totalPrimes);

// Struct that is passed in to the creation of each thread
typedef struct threadVariables{
  int totalPrimes;
  int threadNumber;
}threadVariables;

// Initializes all the Threads 
// Creates a timer which begins from the creation of the first thread
// and ends at the termination of the final procss.
// 
// @param argc - number of user defined command line arguments
// @param argv - array of the string variables
//             - argv[0] - number of threads to be created
//             - argv[1] - max number of the range on which primes are determined
int main (int argc, char* argv[])
{
  struct timeval t1;
  struct timeval t2;
  double elapsedTime;
  int totalThreads, totalComputations, i, j;
  pthread_t thread[atoi(argv[1])];
  threadVariables *threadVariables;

  //command line argument verification
  if(argc != 3)
  {
    printf("Illegal Usage");
    return 0;
  }
  
  totalThreads = atoi(argv[1]);
  totalComputations = atoi(argv[2]);

  //This file will list the completion time of the execution
  FILE *ComputationTimeLog = fopen("ThreadTimes.txt", "a+");

  if (ComputationTimeLog == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }

  //start timer
  gettimeofday(&t1, NULL);

  //Create new threads
  for(i = 0; i < totalThreads; i++)
  {
    threadVariables = malloc(sizeof (struct threadVariables));
    threadVariables->totalPrimes = totalComputations;
    threadVariables->threadNumber = i;
    pthread_create (&thread[i], NULL, OptimusPrime, threadVariables);
    //pthread_create (&thread[i], NULL, OptimusPrime, (void*)totalPrimes);
  }

  //wait for all threads to end before continuing 
  for(j = 0; j < totalThreads; j++)
  {
    pthread_join (thread[j], NULL);
  }
  
  //end timer
  gettimeofday(&t2, NULL);
    
  // calculate the elapsed time and print it to the log file and stdout
  elapsedTime = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
  printf("%lf\n", elapsedTime);
  fprintf(ComputationTimeLog, "TOTALTHREADS:%d || TOTALCOMPUTATIONS:%d || ELAPSEDTIME:%lf\n", totalThreads, totalComputations, elapsedTime);
  fclose(ComputationTimeLog);

  return 0;
}

// OptimusPrime
// Determines all the prime numbers from 0 to a given maximum and
// prints all the prime numbers into a newly created .txt file.
// 
// @param threadVars - pointer to a struct which contains the maximum range and the thread number 
//
void* OptimusPrime(void* threadVars)
{
    int count, c, halfCount;
    char fileName[50];
    threadVariables *threadVariables = threadVars;

    sprintf(fileName, "%d", threadVariables->threadNumber);
    strcat(fileName, "-Thread_PrimeNumLog.txt");

    //This file will list the prime numbers
    FILE *PrimeNumberLogFile = fopen(fileName, "w");

    if (PrimeNumberLogFile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Find all prime numbers from 3 to totalprimes (specified by user) 
    for ( count = 3 ; count <= threadVariables->totalPrimes; count+=2)
    {
        halfCount = count / 2;

        for ( c = 2 ; c < halfCount ; c++ )
        {
            if ( count%c == 0 )
                break;
        }
        if ( c >= halfCount )
        {
            fprintf(PrimeNumberLogFile, "%d\n", count);
        }
    }

    fclose(PrimeNumberLogFile);
    free(threadVariables);
    printf("FINISHED THREAD NUMBER: %d\n", threadVariables->threadNumber);
}
