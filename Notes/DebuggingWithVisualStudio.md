# Debugging with Visual Studio

---

Start the debugger using F10 after compilation.

"Autos" or "Locals" windows shows values of active local variables.
Other variables and expressions can be viewed in the "Watch" window.

The current line of code can be run using F10 or F11.

F10 and F11 have slightly different functions:
  - F10 runs the entire function
  - F11 will go into the function and allow you to trace through line-by-line

To stop tracing line by line, use F5, then the program will run to completion.

Breakpoints are used to avoid tracing through the entire program from the
beginning. Breakpoints are added by pressing F9 or clicking in the left margin.
Press F5 to run until the breakpoint, then step through each line afterwards.
