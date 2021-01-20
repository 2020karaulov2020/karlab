PROJECT = karaulovlab_amxx_i386.so

INCLUDE = -I. \
	-IHLSDK -IMETAMOD

OBJECTS = amxxmodule.cpp \
	Source.cpp

FLAGS = -m32 -O3 -msse3 -funroll-loops -fomit-frame-pointer -fno-stack-protector -g -DNDEBUG -Dlinux -D__linux__ -D__USE_GNU -D_vsnprintf=vsnprintf -std=c++11 -shared
LINK = -m32 -static-libgcc -static-libstdc++ -pthread -L/usr/lib32 -L/opt/diet 

default:
	gcc-7 $(OBJECTS) $(FLAGS) $(INCLUDE) $(LINK) -o $(PROJECT)
