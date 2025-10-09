export OS
export CC=g++
export CFLAGS= -Wall -I include
export LDFLAGS= -Wall
export EXEC=test
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
	SRCNAMES+= windows/monitoring.cpp
	FILE_SLASH=\\

else
ifeq ($(OS), Windows_NT)
	OS=Windows
	RMDIR= rmdir
	RMFILE= del /s /q
	COPYFILE= copy
	SHARED_LIBRARY_EXT= dll
	SRCNAMES+= windows/monitoring.cpp
	FILE_SLASH=\\

else
ifeq ($(OS), Linux)
	RMDIR= rmdir
	RMFILE= rm
	COPYFILE= cp
	SHARED_LIBRARY_EXT= so
	SRCNAMES+= linux/monitoring.cpp
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
compile:obj $(OBJNAMES)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

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
	$(RMFILE) $(OBJDIR)$(FILE_SLASH)*
	$(RMDIR) $(OBJDIR)$(FILE_SLASH)windows
	$(RMDIR) $(OBJDIR)$(FILE_SLASH)linux
	$(RMDIR) $(OBJDIR)
	$(RMFILE) $(BINDIR)$(FILE_SLASH)*
	$(RMDIR) $(BINDIR)
	$(RMFILE) *.exe







# compile:
# 	g++ .\\src\\*.cpp .\\src\\windows\\*.cpp -I include -o test.exe

# compile:
# 	g++ ./src/*.cpp ./src/linux/*.cpp -I include -o test.exe