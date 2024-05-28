# Zombie-Slayer

This is a C program that simulates a scenario where zombies enter a room, and slayers try to kill them. The program uses POSIX threads (pthreads) to simulate the concurrent behavior of zombies entering the room and slayers killing them.

## Description

The program creates a specified number of "doorman" threads and three "slayer" threads. The doorman threads randomly decide whether to let a zombie enter the room or not, while the slayer threads kill zombies if there are any in the room.
The simulation continues until either 100 zombies have been killed or there are 100 or more zombies in the room. The program then prints the outcome (whether the zombies were defeated or not) and the number of zombies killed by each slayer.

## Usage

To run the program, compile the source code and execute the resulting binary with a single command-line argument specifying the number of doorman threads:
            
    ./zombie_slayer <number_of_doormen>

For example, to run the program with 5 doorman threads:

    ./zombie_slayer 5
    
## Implementation Details

- The program uses a mutex (`pthread_mutex_t`) to protect shared variables (`zombieCounter`, `killedCount`, and `slayerKilledCount`) from race conditions.
- The `zombieEntered()` function increments the `zombieCounter` when a new zombie enters the room.
- The `zombieKilled(int slayerId)` function increments the `killedCount` and the corresponding `slayerKilledCount[slayerId]`, and decrements the `zombieCounter` when a zombie is killed.
- The `tooManyZombiesInTheRoom()` function checks if there are 100 or more zombies in the room.
- The `killed100Zombies()` function checks if 100 or more zombies have been killed.
- The `zombiesExist()` function checks if there is at least one zombie in the room.
- The `_doorMan(void* arg)` function simulates the behavior of a doorman thread, allowing zombies to enter the room randomly.
- The `_slayer(void* arg)` function simulates the behavior of a slayer thread, killing zombies if there are any in the room.
- The `main()` function creates the doorman and slayer threads, waits for them to finish, and prints the final result.

Note: The program uses the `usleep()` function to simulate delays, but this may not be precise on all systems.
