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
