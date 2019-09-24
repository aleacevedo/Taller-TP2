#include "safe_queue.h"

SafeQueue::SafeQueue(size_t queue_limit, size_t index)
                    : queue_limit(queue_limit),
                      index(index) {}

void SafeQueue::push(std::string &value) {
  std::unique_lock<std::mutex> lock_push(this->lock);
  while (this->my_queue.size() >= this->queue_limit) {
    this->notify_pop.wait(lock_push);
  }
  this->my_queue.push(value);
  this->notify_push.notify_all();
}

std::string SafeQueue::pop() {
  std::unique_lock<std::mutex> lock_pop(this->lock);
  while (this->my_queue.empty()) {
    if (this->work_done) {
      return "";
    }
    this->notify_push.wait(lock_pop);
  }
  std::string &value = this->my_queue.front();
  this->my_queue.pop();
  this->notify_pop.notify_one();
  return value;
}

void SafeQueue::set_work_done() {
  this->lock_work_done.lock();
  this->work_done = true;
  this->lock_work_done.unlock();
  this->notify_push.notify_one();
}

bool SafeQueue::get_work_done() {
  this->lock_work_done.lock();
  bool result = this->work_done && this->my_queue.empty();
  this->lock_work_done.unlock();
  return result;
}

SafeQueue::~SafeQueue() {}
