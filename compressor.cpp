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

Compressor::Compressor(std::ifstream &file_in, const size_t size) :
    file_in(file_in),
    size(size) {
}

int Compressor::one_run() {
  if (this->read() == 0) return 0;
  this->reference = this->get_min();
  this->new_len = this->subtract();
  this->size_compressed = this->calculate_size_compresed();
  this->compress();
  this->save();
  return 0;
}

vector<char> &Compressor::get_compressed() {
  return this->packed;
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
  printf("Reading \n");
  for (size_t readed = 0; readed < this->size; readed++) {
    if (!this->file_in.good()) {
      this->numbers.pop_back();
      if (this->numbers.size() == 0)
        return 0;
      for (; readed< this->size; readed++) {
        this->numbers.push_back(0);
      }
      return this->file_in.gcount();
    }
    this->file_in.read(aux, 4);
    unsigned int *aux_in = (unsigned int *) aux;
    this->numbers.push_back(ntohl(*aux_in));
    printf("Number: %u  ", this->numbers[readed]);
  }
  printf("\n");
  return this->file_in.gcount();
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
  char *dest = new char[this->size_compressed]();
  unsigned int aux_big = (htonl(this->reference));
  char *aux_char = (char *) &aux_big;
  this->packed.clear();
  for (size_t i = 0; i < 4; i++) {
    this->packed.push_back(aux_char[i]);
  }
  aux_char = (char *) &(this->new_len);
  this->packed.push_back(*aux_char);
  size_t ind = 0;
  int pos = BYTE_SIZE;
  bool remain = false;
  for (size_t i = 0; i < this->compressed.size(); i++) {
    remain = true;
    uint8_t bit_mask = pow(2, pos);
    if (this->compressed[i]) {
      dest[ind] = dest[ind] | bit_mask;
    }
    pos--;
    if (pos == 0) {
      remain = false;
      this->packed.push_back(dest[ind]);
      ind++;
      pos = BYTE_SIZE;
    }
  }
  if (remain)
    this->packed.push_back(dest[ind]);
  this->size_packed = this->size_compressed + 5;
  delete[] dest;
}

size_t Compressor::calculate_size_compresed() {
  return (size_t) ceil(((this->new_len + 0.0) * this->numbers.size()) / BYTE_SIZE);
}