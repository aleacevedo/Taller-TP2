#ifndef SAFE_QUEUE_H_
#define SAFE_QUEUE_H_

#include <queue>
#include <string>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>

class SafeQueue {
  std::queue<std::vector<uint8_t>> queue;
  size_t queue_limit;
  std::mutex lock_queue;
  std::mutex lock;
  std::condition_variable notify_push;
  std::condition_variable notify_pop;
  std::atomic<bool> work_done;
 public:
  SafeQueue(size_t queue_limit);
  void push(const std::vector<uint8_t> &value);
  size_t size();
  const std::vector<uint8_t> pop();
  bool is_empty();
  size_t get_size();
  void notify_work_done();
  ~SafeQueue();
 private:
  void safe_push(const std::vector<uint8_t> &value);
  const std::vector<uint8_t> safe_pop();
};

#endif  //  SAFE_QUEUE_H_
