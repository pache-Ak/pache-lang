#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include "parser.tab.hh"
#include "location.hh"
#include "ast/compunit.h"

namespace pache {
class driver
{
public:
  explicit driver(std::string const &f);

  // Run the parser on file f.  Return 0 on success.
  int parse ();
  
    // Handling the scanner.
  void scan_begin ();
  void scan_end ();
     // The token's location used by the scanner.
  pache::location location; 

  // The name of the file being parsed.
  std::string file_name;

  // Whether to generate parser debug traces.
  bool trace_parsing;
// Whether to generate scanner debug traces.
  bool trace_scanning;
  // ast
  std::unique_ptr<compunit_ast> root_ast;
};
}

// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  pache::parser::symbol_type yylex (pache::driver &drv)
// ... and declare it for the parser's sake.
YY_DECL;

#endif // ! DRIVER_HH
