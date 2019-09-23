#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include "producer.h"

class Consumer {

  Producer &producer;
  std::ofstream &file_out;
 public:
  Consumer(Producer &producer, std::ofstream &file_out);
  void operator() ();
  ~Consumer();
 private:
  bool all_producer_finish();
  bool is_queue_empty(size_t ind);
};

#endif  //  _CONSUMER_H_
