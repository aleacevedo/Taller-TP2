#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_
#include <fstream>

using std::fstream;

class Compressor {

  fstream &file_in;
  const size_t size;
  const size_t in_bytes;
  unsigned int *numbers;
  char *compressed;

 public:
  Compressor(fstream &file_in, const size_t size);
  int one_run();
  ~Compressor();

 private:
  int read();
  void compress(size_t new_len);
  unsigned int get_min();
  int subtract(unsigned int value);
  static size_t get_MSB_position(unsigned int number);
};

#endif // _COMPRESSOR_H_
