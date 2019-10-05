#ifndef SAFE_QUEUE_H_
#define SAFE_QUEUE_H_

#include <queue>
#include <string>
#include <condition_variable>
#include <mutex>

class SafeQueue {
  std::queue<std::string> my_queue;
  size_t queue_limit;
  std::mutex lock_work_done;
  std::mutex lock;
  std::condition_variable notify_push;
  std::condition_variable notify_pop;
  bool work_done;
 public:
  SafeQueue(size_t queue_limit);
  void push(std::string &value);
  size_t size();
  std::string pop();
  void set_work_done();
  bool get_work_done();
  ~SafeQueue();
};

#endif  //  SAFE_QUEUE_H_
