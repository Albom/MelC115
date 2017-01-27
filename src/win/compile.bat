del c115.exe
del c115.res
windres c115.rc -O coff -o c115.res
del c115.o
gcc -o c115.o -c c115.c
gcc -o c115.exe c115.o c115.res -s -lcomdlg32 -lwinmm -lgdi32 -lcomctl32 -Wl,--subsystem,windows
del c115.o
del c115.res