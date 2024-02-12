#ifndef REFERENCE_PTR
#define REFERENCE_PTR

#include <cstddef>
#include <memory>
#include <functional>

namespace pache {
template<class T>
class reference_ptr{
  public:
  using pointer = T *;

  constexpr reference_ptr() noexcept :  m_ptr(nullptr){}
  constexpr reference_ptr( std::nullptr_t ) noexcept : m_ptr(nullptr){};

  reference_ptr( std::unique_ptr<T>const& p ) noexcept :m_ptr(p.get()){};
  reference_ptr( std::shared_ptr<T>const & p ) noexcept :m_ptr(p.get()){};
  ~reference_ptr() {}

friend bool operator==(reference_ptr lhs, std::nullptr_t rhs) noexcept {
  return lhs.m_ptr == nullptr;
}

friend bool operator!=(reference_ptr lhs, std::nullptr_t rhs) noexcept {
  return lhs.m_ptr != nullptr;
}
friend bool operator==(reference_ptr lhs, reference_ptr rhs) noexcept {
  return lhs.m_ptr == rhs.m_ptr;
  }

friend bool operator!=(reference_ptr lhs, reference_ptr rhs) noexcept {
  return lhs.m_ptr != rhs.m_ptr;
  }

typename std::add_lvalue_reference<T>::type operator*() const
   noexcept(noexcept(*std::declval<pointer>())) {
     *m_ptr;
   }
    pointer operator->() const noexcept {
      return m_ptr;
    }
    template <typename U>
    friend struct std::hash;
private:
  T * m_ptr;
};
// 
template<class T>
class reference_ptr<T []>{
  public:
  using pointer = T *;
  constexpr reference_ptr() noexcept : m_ptr(nullptr){}
 constexpr reference_ptr( std::nullptr_t ) noexcept : m_ptr(nullptr){};
   reference_ptr( std::unique_ptr<T>const& p ) noexcept :m_ptr(p.get()){};
   reference_ptr( std::shared_ptr<T>const & p ) noexcept :m_ptr(p.get()){};

T& operator[]( std::size_t i ) const {
  return m_ptr[i];
}
private:
  T *const m_ptr;
};

}
template<class T>
struct std::hash<pache::reference_ptr<T>> {
  std::size_t operator()(pache::reference_ptr<T> const &r) const noexcept {
    return std::hash<T*>{}(r.m_ptr);
  }
};
template<class T>
struct std::hash<pache::reference_ptr<T []>> {
  std::size_t operator()(pache::reference_ptr<T []> const &r) const noexcept {
    return std::hash<T*>{}(r.m_ptr);
  }
};

#endif
