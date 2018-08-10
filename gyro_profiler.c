#include <stdio.h>
#include <czmq.h>
#include <pthread.h>
#include "lib/stick_sdk.h"

typedef struct{
    pthread_mutex_t mp;
    pthread_t thread;
    int is_stop;
    void (*callback)(short* gyro, short* accel);
} profiler_handle_t;

int is_stop(profiler_handle_t* handle){
    int ret;
    pthread_mutex_lock(&(handle->mp));
    ret = handle->is_stop;
    pthread_mutex_unlock(&(handle->mp));
    return ret;
}

void stop(profiler_handle_t* handle, int is_stop){
    pthread_mutex_lock(&(handle->mp));
    handle->is_stop = is_stop;
    pthread_mutex_unlock(&(handle->mp));
}
void* profile_func(void* handle)
{
    profiler_handle_t* profiler = (profiler_handle_t*)handle; 
    zsock_t* pub = zsock_new_pub("@tcp://*:5801");

    printf("publisher thread has started.\n");
    short gyro[3], accel[3];
    while(!is_stop(profiler)){
        get_accel(accel);
        get_gyro(gyro);
        zstr_sendf(pub,
                "{\"gx\":%d, \"gy\":%d, \"gz\":%d, \"ax\":%d, \"ay\":%d, \"az\":%d}" , 
                gyro[0],
                gyro[1],
                gyro[2],
                accel[0],
                accel[1],
                accel[2]);
        profiler->callback(gyro, accel);
    }
    zsock_destroy(&pub);
    printf("publisher thread has terminated.\n");
    return NULL;
}

void *init_gyro_profiler()
{
    profiler_handle_t* handle = (profiler_handle_t*)malloc(sizeof(profiler_handle_t));
    memset(handle, 0, sizeof(profiler_handle_t));
    handle->mp = PTHREAD_MUTEX_INITIALIZER;

    if(!init_sdk()){
          return NULL;
    }
    stop_led_demo();
    pthread_mutex_init(&(handle->mp), NULL);
}

int start_gyro_profiler(void* handle, void(*callback)(short* gyro, short* accell))
{
    profiler_handle_t* profiler = (profiler_handle_t*)handle;
    profiler->callback = callback;
    pthread_create(&(profiler->thread), NULL, &profile_func, handle);  
    return 1;
}

void stop_gyro_profiler(void* handle)
{
    profiler_handle_t* profiler = (profiler_handle_t*)handle;
    stop(profiler, 1);
    pthread_join(profiler->thread, NULL);
    free((profiler_handle_t*)handle);
}


