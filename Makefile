MYLIBS=/usr/local

OBJ     =       .o
CFLAGS  =       -O3
IFLAGS  =       -I$(MYLIBS)/include
LFLAGS  =       -L$(MYLIBS)/lib -lgsl -lgslcblas
CC      =       c++
PROGNAME=       benchmark.exe

MAIN = benchmark$(OBJ)
CPP1 = S2_Gillespie$(OBJ)
CPP2 = Psampler$(OBJ)
CPP3 = parse_nominals$(OBJ)
CPP4 = extrinsic_noise$(OBJ)

program: $(MAIN) $(CPP1) $(CPP2) $(CPP3) $(CPP4)
	$(CC) $(CFLAGS) $(IFLAGS) $(MAIN) $(CPP1) $(CPP2) $(CPP3) $(CPP4)  -o $(PROGNAME) $(LFLAGS)

clean:
	rm -f $(MAIN) $(CPP1) $(CPP2) $(CPP3) $(CPP4) $(PROGNAME)


