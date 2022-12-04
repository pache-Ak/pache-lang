#ifndef DRIVE_H
#define DRIVE_H

#include "../build/parser.hpp"

namespace pache {
  class driver
{
public:
  driver ()= default;


  int result;

  // Run the parser on file F.  Return 0 on success.
  int parse (const std::string& f);
  // The name of the file being parsed.
  std::string file;
  // Whether to generate parser debug traces.
  bool trace_parsing;

  // Handling the scanner.
  void scan_begin (FILE *f);
  void scan_end ();
  // Whether to generate scanner debug traces.
  bool trace_scanning;
  // The token's location used by the scanner.
  pache::location location;
};
}

// Gives flex the yylex prototype we want.
#define YY_DECL                                                         \
  auto yylex()->pache::parser::symbol_type

// Declares yylex for the parser's sake.
YY_DECL;

#endif
