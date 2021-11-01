# Rendor
<img src="RendorLogo.png" width="240" height="240">

GitHub for the Rendor programming language

Rendor is the successor to the EnderKnight programming language. It aims to fix certain bugs and add:
* Actual functions with arguments and return statements
* Updating variables -> Done
* Scopes
* Converting variables from string to their actual values -> Done
* C++ inspired syntax -> WIP
* Better if statements
* Operators such as or, and, and modulo(%)
* Bytecode -> WIP
* Better lexer
* Better parser
* C++17 features(in the interpreter and compiler)
* C++17 transpiling
* Incrementing and decrementing -> Done

Officially this is called the Rendor Engine, named after the term render engine(since autocorrect always corrects Rendor to render)
# Compiling requirements for building from source:
* You have to compile using a C++17 compatible compiler. On Linux, Mac, and Windows(using gcc and make) you can use the make file 
* Boost 1.77 is a required dependency(You may need to edit the makefile's BOOSTFILE, BOOSTSYS, and LIBRARIES variables)
