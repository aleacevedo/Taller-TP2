#include <stdio.h>
#include "compressor.h"
#include "utils.h"


int main(int argc, char *argv[]) {
  fstream file_in("alotMod");
  fstream file_out("outPut", fstream::out);
  Compressor comp(file_in, 4);
  comp.one_run();
  file_out.write(comp.get_compressed(), comp.get_size_compressed());
  for (int i = 0; i < comp.get_size_compressed(); i++) {
    print_byte_as_bits(comp.get_compressed()[i]);
  }
  file_in.close();
  file_out.close();
  return 0;
}

