/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains functions used in the main executing function of the database program
Last Modified:	22 August 2013

******/

#include "import.h"
#include "validate.h"
#include "menus.h"
#include "header.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

int main(){

	// contains the global values that will be used across the entire program

	// the struct array that will contain the information of the uploaded students
	student database[1000];
	// the user value that will determine whether the user exits the main menu 
	char userVal;
	// the number of enrolments that will be determined on the amount of records uploaded
	int enrolments = 0;
	// signals representing different sections of a valid input file
	char signals[3][13] = {"###START###", "###END###", "###RECORD###"};
	int validLength = 4;
	// valid lengths allowed in the main menu
	char validVals[4];
	validVals[0] = 'q'; 
	validVals[1] = 'i';
	validVals[2] = 'e';
	validVals[3] = 's';
	
	// initial displays the main menu upon the program starting for the first time
	displayMainMenu();
	
	// the entire function is contained in a main outer while loop, terminated by the input of the
	// character value 'q'
	while(userVal != 'q'){
		cin.clear();
		// input buffer is cleared if the input is incorrect
		userVal = getUserInput(validLength, validVals);
		// returns the user input which is then passed on to another function to handle the processes based on
		// the choice the user has made
		actionSelect(database, userVal, enrolments, signals);
	}
	
	return 0;
}