#include <stdio.h>
#include <string>
#include "parallel_compressor.h"


int main(int argc, char *argv[]) {
  if (argc < 6) return -1;
  size_t block_size = std::stoul(std::string(argv[1]));
  size_t thrads_number = std::stoul(std::string(argv[2]));
  size_t queue_limit = std::stoul(std::string(argv[3]));
  std::string infile(argv[4]);
  std::string outfile(argv[5]);
  if (infile == "-") infile = "stdin";
  if (outfile == "-") outfile = "stdout";
  ParallelCompressor parallelCompressor(block_size,
                                        queue_limit,
                                        thrads_number,
                                        infile, outfile);
  parallelCompressor.run();
  parallelCompressor.wait_to_end();
  return 0;
}

/*
int main(int argc, char *argv[]) {
  std::ifstream file_in("alot", std::ifstream::binary);
  std::ofstream file_out("outPutWithConsumerAndQueue", std::ofstream::binary);
  Compressor *comp = new Compressor(file_in, 4);
  std::vector<Compressor*> comps;
  std::vector<std::thread*> threads;
  std::thread *consumer_thread;
  comps.push_back(comp);
  Producer producer(comps, file_in, 4);
  Consumer consumer(producer, file_out);
  for (size_t ind = 0; ind<comps.size(); ind++){
    threads.push_back(new std::thread(std::ref(producer), ind));
  }
  consumer_thread = new std::thread(std::ref(consumer));
  threads[0]->join();
  consumer_thread->join();
  for (size_t ind = 0; ind<comps.size(); ind++){
    delete threads[ind];
    delete comps[ind];
  }
  delete consumer_thread;
  file_in.close();
  file_out.close();
  return 0;
}
*/
