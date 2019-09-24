#include "safe_queue.h"

SafeQueue::SafeQueue(size_t queue_limit)
                    : queue_limit(queue_limit) {}

void SafeQueue::push(std::string &value) {
  std::unique_lock<std::mutex> lock_push(this->lock);
  while (this->my_queue.size() >= this->queue_limit) {
    printf("VOY A ESPERAR QUE LEAN SIZE: %zu\n", this->my_queue.size());
    this->notify_pop.wait(lock_push);
  }
  this->my_queue.push(value);
  this->notify_push.notify_all();
}

std::string SafeQueue::pop() {
  std::unique_lock<std::mutex> lock_pop(this->lock);
  while (this->my_queue.empty()) {
    if(this->work_done) return "";
    printf("VOY A ESPERAR QUE ESCRIBAN SIZE: %zu\n", this->my_queue.size());
    this->notify_push.wait(lock_pop);
  }
  std::string &value = this->my_queue.front();
  this->my_queue.pop();
  this->notify_pop.notify_one();
  return value;
}

void SafeQueue::set_work_done() {
  printf("EL TRABAJO SE TERMINO\n");
  this->lock_work_done.lock();
  this->work_done = true;
  this->lock_work_done.unlock();
  this->notify_push.notify_all();
  printf("WORKDONE SETEADO\n");
}

bool SafeQueue::get_work_done() {
  printf("QUEDA TRABAJO?;");
  this->lock_work_done.lock();
  bool result = this->work_done && this->my_queue.empty();
  this->lock_work_done.unlock();
  printf("%s QUEDA\n", result? "NO" : "SI");
  return result;
}

SafeQueue::~SafeQueue() {}
