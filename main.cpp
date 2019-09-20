#include <stdio.h>
#include "compressor.h"
#include "utils.h"


int main(int argc, char *argv[]) {
  std::ifstream file_in("alotMod", std::ifstream::binary);
  std::ofstream file_out("outPut", std::ofstream::binary);
  Compressor comp(file_in, 4);
  while(file_out.good()){
    comp.one_run();
    vector<char> &buffer = comp.get_compressed();
    printf("New len: %zu \n", comp.get_new_len());
    printf("Reference: %zu \n", comp.get_reference());
    for (size_t i = 0; i < comp.get_size_packed(); i++) {
      file_out.write(&(buffer[i]), 1);
      print_byte_as_bits(buffer[i]);
      printf(" ");
    }
    printf("\n");
  }
  file_in.close();
  file_out.close();
  return 0;
}

