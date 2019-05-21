#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define TEMP_HIGH 35
#define TEMP_LOW 18
#define PRESS_HIGH 1015
#define PRESS_LOW 1005
#define HUM_HIGH 61
#define HUM_LOW 41
sem_t s1,s2,s3,dis1;
int temp_value,humid_value,press_value;
int closefirst=0;
int closesecond=0;
int closethree=0;

void* humidity(void* args){
sem_wait(&s1);
FILE fl = fopen("humidity.txt" , "r+");
if(fl == NULL){
printf("Humidity doesnt exist");
}
else{
 while (fscanf(fl, "%d", &humid_value) == 1)
    {
        sleep(2);
    }
closesecond =1;
    fclose(fl);
}
sem_post(&s1);
    pthread_exit(NULL);
}


void pressure(void* args){
sem_wait(&s2);
FILE fp = fopen("pressure.txt" , "r+");
if(fp == NULL){
printf("press girilmedi");
}
else{
 while (fscanf(fp, "%d", &press_value) == 1)
    {
        sleep(2);
    }
closethree =1;
    fclose(fp);
}
sem_post(&s2);
    pthread_exit(NULL);
}


void temperature(void* args){
sem_wait(&s1);
FILE *ft = fopen("temperature.txt" , "r+");
if(ft == NULL){
printf("Temp value doesnt exits");
}
else{
 while (fscanf(ft, "%d", &temp_value) == 1)
    {
        sleep(2);
    }
closefirst=1;
    fclose(ft);
}
sem_post(&s1);
    pthread_exit(NULL);
}


void* dispal(void* args){
sem_wait(&dis1);
while(closefirst==0  closesecond==0  closethree==0){
sleep(2);
if(TEMP_HIGH <= temp_value)
printf("tempdisplay : temperature is %d , too high tempalarm : on\n\n",temp_value);
if(TEMP_LOW >= temp_value)
printf("tempdisplay : temperature is %d , too low tempalarm : on\n\n",temp_value);
if(HUM_HIGH <= humid_value)
printf("humidity : humidity is %d , too high humidalarm : on\n\n",humid_value);
if(HUM_LOW >= humid_value)
printf("humidity : humidity is %d , too low  humidalarm : on\n\n",humid_value);
if(PRESS_HIGH <= press_value)
printf("pressdisplay : pressure is %d , too high  pressalarm : on\n\n",press_value);
if(PRESS_LOW >= press_value)
printf("pressdisplay : pressure is %d , too low pressalarm : on\n\n",press_value);

if(TEMP_HIGH > temp_value && TEMP_LOW < temp_value)
printf("tempdisplay : temperature is %d ,  tempalarm : off \n\n",temp_value);

if(HUM_HIGH > humid_value && HUM_LOW < humid_value)
printf("humidity : humidity is %d ,  humidityalarm : off \n\n",humid_value);

if(PRESS_HIGH > press_value && PRESS_LOW < press_value)
printf("pressdisplay : pressure is %d ,  pressalarm : off\n\n",press_value);

}
sem_post(&dis1);
    pthread_exit(NULL);
}
int main(){
pthread_t  thread1,thread2,thread3,thread4;
sem_init(&s1, 0,1);
sem_init(&s2,0,1);
sem_init(&s3,0,1);
sem_init(&dis1,0,1);
pthread_create(&thread1,NULL,temperature,NULL);
pthread_create(&thread2,NULL,humidity,NULL);
pthread_create(&thread3,NULL,pressure,NULL);
pthread_create(&thread4,NULL,dispal,NULL);
pthread_join(thread4,NULL);
pthread_join(thread3,NULL);
pthread_join(thread2,NULL);
pthread_join(thread1,NULL);
sem_destroy(&s1);
sem_destroy(&s2);
sem_destroy(&s3);
sem_destroy(&dis1);
return 0;
}