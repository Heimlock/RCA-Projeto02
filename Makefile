
#
#		Redes de Computadores A
# 		Projeto 02 - WhatsAp2p, 
#		Sistema de Mensageiro peer-to-peer hibrido
#
#	Integrantes:
#		Felipe Moreira Ferreira       RA: 16116469
#
#	  Makefile Geral
#

FLAGS	= -pthread -g -std=c99 -Wall -D_SVID_SOURCE -D_GNU_SOURCE
CMP 	= gcc
OBJECTS = *.o

ClIENT 		= ./client/client
LIB_CLIENT	= ./commonLibs/*.h ./client/*.h
SRC_CLIENT	= ./commonLibs/*.c ./client/*.c

SERVER 		= ./server/server
LIB_SERVER	= ./commonLibs/*.h ./server/*.h
SRC_SERVER	= ./commonLibs/*.c ./server/*.c

all: clean BUILD_SERVER BUILD_CLIENT clean

BUILD_SERVER:
	$(CMP) -c $(FLAGS) $(LIB_SERVER)
	$(CMP) -c $(FLAGS) $(SRC_SERVER)
	$(CMP) -o $(SERVER) $(OBJECTS) $(FLAGS)
	@echo

BUILD_CLIENT:
	$(CMP) -c $(FLAGS) $(LIB_CLIENT)
	$(CMP) -c $(FLAGS) $(SRC_CLIENT)
	$(CMP) -o $(ClIENT) $(OBJECTS) $(FLAGS)
	@echo

clean:
	@rm -Rf ./server/*.o ./server/*.gch
	@rm -Rf ./client/*.o ./client/*.gch
	@rm -Rf *.o *.gch ./commonLibs/*.o ./commonLibs/*.gch