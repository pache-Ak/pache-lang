#ifndef SCOPE_H
#define SCOPE_H

#include <string_view>
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
  void append(std::string_view name);
  virtual ~qualified_scope_ast() = default ;
private:
    std::vector<std::string_view> m_names;   
};

class relative_scope_ast final : public qualified_scope_ast {

};

class root_scope_ast final : public qualified_scope_ast {
public:

private:
};
}

#endif
