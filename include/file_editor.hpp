// file_editor.hpp

#pragma once

#include <expected>
#include <string>
#include <vector>

enum class FileEditorError {
  OpenSourceFailed,
  OpenTempFailed,
  WriteFailed,
  FileSwapFailed,
  LineNotFound,
  RemoveFailed
};

class FileEditor {
public:
  FileEditor(const std::string &file_name);

  std::expected<void, FileEditorError> Append(const std::string &text);
  std::expected<std::string, FileEditorError> GetLine(const int line_number);
  std::expected<void, FileEditorError> EditLine(const int line_numer,
                                                const std::string &text);
  std::expected<void, FileEditorError> RemoveLine(const int line_number);
  std::expected<std::vector<std::string>, FileEditorError> GetLines() const;
  std::expected<void, FileEditorError> Delete();

private:
  const std::string file_name_;
};