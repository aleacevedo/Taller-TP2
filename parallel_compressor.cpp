#include "parallel_compressor.h"

ParallelCompressor::ParallelCompressor(size_t block_size,
                                      size_t queue_limit,
                                      size_t thread_number,
                                      std::string infile,
                                      std::string outfile)
                                    : block_size(block_size),
                                      queue_limit(queue_limit),
                                      thread_number(thread_number),
                                      file_in(infile, std::ifstream::binary),
                                      file_out(outfile, std::ofstream::binary) {
  this->init_compressors();
  this->producer = new Producer(this->compressors,
                                this->file_in,
                                this->queue_limit);
  this->consumer = new Consumer(*this->producer, this->file_out);
}

void ParallelCompressor::run() {
  this->init_threads();
  this->consumer_thread = new std::thread(std::ref(*this->consumer));
}

ParallelCompressor::~ParallelCompressor() {
  this->destroy_threads();
  this->destroy_compressors();
  delete this->consumer_thread;
  delete this->producer;
  delete this->consumer;
  file_in.close();
  file_out.close();
}

void ParallelCompressor::init_compressors() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    this->compressors.push_back(new Compressor(this->file_in,
                                                this->block_size));
  }
}

void ParallelCompressor::init_threads() {
    for (size_t ind = 0; ind < this->thread_number; ind++) {
      this->threads.push_back(new std::thread(std::ref(*this->producer), ind));
  }
}

void ParallelCompressor::destroy_compressors() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    delete this->compressors[ind];
  }
}

void ParallelCompressor::destroy_threads() {
  for (size_t ind = 0; ind < this->thread_number; ind++) {
    delete this->threads[ind];
  }
}