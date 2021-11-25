# metadata

Project for crossover

==============================================

Notes regarding submission:

- You will need C++20 support to build this project, I built it on clang-13

- Copy the compiled executable to your PATH and run the cmd: metadata

- I chose to implement a more functional-oriented solution since the C++17 standard and above has already implemented an object-oriented solution for file-traversal and file-management. Using a more complex object-oriented approach on this would over-complicate the solution and be more difficult to manage. Assuming others will contribute to this, this solution feels easy to iterate on, has fewer bugs and has a smaller learning curve

- We only go to a depth level of 1, sub-directories within sub-directories are ignored; somewhat similar to 'ls *'

==============================================
