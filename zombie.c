/*autor:
Jelsin Stiben Sanchez Almanza
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

int zombieCounter = 0; /* number of zombies in the room */
int killedCount = 0; /* number of zombies killed */
int slayerKilledCount[3] = {0}; /* number of zombies killed by each slayer */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Keeps track of number of zombies entered */
void zombieEntered() {
    pthread_mutex_lock(&mutex);
    zombieCounter++;
    pthread_mutex_unlock(&mutex);
}

/* Keeps track of number of zombies killed */
void zombieKilled(int slayerId) {
    pthread_mutex_lock(&mutex);
    killedCount++;
    slayerKilledCount[slayerId]++;
    zombieCounter--;
    pthread_mutex_unlock(&mutex);
}

/* Returns true if number of zombies in the room are greater than or equal to 100 */
int tooManyZombiesInTheRoom() {
    pthread_mutex_lock(&mutex);
    int result = (zombieCounter >= 100);
    pthread_mutex_unlock(&mutex);
    return result;
}

/* Returns true if more than 100 zombies have been killed */
int killed100Zombies() {
    pthread_mutex_lock(&mutex);
    int result = (killedCount >= 100);
    pthread_mutex_unlock(&mutex);
    return result;
}

/* Returns true if there is at least one zombie in the room */
int zombiesExist() {
    pthread_mutex_lock(&mutex);
    int result = (zombieCounter > 0);
    pthread_mutex_unlock(&mutex);
    return result;
}

/* doorman thread */
void *doorMan(void *arg) {
    while (!tooManyZombiesInTheRoom() && !killed100Zombies()) {
        if (rand() % 2 == 0) {
            usleep(2000); /* 2ms delay */
            zombieEntered();
        }
    }
    return NULL;
}

/* slayer thread */
void *slayer(void *arg) {
    int slayerId = *((int *)arg);
    while (!tooManyZombiesInTheRoom() && !killed100Zombies()) {
        if (zombiesExist()) {
            usleep(2000); /* 2ms delay */
            zombieKilled(slayerId);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <number_of_doormen>\n", argv[0]);
        return 1;
    }

    int numDoormen = atoi(argv[1]);
    pthread_t doormanThreads[numDoormen];
    pthread_t slayerThreads[3];
    int slayerIds[3] = {0, 1, 2};

    /* Create doorman threads */
    for (int i = 0; i < numDoormen; i++) {
        pthread_create(&doormanThreads[i], NULL, doorMan, NULL);
    }

    /* Create slayer threads */
    for (int i = 0; i < 3; i++) {
        pthread_create(&slayerThreads[i], NULL, slayer, &slayerIds[i]);
    }

    /* Wait for all threads to finish */
    for (int i = 0; i < numDoormen; i++) {
        pthread_join(doormanThreads[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(slayerThreads[i], NULL);
    }

    /* Print the result */
    if (killed100Zombies()) {
        printf("You have killed 100 zombies!\n");
    } else {
        printf("You have been killed by the zombies!\n");
    }

    printf("Number of zombies killed by each slayer:\n");
    for (int i = 0; i < 3; i++) {
        printf("Slayer %d: %d\n", i + 1, slayerKilledCount[i]);
    }

    return 0;
}
