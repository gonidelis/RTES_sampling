#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
    struct timeval tv;

    float t = atoi(argv[1]);  //sampling time period in sec's
    float dt = atoi(argv[2]);   //sampling rate in msec's
    double time;
    int nsamples = t/dt * 1000; //number of samples floored

    //samples storage array
    double *samples;
    samples = malloc(nsamples * sizeof(*samples));
    printf("%d\n\n",nsamples);

    int c = 0;  //array index
    int er = 0; //error index
    double *error;   //error between wanted and actual timestamp
    error = malloc(nsamples * sizeof(*error));
    double divergance = 0;  //keeps history of past errors
    double factor = 0;  //average of all past errors

    //open rtes_1.txt file to record data for plotting
    FILE *f = fopen("rtes_1_errors.txt", "w");
    FILE *g = fopen("rtes_1_stamps.txt", "w");
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

    gettimeofday(&tv, NULL);
    time =(double) tv.tv_sec + tv.tv_usec / 1000000.0f;
    samples[c] = time;
    printf("time: %f\n", samples[c]);

    fprintf(g, "%f\n", time);    //record timestamps
    /*
    //casting check
    printf("%ld, %ld", tv.tv_sec, tv.tv_usec);
    return(0);
    */

    usleep(dt * 1000);

    while(c<nsamples){
      c++;

      gettimeofday(&tv, NULL);
      time = (double) tv.tv_sec + tv.tv_usec / 1000000.0f;
      samples[c] = time;

      //OPTIMIZATION
      //error calculated in msec's
      error[er] = (samples[c] - samples[c-1])*1000 - dt;

      //calculated divergance on recent errors
      if (er<3){
        divergance += error[er];
        factor = divergance /c;
      }else{
        divergance += error[er] - error[er-3];
        factor = divergance/3;
      }

      printf("%d\n",c );

      printf("time: %f\nerror: %f ms\ndivergence: %f\n", samples[c], error[er], divergance);
      printf("factor %f\n\n", factor);

      fprintf(g, "%f\n", time);    //record timestamps
      fprintf(f, "%f\n", error[er]);    //record errors

      usleep((dt-2*factor) *1000);


      er++;

    }

}
