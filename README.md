# CPP To-Do List Utility

> A a to-do list utility that uses command-line arguments to add tasks, remove tasks, and list all tasks.

### Example

```
$ ./todo help
Help:
-------------------------------------------------------------
  add (a) [h/m/l] <text>   : Add a new task (Default: [m])
  edit (e) <line> <text>   : Edit a task
  remove (r) <line>        : Remove a task
  done (d) <line>          : Mark a task as complete
  list (l)                 : List all tasks
  clear (c)                : Clear all tasks
  help (h)                 : List all utility commands
-------------------------------------------------------------

$ ./todo add "Call dentist"
Successfully added '[m] Call dentist'.

$ ./todo a "[h] Finish report"
Successfully added '[h] Finish report'.

$ ./todo a "[L] Water plants"
Successfully added '[l] Water plants'.

$ ./todo a "Buy milk"
Successfully added '[m] Buy milk'.

$ ./todo list
To-Do List:
-------------------------------------------------------------
To-do:
  [1]: [h] Finish report
  [2]: [m] Call dentist
  [3]: [m] Buy milk
  [4]: [l] Water plants
-------------------------------------------------------------

$ ./todo edit 2 "Call dentist (tomorrow)"
Successfully edited #2 to 'Call dentist (tomorrow)'.

$ ./todo done 1
Successfully marked #1 as done.

$ ./todo r 3
Successfully removed #3.

$ ./todo l
To-Do List:
-------------------------------------------------------------
To-do:
  [1]: [m] Call dentist (tomorrow)
  [2]: [l] Water plants
-------------------------------------------------------------
Done:
  [1]: [h] Finish report
-------------------------------------------------------------

$ ./todo clear
Successfully cleared to-do list.