# trade

## about
The application process messages to add/delete/modify order to buy/sell
give amount of something at given price.
* After each message all matched sell and buy orders are processed and
print out information about deal. Example:
```
Trade: 2131->1934: 11@116 (total: 11@116)
```
Two orders were executed: 2131 sold 11 of something at 116 to 1934.
* After each message application print out "midquote". Example:
```
Midquote: 141.5
```
* Every 10 message it prints out all sell and buy orders that weren't
executed yet.
* At the end it print some statistics about: corrupted messages, wrong
 numbers...

## build
```
cd build
cmake ..
make
```
or you can do `cmake .. -DDISABLE_OUT` to disable any output.

## performans text
On the test input file `input.txt` (100К orders buy/sell) it took:
* *53s* with enabled output `time ./trade ../input.txt &> /dev/null`
* *0.35s* with disable output `time ./trade ../input.txt`
