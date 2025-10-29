SRC:=src/%.c #usage of wildcard
INC:=include
BUILD:=src/%.o
LIST_SRC:=$(wildcard src/*.c)
OBJECT:=$(LIST_SRC:%.c=%.o)
CLOSE_OBJECT:=close_server/close_main.o src/client.o src/socket.o 
CC:=gcc
TEMP_FLAG:=
CFLAG:=-c -Wall  -g $(TEMP_FLAG)
LFLAG:=-g $(TEMP_FLAG)
INC_FLAG:= -I$(INC)/


#build all target
.PHONY: all
all: main close 
	rm src/*.o
	rm close_server/*.o
	@echo "You can run the program"

#build relocatable object file
$(BUILD): $(SRC) 
	$(CC) $(CFLAG) $^ -o $@ $(INC_FLAG)

#build executable file
main: $(OBJECT)
	$(CC) $(LFLAG)  $^ -o $@ 

	@echo "$@ program has been compiled sucessfully"

close_server/close_main.o: close_server/close_main.c
	$(CC) $(CFLAG) $^ -o $@ $(INC_FLAG)

close: $(CLOSE_OBJECT)
	$(CC) $(LFLAG)  $^ -o $@ 
	

#debug command
.PHONY: debug
debug: main
	@gdb main

#execute the program
.PHONY: excecute
excecute: main
	@./main

#clean the program and unessesary file
.PHONY: clean
clean:
	- rm main 
	- rm close
	