set CXX=g++
%CXX% -c -o TaskbarProgress.o TaskbarProgress.cpp
%CXX% -c -o main.o main.cpp
%CXX% -mwindows -o TaskbarProgress TaskbarProgress.o main.o -lole32 -lcomctl32
