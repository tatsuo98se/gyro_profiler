#include <stdio.h>
#include "gyro_profiler.h"

void callback(short* gyro, short* accel){
    printf("%d, %d\r", *(gyro), *(accel));
}


int main (void)
{
    void* handle = init_gyro_profiler();
    start_gyro_profiler(handle, callback);

    for(;;){
	printf("*** pressing 'q' then 'Enter' quite the program. ***\n");
	if(getchar() == 'q'){
	    break;
	}
    }
    
    stop_gyro_profiler(handle);
    printf("end of program.");
    return 0;
}

