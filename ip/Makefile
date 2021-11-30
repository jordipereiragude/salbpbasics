# add defines to build four versions

SYSTEM  = x86-64_linux
LIBFORMAT  = static_pic
#donde tengas instalado cplex
CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio1210/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio1210/concert

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD
COPT  = -m64 -fPIC -fno-strict-aliasing

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread -ldl
CLNFLAGS  = -l$(CPLEXLIB) -lm -lpthread -ldl

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CPLEXCFLAGS  = $(COPT)  -I$(CPLEXINCDIR)
CPLEXCCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)

SRCS = main.cpp utils.cpp milp.cpp 
#CFLAGS =
#LDFLAGS =
#EXEC = ../ip

CCC = c++

OBJS = ../obj/main.o ../obj/utils.o ../obj/milp.o

TARGETS = ../ipsolver

all: $(TARGETS) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGETS)

../obj/main.o : main.cpp
	$(CCC) -c $(CCFLAGS) $(CPLEXCCFLAGS) main.cpp -o ../obj/main.o

../obj/milp.o : milp.cpp
	$(CCC) -c $(CCFLAGS) $(CPLEXCCFLAGS) milp.cpp -o ../obj/milp.o

../obj/utils.o : utils.cpp
	$(CCC) -c $(CCFLAGS) $(CPLEXCCFLAGS) utils.cpp -o ../obj/utils.o

../ipsolver: $(OBJS)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJS) -o ../ipsolver $(CCLNFLAGS)
