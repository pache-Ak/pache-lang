
#include "type.h"
#include "ast.h"
#include "compunit.h"

namespace pache {
const void_type_t void_type_t::void_type{};

const bool_type_t bool_type_t::bool_type{};

const size_type_t size_type_t::size_type{};

const i8_type_t i8_type_t::i8_type{};
const i16_type_t i16_type_t::i16_type{};
const i32_type_t i32_type_t::i32_type{};
const i64_type_t i64_type_t::i64_type{};
const i128_type_t i128_type_t::i128_type{};

const u8_type_t u8_type_t::u8_type{};
const u16_type_t u16_type_t::u16_type{};
const u32_type_t u32_type_t::u32_type{};
const u64_type_t u64_type_t::u64_type{};
const u128_type_t u128_type_t::u128_type{};

const f16_type_t f16_type_t::f16_type{};
const f32_type_t f32_type_t::f32_type{};
const f64_type_t f64_type_t::f64_type{};
const f128_type_t f128_type_t::f128_type{};

const d32_type_t d32_type_t::d32_type{};
const d64_type_t d64_type_t::d64_type{};

const c8_type_t c8_type_t::c8_type{};
const c16_type_t c16_type_t::c16_type{};
const c32_type_t c32_type_t::c32_type{};

func_type::func_type(const std::vector<variable_ast *> &args,
                     const type_ast *return_type)
    : m_return_type(return_type) {
  std::for_each(args.begin(), args.end(), [=](variable_ast *var) {
    m_args_type.push_back(var->get_type());
  });
}

type_ast *user_def_type::get(compunit_ast *comp) {
  return comp->find_class_def(m_iden)
             ? new class_type(comp->find_class_def(m_iden))
             : nullptr;
}
} // namespace pache
