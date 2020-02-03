#include <iostream>
#include <queue>

int main(){
	std::queue<int> test;
	return 0;
}

/*
	signals cars taking from both sides according to specified rules
	param = [buf[2], bufLock[2], emptyLock, intersectCount,intersectCountLock, intersectLock]
*/
void* carSpawn(void* param) {
	
}

/*
	spawns cars according to the rules specified and inserts them at the back of a queue
	param = [buf, bufLock, emptyLock]
*/
void* signaler(void* param) {
	
}

/*
	Sleeps for 1000 ms then writes the car to output file
	param = [car, intersectCount, intersectCountLock, intersectLock, fileLock]
*/
void* intersection(void* param) {
	
}

struct Car {
	int val = 0;
};
