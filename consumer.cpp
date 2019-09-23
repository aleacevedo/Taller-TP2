#include "consumer.h"
#include <string>

Consumer::Consumer(Producer &producer,
                   std::ofstream &file_out) : producer(producer),
                   file_out(file_out) {
}

bool Consumer::all_producer_finish() {
  for (size_t ind = 0; ind < this->producer.get_outputs().size(); ind++) {
    bool work_done = this->producer.get_work_done()[ind];
    size_t outputs = this->producer.get_outputs()[ind]->size();
    printf("EL HILO %zu ESTA %s Y LE QUEDAN %zu COSAS\n", ind, work_done? "TERMINADO" : "NO TERMINADO", outputs);
    if (!work_done ||
        !this->producer.get_outputs()[ind]->empty())
      return false;
  }
  return true;
}

bool Consumer::is_queue_empty(size_t ind) {
  this->producer.get_lockers_queue()[ind]->lock();
  bool isEmpty = this->producer.get_outputs()[ind]->empty();
  this->producer.get_lockers_queue()[ind]->unlock();
  return isEmpty;
}

Consumer::~Consumer() {}

void Consumer::operator() () {
  printf("\n------------------CONSUMER---------------\n");
  while (!this->all_producer_finish()) {
    for (size_t ind = 0; ind < this->producer.get_outputs().size(); ind++) {
      std::unique_lock<std::mutex> lock_read(
                                    *this->producer.get_lockers_read()[ind]);
      printf("\nTamano de cola del hilo %zu es : %zu\n", ind, this->producer.get_outputs()[ind]->size());
      while (this->producer.get_outputs()[ind]->empty()) {
        if (this->producer.get_work_done()[ind]) break;
        printf("\nESPERO LA ESCRITURA DEL HILO: %zu\n", ind);
        this->producer.get_notifiers_read()[ind]->notify_one();
        this->producer.get_notifiers_write()[ind]->wait(lock_read);
      }
      if (!this->producer.get_outputs()[ind]->empty()) {
        std::string out = this->producer.get_outputs()[ind]->front();
        this->file_out.write(out.c_str(), out.size());
        this->producer.get_outputs()[ind]->pop();
        this->producer.get_notifiers_read()[ind]->notify_one();
        printf("\nYA LEI DEL HILO: %zu\n", ind);
      }
    }
  }
  printf("SE TERMINO DE CONSUMIR\n");
}