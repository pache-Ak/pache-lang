#ifndef FLEX_H
#define FLEX_H

#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <istream>
#include <memory>
#include <memory_resource>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../IRbuild/comp_unit.h"
#include "../ast/ast.h"
#include "../ast/compunit.h"
#include "ast/function.h"
#include "ast/statement.h"
#include "ast/type.h"

namespace pache {
using namespace std::literals::string_view_literals;
std::pmr::set<std::string_view> const key_words{};

std::unique_ptr<base_ast> flex(std::istream &in) {
  std::vector<std::string> vec_str;
  std::string s;
  std::getline(in, s);

  std::string identifier;
  auto beg = s.begin();
  while (beg != s.end()) {
    if (std::isspace(*beg)) {
      ++beg;
      continue;
    } else if (*beg == '/') {
      ++beg;
      if (*beg == '/') {
        break;
      } else {
        std::cerr << "error\n";
        // TODO
      }

    } else if (std::isalpha(*beg)) {
      identifier.push_back(*beg);
    }
  }
}

class location {

private:
  std::string_view m_file_name;
  std::size_t m_line_number;
  std::string m_line;
};

std::string error() { return "error\n"; }

std::string end_of_file() { return ""; }

class flex {

private:
  std::istream fin;
  std::string_view file_name;
  std::vector<std::string> lines;
  std::string::iterator begin;
  std::string::iterator end;
  std::string current_line;

  void next_line() {
    lines.push_back(std::move(current_line));
    std::getline(fin, current_line);
    begin = current_line.begin();
    end = current_line.end();
  }

  void line_comment() {
    if (*begin == '/') {
      ++begin;
      if (begin != end && *begin == '/') {
        next_line();
      } else {
        std::cerr << "error expect '/'";
      }
    } else {
      std::cerr << "shouldn't enter this function \"line_comment\"\n";
    }
  }

  static bool isidentifier_start_char(int c) {
    return c == '_' || std::isalpha(c);
  }
  static bool isidentifier_continue_char(int c) {
    return c == '_' || std::isalnum(c);
  }

  // call if need an identifier
  // if curent line doesn't have, this function can read next line.
  std::string_view get_identifier() {
    // std::string_view identifier;
    std::string::iterator view_begin;
    // std::string::iterator view_end;
    ignore_space();
    while (true) {
      // if (begin == end) {
      //   next_line();
      // } else if (std::isspace(*begin)) {
      //   ++begin;
      // } else
      if (*begin == '/') {
        line_comment();
        ignore_space();
      } else if (isidentifier_start_char(*begin)) {
        view_begin = begin;
        // identifier.push_back(*begin);
        ++begin;
        while (begin != end && isidentifier_start_char(*begin)) {
          // identifier.push_back(*begin);
          ++begin;
        }
        return std::string_view identifier(view_begin, begin);
      } else {
        std::cerr << "get identifier error.\n";
        return ""sv;
      }
    }
  }

  std::size_t get_integral() {
    ignore_space();
    std::size_t size;
    std::size_t s = std::stoull(current_line, &size, 0);
    begin += size;
    return s;
  }

  std::unique_ptr<compunit_ast> flex_file() {
    std::string_view sv;
    std::unique_ptr<compunit_ast> ast{};

    while (begin != end) {
      sv = get_identifier();
      if (sv == "func") {
      } else if (sv == "class") {

      } else if (sv == "let") {
      }
    }
  }
  std::unique_ptr<block_ast> block_flex() {}

  void ignore_space() {
    for (;;) {
      if (begin == end) {
        next_line();
      } else if (begin != end && std::isspace(*begin)) {
        ++begin;
      }
    }
  }
  std::unique_ptr<func_ast> func_flex() {
    // func has read

    std::unique_ptr<func_ast> ast = std::make_unique<func_ast>();
    ast->set_name(get_identifier());
    // std::string func_name = get_identifier();
    std::vector<std::string> args_type;
    std::vector<std::string> args_name;

    ignore_space();

    if (*begin == '(') {
      ++begin;
      for (;;) {
        ignore_space();
        if (*begin == ')') {
          ++begin;

          ast->set_type_name(get_identifier());
          ignore_space();
          ast->set_block(block_flex());
          return ast;
        } else if (isidentifier_start_char(*begin)) {
          args_type.emplace_back(get_identifier());

          args_name.emplace_back(get_identifier());

          ignore_space();
          if (*begin == ',') {
            ++begin;
            if (isidentifier_start_char(*begin)) {
              args_type.emplace_back(get_identifier());

              args_name.emplace_back(get_identifier());
            }
          }

        } else {
          std::cerr << "TODO function arguments error.\n";
        }
      }
    } else {

      std::cerr << "error in fuction defintion.\n";
    }
  }

  std::unique_ptr<compunit_ast> file_flex() {
    std::unique_ptr<compunit_ast> ast = std::make_unique<compunit_ast>();

    std::string key_word = get_identifier();
    if (key_word == "func") {
    }
  }

  std::unique_ptr<type_ast> type_flex() {
    std::unique_ptr<type_ast> primary =
        std::make_unique<user_def_type>(get_identifier());

    for (;;) {
      ignore_space();
      if (*begin == '*') {
        primary = std::make_unique<pointer_type>(std::move(primary));
        continue;
      } else if (*begin == '&') {
        primary = std::make_unique<reference_type>(std::move(primary));
      } else if (*begin == '[') {
        std::vector<std::size_t> size_list;
        for (;;) {
          size_list.emplace_back(get_integral());
          ignore_space();
          if (*begin == ',') {
            ++begin;
            continue;
          } else if (*begin == ']') {
            ++begin;
            break;
          } else {
            // TODO error
          }
        }
        9
      } else if (isidentifier_start_char(*begin)) {
        std::string s = get_identifier();
        std::string::iterator it = begin;
        if (s == "const"s) {

        } else if (s == "volatile") {

        } else {
          begin = it;
        }
      } else {
        // TODO error
      }
    }
  }
};

} // namespace pache