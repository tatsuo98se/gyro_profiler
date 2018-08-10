void *init_gyro_profiler();
int start_gyro_profiler(void* handle, void(*)(short* gyro, short* accell));
void stop_gyro_profiler(void* handle);

