#include "generator.h"
#include <fstream>


Generator::Generator(const std::vector<Compressor*> &compressors,
                      std::ifstream &in_file) :
                                      compressors(compressors),
                                      in_file(in_file) {
  for (size_t ind = 0; ind < this->compressors.size(); ind++) {
    this->outputs.push_back(new std::queue<std::string>());
    // this->threads.push_back(new std::thread(this, ind));
  }
}

void Generator::run(size_t index) {
  size_t size_block = this->compressors[index]->get_size_block();
  size_t shift_file = this->calc_offset(size_block,
                                        index,
                                        this->thread_process[index]);
  this->mutex.lock();
  this->in_file.seekg(shift_file, this->in_file.beg);
  if (!this->in_file.good()) return;
  this->compressors[index]->read();
  this->mutex.unlock();
  this->compressors[index]->one_run();
  this->thread_process[index]++;
}

std::vector<std::queue<std::string>*> Generator::get_outputs() {
  return this->outputs;
}

size_t Generator::calc_offset(size_t size_block,
                              size_t index,
                              size_t run_number) {
  size_t threads_num = this->compressors.size();
  size_t offset = (size_block * index);
  size_t shift_len = (size_block * (threads_num * run_number));
  size_t index_file = shift_len + offset;
  return index_file;
}

void Generator::operator()(size_t index) {
  size_t size_block = this->compressors[index]->get_size_block();
  size_t shift_file = this->calc_offset(size_block,
                                        index,
                                        this->thread_process[index]);
  this->mutex.lock();
  this->in_file.seekg(shift_file, this->in_file.beg);
  if (!this->in_file.good()) return;
  this->compressors[index]->read();
  this->mutex.unlock();
  this->compressors[index]->one_run();
  this->thread_process[index]++;
}
