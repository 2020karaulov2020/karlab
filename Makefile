PROJECT = karaulovlab_amxx_i386.so

INCLUDE = -I. \
	-IHLSDK -IMETAMOD

OBJECTS = amxxmodule.cpp \
	Source.cpp

FLAGS = -O3 -msse3 -ipo -no-prec-div -fp-model fast=2 -funroll-loops -fomit-frame-pointer -fno-stack-protector -g -DNDEBUG -Dlinux -D__linux__ -D__USE_GNU -D_vsnprintf=vsnprintf -std=c++0x -shared -fasm-blocks
LINK = -static-intel -static-libgcc -no-intel-extensions -m32  -Bstatic -static-libstdc++ -pthread -L/usr/lib32 -L/opt/diet 

default:
	gcc $(OBJECTS) $(FLAGS) $(INCLUDE) $(LINK) -o $(PROJECT)
