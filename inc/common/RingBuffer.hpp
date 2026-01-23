#include <optional>
#include <vector>

template <typename T> class RingBuffer {
private:
  // TODO : ask if we should just use std::array instead
  std::vector<std::optional<T>> buffer_;

  size_t head_ = 0;
  size_t tail_ = 0;

  size_t size_ = 0;
  size_t capacity_ = 0;

public:
  RingBuffer(size_t capacity) : capacity_(capacity) , buffer_(capacity) {}

  // move semantics:
  RingBuffer(RingBuffer &&other) noexcept
      : buffer_(std::move(other.buffer_)), head_(std::move(other.head_)),
        tail_(std::move(other.tail_)), size_(std::move(other.size_)),
        capacity_(std::move(other.capacity_)) {

    other.head_ = 0;
    other.tail_ = 0;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  RingBuffer &operator=(RingBuffer &&other) noexcept {
    //check for self move
    if (this == &other) {
      return *this;
    }

    buffer_ = std::move(other.buffer_);
    head_ = std::move(other.head_);
    tail_ = std::move(other.tail_);
    size_ = std::move(other.size_);
    capacity_ = std::move(other.capacity_);

    other.head_ = 0;
    other.tail_ = 0;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
  }


  // no copy semantics
  RingBuffer(const RingBuffer &) = delete;
  RingBuffer &operator=(const RingBuffer &) = delete;


  
  bool isFull() { return size_ == capacity_; }
  bool isEmpty() { return size_ == 0; }


  bool tryPush(T value) {
    if (!isFull()) {
      return false;
    }
    buffer_[tail_] = value;
    tail_ = (tail_ + 1) % capacity_;
    size_++;
    return true;
  }

  std::optional<T> tryPop() {
    if (isEmpty()) {
      return std::nullopt;
    }
    auto value = buffer_[head_];
    head_ = (head_ + 1) % capacity_;
    size_--;
    return value;
  }

};