#include "todo_list.hpp"
#include <expected>
#include <print>

TodoList::TodoList() : todo_("todo.txt"), done_("done.txt") {}

std::expected<void, TodoListError> TodoList::Add(const std::string &text) {
  if (!todo_.Append(text)) {
    return std::unexpected(TodoListError::SaveFailed);
  }

  return {};
}

std::expected<void, TodoListError> TodoList::Edit(const int line_number,
                                                  const std::string &text) {
  if (text.empty()) {
    return std::unexpected(TodoListError::InvalidTaskText);
  }

  if (auto result = todo_.EditLine(line_number, text); !result) {
    if (result.error() == FileEditorError::LineNotFound) {
      return std::unexpected(TodoListError::TaskNotFound);
    } else {
      return std::unexpected(TodoListError::SaveFailed);
    }
  }

  return {};
}
std::expected<void, TodoListError> TodoList::Remove(const int line_number) {
  if (auto result = todo_.RemoveLine(line_number); !result) {
    if (result.error() == FileEditorError::LineNotFound) {
      return std::unexpected(TodoListError::TaskNotFound);
    } else {
      return std::unexpected(TodoListError::SaveFailed);
    }
  }

  return {};
}

std::expected<void, TodoListError> TodoList::Done(const int line_number) {
  if (auto result = todo_.GetLine(line_number)) {
    auto task_text = result.value();

    if (!done_.Append(task_text)) {
      return std::unexpected(TodoListError::SaveFailed);
    }

    if (auto result_2 = Remove(line_number); !result_2) {
      return result_2;
    }
  } else {
    if (result.error() == FileEditorError::LineNotFound) {
      return std::unexpected(TodoListError::TaskNotFound);
    } else {
      return std::unexpected(TodoListError::LoadFailed);
    }
  }

  return {};
}

std::expected<void, TodoListError> TodoList::Clear() {
  if (!todo_.Delete() || !done_.Delete()) {
    return std::unexpected(TodoListError::ClearFailed);
  }

  return {};
}

std::expected<void, TodoListError> TodoList::List() const {
  if (auto result = todo_.GetLines()) {
    auto todo_lines = result.value();

    std::println("To-Do List:");
    std::println(
        "-------------------------------------------------------------");
    std::println("To-do:");
    for (size_t i = 0; i < todo_lines.size(); ++i) {
      std::println("  [{}]: {}", i + 1, todo_lines[i]);
    }
    std::println(
        "-------------------------------------------------------------");

    if (auto result_2 = done_.GetLines()) {
      auto done_lines = result_2.value();

      std::println("Done:");
      for (size_t i = 0; i < done_lines.size(); ++i) {
        std::println("  [{}]: {}", i + 1, done_lines[i]);
      }
      std::println("----------------------------------------");
    }
  } else {
    return std::unexpected(TodoListError::LoadFailed);
  }

  return {};
}