// file_editor.hpp

#pragma once

#include <string>
#include <vector>

class FileEditor {
public:
  FileEditor(const std::string &file_name);

  int Append(const std::string &text);
  int RemoveLine(const int line_number);
  std::vector<std::string> GetLines() const;

private:
  const std::string file_name_;
};