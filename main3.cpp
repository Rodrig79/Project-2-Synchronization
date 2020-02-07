//fixed to Zu's specifications

#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <ctime>
#include <fstream>

void* intersection(void* param);
void* carSpawn(void* param);
void* signaler(void* param);
void run(int x);
std::string getCurrentTimeString();

struct Car {
	int ID = 0;
	std::string arrivalTime;
	std::string startTime;
	std::string endTime;
	std::string side;
};

int main(){
	srand(time(NULL));
	run(20);
	return 0;
}

int maxCars = 0;

void run(int numC) {
	maxCars = numC;
	
	void* spawnNArgs = new void*[6];
	void* spawnSArgs = new void*[6];
	void* signalArgs = new void*[7];
	
	std::queue<Car*>** buf = new std::queue<Car*>*[2];
	buf[0] = new std::queue<Car*>();
	buf[1] = new std::queue<Car*>();
	
	std::string** side = new std::string*[2];
	side[0] = new std::string("North");
	side[1] = new std::string("South");
	
	int* numCars = new int(0);
	int* carCount = new int(0);
	
	std::ofstream* carLog = new std::ofstream();
	std::ofstream* flagPersonLog = new std::ofstream();
	carLog -> open("car.log");
	flagPersonLog -> open("flagperson.log");
	
	pthread_mutex_t* bufLock = new pthread_mutex_t();
	pthread_mutex_t* fileLock = new pthread_mutex_t();
	sem_t* emptyLock = new sem_t();
	
	pthread_mutex_init(bufLock, NULL);//1
	pthread_mutex_init(fileLock, NULL);//2
	sem_init(emptyLock, 0, 0);//needed
	
	((int**)spawnNArgs)[0] = numCars;
	((int**)spawnSArgs)[0] = numCars;
	((std::queue<Car*>**)spawnNArgs)[1] = buf[0];
	((std::queue<Car*>**)spawnSArgs)[1] = buf[1];
	((pthread_mutex_t**)spawnNArgs)[2] = bufLock;
	((pthread_mutex_t**)spawnSArgs)[2] = bufLock;
	((sem_t**)spawnNArgs)[3] = emptyLock;
	((sem_t**)spawnSArgs)[3] = emptyLock;
	((std::string**)spawnNArgs)[4] = side[0];
	((std::string**)spawnSArgs)[4] = side[1];
	((std::ofstream**)spawnNArgs)[5] = flagPersonLog;
	((std::ofstream**)spawnSArgs)[5] = flagPersonLog;
	
	((std::queue<Car*>***)signalArgs)[0] = buf;
	((pthread_mutex_t**)signalArgs)[1] = bufLock;
	((sem_t**)signalArgs)[2] = emptyLock;
	((pthread_mutex_t**)signalArgs)[3] = fileLock;
	((int**)signalArgs)[4] = carCount;
	((std::ofstream**)signalArgs)[5] = carLog;
	((std::ofstream**)signalArgs)[6] = flagPersonLog;
	
	
	
	pthread_t SpawnNtid;
	pthread_t SpawnStid;
	pthread_t Signaltid;
	pthread_create(&SpawnNtid, NULL, &carSpawn, spawnNArgs);
	pthread_create(&SpawnStid, NULL, &carSpawn, spawnSArgs);
	pthread_create(&Signaltid, NULL, &signaler, signalArgs);
	pthread_join(SpawnNtid, NULL);
	pthread_join(SpawnStid, NULL);
	pthread_join(Signaltid, NULL);
	//do we really need to free all memory at the end of the program?
	//it gets freed anyway, we can just free the memory that needs to be deallocated
	// delete [] ((int*)spawnNArgs);
	// delete [] ((int*)spawnSArgs);
	// delete [] ((int*)signalArgs);
	// delete buf[0];
	// delete buf[1];
	// delete [] buf;
	// delete side[0];
	// delete side[1];
	// delete [] side;
	// delete numCars;
	// delete bufLock;
	// delete fileLock;
	// delete emptyLock;
}

int pthread_sleep(int seconds) {
	pthread_mutex_t mutex;
	pthread_cond_t conditionvar;
	struct timespec timetoexpire;
	if (pthread_mutex_init(&mutex, NULL))
		return -1;
	if (pthread_cond_init(&conditionvar, NULL))
		return -1;

	timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
	timetoexpire.tv_nsec = 0;
	return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
}

void* carSpawn(void* param) {
	int *numCars = ((int **)param)[0];
	std::queue<Car*>*buf = ((std::queue<Car*>**)param)[1];
	pthread_mutex_t* bufLock = ((pthread_mutex_t **)param)[2];
	sem_t *emptyLock = ((sem_t **)param)[3];
	std::string *side = ((std::string **)param)[4];

	while (*numCars < maxCars)
	{
		int randInt = (rand() % 10);
		while (randInt < 8 && *numCars < maxCars)
		{
			randInt = (rand() % 10);
			
			Car* newCar = new Car();
			newCar->side = *side;
			newCar->arrivalTime = getCurrentTimeString();
			newCar->ID = *numCars;
			pthread_mutex_lock(bufLock);
			if(*numCars >= maxCars){
				pthread_mutex_unlock(bufLock);
				delete newCar;
				return NULL;
			}
			buf->push(newCar);
			*numCars = *numCars + 1;
			//if value of semaphore is 0, log a wake up action
			//then post
			//what if it hasn't come around again, so it never slept in the first place
			sem_post(emptyLock);
			pthread_mutex_unlock(bufLock);
		}
		pthread_sleep(2);
		
	}
	
	// delete numCars;
	
	return NULL;
}

void* signaler(void* param) {
	int carCount = 0;
	std::queue<Car*>** buf = ((std::queue<Car*>***)param)[0];
	pthread_mutex_t* bufLock = ((pthread_mutex_t**)param)[1];
	sem_t* emptyLock = ((sem_t**)param)[2];
	pthread_mutex_t* fileLock = ((pthread_mutex_t**)param)[3];
	int* carCounter = ((int**)param)[4];
	std::ofstream* carLog = ((std::ofstream**)param)[5];
	bool side = false;
	
	while(carCount < maxCars){
		Car* c = NULL;
		
		//needs to log a went to sleep if things are empty
		//what if between the check and the wait theres something in the list so it didn't really sleep
		sem_wait(emptyLock);
		pthread_mutex_lock(bufLock);
		if(buf[side]->size() == 0 || buf[!side]->size() >= 10)
			side = !side;
		c = buf[side]->front();
		buf[side]->pop();
		pthread_mutex_unlock(bufLock);
		
		carCount++;
		c->startTime = getCurrentTimeString();
		
		void* args = new void*[4];
		((Car**)args)[0] = c;
		((int**)args)[1] = carCounter;
		((pthread_mutex_t**)args)[2] = fileLock;
		((std::ofstream**)args)[3] = carLog;
		pthread_t tid;
		pthread_create(&tid, NULL, &intersection, args);
		//this thread could return and main could begin deleting variables
		//before intersection is done with delete vars
		//deleting the queues here adds the same worry of deletion for the 
		//car spawning thread
		pthread_detach(tid);
	}
	return NULL;
}

void* intersection(void* param) {
	//need maxCars variable
    Car* c = ((Car**)param)[0];
    int* carCount = ((int**)param)[1];
    pthread_mutex_t* fileLock = ((pthread_mutex_t**)param)[2];
	std::ofstream* log = ((std::ofstream**)param)[3];
	
    pthread_sleep(1);
	c -> endTime = getCurrentTimeString();
	
    pthread_mutex_lock(fileLock);
	*carCount = *carCount + 1;
	*log << c -> ID << " " << c -> arrivalTime << " " << c -> startTime << " " << c -> endTime << std::endl;
	if(*carCount >= 20) {
		log -> close();
	}
	pthread_mutex_unlock(fileLock);
	
	//again the worry is the 20th rushes before the 19th car skips over the if statement
	//then the file gets closed and deleted before car 19 gets there
	//and the 20th deletes 
	delete c;
	// if(*carCount >= 20){
		// log -> close();
		// delete log;
		// delete fileLock;
		// delete carCount;
	// }
	// fflush(stdout); in main
	return NULL;
}

std::string getCurrentTimeString() {
	char buf[8];
	time_t Time;
	struct tm * info;
	Time = time(NULL);
	info = localtime(&Time);
	strftime(buf, 80, "%H:%M:%S", info);
	return std::string(buf);
}
