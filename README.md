Installation
===
Install the following tools
1. sudo apt-get install cmake
2. sudo apt-get install gcc
3. sudo apt-get install g++
4. sudo apt-get install libgtest-dev
5. sudo apt-get install libspdlog-dev
5. sudo apt-get install libcli11-dev


```
/*
             [XXXXXXXXXXXXX)              //  curr state, the state we are reasoning about !!!
                           [_____)        //  next state, used to understand where curr ends in time

             ^             ^
             |             |
             |             \-------------     begin of the current state 
             \---------------------------     end of the current state
[lo...hi]                                 //  case 1 - ends before the current state
        [lo...hi]                         //  case 2 - starts before but ends within the current state
                [lo...hi]                 //  case 3 - totally included within the current
                        [lo...hi]         //  case 4 - starts within but ends after the current state
                                [lo...hi] //  case 5 - starts after the current state
          [lo...................hi]       //  case 6 - fully includes the current state

a Uw[lo:hi] b  = start@(
     (
           (X timestamp - start.timestamp <= lo)   //  case 5
       ||  (timestamp - start.timestamp <= hi)     //  case 2/3/4/6
       &&  a
     ) U (
           (X timestamp - start.timestamp > lo) &&  b  
       ||  (timestamp - start.timestamp > hi)
     )
   )
*/

```
