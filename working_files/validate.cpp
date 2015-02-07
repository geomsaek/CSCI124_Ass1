/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains function definitions used in the validation processes
Last Modified:	24 August 2013

******/

#include "import.h"
#include "validate.h"
#include "menus.h"
#include "header.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include<cstdlib>

using namespace std;

// function checks the input values where the user needs to answer a yes or no question and returns
// boolean value if the value matches or not
bool checkInput(char userInput){
	
	bool valid = false;
	int counter = 0;
	
	char validVals[4];
	validVals[0] = 'y';
	validVals[1] = 'Y';
	validVals[2] = 'N';
	validVals[3] = 'n';
	
	// checks values against an array of valid vals
	for(int i= 0; i < 4; i++){
		if(validVals[i] == userInput){
			counter = counter + 1;
		}
	}
	
	if(counter == 1){
		cin.ignore(1000, '\n');
		cin.clear();
		return true;
	}
	else {
		return false;
	}	
}

//function gets the user input and checks against a set of values and returns the valid
// character. validation occurs within this function
char getUserInput(int checkLength, char valArray[]) {
	char userVal;
	int error = 0;
	bool userInput = false;
	int confirm = 0;
	
	// remain in the while loop, whilst there are errors and prompt the user until the string is clean
	while(userInput == false){
		cin >> userVal;
		for(int i = 0; i < checkLength; i++){
			if(userVal == valArray[i]){
				confirm = confirm + 1;
			}
			if(i == (checkLength - 1)){
				if(confirm > 0){
					userInput = true;
					cin.ignore(1000,'\n');
					cin.clear();
				}
				else {
					cout << "Please enter a correct selection: ";
					cin.ignore(1000,'\n');
					cin.clear();
				}
			}
		}
	}
	// return the correct character
	return userVal;
}

// function takes a char string, breaks it apart and tests whether it is valid or not for a subject search
bool validateSubjectID(char searchString[], int & year, char & session, char code[]){
	
	int length = strlen(searchString);
	char temp[10] = { '\0' };
	int errorCount = 0;
	int codeCount = 0;
	bool complete = false;
	bool error = false;
	
	// if the user enters a wild card, set the year as an indicator to show this and return to the calling function
	if(length == 1 && searchString[0] == '*'){
		year = -1;
		return true;
	}else {
		if(length > 14 || length < 14){
			cout << "You have entered an invalid search string" << endl;
			year = -1;
			return false;
		}
		else {
			// copy the first 4 characters to a temporary array. these will make up the year
			strncpy(temp, searchString, 4); 
			for(int i = 0; i < length; i++){
				if(i < 4){
					if(!(searchString[i] >= '0' && searchString[i] <= '9')){
						error = true;
					}
					else {
						year = year + 1;
					}
				}
				if(i == 5){
					if(year == 4){
						// if there is year is valid then convert it to a number
						year = atoi(temp);
					}
					// store the session value if it is within the given set
					if((searchString[i] >= 'a' && searchString[i] <= 'z') || (searchString[i] >= 'A' && searchString[i] <= 'Z')){
						session = searchString[i];
					}
					else {
						error = true;
					}
				}
				// store a collection of values that will make up the subject code
				// initial values will make up the letters
				if(i > 6 && i < 11){
					if((searchString[i] >= 'a' && searchString[i] <= 'z') || (searchString[i] >= 'A' && searchString[i] <= 'Z')){
						code[codeCount] = searchString[i];
						codeCount = codeCount + 1;
					}
				}
				// these values will make up the number code of the subject code
				if(i > 10 && i < length){
					if(!(searchString[i] >= '0' && searchString[i] <= '9')){
						error = true;
					}else {
						code[codeCount] = searchString[i];
						codeCount = codeCount + 1;
					}
				}
			}
		}
		// set the end of the subject code to null so that it is not a longer string
		code[7] = '\0';
		// set the session value to the end of the array to store later
		code[8] = session;
		
		// return the appropriate value to the calling function based on whether the input was correct
		// or not.
		if(error == true){
			cout << "You didn't enter a correct search string" << endl;
			return false;
		}
		else {
			return true;
		}
	}
}

// function checks, whether the correct session input has been inserted
// takes a user value, an array to match the values against and the length of the match value array
// returns a boolean based on whether the string is valid or not
bool sessionCheck(char userInput, char validVals[], int checkLength){
	
	bool valid = false;
	int counter = 0;
	
	for(int i= 0; i < checkLength; i++){
		if(validVals[i] == userInput ){
			counter = counter + 1;
		}
	}
	
	if(counter == 1){
		cin.ignore(1000, '\n');
		cin.clear();
		return true;
	}
	else {
		return false;
	}	
}

// validates a passed string that is used in the editing the values of a student record. can be passed booleans and limit values
// to indicate lengths that need to be met, to include extra punctuation and special characters in a valid string and also numbers
// returns a boolean regarding whether or not the passed value is valid or not and ready to store
bool validateString(char string[], int length, bool numInclude, char alphabetLower[], char alphabetUpper[], char currentName[], int numGreater, bool extraPunc, int spaceMax){
	
	int spaceCount = 0;
	int alphaCount = 0;
	bool valid = false;
	int subjectNum = 0;
	char numbers[] = { '0','1','2','3','4','5','6','7','8','9' };
	char punct[] = { ':', '-' };
	int punCount = 0;
	
	// if the string is the same as the current name, output an error
	if(strcmp(currentName, string) == 0){
		cout << endl;
		cout << "Please enter a different string than is already there" << endl;
		cout << endl;
		valid = false;
	}
	// if the string is less than 3 then output an error as all string values require to be longer than this
	else if(strlen(currentName) < 3){
		valid = false;
		
	//otherwise enter a loop to check the values of the string
	}else {
		for(int i = 0; i < length; i++){
		
			// if there is a space count it provided that there is no inclusion of numbers
			if(string[i] == ' ' && !(numInclude)){
				spaceCount = spaceCount + 1;
			}
			
			// enter a loop to match the alphabetic values and count these. will match all valid values
			// against the current value to determine if it is valid or not
			for(int k = 0; k < 26; k++){
				if(string[i] == alphabetLower[k] || string[i] == alphabetUpper[k]){
					alphaCount = alphaCount + 1;
				}
				
				// if the inclusion of numbers has been specified, add this to the count and count the numbers in a
				// separate counter
				else if(k < 10 && numInclude){
					if(string[i] == numbers[k]){
						alphaCount = alphaCount + 1;
						subjectNum = subjectNum + 1;
					}
				
				// if the there is a true boolean for the inclusion of extra punctuation characters, add these to the count
				// and also set up a punctuation count
				}else if(k < 2 && extraPunc){
					if(string[i] == punct[k]){
						alphaCount = alphaCount + 1;
						punCount = punCount + 1;
					}
				}
			}
			
			// if the count is the last, that is the final character of the new string
			if(i == length - 1){
			
				// if there is the inclusion of numbers then the space count has to be 0 and the
				// amount of numbers has to be equal to the maximum specified value. main used for subject codes
				//otherwise return false
				if(numInclude){
					if(alphaCount == length && spaceCount == 0 && subjectNum == numGreater){
						valid = true;
					}else {
						valid = false;
					}
				}
				
				// the boolean specifies the inclusion of extra punctuation characters
				else if(extraPunc){
				
					// the alphabetic count including all the values has to be equal to the length
					// that is the length of the string has be equal to the value of all counted valid values
					// and the number of spaces has to less than or equal to the max amount and the punctuation max has
					// to be less than 3
					if((alphaCount + spaceCount == length) && (spaceCount <= spaceMax) && (punCount < 3)){
								valid = true;
					}else {
						valid = false;
					}
				}else {
				
					// otherwise it is a normal character string, i.e a name, so there has to be a match of
					// all char values with the exception of one space to eventuate true
					if(alphaCount + spaceCount == length && spaceCount == 1){
						valid = true;
					}
					else {
						valid = false;
					}
				}
			}
		}
	}
	
	// return a boolean indicating whether the string is valid or not
	return valid;
}

//function handles the changing of a string value, but calls on the validateString function to perform the actual validation checking.
// this function will prompt the user until a valid string is entered based on the specifications that are passed to it
// takes the string to be changed, whether it needs to include numbers, a max length of the string, the amount of numbers allowed, any
// extra punctuation symbols and the number of spaces allowed
void nameChange(char stringBeingChanged[], bool numInclude, char alphabetUpper[], char alphabetLower[], int maxLength, int numIncludeLength, bool extraPunc, int spaceMax){
	
	bool valid = false;
	int length = 0;
	char decide[500];
	char reset[500] = { '\0' };
	
	// while loop iterates until valid string has been entered indicated by the validateString function
	while(valid == false){
	
		// gets the value from the user
		cin.getline(decide, maxLength, '\n');
		length = strlen(decide);
		
		// passes the string and the specification values to the validate function 
		valid = validateString(decide, length, numInclude, alphabetLower, alphabetUpper, stringBeingChanged, numIncludeLength, extraPunc, spaceMax);
		
		// if the value is invalid, then output an error, reset the string to null and clear any errors
		if(valid == false){
			cout << "Please enter a valid string" << endl;
			cout << endl;
			strcpy(decide, reset);
			cin.clear();
		}
	}
	
	// otherwise copy the string that was entered to the one that was passed to it
	// which is also the one that will return back to the calling function
	strcpy(stringBeingChanged, decide);
	
	return; 
}

// function checkes whether a number is within a certain range. takes the variable to store the
// valid number in and the number range values. function returns the valid number
int numberChange(int currentNumber, int minValue, int MaxValue){
	
	int newNum = 0;
	bool invalidNum = false;
	
	
	while(invalidNum == false){
		cin >> newNum;
		if(newNum >= minValue && newNum <= MaxValue && currentNumber != newNum){
			invalidNum = true;
		}
		else{
			invalidNum = false;
			cin.clear();
			cin.ignore(1000,'\n');
			cout << "Please enter a valid number." << endl;
			cout << "Please make sure the value is not the same as the current one, which is: " << currentNumber << ": ";
			cout << endl;
		}
	}
	
	return newNum;
}
