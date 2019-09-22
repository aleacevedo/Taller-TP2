#include <stdio.h>
#include <fstream>
#include "compressor.h"
#include "generator.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  std::ifstream file_in("alot", std::ifstream::binary);
  std::ofstream file_out("outPut1", std::ofstream::binary);
  Compressor *comp = new Compressor(file_in, 4);
  std::vector<Compressor*> comps;
  comps.push_back(comp);
  Generator *generator = new Generator(comps, file_in);
  
  file_in.close();
  file_out.close();
  return 0;
}

