// file_editor.cpp

#include "file_editor.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

FileEditor::FileEditor(const std::string &file_name) : file_name_(file_name) {}

int FileEditor::Append(const std::string &text) {
  std::ofstream file(file_name_, std::ios::app);

  if (!file.is_open()) {
    std::cerr << "Error: Could not open " << file_name_ << '\n';
    return 1;
  }

  file << text << '\n';

  return 0;
}

int FileEditor::RemoveLine(const int line_number) {
  std::ifstream file(file_name_);

  if (!file.is_open()) {
    std::cerr << "Error: Could not open " << file_name_ << '\n';
    return 1;
  }

  const std::string temp_file_name{file_name_ + ".tmp"};
  std::ofstream temp_file(temp_file_name);

  if (!temp_file.is_open()) {
    std::cerr << "Error: Could not open " << temp_file_name << '\n';
    return 1;
  }

  bool line_found{};
  int i{1};
  std::string line;
  while (std::getline(file, line)) {
    if (line_number == i) {
      line_found = true;
    } else {
      temp_file << line << '\n';
    };

    ++i;
  }

  if (!line_found) {
    std::cerr << "Error: Line " << line_number
              << " not found. No changes made.\n";
    std::filesystem::remove(temp_file_name);
    return 1;
  }

  try {
    std::filesystem::remove(file_name_);
    std::filesystem::rename(temp_file_name, file_name_);
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << '\n';
    std::filesystem::remove(temp_file_name);
    return 1;
  }

  return 0;
}

std::vector<std::string> FileEditor::GetLines() const {
  std::ifstream file(file_name_);

  if (!file.is_open()) {
    std::cerr << "Error: Could not open " << file_name_ << '\n';
    return {};
  }

  std::vector<std::string> lines{};
  std::string s;
  while (std::getline(file, s)) {
    lines.push_back(s);
  }

  return lines;
}