# CPP To-Do List Utility

> A a to-do list utility that uses command-line arguments to add entrees, remove entrees, and list all entrees.

### Example
$ ./todo add "Walk dog."
Appended 'Walk dog.'.

$ ./todo add "Clean room."
Appended 'Clean room.'.

$ ./todo add "Go for a two mile run."
Appended 'Go for a two mile run.'.

$ ./todo add "Do the laundry."
Appended 'Do the laundry.'.

$ ./todo list
To-Do List:
  [1]: Walk dog.
  [2]: Clean room.
  [3]: Go for a two mile run.
  [4]: Do the laundry.

$ ./todo remove 3
Removed #3.

$ ./todo list
To-Do List:
  [1]: Walk dog.
  [2]: Clean room.
  [3]: Do the laundry.

$ ./todo remove 1
Removed #1.

$ ./todo list
To-Do List:
  [1]: Clean room.
  [2]: Do the laundry.