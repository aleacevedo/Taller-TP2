#include "compressor.h"
#include <string>
#include <fstream>
#include <math.h>

Compressor::Compressor(fstream &file_in, const size_t size) :
    file_in(file_in),
    size(size),
    in_bytes(size * 4) {
  this->numbers = new unsigned int[size]();
}

int Compressor::one_run() {
  if (this->read() == -1) return -1;
  unsigned int min = this->get_min();
  size_t position_MSB = this->subtract(min);
  compress(position_MSB);
  return 0;
}

Compressor::~Compressor() {
  delete[] this->numbers;
}

int Compressor::read() {
  if (!this->file_in.good()) return -1;
  this->file_in.read((char *) this->numbers, this->in_bytes);
  return this->file_in.gcount();
}
void Compressor::compress(size_t new_len) {
  auto size_char = (size_t) ceil(new_len / 8);
  this->compressed = new char[size_char]();
  //  char *aux = NULL;
  for (int ind = 0; ind < this->size; ind++) {
    size_t shift_size = (32 * size_char) - (new_len * ind);
    unsigned int aux = this->numbers[ind] << shift_size;
    *(this->compressed) = *(this->compressed) & aux;
  }
}
unsigned int Compressor::get_min() {
  unsigned int min = this->numbers[0];
  for (int ind = 1; ind < this->size; ind++) {
    min = this->numbers[ind] < min ? this->numbers[ind] : min;
  }
  return min;
}
int Compressor::subtract(unsigned int value) {
  size_t position = 0;
  for (int ind = 0; ind < this->size; ind++) {
    this->numbers[ind] = this->numbers[ind] - value;
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

