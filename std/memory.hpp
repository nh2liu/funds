#pragma once
#include <memory>
#include <utility>

namespace funds {
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
  T *p_;

public:
  unique_ptr(T *ptr) : p_{ptr} {};
  unique_ptr(const unique_ptr<T> &other) = delete;
  unique_ptr<T> &operator=(const unique_ptr<T> &other) = delete;

  unique_ptr(unique_ptr<T> &&other) : p_{std::move(other.p_)} {}

  unique_ptr<T> &operator=(unique_ptr<T> &&other) {
    std::swap(p_, other.p_);
    return *this;
  };
  ~unique_ptr() { Deleter(p_); }

  T *operator->() noexcept { return p_; }

  T *reset() noexcept {
    Deleter(p_);
    p_ = nullptr;
  }

  T *release() noexcept {
    T *tmp = p_;
    p_ = nullptr;
    return tmp;
  }
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args &&... args) {
  return unique_ptr<T>(new T{std::forward<Args>(args)...});
}
} // namespace funds