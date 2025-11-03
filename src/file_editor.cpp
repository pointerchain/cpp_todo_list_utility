// file_editor.cpp

#include "file_editor.hpp"
#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

FileEditor::FileEditor(const std::string &file_name) : file_name_(file_name) {}

std::expected<void, FileEditorError>
FileEditor::Append(const std::string &text) {
  std::ofstream file(file_name_, std::ios::app);

  if (!file.is_open()) {
    return std::unexpected(FileEditorError::OpenSourceFailed);
  }

  file << text << '\n';

  if (file.fail()) {
    return std::unexpected(FileEditorError::WriteFailed);
  }

  return {};
}

std::expected<std::string, FileEditorError>
FileEditor::GetLine(const int line_number) {
  if (line_number < 1) {
    return std::unexpected(FileEditorError::LineNotFound);
  }

  std::ifstream file(file_name_);

  if (!file.is_open()) {
    return std::unexpected(FileEditorError::OpenSourceFailed);
  }

  int current_line_number{1};
  std::string line;
  while (std::getline(file, line)) {
    if (current_line_number == line_number)
      return line;

    ++current_line_number;
  }

  return std::unexpected(FileEditorError::LineNotFound);
}

std::expected<void, FileEditorError>
FileEditor::EditLine(const int line_number, const std::string &text) {
  if (line_number < 1) {
    return std::unexpected(FileEditorError::LineNotFound);
  }

  std::ifstream file(file_name_);

  if (!file.is_open()) {
    return std::unexpected(FileEditorError::OpenSourceFailed);
  }

  const std::string temp_file_name{file_name_ + ".tmp"};
  std::ofstream temp_file(temp_file_name);

  if (!temp_file.is_open()) {
    return std::unexpected(FileEditorError::OpenTempFailed);
  }

  bool line_found{false};
  int current_line_number{1};
  std::string line;
  while (std::getline(file, line)) {
    if (line_number == current_line_number) {
      if (!text.empty()) {
        temp_file << text << '\n';

        if (temp_file.fail()) {
          return std::unexpected(FileEditorError::WriteFailed);
        }
      }

      line_found = true;
    } else {
      temp_file << line << '\n';

      if (temp_file.fail()) {
        return std::unexpected(FileEditorError::WriteFailed);
      }
    }

    ++current_line_number;
  }

  if (!line_found) {
    std::filesystem::remove(temp_file_name);
    return std::unexpected(FileEditorError::LineNotFound);
  }

  try {
    std::filesystem::rename(temp_file_name, file_name_);
  } catch (const std::filesystem::filesystem_error &) {
    std::filesystem::remove(temp_file_name);
    return std::unexpected(FileEditorError::FileSwapFailed);
  }

  return {};
}

std::expected<void, FileEditorError>
FileEditor::RemoveLine(const int line_number) {
  return EditLine(line_number, "");
}

std::expected<std::vector<std::string>, FileEditorError>
FileEditor::GetLines() const {
  std::ifstream file(file_name_);

  if (!file.is_open()) {
    return std::unexpected(FileEditorError::OpenSourceFailed);
  }

  std::vector<std::string> lines{};
  std::string s;
  while (std::getline(file, s)) {
    lines.push_back(s);
  }

  return lines;
}

std::expected<void, FileEditorError> FileEditor::Delete() {
  try {
    std::filesystem::remove(file_name_);
  } catch (const std::filesystem::filesystem_error &) {
    return std::unexpected(FileEditorError::RemoveFailed);
  }

  return {};
}