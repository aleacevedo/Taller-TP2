#ifndef CONSUMER_H_
#define CONSUMER_H_

#include <vector>
#include "producer.h"

class Consumer {
  std::vector<Producer*> &producers;
  std::ostream &file_out;
 public:
  Consumer(std::vector<Producer*> &producers, std::ostream &file_out);
  Consumer(Consumer&& other);
  void operator() ();
  ~Consumer();
 private:
  bool all_producer_finish();
  bool is_queue_empty(size_t ind);
};

#endif  //  CONSUMER_H_
