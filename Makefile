PROJECT = karaulovlab_amxx_i386.so

INCLUDE = -I. \
	-IHLSDK -IMETAMOD

OBJECTS = amxxmodule.cpp \
	Source.cpp

FLAGS = -pthread -O3 -DNDEBUG -fPIC -Dlinux -D__linux__ -D__USE_GNU -D_vsnprintf=vsnprintf -std=gnu++14
LINK = -m32 -static-libgcc -static-libstdc++ -L/usr/lib32 -L/opt/diet 

default:
	gcc-7 $(OBJECTS) $(FLAGS) $(INCLUDE) $(LINK) -o $(PROJECT)
