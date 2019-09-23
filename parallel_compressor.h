#ifndef _PARALLEL_COMPRESSOR_H_
#define _PARALLEL_COMPRESSOR_H_

#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "compressor.h"
#include "producer.h"
#include "consumer.h"

class ParallelCompressor {
  size_t block_size;
  size_t queue_limit;
  size_t thread_number;
  std::ifstream file_in;
  std::ofstream file_out;
  std::vector<Compressor*> compressors;
  std::vector<std::thread*> threads;
  Producer *producer;
  Consumer *consumer;
  std::thread *consumer_thread;
 public:
  ParallelCompressor(size_t block_size,
                     size_t queue_limit,
                     size_t thread_number,
                     std::string infile,
                     std::string outfile);
  void run();
  void wait_to_end();
  ~ParallelCompressor();
 private:
  void init_compressors();
  void init_threads();
  void destroy_compressors();
  void destroy_threads();
};

#endif
