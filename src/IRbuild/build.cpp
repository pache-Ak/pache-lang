#include "build.h"
#include "type.h"

using namespace std::literals::string_view_literals;

namespace pache {
std::unordered_map<std::string_view, std::unique_ptr<build_type>> const
base_build::prprimary_type{
  []{
    std::unordered_map<std::string_view, std::unique_ptr<build_type>> val;
    val.emplace("void"sv, std::make_unique<void_type_t>());
    val.emplace("bool"sv, std::make_unique<bool_type_t>());
    val.emplace("s"sv,    std::make_unique<size_type_t>());

    return val;
  }()
};
std::function<std::unique_ptr<build_variable>(std::string_view, literal)>
base_build::find_integer_literal_operator(std::string_view suffix) {
  auto search = integer_literal_operators.find(suffix);
  if (search != integer_literal_operators.end()) {
    return search->second;
  } else if (m_father != nullptr) {
    return m_father->find_integer_literal_operator(suffix);
  } else {
    return {};
  }
}
} // namespace pache