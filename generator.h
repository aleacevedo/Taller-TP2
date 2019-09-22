#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include <stdio.h>
#include <queue>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include "compressor.h"
#include "thread_params.h"
#include "output.h"


class Generator {
  const std::vector<Compressor&> &compressors;
  std::vector<Output*> outputs;
  std::mutex mutex;
  std::ifstream &in_file;
 public:
  Generator(const std::vector<Compressor&> &Compressors, std::ifstream &in_file);
  void run();
  std::vector<Output*>& get_outputs();
  ~Generator();
 private:
  void operator() (Params* params);
  size_t calc_offset(size_t size_block, size_t index, size_t run_number);
};

#endif  //  GENERATOR_H_

