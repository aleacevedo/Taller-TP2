#include "safe_queue.h"
#include <iostream>
#include <string>

SafeQueue::SafeQueue(size_t queue_limit) : queue(),
                                              queue_limit(queue_limit),
                                              lock_queue(),
                                              lock(),
                                              notify_push(),
                                              notify_pop(),
                                              work_done(false) {}

void SafeQueue::push(const std::vector<uint8_t> &value) {
  std::unique_lock<std::mutex> lock_push(this->lock);
  while (this->get_size() >= this->queue_limit) {
    this->notify_pop.wait(lock_push);
  }
  this->safe_push(value);
  this->notify_push.notify_all();
}

const std::vector<uint8_t> SafeQueue::pop() {
  std::unique_lock<std::mutex> lock_pop(this->lock);
  while (this->is_empty()) {
    if (work_done) {
      return std::vector<uint8_t>();
    }
    this->notify_push.wait(lock_pop);
  }
  const std::vector<uint8_t> value = this->safe_pop();
  this->notify_pop.notify_all();
  return value;
}

void SafeQueue::notify_work_done(){
  this->work_done = true;
  this->notify_pop.notify_all();
  this->notify_push.notify_all();
}

size_t SafeQueue::get_size() {
  std::lock_guard<std::mutex> lock(this->lock_queue);
  return this->queue.size();
}

bool SafeQueue::is_empty() {
  std::lock_guard<std::mutex> lock(this->lock_queue);
  return this->queue.empty();
}

SafeQueue::~SafeQueue() {}


void SafeQueue::safe_push(const std::vector<uint8_t> &value) {
  std::lock_guard<std::mutex> lock_guard(this->lock_queue);
  this->queue.push(value);
}

const std::vector<uint8_t> SafeQueue::safe_pop() {
  std::lock_guard<std::mutex> lock_guard(this->lock_queue);
  const std::vector<uint8_t> value = this->queue.front();
  this->queue.pop();
  return value;
}
