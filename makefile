#CPP = mpic++
CPP = mpic++.mpich
CPPFLAGS = -std=c++11 -lm -Wall -I/usr/include/lua5.2
LFLAGS = -Wall -lreadline -llua5.2

BINDIR = ./bin
SRCDIR = ./src
BUILDDIR = ./build

OBJS_NAMES = main.o para.o random.o grid.o photon.o image.o util.o fileio.o fileio_mg.o console.o mcscatter.o commands.o log.o lua.o
OBJS = $(addprefix $(BUILDDIR)/, $(OBJS_NAMES))

all: CPPFLAGS += -O2
all: scatter

debug: CPPFLAGS += -g -O0
debug: scatter

prof: CPPFLAGS += -pg -O2
prof: LFLAGS += -pg
prof: scatter

scatter : $(OBJS)
	@mkdir -p $(BINDIR)
	$(CPP) $(OBJS) $(LFLAGS) -o $(BINDIR)/scatter

./$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c $< -o $@
	
clean:
	@echo "Cleaning..."
	rm -f $(BUILDDIR)/*.o $(BINDIR)/scatter
