CPP = mpic++
CPPFLAGS = -lm -Wall -L/usr/lib64
LFLAGS = -Wall

OBJS = main.o para.o random.o grid.o photon.o prob.o image.o util.o

all: CPPFLAGS += -O2
all: scatter

debug: CPPFLAGS += -g -O0
debug: scatter

scatter : $(OBJS)
	$(CPP) $(LFLAGS) $(OBJS) -o scatter

%.o : %.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) -c $<
	
clean:
	rm -f *.o scatter
