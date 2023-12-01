#ifndef REFERENCE_PTR
#define REFERENCE_PTR

#include <cstddef>
#include <memory>
namespace pache {
template<class T>
class reference_ptr{
  public:
  using pointer = T *;


  constexpr reference_ptr( std::nullptr_t ) noexcept : m_ptr(nullptr){};

  explicit reference_ptr( std::unique_ptr<T>const& p ) noexcept :m_ptr(p.get()){};
  explicit reference_ptr( std::shared_ptr<T>const & p ) noexcept :m_ptr(p.get()){};


friend bool operator==(reference_ptr lhs, std::nullptr_t rhs) noexcept;

friend bool operator!=(reference_ptr lhs, std::nullptr_t rhs) noexcept;


typename std::add_lvalue_reference<T>::type operator*() const
    noexcept(noexcept(*std::declval<pointer>())) {
      *m_ptr;
    }
    pointer operator->() const noexcept {
      return m_ptr;
    }

private:
  T *const m_ptr;
};

template<class T>
class reference_ptr<T []>{
  public:
  using pointer = T *;

 constexpr reference_ptr( std::nullptr_t ) noexcept : m_ptr(nullptr){};
  explicit reference_ptr( std::unique_ptr<T>const& p ) noexcept :m_ptr(p.get()){};
  explicit reference_ptr( std::shared_ptr<T>const & p ) noexcept :m_ptr(p.get()){};

T& operator[]( std::size_t i ) const {
  return m_ptr[i];
}

friend bool operator==(reference_ptr lhs, std::nullptr_t rhs) noexcept;

friend bool operator!=(reference_ptr lhs, std::nullptr_t rhs) noexcept;

private:
  T *const m_ptr;
};
template<class T>
 bool operator==(reference_ptr<T> lhs, std::nullptr_t rhs) noexcept{
  return lhs.m_ptr == rhs;
}
template<class T>
 bool operator!=(reference_ptr<T> lhs, std::nullptr_t rhs) noexcept{
  return lhs.m_ptr != rhs;
}
}

#endif