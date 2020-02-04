#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <ctime>
#include <string>

struct Car {
	int val = 0;
	string side;
};

int main(){
	srand(time(NULL)); //initialize random number generator
	std::queue<int> test;
	return 0;
}

int pthread_sleep(int seconds)
{
    pthread_mutex_t mutex;
    pthread_cond_t conditionvar;
    struct timespec timetoexpire;
    if (pthread_mutex_init(&mutex, NULL))
    {
        return -1;
    }
    if (pthread_cond_init(&conditionvar, NULL))
    {
        return -1;
    }
    //When to expire is an absolute time, so get the current time and add
    //it to our delay time
    timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
    timetoexpire.tv_nsec = 0;
    return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
}

//parameter is an array of the format [buffer, lock for buffer]
//buffer is a queue


//spawning cars
//signaler
//intersection

/*
	spawns cars according to the rules specified and inserts them at the back of a queue
	param = [numCars, buf, bufLock, emptyLock, side]
*/

void *carSpawn(void *param)
{
    int *numCars = ((int **)param)[0];
    queue<Car *> *buf = ((int **)param)[1];
    pthread_mutex_t **bufLock = ((pthread_mutex_t ***)param)[2];
    sem_t *emptyLock = ((sem_t **)param)[3];
    string *side = ((string **)param[4]);
	int *maxCars = ((int **)param)[5];
	pthread_mutex_t **numCarsLock = ((pthread_mutex_t ***)param)[6];

    //create cars
    int randInt = (rand() % 10); //generates an int 0-9
    int carsSpawning = 1;
    //80% chance of spawning another car
    while (numCars > 0)
    {
        while (randInt < 8)
        {
        cout << randInt << endl;  
	randInt = (rand() % 10);
        }

        cout << carsSpawning << " cars created going ";
        if (side = 'n')
        {
            cout << "North" << endl;
        }
        else
        {
            cout << "South" << endl;
        }

        //fill list

	*numCars = *numCars + carsSpawning
        for (int i = 0, i < carsSpawning; i++){
            
            //insert car into queue
        }
            pthread_sleep(20);
    }
/*
	signals cars taking from both sides according to specified rules
	param = [buf[2], bufLock[2], emptyLock, intersectCount,intersectCountLock, intersectLock, fileLock]
*/

void* signaler(void* param) {
	//0 is North 1 is South
	
	int carCount = 0;
	std::queue<Car*>* buf = ((std::queue<Car*>**)param)[0];
	pthread_mutex_t** bufLock = ((pthread_mutex_t***)param)[1];
	sem_t* emptyLock = ((sem_t**)param)[2];
	int* intersectCount = ((int**)param)[3];
	pthread_mutex_t* intersectCountLock = ((pthread_mutex_t**)param)[4];
	pthread_mutex_t* intersectLock = ((pthread_mutex_t**)param)[5];
	pthread_mutex_t* fileLock = ((pthread_mutex_t**)param)[6];
	bool side = false;
	
	while(carCount < 20){
		
		//false is north, true is south
		sem_wait(emptyLock);
		pthread_mutex_lock(bufLock[0]);
		pthread_mutex_lock(bufLock[1]);
		if(buf[side].size() == 0 || buf[!side].size() >= 10) {
			pthread_mutex_unlock(bufLock[0]);
			pthread_mutex_unlock(bufLock[1]);
			pthread_mutex_lock(intersectLock);
			pthread_mutex_unlock(intersectLock);
			side = !side;
			pthread_mutex_lock(bufLock[side]);
		} else {
			pthread_mutex_unlock(bufLock[!side]);
		}
		//at this point only bufLock[side] should be locked
		Car* c;
		c = buf[side].front();
		buf[side].pop();
		if(buf[side].size() == 0) {
			sem_post(emptyLock);
		}
		pthread_mutex_unlock(bufLock[side]);
		pthread_mutex_lock(intersectCountLock);
		if(intersectCount == 0) {
			pthread_mutex_lock(intersectLock);
		}
		intersectCount++;
		pthread_mutex_unlock(intersectCountLock);
		void* args = new void*[5];
		((Car**)args)[0] = c;
		((int**)args)[1] = intersectCount;
		((pthread_mutex_t**)args)[2] = intersectCountLock;
		((pthread_mutex_t**)args)[3] = intersectLock;
		((pthread_mutex_t**)args)[4] = fileLock;
		
		pthread_t tid;
		pthread_create(&tid, NULL, &intersection, args);
	}
	return param;
}

/*
	Sleeps for 1000 ms then writes the car to output file
	param = [car, intersectCount, intersectCountLock, intersectLock, fileLock]
*/
void* intersection(void* param) {
	return param;
}
