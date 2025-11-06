// todo_list.cpp

#include "todo_list.hpp"
#include "file_editor.hpp"
#include "utils.hpp"

#include <expected>
#include <print>
#include <sstream>
#include <string>
#include <utility>

std::expected<TodoList::FileEditorLocation, TodoListError>
TodoList::GetEditorLocationFromLineNumber(int line_number) {
  if (line_number < 1) {
    return std::unexpected(TodoListError::TaskNotFound);
  }

  const auto result_high = todo_high_.GetLineCount();
  if (!result_high) {
    return std::unexpected(TodoListError::LoadFailed);
  }

  const auto high_lines = result_high.value();
  if (line_number <= high_lines) {
    return FileEditorLocation(todo_high_, line_number);
  }

  line_number -= high_lines;

  const auto result_medium = todo_medium_.GetLineCount();
  if (!result_medium) {
    return std::unexpected(TodoListError::LoadFailed);
  }

  const auto medium_lines = result_medium.value();
  if (line_number <= medium_lines) {
    return FileEditorLocation(todo_medium_, line_number);
  }

  line_number -= medium_lines;

  const auto result_low = todo_low_.GetLineCount();
  if (!result_low) {
    return std::unexpected(TodoListError::LoadFailed);
  }

  const auto low_lines = result_low.value();
  if (line_number <= low_lines) {
    return FileEditorLocation(todo_low_, line_number);
  }

  return std::unexpected(TodoListError::TaskNotFound);
}

TodoList::TodoList()
    : todo_high_("todo_high.txt"), todo_medium_("todo_medium.txt"),
      todo_low_("todo_low.txt"), done_("done.txt") {}

std::expected<void, TodoListError> TodoList::Add(const std::string &text) {
  std::stringstream ss(text);
  std::string first_word;
  ss >> first_word;

  const auto first_lower = StringToLower(first_word);

  auto [file_editor,
        text_to_append] = [&]() -> std::pair<FileEditor *, std::string> {
    if (first_lower == "[h]" || first_lower == "[m]" || first_lower == "[l]") {

      std::string rest_of_line{};
      std::getline(ss, rest_of_line);

      if (!rest_of_line.empty() && rest_of_line[0] == ' ') {
        rest_of_line = rest_of_line.substr(1);
      }

      std::string final_text = first_lower + ' ' + rest_of_line;

      if (first_lower == "[h]") {
        return {&todo_high_, final_text};
      } else if (first_lower == "[l]") {
        return {&todo_low_, final_text};
      } else {
        return {&todo_medium_, final_text};
      }

    } else {
      std::string final_text = "[m] " + text;
      return {&todo_medium_, final_text};
    }
  }();

  if (!file_editor->Append(text_to_append)) {
    return std::unexpected(TodoListError::SaveFailed);
  }

  return {};
}

std::expected<void, TodoListError> TodoList::Edit(const int line_number,
                                                  const std::string &text) {
  if (text.empty()) {
    return std::unexpected(TodoListError::InvalidTaskText);
  }

  const auto result_fel = GetEditorLocationFromLineNumber(line_number);
  if (!result_fel) {
    return std::unexpected(result_fel.error());
  }

  FileEditorLocation fel = result_fel.value();

  if (const auto result =
          fel.file_editor.EditLine(fel.relative_line_number, text);
      !result) {
    if (result.error() == FileEditorError::LineNotFound) {
      return std::unexpected(TodoListError::TaskNotFound);
    } else {
      return std::unexpected(TodoListError::SaveFailed);
    }
  }

  return {};
}
std::expected<void, TodoListError> TodoList::Remove(const int line_number) {
  const auto result_fel = GetEditorLocationFromLineNumber(line_number);
  if (!result_fel) {
    return std::unexpected(result_fel.error());
  }

  FileEditorLocation fel = result_fel.value();

  if (const auto result = fel.file_editor.RemoveLine(fel.relative_line_number);
      !result) {
    if (result.error() == FileEditorError::LineNotFound) {
      return std::unexpected(TodoListError::TaskNotFound);
    } else {
      return std::unexpected(TodoListError::SaveFailed);
    }
  }

  return {};
}

std::expected<void, TodoListError> TodoList::Done(const int line_number) {
  const auto result_fel = GetEditorLocationFromLineNumber(line_number);
  if (!result_fel) {
    return std::unexpected(result_fel.error());
  }

  FileEditorLocation fel = result_fel.value();

  if (const auto result = fel.file_editor.GetLine(fel.relative_line_number)) {
    const auto task_text = result.value();

    if (!done_.Append(task_text)) {
      return std::unexpected(TodoListError::SaveFailed);
    }

    if (const auto result_2 = Remove(line_number); !result_2) {
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
  if (!todo_high_.Delete() || !todo_medium_.Delete() || !todo_low_.Delete() ||
      !done_.Delete()) {
    return std::unexpected(TodoListError::ClearFailed);
  }

  return {};
}

std::expected<void, TodoListError> TodoList::List() const {
  const auto result_high = todo_high_.GetLines();
  const auto result_medium = todo_medium_.GetLines();
  const auto result_low = todo_low_.GetLines();

  if (!result_high && !result_medium && !result_low) {
    return std::unexpected(TodoListError::LoadFailed);
  };

  std::println("To-Do List:");
  print_separator();
  std::println("To-do:");

  int relative_line_number{1};

  if (result_high) {
    const auto high_lines = result_high.value();
    for (const auto &line : high_lines) {
      std::println("  [{}]: {}", relative_line_number, line);
      ++relative_line_number;
    }
  }
  if (result_medium) {
    const auto medium_lines = result_medium.value();
    for (const auto &line : medium_lines) {
      std::println("  [{}]: {}", relative_line_number, line);
      ++relative_line_number;
    }
  }
  if (result_low) {
    const auto low_lines = result_low.value();
    for (const auto &line : low_lines) {
      std::println("  [{}]: {}", relative_line_number, line);
      ++relative_line_number;
    }
  }

  print_separator();

  if (const auto result_2 = done_.GetLines()) {
    const auto done_lines = result_2.value();

    std::println("Done:");
    for (size_t i = 0; i < done_lines.size(); ++i) {
      std::println("  [{}]: {}", i + 1, done_lines[i]);
    }
    print_separator();
  }

  return {};
}