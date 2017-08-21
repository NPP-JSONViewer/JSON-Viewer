ARCH = i686
PROJECT  = NPPJSONViewer
CPP      = $(ARCH)-w64-mingw32-g++
CC       = $(ARCH)-w64-mingw32-gcc
WINDRES  = $(ARCH)-w64-mingw32-windres

CPP_SOURCES = JSONDialog.cpp NppPlugin.cpp Hyperlinks.cpp StaticDialog.cpp PluginDefinition.cpp 
C_SOURCES = json.c
DLLRES = resource.res

OBJ = JSONDialog.o NppPlugin.o Hyperlinks.o StaticDialog.o PluginDefinition.o json.o $(DLLRES)

LIBS     = -L"/usr/$(ARCH)-w64-mingw32/lib" -static-libgcc -lshlwapi -luser32 -lgdi32 -ladvapi32 -lshell32 

CXXINCS  = -I"/usr/$(ARCH)-w64-mingw32/include"

BIN      = $(PROJECT).dll
CXXFLAGS = $(CXXINCS) -O2 -mtune=$(ARCH) -Wall -g3 -DBUILDING_DLL=1 -DUNICODE  -W -gstabs -mwindows

RM       = rm -f
DEF      = lib$(PROJECT).def
STATIC   = lib$(PROJECT).a

all: $(BIN)

clean:
	rm $(OBJ) $(BIN) $(DEF) $(STATIC)

$(BIN): $(OBJ)
	$(CPP) -shared $(OBJ) -o $(BIN) $(LIBS) -municode -mthreads -Wl,-Bstatic,--output-def,$(DEF),--out-implib,$(STATIC),--add-stdcall-alias

NppPlugin.o: NppPlugin.cpp
	$(CPP) -c NppPlugin.cpp -o NppPlugin.o $(CXXFLAGS)

PluginDefinition.o: PluginDefinition.cpp
	$(CPP) -c PluginDefinition.cpp -o PluginDefinition.o $(CXXFLAGS)

StaticDialog.o: StaticDialog.cpp
	$(CPP) -c StaticDialog.cpp -o StaticDialog.o $(CXXFLAGS)

Hyperlinks.o: Hyperlinks.cpp
	$(CPP) -c Hyperlinks.cpp -o Hyperlinks.o $(CXXFLAGS)

JSONDialog.o: JSONDialog.cpp
	$(CPP) -c JSONDialog.cpp -o JSONDialog.o $(CXXFLAGS)


json.o: json.c
	$(CC) -c json.c -o json.o $(CXXFLAGS)

  
resource.res: resource.rc
	$(WINDRES) -i resource.rc --input-format=rc -o resource.res -O coff
