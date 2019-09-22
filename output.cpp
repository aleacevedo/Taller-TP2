#include "output.h"

  Output::Output(std::queue<std::string> *output,
                 std::thread *my_thread) : output(output),
                                           my_thread(my_thread) {

  }
  std::queue<std::string>* Output::get_output() {
    return this->output;
  }
  std::thread* Output::get_my_thread() {
    return this->my_thread;
  }
  Output::~Output() {}
