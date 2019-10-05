#include "compressor.h"
#include <iostream>
#include <string>
#include <math.h>
#include <arpa/inet.h>
#include <vector>



Compressor::Compressor(std::ifstream &file_in, const size_t size_block) :
    file_in(file_in),
    size_block(size_block) {
}

int Compressor::one_run() {
  //  if (this->read() == 0) return -1;
  this->reference = this->get_min();
  this->new_len = this->subtract();
  this->size_compressed = this->calculate_size_compresed();
  this->compress();
  this->save();
  return 0;
}

vector<uint8_t> &Compressor::get_compressed() {
  return this->packed;
}

size_t Compressor::get_size_block() {
  return this->size_block;
}

size_t Compressor::get_size_compressed() {
  return this->size_compressed;
}

size_t Compressor::get_reference() {
  return this->reference;
}

size_t Compressor::get_new_len() {
  return this->new_len;
}

size_t Compressor::get_size_packed() {
  return this->size_packed;
}

Compressor::~Compressor() {
}

int Compressor::read() {
  char aux[4];
  this->numbers.clear();
  for (size_t readed = 0; readed < this->size_block; readed++) {
    this->file_in.read(aux, 4);
    unsigned int *aux_in = (unsigned int *) aux;
    if (!this->file_in.good()) {
      if (this->numbers.size() == 0) {
        return 1;
      }
      unsigned int last_value = this->numbers[readed-1];
      for (; readed < this->size_block; readed++) {
        this->numbers.push_back(last_value);
      }
      return 0;
    }
    this->numbers.push_back(ntohl(*aux_in));
  }
  return 0;
}

void Compressor::compress() {
  this->compressed.clear();
  for (size_t ind = 0; ind < this->numbers.size(); ind++) {
    unsigned int aux = this->numbers[ind];
    for (size_t cont = this->new_len; cont > 0; cont--) {
      unsigned int bit_mask = pow(2, cont - 1);
      bool bit = bit_mask & aux;
      this->compressed.push_back(bit);
    }
  }
}

unsigned int Compressor::get_min() {
  unsigned int min = this->numbers[0];
  for (size_t ind = 1; ind < this->numbers.size(); ind++) {
    min = this->numbers[ind] < min ? this->numbers[ind] : min;
  }
  return min;
}

int Compressor::subtract() {
  size_t position = 0;
  for (size_t ind = 0; ind < this->numbers.size(); ind++) {
    this->numbers[ind] = this->numbers[ind] - this->reference;
    size_t aux_pos = get_MSB_position(this->numbers[ind]);
    position = aux_pos > position ? aux_pos : position;
  }
  return position;
}

size_t Compressor::get_MSB_position(unsigned int number) {
  size_t position = 0;
  while (number != 0) {
    position++;
    number = number >> 1;
  }
  return position;
}

void Compressor::save() {
  std::vector<uint8_t> dest(this->size_compressed);
  unsigned int aux_big = (htonl(this->reference));
  char *aux_char = (char *) &aux_big;
  this->packed.clear();
  for (size_t i = 0; i < 4; i++) {
    this->packed.push_back(aux_char[i]);
  }
  aux_char = (char *) &(this->new_len);
  this->packed.push_back(*aux_char);
  size_t ind = 0;
  int pos = BYTE_SIZE - 1;
  bool remain = false;
  for (size_t i = 0; i < this->compressed.size(); i++) {
    remain = true;
    uint8_t bit_mask = pow(2, pos);
    if (this->compressed[i]) {
      dest[ind] = dest[ind] | bit_mask;
    }
    pos--;
    if (pos < 0) {
      remain = false;
      this->packed.push_back(dest[ind]);
      ind++;
      pos = BYTE_SIZE - 1;
    }
  }
  if (remain)
    this->packed.push_back(dest[ind]);
  this->size_packed = this->size_compressed + 5;
}

size_t Compressor::calculate_size_compresed() {
  float aux = ((this->new_len + 0.0) * this->numbers.size())/ BYTE_SIZE;
  return (size_t) ceil(aux);
}
