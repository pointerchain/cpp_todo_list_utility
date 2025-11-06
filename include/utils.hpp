// utils.hpp

#pragma once

#include <cctype>
#include <print>
#include <string>

inline void print_separator() {
  std::println("-------------------------------------------------------------");
}

inline std::string StringToLower(const std::string &str) {
  std::string new_string{};
  new_string.reserve(str.size());

  for (const auto &c : str) {
    new_string += tolower(static_cast<unsigned char>(c));
  }

  return new_string;
}