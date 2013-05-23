
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <semaphore.h>

typedef struct qu_buffer_s
{
    int index_input;
    int index_output;
    int unit_count;
    int unit_number;
    int unit_size;
    int buffer_size;
    void *buffer;

#ifdef  WIN32
    HANDLE hSemaphoreIn;
    HANDLE hSemaphoreOut;
#else
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    sem_t semaphore_in;
    sem_t semaphore_out;
#endif
} qu_buffer_t;

int qu_init(qu_buffer_t *qu, int unit_size, int unit_number);
int qu_finalize(qu_buffer_t *qu);
int qu_input_lock(qu_buffer_t *qu, void ** buffer, int wait_time);
int qu_input_release(qu_buffer_t *qu);
int qu_output_lock(qu_buffer_t *qu, void ** buffer, int wait_time);
int qu_output_release(qu_buffer_t *qu);

#endif
