/*
Consumer Wait:
1) Car in interesction traverling opposite direction that wants to take from
2) No cars

Producer Wait:
Never, unbounded
*/
#include <iostream>
#include <phtread.h>
#include <semaphore.h>

/*
Shared Variables:
Buf[2]
Mut[2]
Empty
Intersect
InterCount
IntLock (Mutex)


Local Variable for Consumer
Bool side
*/


//Consumer:
wait(empty);
wait(mutex);
//access buffer
if(size == 0){
    signal()

}
wait(mutex)


wait(empty)
wait(Buf[side])
wait(Buf[side])
if(Buf[side].size()==0||Buf[!side].size>=10){
    [size!=side];
}
signal(Buf[!side]);
car = Buf[side].pop();
if(Buf[side]).size!=0)
    signal(empty)
signal(Buf[side]);

//Consume Car here
}
