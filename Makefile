PROJECT = karlab_amxx_i386.so

INCLUDE = -I. \
	-IHLSDK -IMETAMOD

OBJECTS = amxxmodule.cpp \
	Source.cpp

FLAGS =  -fPIC -fno-rtti -O3 -DNDEBUG -D_vsnprintf=vsnprintf -std=gnu++2a -fexceptions
LINK = -m32 -static-libstdc++ -static-libgcc -shared

default:
	gcc $(OBJECTS) $(FLAGS) $(INCLUDE) $(LINK) -o $(PROJECT)
