CGREEN=/Users/olenive/cgreen-Cpp
MYLIBS=/usr/local

# gcc -I/Users/olenive/cgreen-C/include/  -c first_test.c
# gcc first_test.o -L/Users/olenive/cgreen-C/lib -lcgreen -o first_test

OBJ     =       .o
CFLAGS  =       -O3
IFLAGS  =       -I$(MYLIBS)/include -I$(CGREEN)/include
LFLAGS  =       -L$(MYLIBS)/lib -lgsl -lgslcblas -L$(CGREEN)/lib -lcgreen++
CC      =       c++
PROGNAME=       cgreen_tests.exe

DIR_SUT =       ../../

MAIN = all_tests
CPP0 = common_test_functions
CPP1 = parse_nominals_tests
CPP2 = parse_nominals
CPP3 = extrinsic_noise_tests
CPP4 = extrinsic_noise
CPP5 = Psampler_tests
CPP6 = Psampler
CPP7 = S2_Gillespie_tests
CPP8 = S2_Gillespie

program: $(MAIN).cpp $(CPP0).cpp $(CPP1).cpp $(DIR_SUT)/$(CPP2).cpp  $(CPP3).cpp $(DIR_SUT)/$(CPP4).cpp $(CPP5).cpp $(DIR_SUT)/$(CPP6).cpp $(CPP7).cpp $(DIR_SUT)/$(CPP8).cpp
	$(CC) $(IFLAGS) -c $(MAIN).cpp $(CPP0).cpp $(CPP1).cpp $(DIR_SUT)/$(CPP2).cpp $(CPP3).cpp $(DIR_SUT)/$(CPP4).cpp $(CPP5).cpp $(DIR_SUT)/$(CPP6).cpp $(CPP7).cpp $(DIR_SUT)/$(CPP8).cpp
	$(CC) $(MAIN).o $(CPP0).o $(CPP1).o $(CPP2).o $(CPP3).o $(CPP4).o $(CPP5).o $(CPP6).o $(CPP7).o $(CPP8).o  $(LFLAGS) -o $(PROGNAME)

clean:
	rm $(MAIN).o $(CPP0).o $(CPP1).o $(CPP2).o $(CPP3).o $(CPP4).o $(CPP5).o $(CPP6).o $(CPP7).o $(CPP8).o  $(PROGNAME)



