CC=gcc
CFLAGS=-W -Wall -g
#PLACE YOUR ROOT PATH HERE PLEASE BEFORE MAKE FILE
ROOT_PROJ=C:\Users\wallouf\Desktop\ia-all-chainage-ok\IA
INC_DIR=$(ROOT_PROJ)/inc
SRC_DIR=$(ROOT_PROJ)/src
BIN_DIR=$(ROOT_PROJ)/bin
OBJ_DIR=$(ROOT_PROJ)/obj

EXT_INCLUDE_DIR=-I$(INC_DIR)
FILE_INCLUDE=$(INC_DIR)/calcul_impots.h $(INC_DIR)/moteur_impots.h $(INC_DIR)/outils_struc_impots.h

#############################################################################################
#	Target File
#############################################################################################

OBJECT1= $(OBJ_DIR)/calcul_impots.o
SOURCE1= $(SRC_DIR)/calcul_impots.c

TARGET1= $(BIN_DIR)/main_impots
OBJECT2= $(OBJ_DIR)/main_impots.o
SOURCE2= $(SRC_DIR)/main_impots.c

OBJECT3= $(OBJ_DIR)/moteur_impots.o
SOURCE3= $(SRC_DIR)/moteur_impots.c

OBJECT4= $(OBJ_DIR)/outils_struc_impots.o
SOURCE4= $(SRC_DIR)/outils_struc_impots.c


##############################################################################################
#	Default Command
##############################################################################################

all : $(TARGET1)

$(TARGET1) : $(OBJECT1) $(OBJECT2) $(OBJECT3) $(OBJECT4)
	$(CC) -o $(TARGET1) $(OBJECT4) $(OBJECT3) $(OBJECT2) $(OBJECT1) $(CFLAGS)

$(OBJECT1) : $(SOURCE1) $(FILE_INCLUDE)
	$(CC) -o $(OBJECT1) -c $(SOURCE1) $(CFLAGS) 

$(OBJECT2) : $(SOURCE2) $(FILE_INCLUDE)
	$(CC) -o $(OBJECT2) -c $(SOURCE2) $(CFLAGS)  

$(OBJECT3) : $(SOURCE3) $(FILE_INCLUDE)
	$(CC) -o $(OBJECT3) -c $(SOURCE3) $(CFLAGS)

$(OBJECT4) : $(SOURCE4) $(FILE_INCLUDE)
	$(CC) -o $(OBJECT4) -c $(SOURCE4) $(CFLAGS)

##############################################################################################
#	Clean command
##############################################################################################

clean :
	rm -f $(TARGET1) $(OBJECT4) $(OBJECT3) $(OBJECT2) $(OBJECT1)