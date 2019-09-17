#include "compressor.h"
#include <string>
#include <math.h>
#include "utils.h"

#define SHOW(T, V) do { T x = V; print_bits((unsigned char*) &x, sizeof(x)); } while(0)

void testing_print_int(char *test) {
  print_byte_as_bits(test[3]);
  printf(" ");
  print_byte_as_bits(test[2]);
  printf(" ");
  print_byte_as_bits(test[1]);
  printf(" ");
  print_byte_as_bits(test[0]);
  printf("\n");
}

Compressor::Compressor(fstream &file_in, const size_t size) :
    file_in(file_in),
    size(size) {
  this->numbers = new unsigned int[size]();
}

int Compressor::one_run() {
  if (this->read() == -1) return -1;
  unsigned int min = this->get_min();
  size_t len_numbers = this->subtract(min);
  this->size_compressed = (size_t) ceil(((len_numbers + 0.0) * this->size) / BYTE_SIZE);
  this->compress(len_numbers);
  return 0;
}

char *Compressor::get_compressed(char *dest) {
  size_t ind = 0;
  int pos = 7;
  for (size_t i = 0; i < this->compressed.size(); i++) {
    uint8_t bit_mask = pow(2, pos);
    if (this->compressed[i]) {
      dest[ind] = dest[ind] | bit_mask;
    }
    pos--;
    if(pos < 0){
      ind++;
      pos = 7;
    }
  }
  return dest;
}
size_t Compressor::get_size_compressed() {
  return this->size_compressed;
}

Compressor::~Compressor() {
  delete[] this->numbers;
}

int Compressor::read() {
  if (!this->file_in.good()) return -1;
  char aux[4];
  for (size_t readed = 0; readed < this->size; readed++) {
    this->file_in.read(aux, 4);
    int *aux_in = (int *) aux;
    this->numbers[readed] = ntohl(*aux_in);
  }
  return this->file_in.gcount();
}
void Compressor::compress(size_t new_len) {
  for (size_t ind = 0; ind < this->size; ind++) {
    unsigned int aux = this->numbers[ind];
    for (size_t cont = new_len; cont > 0; cont--) {
      unsigned int bit_mask = pow(2, cont-1);
      bool bit = bit_mask & aux;
      this->compressed.push_back(bit);
    }
  }
}

unsigned int Compressor::get_min() {
  unsigned int min = this->numbers[0];
  for (size_t ind = 1; ind < this->size; ind++) {
    min = this->numbers[ind] < min ? this->numbers[ind] : min;
  }
  return min;
}
int Compressor::subtract(unsigned int value) {
  size_t position = 0;
  for (size_t ind = 0; ind < this->size; ind++) {
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

