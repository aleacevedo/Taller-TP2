#include "producer.h"
#include <fstream>


Producer::Producer(const std::vector<Compressor*> &compressors,
                     std::ifstream &in_file,
                     size_t queue_limit) :
                                      compressors(compressors),
                                      in_file(in_file),
                                      thread_process(),
                                      mutex(),
                                      queue_limit(queue_limit),
                                      work_done() {
  for (size_t ind = 0; ind < this->compressors.size(); ind++) {
    this->thread_process.push_back(0);
    this->outputs.push_back(new std::queue<std::string>());
    this->work_done.push_back(false);
    this->notifiers_read.push_back(new std::condition_variable());
    this->notifiers_write.push_back(new std::condition_variable());
    this->lockers_read.push_back(new std::mutex());
    this->lockers_write.push_back(new std::mutex());
  }
}

std::vector<std::queue<std::string>*> Producer::get_outputs() {
  return this->outputs;
}

std::vector<bool> Producer::get_work_done() {
  return this->work_done;
}

std::vector<std::condition_variable*>  Producer::get_notifiers_write() {
  return this->notifiers_write;
}

std::vector<std::condition_variable*>  Producer::get_notifiers_read() {
  return this->notifiers_read;
}

std::vector<std::mutex*> Producer::get_lockers_write() {
  return this->lockers_write;
}

std::vector<std::mutex*> Producer::get_lockers_read() {
  return this->lockers_read;
}

Producer::~Producer() {
  for (size_t ind = 0; ind < this->compressors.size(); ind++) {
    delete this->outputs[ind];
    delete this->notifiers_read[ind];
    delete this->notifiers_write[ind];
    delete this->lockers_read[ind];
    delete this->lockers_write[ind];
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
  printf("\n---------------PRODUCER---------------\n");
  std::unique_lock<std::mutex> lock_write(*this->lockers_write[index]);
  while (true) {
    while (this->outputs[index]->size() >= this->queue_limit) {
      printf("\nESPERO LA LECTURA\n");
      this->notifiers_write[index]->notify_all();
      this->notifiers_read[index]->wait(lock_write);
    }
    size_t size_block = this->compressors[index]->get_size_block();
    size_t shift_file = this->calc_offset(size_block,
                                          index,
                                          this->thread_process[index]);
    this->mutex.lock();
    this->in_file.seekg(shift_file, this->in_file.beg);
    if (!this->in_file.good()) {
      this->work_done[index] = true;
      this->mutex.unlock();
      this->notifiers_write[index]->notify_all();
      printf("\n SE TERMINO DE PRODUCIR \n");
      return;
    }
    if (this->compressors[index]->read() == 0) {
      this->work_done[index] = true;
      this->mutex.unlock();
      this->notifiers_write[index]->notify_all();
      printf("\n SE TERMINO DE PRODUCIR \n");
      return;
    }
    this->mutex.unlock();
    this->compressors[index]->one_run();
    std::vector<char> &packed = this->compressors[index]->get_compressed();
    std::string out(packed.begin(), packed.end());
    this->outputs[index]->push(out);
    this->thread_process[index]++;
    this->notifiers_write[index]->notify_all();
    printf("\nESCRIBO UNO\n");
  }
}
