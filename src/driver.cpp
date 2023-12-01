#include "driver.h"

pache::driver::driver(std::string const &f)
  : file_name(f), location(&f), trace_parsing(false), trace_scanning (false){};

int
pache::driver::parse ()
{
  scan_begin ();
  pache::parser parse (*this);
  parse.set_debug_level (trace_parsing);
  int res = parse ();
  scan_end ();
  return res;
}
