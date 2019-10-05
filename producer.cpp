#include "producer.h"
#include <iostream>
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
                                      mutex(mutex),
                                      compressed(0),
                                      index(index),
                                      threads_num(threads_num),
                                      work_done(false),
                                      my_queue(queue_limit) {}

std::string Producer::get_product() {
  if (this->get_work_done())
    return "";
  return this->my_queue.pop();
}

bool Producer::get_work_done() {
  return this->my_queue.is_empty() && this->work_done;
}

void Producer::set_work_done() {
  std::condition_variable notify_work_done;
  this->work_done = true;
  this->my_queue.notify_work_done();
  notify_work_done.notify_all();
}

Producer::~Producer() {}

size_t Producer::calc_offset() {
  size_t size_block = this->compressor.get_size_block();
  size_t offset = (size_block * this->index);
  size_t shift_len = (size_block * (this->threads_num * this->compressed));
  size_t index_file = (shift_len + offset) * 4;
  return index_file;
}

bool Producer::is_file_ended() {
  std::lock_guard<std::mutex> lock(this->mutex);
  char aux;
  size_t shift_file = this->calc_offset();
  this->in_file.clear();
  this->in_file.seekg(shift_file, this->in_file.beg);
  this->in_file.read(&aux, 1);
  return !this->in_file.good();
}

int Producer::read_file() {
  std::lock_guard<std::mutex> lock(this->mutex);
  size_t shift_file = this->calc_offset();
  this->in_file.clear();
  this->in_file.seekg(shift_file, this->in_file.beg);
  return this->compressor.read();
}

void Producer::operator()() {
  while (!this->get_work_done()) {
    if (this->read_file() == 1) {
      this->set_work_done();
      return;
    }
    this->compressor.one_run();
    std::vector<uint8_t> &packed = this->compressor.get_compressed();
    std::string out(packed.begin(), packed.end());
    this->my_queue.push(out);
    this->compressed++;
    if (this->is_file_ended()) {
      this->set_work_done();
    }
  }
}
