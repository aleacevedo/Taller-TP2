
#ifndef _THREADS_PARAMS_H_
#define _THREADS_PARAMS_H_
#include <stdio.h>
#include <queue>
#include <string>
#include "compressor.h"


class Params{
  Compressor *compressor;
  size_t index;
  size_t processed;
  std::queue<std::string> *output;

 public:
  Params(Compressor *compressor,
              size_t index,
              std::queue<std::string> *output);
  Compressor* get_compressor();
  size_t get_index();
  size_t get_processed();
  std::queue<std::string>* get_queue();
  void inc_processed();
  ~Params();
};

#endif  //  _THREADS_PARAMS_H_
