

CC=mpicc

TARGET=mpi_sim

REG_INC=-I${REG_STEER_HOME}/include
REG_LIB=-L${REG_STEER_HOME}/lib64 -Wl,-Bstatic -lReG_Steer -lReG_Steer_Utils -lReG_Steer_SOAP -Wl,-Bdynamic -lxml2 -lssl -lcrypto

CCFLAGS=-O2 -pipe -std=c99 -Werror

${TARGET}: mpi_simulation.c
	${CC} -o ${TARGET} mpi_simulation.c ${CCFLAGS} ${REG_INC} ${REG_LIB}

clean:
	rm -f ${TARGET}
	rm -f *~
