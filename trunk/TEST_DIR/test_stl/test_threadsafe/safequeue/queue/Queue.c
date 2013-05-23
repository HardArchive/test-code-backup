
#ifdef WIN32
#include "windows.h"
#else
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#include "Queue.h"

int qu_init( qu_buffer_t *qu, int unit_size, int unit_number )
{
    qu->buffer_size = unit_size * unit_number;
    qu->buffer = malloc(qu->buffer_size);
    qu->index_input = 0;
    qu->index_output = 0;
    qu->unit_count = 0;
    qu->unit_number = unit_number;
    qu->unit_size = unit_size;

    if(NULL == qu->buffer)
    {
        printf("memory error\n");
        exit(1);
    }

#ifdef WIN32
    qu->hSemaphoreIn = CreateSemaphore(NULL, unit_number - 3, unit_number, NULL);
    qu->hSemaphoreOut = CreateSemaphore(NULL, 0, unit_number, NULL);
#else
    pthread_mutex_init(&qu->mutex, NULL);

    sem_init(&qu->semaphore_in, 0, unit_number - 3);
    sem_init(&qu->semaphore_out, 0, 0);
#endif

    return 0;
}

int qu_finalize(qu_buffer_t *qu)
{
    free(qu->buffer);

#ifdef WIN32
    CloseHandle(qu->hSemaphoreIn);
    CloseHandle(qu->hSemaphoreOut);
#else
    pthread_mutex_destroy(&(qu->mutex));

//     sem_destroy(&qu->semaphore_in);
//     sem_destroy(&qu->semaphore_out);
#endif

    return 0;
}

int qu_input_lock(qu_buffer_t *qu, void ** buffer, int wait_time)
{
#ifdef WIN32
    WaitForSingleObject(qu->hSemaphoreIn, wait_time);
#else
    pthread_mutex_lock(&(qu->mutex));

//     struct timespec ts;
//     ts.tv_nsec = time(NULL) + wait_time / 1000;
//     ts.tv_nsec = (wait_time % 1000) * 1000000;
//     if(0 != sem_timedwait(&qu->semaphore_in, &ts))
//     {
//         *buffer = NULL;
//         return -1;
//     }
#endif

    *buffer = ((unsigned char *)qu->buffer) + (qu->unit_size * qu->index_input);

    return 0;
}

int qu_input_release(qu_buffer_t *qu)
{
    if(qu->unit_count < qu->unit_number - 3)
    {
        qu->index_input++;
        qu->index_input %= qu->unit_number;
        qu->unit_count++;
    }

#ifdef WIN32
    ReleaseSemaphore(qu->hSemaphoreOut, 1, NULL);
#else
    pthread_mutex_unlock(&qu->mutex);
    pthread_cond_broadcast(&qu->cond);

    // sem_post(&qu->semaphore_out);
#endif

    return 0;
}

int qu_output_lock(qu_buffer_t *qu, void ** buffer, int wait_time)
{
#ifdef WIN32
    DWORD dwReturn = WaitForSingleObject(qu->hSemaphoreOut, wait_time);
    if(WAIT_OBJECT_0 != dwReturn)
    {
        *buffer = NULL;
        return -1;
    }
#else
    pthread_mutex_lock(&qu->mutex);
    while(qu->unit_count <= 0)
    {
        pthread_cond_wait(&qu->cond, &qu->mutex);
    }

//     struct timespec ts;
//     ts.tv_nsec = time(NULL) + wait_time / 1000;
//     ts.tv_nsec = (wait_time % 1000) * 1000000;
//     if(0 == sem_timedwait(&qu->semaphore_out, &ts))
//     {
//         *buffer = NULL;
//         return -1;
//     }
#endif

    *buffer = ((unsigned char *)qu->buffer) + (qu->unit_size * qu->index_output);

    return 0;
}

int qu_output_release(qu_buffer_t *qu)
{
    if(qu->unit_count > 0)
    {
        qu->index_output++;
        qu->index_output %= qu->unit_number;
        qu->unit_count--;
    }
#ifdef WIN32
    ReleaseSemaphore(qu->hSemaphoreIn, 1, NULL);
#else
    pthread_mutex_unlock(&qu->mutex);

    // sem_post(&qu->semaphore_in);
#endif
    return 0;
}

