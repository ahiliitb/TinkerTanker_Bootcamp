#include "ringbuffer.h"

template <typename T, size_t Size>
RingBuffer<T, Size>::RingBuffer() : head(0), tail(0), isFull(false) {}

template <typename T, size_t Size>
void RingBuffer<T, Size>::push(const T &item) {
  buffer[head] = item;
  if (isFull) {
    tail++;
    if (tail >= Size) {
      tail -= Size;
    }
  }
  head++;
  if (head >= Size) {
    head -= Size;
  }
  isFull = (head == tail);
}

template <typename T, size_t Size> T &RingBuffer<T, Size>::pop() {
  T &item = buffer[tail];
  isFull = false;
  tail++;
  if (tail >= Size) {
    tail -= Size;
  }
  return item;
}

template <typename T, size_t Size> bool RingBuffer<T, Size>::isEmpty() const {
  return (!isFull && (head == tail));
}

template <typename T, size_t Size> size_t RingBuffer<T, Size>::size() const {
  size_t size = Size;
  if (!isFull) {
    if (head >= tail) {
      size = head - tail;
    } else {
      size = Size + head - tail;
    }
  }
  return size;
}

template <typename T, size_t Size> void RingBuffer<T, Size>::reset() {
  head = 0;
  tail = 0;
  isFull = false;
}
