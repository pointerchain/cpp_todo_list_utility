// todo_list.hpp

#pragma once

#include "file_editor.hpp"
#include <string>

enum class TodoListError {
  SaveFailed,
  LoadFailed,
  TaskNotFound,
  InvalidTaskText,
  ClearFailed
};

class TodoList {
public:
  TodoList();

  std::expected<void, TodoListError> Add(const std::string &text);
  std::expected<void, TodoListError> Edit(const int line_number,
                                          const std::string &text);
  std::expected<void, TodoListError> Remove(const int line_number);
  std::expected<void, TodoListError> Done(const int line_number);
  std::expected<void, TodoListError> Clear();
  std::expected<void, TodoListError> List() const;

private:
  FileEditor todo_;
  FileEditor done_;
};