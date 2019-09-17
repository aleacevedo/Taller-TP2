#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_
#include <fstream>
#include <vector>


#define BYTE_SIZE 8
#define INT_SIZE 32

using std::fstream;
using std::vector;

class Compressor {

  fstream &file_in;
  const size_t size;
  size_t size_compressed;
  unsigned int *numbers;
  vector<bool> compressed;

 public:
  Compressor(fstream &file_in, const size_t size);
  int one_run();
  char *get_compressed(char* dest);
  size_t get_size_compressed();
  ~Compressor();

 private:
  void save(int source, size_t offset);
  int read();
  void compress(size_t new_len);
  unsigned int get_min();
  int subtract(unsigned int value);
  static size_t get_MSB_position(unsigned int number);
};

#endif // _COMPRESSOR_H_
