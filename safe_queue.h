#ifndef SAFE_QUEUE_H_
#define SAFE_QUEUE_H_

#include <queue>
#include <string>
#include <condition_variable>
#include <mutex>
#include <atomic>

class SafeQueue {
  std::queue<std::string> my_queue;
  size_t queue_limit;
  std::mutex lock_queue;
  std::mutex lock;
  std::condition_variable notify_push;
  std::condition_variable notify_pop;
  std::atomic<bool> work_done;
 public:
  SafeQueue(size_t queue_limit);
  void push(std::string &value);
  size_t size();
  std::string pop();
  bool is_empty();
  size_t get_size();
  void notify_work_done();
  ~SafeQueue();
 private:
  void safe_push(std::string &value);
  std::string safe_pop();

};

#endif  //  SAFE_QUEUE_H_
