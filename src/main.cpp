// main.cpp

#include "todo_list.hpp"
#include "utils.hpp"

#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

bool AddTask(TodoList &todo_list, const std::string &text) {
  if (todo_list.Add(text)) {
    std::println("Successfully added '{}'.", text);
  } else {
    std::cerr << "Error: Failed to save the new task to the list.\n";
    return false;
  }

  return true;
}

bool EditTask(TodoList &todo_list, const std::string &raw_line_number,
              const std::string &text) {
  try {
    const auto line_number = std::stoi(raw_line_number);

    if (auto result = todo_list.Edit(line_number, text)) {
      std::println("Successfully edited #{} to '{}'.", line_number, text);
      return true;
    } else if (result.error() == TodoListError::InvalidTaskText) {
      std::cerr << "Error: Task text cannot be empty.\n";
      return false;
    } else if (result.error() == TodoListError::TaskNotFound) {
      std::cerr << "Error: No task found with line number " << line_number
                << ".\n";
      return false;
    } else {
      std::cerr << "Error: Failed to save your edit.\n";
      return false;
    }
  } catch (const std::invalid_argument &) {
    std::cerr << "Error: Invalid line number. Please enter a number.\n";
    return false;
  } catch (const std::out_of_range &) {
    std::cerr << "Error: Invalid line number. Number is out of range.\n";
    return false;
  }
}

bool RemoveTask(TodoList &todo_list, const std::string &raw_line_number) {
  try {
    const auto line_number = std::stoi(raw_line_number);

    if (auto result = todo_list.Remove(line_number)) {
      std::println("Successfully removed #{}.", line_number);
      return true;
    } else if (result.error() == TodoListError::TaskNotFound) {
      std::cerr << "Error: No task found with line number " << line_number
                << ".\n";
      return false;
    } else {
      std::cerr << "Error: Failed to save your remove.\n";
      return false;
    }
  } catch (const std::invalid_argument &) {
    std::cerr << "Error: Invalid line number. Please enter a number.\n";
    return false;
  } catch (const std::out_of_range &) {
    std::cerr << "Error: Invalid line number. Number is out of range.\n";
    return false;
  }
}

bool DoneTask(TodoList &todo_list, const std::string &raw_line_number) {
  try {
    const auto line_number = std::stoi(raw_line_number);

    if (auto result = todo_list.Done(line_number)) {
      std::println("Successfully marked #{} as done.", line_number);
      return true;
    } else if (result.error() == TodoListError::LoadFailed) {
      std::cerr << "Error: Failed to load your source file.\n";
      return false;
    } else if (result.error() == TodoListError::TaskNotFound) {
      std::cerr << "Error: No task found with line number " << line_number
                << ".\n";
      return false;
    } else {
      std::cerr << "Error: Failed to save your done change.\n";
      return false;
    }
  } catch (const std::invalid_argument &) {
    std::cerr << "Error: Invalid line number. Please enter a number.\n";
    return false;
  } catch (const std::out_of_range &) {
    std::cerr << "Error: Invalid line number. Number is out of range.\n";
    return false;
  }
}

bool ListTasks(const TodoList &todo_list) {
  if (!todo_list.List()) {
    std::cerr << "Error: Failed to load list.\n";
    return false;
  }

  return true;
}

bool ClearTasks(TodoList &todo_list) {
  if (!todo_list.Clear()) {
    std::cerr << "Error: Failed to clear files.\n";
    return false;
  }

  std::println("Successfully cleared to-do list.");

  return true;
}

void OutputHelp(const bool is_interactive) {
  std::println("Help:");
  print_separator();
  std::println("  {:<24} : {}", "add (a) [h/m/l] <text>",
               "Add a new task (Default: [m])");
  std::println("  {:<24} : {}", "edit (e) <line> <text>", "Edit a task");
  std::println("  {:<24} : {}", "remove (r) <line>", "Remove a task");
  std::println("  {:<24} : {}", "done (d) <line>", "Mark a task as complete");
  std::println("  {:<24} : {}", "list (l)", "List all tasks");
  std::println("  {:<24} : {}", "clear (c)", "Clear all tasks");
  std::println("  {:<24} : {}", "help (h)", "List all utility commands");
  if (is_interactive) {
    std::println("  {:<24} : {}", "quit (q)", "Exits out of interactive mode");
  }
  print_separator();
}

bool HandleCommand(const std::vector<std::string> args,
                   const bool is_interactive) {
  TodoList todo_list{};
  const auto command = StringToLower(args[1]);

  if (command == "add" || command == "a") {
    if (args.size() < 3) {
      std::cerr << "Error: Missing text for 'add' command.\n";
      return false;
    } else {
      std::string text{args[2]};
      for (size_t i = 3; i < args.size(); ++i) {
        text += ' ' + args[i];
      }

      return AddTask(todo_list, text);
    }
  } else if (command == "edit" || command == "e") {
    if (args.size() < 4) {
      std::cerr
          << "Error: Missing line number and/or text for 'edit' command.\n";
      return false;
    } else {
      std::string text{args[3]};
      for (size_t i = 4; i < args.size(); ++i) {
        text += ' ' + args[i];
      }

      return EditTask(todo_list, args[2], text);
    }
  } else if (command == "remove" || command == "r") {
    if (args.size() < 3) {
      std::cerr << "Error: Missing line number for 'remove' command.\n";
      return false;
    } else {
      return RemoveTask(todo_list, args[2]);
    }
  } else if (command == "done" || command == "d") {
    if (args.size() < 3) {
      std::cerr << "Error: Missing line number for 'done' command.\n";
      return false;
    } else {
      return DoneTask(todo_list, args[2]);
    }
  } else if (command == "list" || command == "l") {
    return ListTasks(todo_list);
  } else if (command == "clear" || command == "c") {
    return ClearTasks(todo_list);
  } else if (command == "help" || command == "h") {
    OutputHelp(is_interactive);
  } else {
    std::cerr << "Error: Invalid command '" << command
              << "' (Run 'help' for commands).\n";
    return false;
  }

  return true;
}

int main(const int argc, const char *argv[]) {
  bool interactive_mode{false};
  bool success{true};

  if (argc < 2) {
    interactive_mode = true;

    std::println(
        "\nEntering interactive mode. (Type 'h' for help, 'q' to quit)\n");
    print_separator();

    std::string s;
    std::print("\n> ");
    while (std::getline(std::cin, s, '\n')) {
      std::vector<std::string> args{static_cast<std::string>(argv[0])};
      std::stringstream ss(s);

      std::string arg{};
      while (ss >> arg) {
        args.push_back(arg);
      }

      if (args.size() < 2) {
        continue;
      }

      std::println();

      const std::string command = StringToLower(args[1]);
      if (command == "quit" || command == "q") {
        break;
      }

      success = HandleCommand(args, true);

      if (!success) {
        break;
      }

      std::print("\n> ");
    }

    print_separator();
  } else {
    const std::vector<std::string> args(argv, argv + argc);

    success = HandleCommand(args, false);
  }

  return success ? 0 : 1;
}