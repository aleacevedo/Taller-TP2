#include "consumer.h"
#include <iostream>
#include <string>
#include <vector>

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
        const std::vector<uint8_t>& out = this->producers[ind]->get_product();
        for (uint8_t num : out) {
          this->file_out.write((const char *)&num, 1);
        }
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
