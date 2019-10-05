#ifndef PRODUCER_H_
#define PRODUCER_H_
#include <stdio.h>
#include <vector>
#include <string>
#include <mutex>
#include "compressor.h"
#include "safe_queue.h"


class Producer {
  std::ifstream &in_file;
  Compressor compressor;
  SafeQueue my_queue;
  std::mutex &mutex;
  size_t compressed;
  size_t index;
  size_t threads_num;
 public:
  Producer(std::ifstream &in_file,
           size_t queue_limit,
           size_t size_block,
           std::mutex &mutex,
           size_t index,
           size_t threads_num);
  std::string get_product();
  bool get_work_done();
  ~Producer();
  void operator()();
 private:
  size_t calc_offset(size_t size_block, size_t index, size_t run_number);
};

#endif  //  PRODUCER_H_

