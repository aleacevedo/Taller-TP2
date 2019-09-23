#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include <stdio.h>
#include <queue>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "compressor.h"


class Producer {
  const std::vector<Compressor*> &compressors;
  std::vector<std::queue<std::string>*> outputs;
  std::ifstream &in_file;
  std::vector<int> thread_process;
  std::mutex mutex;
  size_t queue_limit;
  std::vector<bool> work_done;
  std::vector<std::condition_variable*> notifiers_write;
  std::vector<std::condition_variable*> notifiers_read;
  std::vector<std::mutex*> lockers_write;
  std::vector<std::mutex*> lockers_read;
 public:
  Producer(const std::vector<Compressor*> &Compressors,
           std::ifstream &in_file,
           size_t queue_limit);
  std::vector<std::queue<std::string>*> get_outputs();
  std::vector<bool> get_work_done();
  std::vector<std::condition_variable*>  get_notifiers_write();
  std::vector<std::condition_variable*>  get_notifiers_read();
  std::vector<std::mutex*> get_lockers_write();
  std::vector<std::mutex*> get_lockers_read();
  ~Producer();
  void operator()(size_t index);
 private:
  size_t calc_offset(size_t size_block, size_t index, size_t run_number);
};

#endif  //  GENERATOR_H_

