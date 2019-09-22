#include "generator.h"
#include <fstream>


Generator::Generator(const std::vector<Compressor*> &compressors,
                      std::ifstream &in_file) :
                                      compressors(compressors),
                                      in_file(in_file) {
  for(size_t ind = 0; ind < this->compressors.size(); ind++) {
    std::queue<std::string> *output = new std::queue<std::string>();
    Compressor *compressor = this->compressors[ind];
    Params *params = new Params(compressor, ind, output);
    this->outputs.push_back(new Output(output, new std::thread(this, params)));
  }
}

void Generator::run() {
}

std::vector<Output*>& Generator::get_outputs() {
  return this->outputs;
}

size_t Generator::calc_offset(size_t size_block,
                              size_t index,
                              size_t run_number) {
  size_t threads_num = this->compressors.size();
  size_t offset = (size_block * index);
  size_t shift_len = (size_block * (threads_num * run_number));
  size_t index_file = shift_len + offset;
  return index_file;
}

void Generator::operator() (Params* params) {
  size_t size_block = params->get_compressor()->get_size_block();
  size_t shift_file = this->calc_offset(size_block,
                                        params->get_index(),
                                        params->get_processed());
  this->mutex.lock();
  this->in_file.seekg(shift_file, this->in_file.beg);
  if (!this->in_file.good()) return;
  params->get_compressor()->read();
  this->mutex.unlock();
  params->get_compressor()->one_run();
}
