# Project-2-Synchronization
Description:
In this project, you will be modeling a common roadway occurrence, where a lane is closed and a flag
person is directing traffic.
The figure above shows the scenario. We have one lane closed of a two-lane road, with traffic coming
from the North and South. Because of traffic lights, the traffic on the road comes in bursts. When a car
arrives, there is an 80% chance of another car following it, but once no car comes, there is a 20 second
delay (use the provided pthread_sleep) before any new car will come.
During the times when no cars are at either end, the flag person will fall asleep. When a car arrives at
either end, the flag person will wake up and allow traffic from that side to pass the construction area, until
there are no more cars from that side, or until there are 10 cars or more lining up on the opposite side. If
there are 10 cars or more on the opposite side, the flag person needs to allow the cars from the opposite
side to pass.
Each car takes 1 second (use the provided pthread_sleep) to go through the construction area.
Your job is to construct a simulation of these events where under no conditions will a deadlock occur. A
deadlock could either be that the flag person does not allow traffic through from either side, or let’s traffic
through from both sides causing an accident.

1. Requirements
You are required to implement a C/C++ multi-threaded program that uses pthread library and avoids
deadlock.
You are required to use pthread synchronization structs such as semaphores and mutex locks to do
synchronization. But, using mutex locks only for synchronization is never a right solution to this problem.
Remember: Right synchronization should keep maximum concurrency!
You are required to output all of the necessary events into two files named “car.log” and
“flagperson.log”. For “car.log”, you need sequentially number (carID) each car (i.e., carID begins with 1)
and indicates the direction it is heading (S or N), the arrival time on the road, the time to start passing the
construction area (start-time), and the time to exit the construction area (end-time). The “car.log” looks
like:
carID direction arrival-time start-time end-time
1 S 12:25:11 12:25:12 12:25:13
2 N 12:25:13 12:25:13 12:25:14
The “flagperson.log” tracks when the flag person goes to sleep and when he/she wakes up to work. The
file looks like:
Time State
12:10:11 sleep
12:25:11 woken-up

Design always goes first. In this assignment, we need a README file that must have the following
components:
• Team member’s names and respective contributions
• How many threads? What is the task for each thread? What is the corresponding thread
function name in your code?
• How many semaphores? Describe each semaphore variable’s name, initial value and purpose.
• How many mutex locks? Describe each mutex lock’s name and purpose.
• Strengths
• Weaknesses.
Please use the provided README template file for this project.
