#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include  <signal.h>

int exit_loop=0;

void  ALARMhandler(int sig)
{
  signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  printf("Hello\n");
  exit_loop = 1;
  signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}


int main(int argc, char **argv){
    struct timeval tv;

    float t = atoi(argv[1]);  //sampling time period in sec's
    float dt = atoi(argv[2]);   //sampling rate in msec's
    double time;
    int nsamples = t/dt * 1000; //number of samples floored
    int c = 0;

    FILE *f = fopen("rtes_2_errors.txt", "w");
    FILE *g = fopen("rtes_2_stamps.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    if (g == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int er = 0; //error index
    double *error;   //error between wanted and actual timestamp
    error = malloc(nsamples * sizeof(*error));

    //samples storage array
    double *samples;
    samples = malloc(nsamples * sizeof(*samples));

    gettimeofday(&tv, NULL);
    time = (double) tv.tv_sec + tv.tv_usec / 1000000.0f;
    samples[c] = time;

    fprintf(g, "%f\n", time);    //record timestamps

    usleep(dt*1000);
    printf("%f\n", time);


    signal(SIGALRM, ALARMhandler);
    alarm(t);                     /* set alarm clock          */
    while (exit_loop == 0){
      c++;

      gettimeofday(&tv, NULL);
      time = (double) tv.tv_sec + tv.tv_usec / 1000000.0f;
      samples[c] = time;

      error[er] = (samples[c] - samples[c-1])*1000 - dt;
      printf("time: %f\nerror: %f ms\n", samples[c], error[er]);

      fprintf(g, "%f\n", time);    //record timestamps
      fprintf(f, "%f\n", error[er]);

      usleep(dt*1000);
    };
    fclose(f);


    return 0;
}
