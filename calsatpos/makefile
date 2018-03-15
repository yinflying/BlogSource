SRC    = ./src
CFLAGS = -Wall -O3 -DENACMP -DENAQZS -I$(SRC) -DENAGLO
CPUSFLAGS = -Wall -O3 -DENACMP -DENAQZS -DENAGLO -std=c++11 -I$(SRC)
LDLIBS = -lm -llapack -lblas
CC = gcc
CPUS = g++

BIN    = calsatpos

all        : $(BIN)
calsatpos     : calsatpos.o rtkcmn.o rinex.o preceph.o ephemeris.o qzslex.o rtcm.o rtcm2.o rtcm3.o rtcm3e.o sbas.o
	$(CPUS) rtkcmn.o calsatpos.o rinex.o preceph.o ephemeris.o qzslex.o rtcm.o rtcm2.o rtcm3.o rtcm3e.o sbas.o -o calsatpos  ${LDLIBS}

rtkcmn.o   : $(SRC)/rtklib.h $(SRC)/rtkcmn.c
	$(CC) -c $(CFLAGS) $(SRC)/rtkcmn.c
rinex.o    : $(SRC)/rtklib.h $(SRC)/rinex.c
	$(CC) -c $(CFLAGS) $(SRC)/rinex.c
rtkpos.o   : $(SRC)/rtklib.h $(SRC)/rtkpos.c
	$(CC) -c $(CFLAGS) $(SRC)/rtkpos.c
lambda.o   : $(SRC)/rtklib.h $(SRC)/lambda.c
	$(CC) -c $(CFLAGS) $(SRC)/lambda.c
geoid.o    : $(SRC)/rtklib.h $(SRC)/geoid.c
	$(CC) -c $(CFLAGS) $(SRC)/geoid.c
ephemeris.o: $(SRC)/rtklib.h $(SRC)/ephemeris.c
	$(CC) -c $(CFLAGS) $(SRC)/ephemeris.c
sbas.o     : $(SRC)/rtklib.h $(SRC)/sbas.c
	$(CC) -c $(CFLAGS) $(SRC)/sbas.c
preceph.o  : $(SRC)/rtklib.h $(SRC)/preceph.c
	$(CC) -c $(CFLAGS) $(SRC)/preceph.c
ppp.o      : $(SRC)/rtklib.h $(SRC)/ppp.c
	$(CC) -c $(CFLAGS) $(SRC)/ppp.c
ppp_ar.o   : $(SRC)/rtklib.h $(SRC)/ppp_ar.c
	$(CC) -c $(CFLAGS) $(SRC)/ppp_ar.c
pntpos.o   : $(SRC)/rtklib.h $(SRC)/pntpos.c
	$(CC) -c $(CFLAGS) $(SRC)/pntpos.c
ionex.o    : $(SRC)/rtklib.h $(SRC)/ionex.c
	$(CC) -c $(CFLAGS) $(SRC)/ionex.c
tle.o      : $(SRC)/rtklib.h $(SRC)/tle.c
	$(CC) -c $(CFLAGS) $(SRC)/tle.c
qzslex.o   : $(SRC)/rtklib.h $(SRC)/qzslex.c
	$(CC) -c $(CFLAGS) $(SRC)/qzslex.c
rtcm.o   : $(SRC)/rtklib.h $(SRC)/rtcm.c
	$(CC) -c $(CFLAGS) $(SRC)/rtcm.c
rtcm2.o   : $(SRC)/rtklib.h $(SRC)/rtcm2.c
	$(CC) -c $(CFLAGS) $(SRC)/rtcm2.c
rtcm3.o   : $(SRC)/rtklib.h $(SRC)/rtcm3.c
	$(CC) -c $(CFLAGS) $(SRC)/rtcm3.c
rtcm3e.o   : $(SRC)/rtklib.h $(SRC)/rtcm3e.c
	$(CC) -c $(CFLAGS) $(SRC)/rtcm3e.c

calsatpos.o   : calsatpos.cpp
	${CPUS} -c ${CPUSFLAGS} ./calsatpos.cpp

test: testnav testsp3
testsp3:
	./calsatpos -I ./igs15905.sp3 -b 20100702001500.54365 -e 20100702011500.0000 -t 1000 -s 5
testnav:
	./calsatpos -I ./30400920.05n -b 20050402020500.54365 -e 20050402051500.0000 -t 1000 -s 5

clean :
	rm -f *.o *.out *.exe $(BIN) *.stackdump gmon.out

