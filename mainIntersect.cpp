#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>  

struct Car {
	int val = 0;
	struct timespec arrivalTime;
  	struct timespec startTime;
  	struct timespec endTime;
};

int main(){
	std::queue<int> test;
	void* args = new void*[5];
	((Car**)args)[0] = c;
	((int**)args)[1] = intersectCount;
	((pthread_mutex_t**)args)[2] = intersectCountLock;
	((pthread_mutex_t**)args)[3] = intersectLock;
	((pthread_mutex_t**)args)[4] = fileLock;
	pthread_t tid;
		pthread_create(&tid, NULL, intersection, args);
	return 0;

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

void* carSpawn(void* param) {
	return param;
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
		pthread_create(&tid, NULL, intersection, args);
	}
	return param;
}

/*
	Sleeps for 1000 ms then writes the car to output file
	param = [car, intersectCount, intersectCountLock, intersectLock, fileLock]
*/
void* intersection(void* param) {

    int count = 0; // cnange later
    // unpack variables
    // sleep
    // car is in intersection

    // car c = (car**)param[0];
    // intersectCount = (int**)param[1];
    // pthread_mutex_t intersectCountLock = (mutex***)param[2];
    // pthread_mutex_t intersectLock = (mutex***)param[3];
    // pthread_mutex_t fileLock = (mutex***)param[3];

    while (count < 20){
        pthread_sleep(1);
		c.endTime = localtime();
		pthread_mutex_lock(intersectCountLock);
		*intersectCount--;
		
		if (*intersectCount == 0)
			pthread_mutex_unlock(intersectLock);

		pthread_mutex_unlock(intersectCountLock);
        pthread_mutex_lock(fileLock);
        carLog.open("car.log", ios_base::app);
        cout << setw(7) << c.id << setw(7) << c.arrivalTime; << setw(7) << c.startTime << setw(7) << c.endTime;
        carLog.close();
        pthread_mutex_unlock(fileLock);
        count++;
		delete c;
    }
}
