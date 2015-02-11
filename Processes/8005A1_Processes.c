/********************************************************************************
* This program creates a fan of processes, 
* The processes are labelled by the value of i at the time they leave the loop.
* The original process creates n-1 children. 
*
* Each Process is given the task to determine all the prime numbers of a range
* with a maximum which is determined by the user.
* Each Process creates their own log files in which all the prime numbers are written
* 
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

/*----------- Function Prototypes ------------------*/
void OptimusPrime(int totalPrimes, int processNumber);

// Initializes all the Processes 
// Creates a timer which begins from the creation of the first process
// and ends at the termination of the final procss.
// 
// @param argc - number of user defined command line arguments
// @param argv - array of the string variables
//             - argv[0] - number of processes to be created
//             - argv[1] - max number of the range on which primes are determined
int main (int argc, char *argv[]) 
{
	pid_t childpid = 0; 
    struct timeval t1;
    struct timeval t2;
    double elapsedTime;
    int totalProcesses, totalComputations, i;

    //command line argument verification
    if(argc != 3)
    {
        printf("Illegal Usage");
        return 0;
    }
  
    totalProcesses = atoi(argv[1]);
    totalComputations = atoi(argv[2]);

    //This file will list the completion time of the execution
    FILE *ComputationTimeLog = fopen("ProcessTimes.txt", "a+");

    if (ComputationTimeLog == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    // start timer
    gettimeofday(&t1, NULL);

    //Fork new processes in a 'fan-like' fashion  
   	for (i = 1; i < totalProcesses; i++)
            //children break out of the loop
      		if ((childpid = fork()) <= 0)
         		break;

    OptimusPrime(totalComputations, i);
    
    // if the process is the parent
    if (childpid > 0)
    {
        // as the parent process, wait for all children to end
        // once there are no children left an ECHILD error will occur
        while (waitpid(-1, NULL, 0))
        {
            // occurs when there are no children left
            if (errno == ECHILD)
            {
                // end timer
                gettimeofday(&t2, NULL);

                // calculate the elapsed time and print it to the log file and stdout
                elapsedTime = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
                printf("%lf\n", elapsedTime);
                fprintf(ComputationTimeLog, "TOTALPROCESSES: %d|| TOTALCOMPUTATIONS: %d|| ELAPSEDTIME:%lf\n", totalProcesses, totalComputations, elapsedTime);
                fclose(ComputationTimeLog);

                break;
            }
        }
    }


   	return 0; 
}

// OptimusPrime
// Determines all the prime numbers from 0 to a given maximum and
// prints all the prime numbers into a newly created .txt file.
// 
// @param totalPrimes - the maximum number of a range starting at 0, of which primes numbers are determined
// @param processNumber - the process id of the process calling this function
//
void OptimusPrime(int totalPrimes, int processNumber)
{
    int count, c, halfCount;
    char fileName[50];

    sprintf(fileName, "%d", processNumber);
    strcat(fileName, "-Process_PrimeNumLog.txt");

    //This file will list the primenumbers
    FILE *PrimeNumberLogFile = fopen(fileName, "w");

    if (PrimeNumberLogFile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Find all prime numbers from 3 to totalprimes (specified by user)
    for ( count = 3 ; count <= (int)totalPrimes; count+=2)
    {
        halfCount = count / 2;

        for ( c = 2 ; c < halfCount ; c++ )
        {
            if ( count%c == 0 )
                break;
        }
        if ( c >= halfCount )
        {
            fprintf(PrimeNumberLogFile, "%d\n",count);
        }
    }

    fclose(PrimeNumberLogFile);
    printf("FINISHED PROCESS NUMBER: %d\n", processNumber);
}
