#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdio.h>
#include <queue>
#include <string>
#include <thread>


class Output{
  std::queue<std::string> *output;
  std::thread *my_thread;

 public:
  Output(std::queue<std::string> *output, std::thread *my_thread);
  std::queue<std::string>* get_output();
  std::thread* get_my_thread();
  ~Output();
};


#endif  //  _OUTPUT_H_
