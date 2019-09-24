#include "consumer.h"
#include <string>

Consumer::Consumer(Producer &producer,
                   std::ofstream &file_out) : producer(producer),
                   file_out(file_out) {
}

Consumer::~Consumer() {}

void Consumer::operator() () {
  printf("------------------CONSUMER---------------\n");
  while (!this->all_producer_finish()) {
    for (size_t ind = 0; ind < this->producer.get_outputs().size(); ind++) {
      SafeQueue *new_queue = this->producer.get_outputs()[ind];
      if (!new_queue->get_work_done()) {
        //printf("VOY A LEER DEL HILO: %zu \n", ind);
        std::string out = new_queue->pop();
        this->file_out.write(out.c_str(), out.size());
      }
    }
  }
  //printf("SE TERMINO DE CONSUMIR\n");
}

bool Consumer::all_producer_finish() {
  for (size_t ind = 0; ind < this->producer.get_outputs().size(); ind++) {
    if (!this->producer.get_outputs()[ind]->get_work_done())
      return false;
  }
  return true;
}
