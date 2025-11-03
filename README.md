# CPP To-Do List Utility

> A a to-do list utility that uses command-line arguments to add tasks, remove tasks, and list all tasks.

### Example

```
$ ./todo help
Help:
-------------------------------------------------------------
  add (a) <text>           : Add a new task
  edit (e) <line> <text>   : Edit a task
  remove (r) <line>        : Remove a task
  done (d) <line>          : Mark a task as complete
  list (l)                 : List all tasks
  clear (c)                : Clear all tasks
  help (h)                 : List all utility commands
-------------------------------------------------------------

$ ./todo add Walk dog
Successfully added 'Walk dog'.

$ ./todo a "Take out garbagge"
Successfully added 'Take out garbagge'.

$ ./todo list
To-Do List:
-------------------------------------------------------------
To-do:
  [1]: Walk dog
  [2]: Take out garbagge
-------------------------------------------------------------

$ ./todo edit 2 "Take out garbage"
Successfully edited #2 to 'Take out garbage'.

$ ./todo a "Read a chapter of C++ book"
Successfully added 'Read a chapter of C++ book'.

$ ./todo remove 2
Successfully removed #2.

$ ./todo done 1
Successfully marked #1 as done.

$ ./todo l
To-Do List:
-------------------------------------------------------------
To-do:
  [1]: Read a chapter of C++ book
-------------------------------------------------------------
Done:
  [1]: Walk dog
----------------------------------------

$ ./todo clear
Successfully cleared to-do list.