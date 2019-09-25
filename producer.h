#ifndef PRODUCER_H_
#define PRODUCER_H_
#include <stdio.h>
#include <vector>
#include <string>
#include <mutex>
#include "compressor.h"
#include "safe_queue.h"


class Producer {
  const std::vector<Compressor*> &compressors;
  std::vector<SafeQueue*> outputs;
  std::ifstream &in_file;
  std::vector<int> thread_process;
  std::mutex mutex;
 public:
  Producer(const std::vector<Compressor*> &Compressors,
           std::ifstream &in_file,
           size_t queue_limit);
  std::vector<SafeQueue*> get_outputs();
  ~Producer();
  void operator()(size_t index);
 private:
  size_t calc_offset(size_t size_block, size_t index, size_t run_number);
};

#endif  //  PRODUCER_H_

