clang++ -std=c++11 app.mm -c -o app.o
clang++ -std=c++11 -c data_types.cpp -o data_types.o
clang++ -std=c++11 -c framebuffer.cpp -o framebuffer.o
clang++ -std=c++11 -c main.cpp -o main.o
clang++ -std=c++11 -c window.cpp -o window.o
clang++ -std=c++11 -framework Cocoa app.o data_types.o main.o window.o framebuffer.o -o super-basoon
