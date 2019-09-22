#include "thread_params.h"

Params::Params(Compressor &compressor,
              size_t index,
              std::queue<std::string> *queue) : compressor(compressor),
                                                index(index),
                                                processed(0),
                                                output(queue) {

                                                }

Compressor& Params::get_compressor() {
  return this->compressor;
}

size_t Params::get_index() {
  return this->index;
}

size_t Params::get_processed() {
  return this->processed;
}

std::queue<std::string>* Params::get_queue() {
  return this->output;
}

void Params::inc_processed() {
  this->processed++;
}
