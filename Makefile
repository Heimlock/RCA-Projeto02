
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

ClIENT 		= client
LIB_CLIENT	= *.h	# Falta especificar
SRC_CLIENT	= *.c	# Falta especificar

SERVER 		= server
LIB_SERVER	= *.h	# Falta especificar
SRC_SERVER	= *.c	# Falta especificar

all: clean BUILD_SERVER BUILD_CLIENT

BUILD_CLIENT:
	$(CMP) -c $(FLAGS) $(LIB_CLIENT)
	$(CMP) -c $(FLAGS) $(SRC_CLIENT)
	$(CMP) -o $(ClIENT) $(OBJECTS) $(FLAGS)
	@echo
	@rm -rf *.o *.gch

BUILD_SERVER:
	$(CMP) -c $(FLAGS) $(LIB_SERVER)
	$(CMP) -c $(FLAGS) $(SRC_SERVER)
	$(CMP) -o $(SERVER) $(OBJECTS) $(FLAGS)
	@echo
	@rm -rf *.o *.gch

clean:
	@rm -rf *.o *.gch $(NOME)