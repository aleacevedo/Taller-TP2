#include "parallel_compressor.h"
#include <string>

ParallelCompressor::ParallelCompressor(size_t block_size,
                                      size_t queue_limit,
                                      size_t thread_number,
                                      std::string infile,
                                      std::string outfile)
                                    : block_size(block_size),
                                      queue_limit(queue_limit),
                                      thread_number(thread_number),
                                      file_in(infile, std::ifstream::binary),
                                      file_out(outfile, std::ofstream::binary),
                                      producers(),
                                      threads(),
                                      consumer(producers, file_out),
                                      mutex() {
                                        this->init_producers();
                                      }

void ParallelCompressor::run() {
  this->init_threads();
  this->consumer_thread = new std::thread(std::move(this->consumer));
}

void ParallelCompressor::wait_to_end() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    this->threads[ind]->join();
  }
  this->consumer_thread->join();
}

ParallelCompressor::~ParallelCompressor() {
  this->destroy_threads();
  this->destroy_producers();
  delete this->consumer_thread;
  file_in.close();
  file_out.close();
}

void ParallelCompressor::init_producers() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    this->producers.push_back(new Producer(this->file_in,
                                           this->queue_limit,
                                           this->block_size,
                                           this->mutex,
                                           ind,
                                           this->thread_number));
  }
}

void ParallelCompressor::init_threads() {
    for (size_t ind = 0; ind < this->thread_number; ind++) {
      this->threads.push_back(new std::thread(std::ref(*this->producers[ind])));
  }
}

void ParallelCompressor::destroy_producers() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    delete this->producers[ind];
  }
}

void ParallelCompressor::destroy_threads() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    delete this->threads[ind];
  }
}
