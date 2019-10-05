#ifndef PARALLEL_COMPRESSOR_H_
#define PARALLEL_COMPRESSOR_H_

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
  std::vector<Producer*> producers;
  std::vector<std::thread*> threads;
  Consumer consumer;
  std::thread *consumer_thread;
  std::mutex mutex;

 public:
  ParallelCompressor(size_t block_size,
                     size_t queue_limit,
                     size_t thread_number,
                     const std::string &infile,
                     const std::string &outfile);
  void run();
  void wait_to_end();
  ~ParallelCompressor();

 private:
  void init_producers();
  void init_threads();
  void destroy_producers();
  void destroy_threads();
};

#endif  // PARALLEL_COMPRESSOR_H_
