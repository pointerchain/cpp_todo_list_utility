// main.cpp

#include "file_editor.hpp"

#include <cctype>
#include <iostream>
#include <print>
#include <string>
#include <vector>

std::string StringToLower(const std::string &str) {
  std::string new_string{};
  new_string.reserve(str.size());

  for (auto &c : str) {
    new_string += tolower(static_cast<unsigned char>(c));
  }

  return new_string;
}

int AddItem(FileEditor &file, const std::string &text) {
  const int success{file.Append(text)};

  if (success == 0) {
    std::println("Appended '{}'.", text);
  }

  return success;
}

int RemoveItem(FileEditor &file, const std::string &raw_line_number) {
  try {
    const auto line_number = std::stoi(raw_line_number);

    const int success{file.RemoveLine(line_number)};

    if (success == 0) {
      std::println("Removed #{}.", line_number);
    }

    return success;
  } catch (const std::invalid_argument &e) {
    std::cerr << "Invalid argument: " << e.what() << '\n';
    return 1;
  } catch (const std::out_of_range &e) {
    std::cerr << "Out of range: " << e.what() << '\n';
    return 1;
  }
}

void ListItems(const FileEditor &file) {
  auto lines = file.GetLines();

  std::println("To-Do List:");
  for (size_t i = 0; i < lines.size(); ++i) {
    std::println("  [{}]: {}", i + 1, lines[i]);
  }
}

int main(const int argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "No args passed\n";
    return 1;
  }

  const std::vector<std::string> args(argv, argv + argc);
  const auto command = StringToLower(args[1]);
  FileEditor file("save.txt");

  if (command == "add") {
    if (argc < 3) {
      std::cerr << "Missing item_to_add arg\n";
      return 1;
    }

    return AddItem(file, args[2]);
  } else if (command == "remove") {
    if (argc < 3) {
      std::cerr << "Missing item_index_to_remove arg\n";
      return 1;
    }

    return RemoveItem(file, args[2]);
  } else if (command == "list") {
    ListItems(file);
  } else {
    std::cerr << "Invalid argument[s]\n";
    return 1;
  }

  return 0;
}