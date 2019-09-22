#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include <stdio.h>
#include <queue>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include "compressor.h"


class Generator {
  const std::vector<Compressor*> &compressors;
  std::vector<std::queue<std::string>*> outputs;
  std::vector<std::thread*> threads;
  std::vector<int> thread_process;
  std::mutex mutex;
  std::ifstream &in_file;
 public:
  Generator(const std::vector<Compressor*> &Compressors, std::ifstream &in_file);
  void run(int index);
  std::vector<std::queue<std::string>*> get_outputs();
  ~Generator();
  void operator()(int index);
 private:
  size_t calc_offset(size_t size_block, size_t index, size_t run_number);
};

#endif  //  GENERATOR_H_

