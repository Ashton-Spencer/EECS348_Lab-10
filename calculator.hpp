#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>

// this function should expect that `expression` has already been
// validated and thus throw no exceptions
double parse_number(const std::string &expression);
bool is_valid_double(const std::string &s);
std::string add_string_numbers(const std::string &a, const std::string &b);


#endif // __CALCULATOR_HPP
