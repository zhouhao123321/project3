SRC = $(wildcard ./src/*.c)
OBJ =  $(patsubst %.c,%.o,$(SRC))
CC  = arm-linux-gcc
TAG = ./bin/main
CFLAGS = -L./lib -I./inc -ljpeg -lfont -lm -lpthread


$(TAG):$(OBJ)
	$(CC) $(^) -o $(@) $(CFLAGS) 
	@echo $(SRC)
	@echo $(OBJ)


    #arm-linux-gcc  add.o main.o  -o main
	
clean:
	$(RM) $(OBJ)  $(TAG