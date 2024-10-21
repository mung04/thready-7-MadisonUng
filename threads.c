// RUNNING EITHER PATTERN WILL RESULT IN INFINITE LOOP

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

struct t_args {
    int num_of_threads;
    int count;
};

void* pattern_1(void *args) {
    struct t_args *thread_args = (struct t_args *)args;
    thread_args->num_of_threads--;
    pthread_t thread;
    pthread_create(&thread, NULL, pattern_1, thread_args);
    pthread_join(thread, NULL);
    pthread_t id = pthread_self();

    if (id <= 0) {
        printf("Thread %d beginning (PID: %d)\n", thread_args->num_of_threads + 1, getpid());
        exit(0);
    }
    else if (id > 0) {
        wait(NULL);
        sleep(1);
        printf("Thread %d exiting\n", thread_args->num_of_threads + 1);
    }
    else {
        printf("Thread failed\n");
    }

    if (thread_args->num_of_threads == 0) {
        exit(0);
    }
}

void* pattern_2(void *args) {
    struct t_args *thread_args = (struct t_args *)args;
    thread_args->num_of_threads--;
    pthread_t thread;
    pthread_create(&thread, NULL, pattern_2, thread_args);
    pthread_join(thread, NULL);
    pthread_t id = pthread_self();

    if (thread_args->num_of_threads == 1) {
        wait(NULL);
        sleep(1);
        printf("Thread %d (PID: %d) exiting\n", thread_args->count, getpid());
    }

    if (id > 0) {
        wait(NULL);
        sleep(1);
        printf("Thread %d (PID: %d) exiting\n", thread_args->count, getpid());
    }
    else if (id <= 0) {
        printf("Thread %d (PID: %d, PPID: %d) creating Thread %d\n", thread_args->count,
                getpid(), getppid(), thread_args->count + 1);
        thread_args->count++;
        pthread_t new_thread;
        pthread_create(&new_thread, NULL, pattern_2, thread_args);
        pthread_join(new_thread, NULL);
        exit(EXIT_SUCCESS);
    }
    else {
        printf("Thread failed\n");
    }

    if (thread_args->num_of_threads == 0) {
        exit(0);
    }
}

int main(int argc, char* argv[]) {

    struct t_args thread_args;
    thread_args.num_of_threads = atoi(argv[1]);
    int pattern_num = atoi(argv[2]);

    if (thread_args.num_of_threads < 1) {
        printf("Number of threads must be at least 1\n");
    }

    if (pattern_num != 1 && pattern_num != 2) {
        printf("Pattern number must be either 1 or 2\n");
    }
    else {
        pthread_t thread;

        if (pattern_num == 1) {
            printf("Running Pattern 1...\n");
            pthread_create(&thread, NULL, pattern_1, (void*)&thread_args);
            pthread_join(thread, NULL);
        }
        else {
            printf("Running Pattern 2...\n");
            thread_args.count = 1;
            pthread_create(&thread, NULL, pattern_2, (void*)&thread_args);
            pthread_join(thread, NULL);
        }
    }

    return 0;
}