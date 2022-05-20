# C++ Stacks and Queues 
This assignment introduces stacks and queues. 
* Eval.cpp uses a stack to evaluate a string using an infix operation format. 
* Mazequeue.cpp uses a queue to solve a maze by pushing the starting coordinate onto the queue. @ represents the current position. It can move up, down, left, and right by one coordinate. Each move is pushed onto the queue. At each turn the top of the queue is popped. When the current coordinate and the end coordinate are the same, the function pathExists returns true. 
* Mazestack.cpp does the same function as mazequeue.cpp, but namely uses a stack instead. 
