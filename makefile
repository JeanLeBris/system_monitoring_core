export OS
export CC=g++
export CFLAGS= -Wall -I include
# export LDFLAGS= -Wall -l argparse
export LDFLAGS= -Wall
export EXEC=test.exe
SRCNAMES= main.cpp utils.cpp output_converter.cpp monitoring.cpp
export SRCDIR=src
export OBJDIR=obj
export BINDIR=bin
export DEPENDENCIES=

ifeq ($(OS), Windows)
	RMDIR= rmdir
	RMFILE= del /s /q
	COPYFILE= copy
	SHARED_LIBRARY_EXT= dll
	SRCNAMES+= windows/monitoring.cpp windows/server.cpp
	LDFLAGS+= -l ws2_32
	FILE_SLASH=\\

else
ifeq ($(OS), Windows_NT)
	OS=Windows
	RMDIR= rmdir
	RMFILE= del /s /q
	COPYFILE= copy
	SHARED_LIBRARY_EXT= dll
	SRCNAMES+= windows/monitoring.cpp windows/server.cpp
	LDFLAGS+= -l ws2_32
	FILE_SLASH=\\

else
ifeq ($(OS), Linux)
	RMDIR= rmdir
	RMFILE= rm
	COPYFILE= cp
	SHARED_LIBRARY_EXT= so
	SRCNAMES+= linux/monitoring.cpp linux/server.cpp
	FILE_SLASH=/
endif
endif
endif

SRC= $(foreach srcname, $(SRCNAMES), $(SRCDIR)/$(srcname))
OBJNAMES= $(SRCNAMES:.cpp=.o)
OBJ= $(foreach objname, $(OBJNAMES), $(OBJDIR)/$(objname))

export RMDIR
export RMFILE
export COPYFILE
export SHARED_LIBRARY_EXT
export FILE_SLASH

ifeq ($(LIBRARY_TYPE), shared)
else
	ifeq ($(LIBRARY_TYPE), static)
	else
		LIBRARY_TYPE=shared
	endif
endif
export LIBRARY_TYPE

# compile:bin obj $(OBJNAMES)
# compile:obj update $(OBJNAMES)
compile:obj $(OBJNAMES)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# update:
# 	git submodule update --recursive --remote
# # 	$(CC) -c ./lib/deallocator/src/deallocator.cpp -o ./obj/deallocator.o
# # # 	ar rcs ./bin/libdeallocator.a ./obj/deallocator.o
# # 	$(CC) -fpic -shared ./obj/deallocator.o -o ./bin/libdeallocator.dll
# 	@(cd ./lib/argparse && $(MAKE) compile)
# 	$(COPYFILE) .$(FILE_SLASH)lib$(FILE_SLASH)argparse$(FILE_SLASH)bin$(FILE_SLASH)* .

%.o:
	$(CC) -c $(SRCDIR)/$(@:.o=.cpp) -o $(OBJDIR)/$@ $(CFLAGS)

.PHONY: compile clean

bin:
	mkdir bin

obj:
	mkdir obj
	mkdir obj$(FILE_SLASH)windows
	mkdir obj$(FILE_SLASH)linux

clean:
	- $(RMFILE) $(OBJDIR)$(FILE_SLASH)windows$(FILE_SLASH)*
	- $(RMFILE) $(OBJDIR)$(FILE_SLASH)linux$(FILE_SLASH)*
	- $(RMFILE) $(OBJDIR)$(FILE_SLASH)*
	- $(RMDIR) $(OBJDIR)$(FILE_SLASH)windows
	- $(RMDIR) $(OBJDIR)$(FILE_SLASH)linux
	- $(RMDIR) $(OBJDIR)
# 	- $(RMFILE) $(BINDIR)$(FILE_SLASH)*
# 	- $(RMDIR) $(BINDIR)
	- $(RMFILE) *.exe
	- $(RMFILE) *.dll
	- $(RMFILE) *.a
	- $(RMFILE) *.so







# compile:
# 	g++ .\\src\\*.cpp .\\src\\windows\\*.cpp -I include -o test.exe

# compile:
# 	g++ ./src/*.cpp ./src/linux/*.cpp -I include -o test.exe