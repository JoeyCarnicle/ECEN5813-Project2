objects = ring.o

#flag: 
# -g = debugging
#-wall = enable all warnings
CFLAGS = -g -Wall 

exec : $(objects)
	gcc $(CFLAGS) -o exec $(objects)
	
ring.o : ring.c ring.h
	gcc $(CFLAGS) -c ring.c 
# Will need to add dependencies for each .o file. Syntax:
# file.o: file.c include_file_1.h include_file_2.h, etc...
# 	gcc $(flags) -c file.c
#
	
clean : 
	rm exec *.o