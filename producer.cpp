#include "producer.h"
#include <fstream>


Producer::Producer(const std::vector<Compressor*> &compressors,
                     std::ifstream &in_file) :
                                      compressors(compressors),
                                      in_file(in_file),
                                      thread_process(0),
                                      mutex() {
  for (size_t ind = 0; ind < this->compressors.size(); ind++) {
    this->thread_process.push_back(0);
    this->outputs.push_back(new std::queue<std::string>());
  }
}

std::vector<std::queue<std::string>*> Producer::get_outputs() {
  return this->outputs;
}

Producer::~Producer() {
  for (size_t ind = 0; ind < this->compressors.size(); ind++) {
    delete this->outputs[ind];
  }
}

size_t Producer::calc_offset(size_t size_block,
                              size_t index,
                              size_t run_number) {
  size_t threads_num = this->compressors.size();
  size_t offset = (size_block * index);
  size_t shift_len = (size_block * (threads_num * run_number));
  size_t index_file = (shift_len + offset) * 4;
  return index_file;
}

void Producer::operator()(size_t index) {
  while(true){
    size_t size_block = this->compressors[index]->get_size_block();
    size_t shift_file = this->calc_offset(size_block, index, this->thread_process[index]);
    printf("\nSHITF_FILE: %zu\n", shift_file);
    this->mutex.lock();
    this->in_file.seekg(shift_file, this->in_file.beg);
    if (!this->in_file.good()) {
      this->mutex.unlock();
      return;
    }
    if(this->compressors[index]->read() == 0) {
      this->mutex.unlock();
      return;
    }
    this->mutex.unlock();
    this->compressors[index]->one_run();
    std::vector<char> &packed = this->compressors[index]->get_compressed();
    std::string out(packed.begin(), packed.end());
    printf("\nGUARDO ESTO: %s\n", out.c_str());
    this->outputs[index]->push(out);
    this->thread_process[index]++;
  }
}
