#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_
#include <fstream>
#include <vector>


#define BYTE_SIZE 8
#define INT_SIZE 32

using std::fstream;
using std::vector;

class Compressor {

  std::ifstream &file_in;
  const size_t size;
  size_t size_compressed;
  size_t size_packed;
  vector<unsigned int> numbers;
  vector<bool> compressed;
  vector<char> packed;
  unsigned int reference;
  unsigned int new_len;

 public:
  Compressor(std::ifstream &file_in, const size_t size);
  int one_run();
  vector<char> &get_compressed();
  size_t get_size_compressed();
  size_t get_reference();
  size_t get_new_len();
  size_t get_size_packed();
  ~Compressor();

 private:
  int read();
  void compress();
  unsigned int get_min();
  int subtract();
  static size_t get_MSB_position(unsigned int number);
  void save();
  size_t calculate_size_compresed();
};

#endif // _COMPRESSOR_H_
