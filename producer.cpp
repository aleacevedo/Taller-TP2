#include "producer.h"
#include <fstream>

Producer::Producer(const std::vector<Compressor*> &compressors,
                     std::ifstream &in_file,
                     size_t queue_limit) :
                                      compressors(compressors),
                                      in_file(in_file) {
  for (size_t ind = 0; ind < this->compressors.size(); ind++) {
    this->thread_process.push_back(0);
    this->outputs.push_back(new SafeQueue(queue_limit, ind));
  }
}

std::vector<SafeQueue*> Producer::get_outputs() {
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
  printf("---------------PRODUCER---------------\n");
  SafeQueue *my_queue = this->outputs[index];
  while (true) {
    size_t size_block = this->compressors[index]->get_size_block();
    size_t shift_file = this->calc_offset(size_block,
                                          index,
                                          this->thread_process[index]);
    this->mutex.lock();
    this->in_file.clear();
    this->in_file.seekg(shift_file, this->in_file.beg);
    printf("SHIFT: %zu \n", shift_file);
    if (this->compressors[index]->read() == 1) {
      printf("WORK DONE PORQUE READ = 1 HILO %zu \n", index);
      my_queue->set_work_done();
      this->mutex.unlock();
      return;
    }
    this->thread_process[index]++;
    this->mutex.unlock();
    this->compressors[index]->one_run();
    std::vector<char> &packed = this->compressors[index]->get_compressed();
    std::string out(packed.begin(), packed.end());
    //  printf("VOY A ESCRIBIR UNO EN EL HILO: %zu\n", index);
    my_queue->push(out);
  }
}
