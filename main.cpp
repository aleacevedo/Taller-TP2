#include <stdio.h>
#include <fstream>
#include <thread>
#include "compressor.h"
#include "generator.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  std::ifstream file_in("alot", std::ifstream::binary);
  std::ofstream file_out("outPut1", std::ofstream::binary);
  Compressor *comp = new Compressor(file_in, 4);
  std::vector<Compressor*> comps;
  std::vector<std::thread*> threads;
  comps.push_back(comp);
  Generator *generator = new Generator(comps, file_in);
  for (size_t ind = 0; ind<comps.size(); ind++){
    threads.push_back(new std::thread(generator, ind));
  }
  file_in.close();
  file_out.close();
  return 0;
}

