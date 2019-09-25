#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_
#include <stdio.h>
#include <vector>
#include <fstream>



#define BYTE_SIZE 8
#define INT_SIZE 32

using std::vector;

class Compressor {
  std::ifstream &file_in;
  const size_t size_block;
  size_t size_compressed;
  size_t size_packed;
  vector<unsigned int> numbers;
  vector<bool> compressed;
  vector<char> packed;
  unsigned int reference;
  unsigned int new_len;

 public:
  Compressor(std::ifstream &file_in, const size_t size_block);
  int one_run();
  vector<char> &get_compressed();
  size_t get_size_block();
  size_t get_size_compressed();
  size_t get_reference();
  size_t get_new_len();
  size_t get_size_packed();
  int read();
  ~Compressor();

 private:
  void compress();
  unsigned int get_min();
  int subtract();
  static size_t get_MSB_position(unsigned int number);
  void save();
  size_t calculate_size_compresed();
};

#endif  //  COMPRESSOR_H_
