#include <stdio.h>
#include <string>
#include "parallel_compressor.h"

#define BLOCK_SIZE_POS 1
#define THREAD_NUM_POS 2
#define QUEUE_LIMIT_POS 3
#define IFILE_PATH_POS 4
#define OFILE_PATH_POS 5


int main(int argc, char *argv[]) {
  if (argc < 6) return -1;
  size_t block_size = std::stoul(std::string(argv[BLOCK_SIZE_POS]));
  size_t threads_number = std::stoul(std::string(argv[THREAD_NUM_POS]));
  size_t queue_limit = std::stoul(std::string(argv[QUEUE_LIMIT_POS]));
  std::string infile(argv[IFILE_PATH_POS]);
  std::string outfile(argv[OFILE_PATH_POS]);
  if (infile == "-") infile = "stdin";
  if (outfile == "-") outfile = "stdout";
  ParallelCompressor parallelCompressor(block_size,
                                        queue_limit,
                                        threads_number,
                                        infile, outfile);
  parallelCompressor.run();
  parallelCompressor.wait_to_end();
  return 0;
}
