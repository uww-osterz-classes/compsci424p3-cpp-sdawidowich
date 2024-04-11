[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=14703186)
# UWW COMPSCI 424 Program 3 in C++

## Your notes for me

*If you have notes or messages for me, please put them here so I can see them.*

## About this repository

Use this template for Operating Systems Program 3 in C++.

### Starter code

To help you get started quickly, the starter code file includes code to process the two command-line arguments and begin processing the setup file. You may use, modify, or delete this code. There are also hints about other steps to complete in the main program.

The included files use a (mostly) typical C++ file structure for a program like this. Feel free to change, add, divide, combine, or delete files; add or combine classes; create more methods; modify the existing methods; and more. Providing correct functionality is more important than having a specific program structure.

### Coding and compiling

When I grade your program, I will compile and run your code on a Linux virtual machine using the GNU C++ compiler (`g++`). You can use all C++ features that are supported by the C++17 standard plus amendments. 
* You can develop on Visual Studio or other IDEs as long as you use only standard C/C++ features in your code. Specifically, please do not use `#include <windows.h>` since your code will be compiled and run on a Linux system.
* If you develop on **washington.uww.edu**, suggested compiler commands are `gcc-5` (C) or `g++-5` (C++). You might need to edit the makefile to use this compiler command.
* You may need to compile your code with the `-lpthread` option (note: second character is a lowercase letter L, not a number 1). The included makefile uses this option.

**Your C++ code files must use the extension `.cpp`**, except header files may use the extension `.h`. The makefile that GitHub Classroom uses to run your code searches only for files with .cpp extensions.

### Testing

Setup files for the two provided test cases are included in the repository's top-level directory as `424-p3-test1.txt` and `424-p3-test2.txt`. 

If you are developing on a system that supports the `make` command, you can run the provided test cases for Program 3 pages with the following commands.

* `make test1manual` runs Test Case 1 in manual mode.
* `make test1auto` runs Test Case 1 in auto mode.
* `make test2manual` runs Test Case 2 in manual mode.
* `make test2auto` runs Test Case 2 in auto mode.

The `make` command should be supported by
* all Unix/Linux systems
* GitHub Codespaces, since each codespace runs on a Linux virtual machine
* Mac OS systems that have the GNU Compiler Collection installed (probably?)
* some Windows systems with the GNU C++ compiler installed
    - on Visual Studio Code, you may need to install the CMake extension

If you are using GitHub Codespaces or Visual Studio Code, I have included predefined run configurations for these test cases using manual or automatic mode. These also rely on "make". It's possible to debug on VSCode or Codespaces without the make command, but this will require some changes to configuration file, and I might not be able to support you with those changes.

### Problems?

If you encounter problems, please commit and push your code, then [email me](osterz@uww.edu) with a description of what's happening. I will look at your repository on GitHub to try to help you debug.


## Advice for C++ programmers

This advice is also posted on the Program 3 page on Canvas. Updates, if any, will be posted there.


### Command-line arguments

Here's a tutorial on [accepting command-line arguments in C++](https://www.site.uottawa.ca/~lucia/courses/2131-05/labs/Lab3/CommandLineArguments.html). The procedure is similar in C.


### Multithreading in C++

You have at least three options for adding concurrency to your program.

1. **Preferred:** The C++11 standard includes a `<thread>` header, and the C11 standard includes a `<thread.h>` header. These might be the easiest and most OS-independent ways to add threads to your C or C++ code. References:
    * [C++ thread support library](https://en.cppreference.com/w/cpp/thread) on cppreference.com
    * [C++ multithreading reference](http://www.cplusplus.com/reference/multithreading/) at cplusplus.com: choose the links to explore the other C++11 thread support libraries
    * [C thread support library](https://en.cppreference.com/w/c/thread) on cppreference.com
    * [Thread Creation - ModernesCPP.com](https://www.modernescpp.com/index.php/thread-creation) (also see other pages available at the "What's Next" link for more example)
    * [Concurrency in C++11](https://www.classes.cs.uchicago.edu/archive/2013/spring/12300-1/labs/lab6/) from University of Chicago
    * Note: The C++20 standard supports a `<semaphore>` header, but please do not use it!
        - Many C++ compilers (including the one I use) do not reliably support C++20 features yet.
        - If you write in C++, plan to implement your own semaphores or use a different synchronization tool (e.g., the `mutex` class from the `<mutex>` standard header).

2. You may use the Pthreads (POSIX Threads) library to make your program multithreaded.

    * Note that Pthreads will probably not work on a Windows system. It will work on Linux if the necessary packages are installed. It may also work on Mac OS.
    * If you use Pthreads, add the option `-lpthread` to your compile command. This will link your code with the Pthreads library. (You will get a cryptic error message if you don't include this option.)
    * Short introduction: [Pthread Primer](http://pages.cs.wisc.edu/~travitch/pthreads_primer.html) from Tristan Ravitch, written while he was a graduate student at UW-Madison.
    * Longer tutorial and/or reference: [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/) from Blaise Barney at Lawrence Livermore National Laboratory. This is widely considered one of the best Pthreads tutorials.

3. You may choose to make your program multiprocess (with the POSIX `fork` and `wait` system calls). Here's a quick overview of [multiprocess programming with `fork` in C or C++](https://ece.uwaterloo.ca/~dwharder/icsrts/Tutorials/fork_exec/) from Douglas Harder at the University of Waterloo.
    * **Important note:** This might only work on a Unix/Linux system. It will not work on Windows (except under the Windows Subsystem for Linux, maybe), and it may not work on Mac OS either.
