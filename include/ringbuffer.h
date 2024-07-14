#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <array>
#include <atomic>

template <typename T, size_t Size> class RingBuffer {
public:
  RingBuffer();

  void push(const T &item);

  T &pop();

  bool isEmpty() const;

  size_t size() const;

  void reset();

private:
  std::array<T, Size> buffer;
  std::atomic<size_t> head;
  std::atomic<size_t> tail;
  bool isFull;
};

#include "ringbuffer.tpp" // Include the template implementation file

#endif // RINGBUFFER_H
