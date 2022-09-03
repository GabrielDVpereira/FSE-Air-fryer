#include <stdio.h>
#include <time.h>

FILE *fpt;
void init_csv_log(){
    fpt = fopen("log.csv", "w+");
    fprintf(fpt,"Hora, Temp. Interna, Temp. Ambiente, Temp. Referencia, Atuador\n");
}

void writte_csv_log(float ti, float room_temp, float tr, int pwm){
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
 
  fprintf(fpt,"%d:%d, %f, %f, %f, %d\n",  timeinfo->tm_hour, timeinfo->tm_min, ti, room_temp, tr, pwm);
}

void close_csv_file(){
    fclose(fpt);
}