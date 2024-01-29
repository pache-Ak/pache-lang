#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace pache {
class scope_ast {
public:
  virtual ~scope_ast() = default;
};

class unqualified_scope_ast final : public scope_ast {
public:
  virtual ~unqualified_scope_ast() = default;
}; 

class qualified_scope_ast : public scope_ast {
public:
  virtual ~qualified_scope_ast() = default ;
private:
};

class relative_scope_ast final : public qualified_scope_ast {
public:
explicit relative_scope_ast(std::unique_ptr<scope_ast> &&father, std::string_view name)
  : m_father(std::move(father)), m_name(name){}

private:
  std::unique_ptr<scope_ast> m_father;
  std::string_view m_name;   
};

class root_scope_ast final : public qualified_scope_ast{
public:
  virtual ~root_scope_ast() = default;
private:
};
}

#endif
