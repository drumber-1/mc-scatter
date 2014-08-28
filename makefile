CPP = mpic++
CPPFLAGS = -std=c++11 -lm -Wall -L/usr/lib64
LFLAGS = -Wall -lreadline

BINDIR = ./bin
SRCDIR = ./src
BUILDDIR = ./build

OBJS_NAMES = main.o para.o random.o grid.o photon.o image.o util.o fileio.o fileio_mg.o console.o problem.o mcscatter.o commands.o log.o
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
	$(CPP) $(LFLAGS) $(OBJS) -o $(BINDIR)/scatter

./$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c $< -o $@
	
clean:
	@echo "Cleaning..."
	rm -f $(BUILDDIR)/*.o $(BINDIR)/scatter
