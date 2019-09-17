#include "compressor.h"
#include <string>
#include <fstream>
#include <math.h>
#include "utils.h"

#define SHOW(T, V) do { T x = V; print_bits((unsigned char*) &x, sizeof(x)); } while(0)

void testing_print_int(char *test){
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
  size_t position_MSB = this->subtract(min);
  compress(position_MSB);
  printf("---------------------------\n");
  for (size_t i = 0; i < this->get_size_compressed(); i++) {
    print_byte_as_bits(this->get_compressed()[i]);
    printf(" ");
  }
  printf("\n---------------------------\n");
  return 0;
}

char *Compressor::get_compressed() {
  return this->compressed;
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

void Compressor::save(int source, size_t offset) {
  char *aux = (char*) &source;
  for (int byte_ind = 0; byte_ind < 4; byte_ind++) {
    offset += byte_ind;
    if (offset >= this->size_compressed) break;
    this->compressed[offset] = this->compressed[offset] | aux[3 - byte_ind];
  }
  printf("Compressed \n");
  for (size_t i = 0; i < this->get_size_compressed(); i++) {
    print_byte_as_bits(this->get_compressed()[i]);
    printf(" ");
  }
  printf("\n");
}

void Compressor::compress(size_t new_len) {
  this->size_compressed = (size_t) ceil(((new_len + 0.0) * this->size) / BYTE_SIZE);
  this->compressed = new char[this->size_compressed]();
  int used_bits = 0;
  int free_bits = BYTE_SIZE;
  int ind_dest = 0;
  for (size_t ind = 0; ind < this->size; ind++) {
    unsigned int aux = this->numbers[ind];
    printf("------------------------------------\n");
    printf("Used bits: %i, New len: %zu, Free bits: %i\n", used_bits, new_len, free_bits);
    size_t shift_size = (INT_SIZE - used_bits - new_len);
    used_bits += new_len;
    free_bits -= new_len;
    printf("Print number %i in position %i\n", aux ,ind);
    testing_print_int((char *)&aux);
    aux = aux << shift_size;
    printf("Shift of size %zu\n", shift_size);
    testing_print_int((char *)&aux);
    printf("Used bits: %i, New len: %zu, Free bits: %i\n", used_bits, new_len, free_bits);
    this->save(aux, ind_dest);
    printf("------------------------------------\n");
    if (free_bits <= 0) {
      used_bits = used_bits - BYTE_SIZE;
      free_bits = free_bits + BYTE_SIZE;
      ind_dest++;
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

