CPP = mpic++
CPPFLAGS = -std=c++11 -lm -Wall -L/usr/lib64
LFLAGS = -Wall -pg -lreadline

OBJS = main.o para.o random.o grid.o photon.o image.o util.o fileio_mg.o console.o problem.o mcscatter.o

all: CPPFLAGS += -O2
all: scatter

debug: CPPFLAGS += -g -O0
debug: scatter

prof: CPPFLAGS += -pg -O2
prof: scatter

scatter : $(OBJS)
	$(CPP) $(LFLAGS) $(OBJS) -o scatter

%.o : %.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) -c $<
	
clean:
	rm -f *.o scatter
