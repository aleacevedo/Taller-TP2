#include "producer.h"
#include <fstream>
#include <string>
#include <vector>

Producer::Producer(std::ifstream &in_file,
                   size_t queue_limit,
                   size_t size_block,
                   std::mutex &mutex,
                   size_t index,
                   size_t threads_num) :
                                      in_file(in_file),
                                      compressor(in_file, size_block),
                                      my_queue(queue_limit),
                                      mutex(mutex),
                                      compressed(0),
                                      index(index),
                                      threads_num(threads_num) {}

std::string Producer::get_product() {
  return this->my_queue.pop();
}

bool Producer::get_work_done() {
  return this->my_queue.get_work_done();
}

Producer::~Producer() {}

size_t Producer::calc_offset(size_t size_block,
                              size_t index,
                              size_t run_number) {
  size_t offset = (size_block * index);
  size_t shift_len = (size_block * (this->threads_num * run_number));
  size_t index_file = (shift_len + offset) * 4;
  return index_file;
}

void Producer::operator()() {
  while (true) {
    size_t size_block = this->compressor.get_size_block();
    size_t shift_file = this->calc_offset(size_block,
                                          this->index,
                                          this->compressed);
    this->mutex.lock();
    this->in_file.clear();
    this->in_file.seekg(shift_file, this->in_file.beg);
    if (this->compressor.read() == 1) {
      this->my_queue.set_work_done();
      this->mutex.unlock();
      return;
    }
    this->compressed++;
    this->mutex.unlock();
    this->compressor.one_run();
    std::vector<char> &packed = this->compressor.get_compressed();
    std::string out(packed.begin(), packed.end());
    my_queue.push(out);
  }
}
