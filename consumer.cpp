#include "consumer.h"
#include <iostream>
#include <string>

Consumer::Consumer(std::vector<Producer*> &producers,
                   std::ofstream &file_out) : producers(producers),
                   file_out(file_out) {}

Consumer::Consumer(Consumer&& other) : producers(other.producers),
                                      file_out(other.file_out) {
}

Consumer::~Consumer() {}

void Consumer::operator() () {
  while (!this->all_producer_finish()) {
    for (size_t ind = 0; ind < this->producers.size(); ind++) {
      if (!this->producers[ind]->get_work_done()) {
        std::string out = this->producers[ind]->get_product();
        this->file_out.write(out.c_str(), out.size());
      }
    }
  }
}

bool Consumer::all_producer_finish() {
  for (size_t ind = 0; ind < this->producers.size(); ind++) {
    if (!this->producers[ind]->get_work_done())
      return false;
  }
  return true;
}
