#!/bin/sh
g++ -c IIPC.cpp

g++ -c  CFifo.cpp
g++ CFifoSnd.cpp -o CFifoSnd
g++ CFifoRcv.cpp -o CFifoRcv

g++ -c CMsgQueue.cpp
g++ CMsgQueueSnd.cpp -o CMsgQueueSnd
g++ CMsgQueueRcv.cpp -o CMsgQueueRcv

g++ -c CSharedMem.cpp
g++ CSharedMemSnd.cpp -o CSharedMemSnd
g++ CSharedMemRcv.cpp -o CSharedMemRcv
