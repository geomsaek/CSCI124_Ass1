main:	main.o functions.o menus.o validate.o import.o
		g++ -Wall -o tester main main.o functions.o menus.o validate.o import.o

main.o:	main.cpp header.h menus.o validate.o import.o
		g++ -c main.cpp

tester:	main.o functions.o menus.o validate.o import.o
		g++ -Wall -o tester main.o functions.o menus.o validate.o import.o

functions.o: functions.cpp header.h
		g++ -c functions.cpp
		
menus.o:	menus.cpp menus.h
		g++ -c menus.cpp

validate.o: validate.cpp validate.h
		g++ -c validate.cpp
		
import.o:	import.cpp import.h
		g++ -c import.cpp

cleanLab:
		rm main.o main

cleanTest:
		rm tester functions.o menus.o validate.o import.o
 
				
