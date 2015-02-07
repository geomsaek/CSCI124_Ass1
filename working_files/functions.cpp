/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains functions used in the main database program. Searching and calculating GPA
				importing and exporting data, handling file duplicates
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

// function checks for doubles in the initial upload of a database file
void initialDoubles(int & counter, struct student database[]){
	
	char userValue;
	bool secondExist = false;
	bool confirmTrue = false;
	int monitor = 1;
	
	// loop through the records
	
	for(int i =0; i < counter; i++){
		for(int k = i+1; k <= counter; k++){
		
			// if there is a duplicate
			if(database[i].id == database[k].id){
				
				// notify the user and validate their response
				
				userValue = doubleOutNotify(userValue, secondExist, database, i, k, database);
				switch(userValue){
					case 'y':
					case 'Y':
						cout << '\n' <<  "You have selected to swap the values. These the contents of File One, will be overwritten by File Two." << '\n' << endl;
						swapVals(database, i, k);
						for(int l = k; l < counter - k; l++){
							swapVals(database, k, k+1);
						}
						counter = counter - 1;
					break;
					
					case 'n':
					case 'N':
						cout << "You have chosen to leave the values as they are. You now have two records with the same ID details" << endl;
					break;
				}
				userValue = '\0';
				confirmTrue = false;
			}
		}
		// keep track of these values to use if the user wants to overwrite duplicates
		monitor = monitor + 1;
	}
	
	return;
}

// function checks the occurrence of doubles during the event of another upload to the current database
void checkDoubles(struct student database[], struct student secondary[], int secondCount, int & enrolments){
	
	int duplicatesOriginal[1000] = { 0 };
	int dupCount = 0;
	int duplicatesNew[1000] = { 0 };
	bool valid = false;
	bool secondExist = true;
	char userInput;
	int tempVals = enrolments;
	int marker = 0;
	
	for(int i = 0; i < enrolments; i++){
		
		for(int k = marker; k < secondCount; k++){
		
			// if there is a match
			if(database[i].id == secondary[k].id){
				
				// notify the user for what to do
				userInput = doubleOutNotify(userInput, secondExist, database, i, k, secondary);
				
				valid = false;
				switch(userInput){
					case 'y':
					case 'Y':
						cout << "The have selected to overwrite the file" << endl;
						appendVal(database, i, k, secondary);
						duplicatesOriginal[dupCount] = i;
						duplicatesNew[dupCount] = k;
						dupCount = dupCount + 1;
					break;
					
					case 'N':
					case 'n':
						cout << "You have not overwritten the file. This record will be duplicated toward the end of the database" << endl;
						appendVal(database, tempVals, k, secondary);
						tempVals = tempVals + 1;
						marker = marker + 1;
					break;
				}
			}
			userInput = '\0';
		}
	}

	enrolments = tempVals;

	return;
}

// function outputs the information of the instances of duplicate records and asks the user for to do. 
// this function then validates the choice and returns this to its calling function to perform the next steps based on the choice that
// has been given
char doubleOutNotify(char userValue, bool secondStruct, struct student database[], int firstPos, int secondPos, struct student secondary[]){
	
	bool confirmTrue = false;
	
	cout << '\n' << "There was an error during the upload of this file, between records:" << '\n' << endl;
	cout << "////////////// ORIGINAL RECORD //////////////" << '\n' << endl;
	outPartStruct(database, firstPos);
	cout << endl;
	cout << '\n' << "////////////// DUPLICATE RECORD //////////////" << '\n' << endl;
	
	outPartStruct(secondary, secondPos);
	
	cout << '\n' << "Would you like to overwrite File One with the contents of FILE TWO? Y/N?";
	
	//cin >> userValue;
	while(confirmTrue == false){
		cin >> userValue;
		confirmTrue = checkInput(userValue);
		if(confirmTrue == false){
			cout << "Please enter a valid selection: ";
			cin.clear();
			cin.ignore(1000,'\n');
		}
	}
	
	return userValue;
}

// function appends the values of the temporary structure to the main database after the user
// has decided on what values to insert and what not to
void appendVal(struct student database[], int locA, int locB, struct student secondary[]){

	char none[100] = { '\0' };
	
	strcpy(database[locA].name, secondary[locB].name);
	database[locA].id = secondary[locB].id;
	database[locA].single.courseNum = secondary[locB].single.courseNum;
	strcpy(database[locA].single.courseName, secondary[locB].single.courseName);
	database[locA].enrolled = secondary[locB].enrolled;
	
	for(int i = 0; i < database[locA].enrolled; i++){
		
		database[locA].person[i].year = secondary[locB].person[i].year;
		database[locA].person[i].session = secondary[locB].person[i].session;
		strcpy(database[locA].person[i].code, secondary[locB].person[i].code);
		database[locA].person[i].credit = secondary[locB].person[i].credit;
		if(secondary[locB].person[i].subjectMark == -1){
			database[locA].person[i].subStatus = secondary[locB].person[i].subStatus;
			database[locA].person[i].subjectMark = -1;
		}
		else{
			database[locA].person[i].subjectMark = secondary[locB].person[i].subjectMark;
			database[locA].person[i].subStatus = '\0';
		}
	}
	return;

}

// function swaps the values within the main database, using the original location and the new location
// that the new information is to go in
void swapVals(struct student database[], int location, int newLoc){
	
	int subSwapLimit = 0;
	int swapDiff = 0;
	int diffCount = 0;	
	char none[100] = { '\0' };
	
	strcpy(database[location].name, database[newLoc].name);
	database[location].id = database[newLoc].id;
	database[location].single.courseNum = database[newLoc].single.courseNum;
	strcpy(database[location].single.courseName, database[newLoc].single.courseName);
	
	if(database[newLoc].enrolled < database[location].enrolled){
		swapDiff = database[location].enrolled - database[newLoc].enrolled;
		subSwapLimit = database[location].enrolled;
	}
	else {
		subSwapLimit = database[newLoc].enrolled;
	}
	database[location].enrolled = database[newLoc].enrolled;
	
	for(int i = 0; i < subSwapLimit; i++){
		
		database[location].person[i].year = database[newLoc].person[i].year;
		database[location].person[i].session = database[newLoc].person[i].session;
		strcpy(database[location].person[i].code, database[newLoc].person[i].code);
		database[location].person[i].credit = database[newLoc].person[i].credit;
		if(database[newLoc].person[i].subjectMark == -1){
			database[location].person[i].subStatus = database[newLoc].person[i].subStatus;
			database[location].person[i].subjectMark = -1;
		}
		else{
			database[location].person[i].subjectMark = database[newLoc].person[i].subjectMark;
			database[location].person[i].subStatus = '\0';
		}
		if(swapDiff > 0 && (i == (subSwapLimit - 1))){
			diffCount = i + 1;
			while(diffCount < swapDiff){
				database[location].person[diffCount].year = 0;
				database[location].person[diffCount].session = '\0';
				strcpy(database[location].person[diffCount].code, none);
				database[location].person[diffCount].credit = 0;
				database[location].person[diffCount].subStatus = '\0';
				database[location].person[diffCount].subjectMark = 0;
				diffCount = diffCount + 1;
			}
		}
	}
	return;
}


// Ouputs the details of a particular record of a data structure
void outPartStruct(struct student question[], int sectionDisplay){
	
		cout << "Name: " << question[sectionDisplay].name << endl;
		cout << "Student ID: " << question[sectionDisplay].id << endl;
		cout << "Course Number: " << question[sectionDisplay].single.courseNum << endl;
		cout << "Course Name: " << question[sectionDisplay].single.courseName << endl;
		cout << "Current GPA: " << question[sectionDisplay].GPA << endl;
		cout << endl;
		cout << "Enrolled Subjects: " << question[sectionDisplay].enrolled << endl;
		for(int k = 0; k < question[sectionDisplay].enrolled; k++){
			cout << "Subject Year: " << question[sectionDisplay].person[k].year << endl;
			cout << "Subject Session: " << question[sectionDisplay].person[k].session << endl;
			cout << "Subject Code: " << question[sectionDisplay].person[k].code << endl;
			cout << "Credit Value of Subject: " << question[sectionDisplay].person[k].credit << endl;
			if(question[sectionDisplay].person[k].subjectMark == -1){
				cout << "Current Subject Status: " << question[sectionDisplay].person[k].subStatus << endl;
			}
			else{
				cout << "Subject Mark: " << question[sectionDisplay].person[k].subjectMark << endl;
			}
		}

	return;
}

// calculate the GPA value of a student
void calculateGPA(struct student database[], int enrolments){
	
	int unknownStatus = 0;
	float creditSum = 0;
	float creditMark = 0;
	
	// loop through the number of subjects of a given student and perform the GPA calculation
	for(int i = 0; i < enrolments; i++){
		for(int k = 0; k < database[i].enrolled; k++){
			if(database[i].person[k].subjectMark == -1) {
				unknownStatus = unknownStatus + 1;
			}
			else {
				// from what i understand the GPA calculation is the addition total of credit points by subject mark
				// divided by the sum of the total number of credit points.
				
				creditSum = creditSum + database[i].person[k].credit;
				creditMark = creditMark + (database[i].person[k].subjectMark * database[i].person[k].credit);
			}
		}
		// perform the final operation on the values of the current enrolments and store for that student
		database[i].GPA = creditMark / creditSum;
		// reset the values ready for the next student
		creditMark = 0;
		creditSum = 0;
	}
		
	return;
}

// function ensures that the values entered for the search GPA section are valid and then calls the function if valid input
// has been entered
int sortGPA(struct student database[], int & enrolments, char signals[][13], char alphabetLower[], char alphabetUpper[]){
	
	float userNumber = 0;
	char nums[10] = { '0','1','2','3','4','5','6','7','8','9' };
	char point = '.';
	int error = 0;
	int denom = 1;
	int digitCount = 0;
	int pointCount = 0;
	char userValue[100];
	char decimalVal[1000] = { '\0' };
	char sign[1];
	bool symbol = false;
	bool decimal = false;
	char file[100];
	int returnType = 0;
	
	// if there are no enrolments, then you need to import them first
	if(enrolments == 0){
		cout << "There are no currently imported data records. Please specify the name of the file you wish to import first: ";
		enrolments = getName(database, file, false, signals, enrolments);
		calculateGPA(database, enrolments);
	}
	
	// ask the user for the input of the search value and repeat until a valid string has been entered
	cout << "Search by GPA - Input format is a symbol, <, > or = followed by a floating point number" << endl;
	while(symbol == false){
		cin.getline(userValue, 100, '\n');
		for(int i = 0; i < strlen(userValue); i++) {
			if(i == 0){
				switch(userValue[i]){
					case '>':
					case '<':
					case '=':
						sign[0] = userValue[0];
					break;
					
					default:
						cout << "You did not enter a valid search comparison value." << endl;
						i = strlen(userValue);
						symbol = false;
					break;
				}
			}
			else if(i > 0){
				// validation of values sentinel
				if(userValue[i] >= '0' && userValue[i] <= '9' || userValue[i] == point)
				{
						if(!(userValue[i] == ' ')){
							for(int k = 0; k < 10; k++){
								if(userValue[i] == nums[k]){
									decimalVal[digitCount] = userValue[i];
									digitCount = digitCount + 1;
								}
								if(k == 9 && digitCount == 0 || pointCount > 1){
									symbol = false;
									pointCount = 0;
								}
								else {
									symbol = true;
								}
							}
						}
						// registers the denomination of which to multiply and divide at the end
						// when converting the characters to a number
						if(userValue[i] == point){
							pointCount = pointCount + 1;
							if(pointCount == 1){
								for(int d = 0; d <  strlen(userValue) - i-1; d++){
									denom = denom * 10;
								}
							}
						}
				}
				else if(userValue[i] >= '!' && userValue[i] <= '-' || userValue[i] >= '?' && userValue[i] >= '~' || userValue[i] == ',' || userValue[i] == ';') {
					symbol = false;
				}
			}
		}
		if(symbol == false){
			cout << "Please re- enter your search terms" << endl;
		}
	}
	// convert the character values into a floating number
	userNumber = atoi(decimalVal);
	userNumber = userNumber /  denom;
	
	// pass the valid values to the next function to perform the search
	returnType = findGPArecords(userNumber, sign[0], database, enrolments, alphabetLower, alphabetUpper);
	
	return returnType;
}

// function finds stores the indexes of GPA records that match the selection input by the user
int findGPArecords(float gpaVal, char comparison, struct student database[], int & enrolments, char alphabetLower[], char alphabetUpper[]){
	
	float addOn = gpaVal + 1;
	float subtract = gpaVal - 1;
	int locations[1000] = { -1 };
	int numberFound = 0;
	int returnType = 0;
	
	cout << endl;
	// search the database for the records that match the passed comparison
	for(int i = 0; i < enrolments; i++){
		
		// switch statement controls what value is matched for the GPA scores
		switch(comparison){
		
			case '<':
				//if GPA is less that input value store the index
				if(database[i].GPA < gpaVal){
					locations[numberFound] = i;
					numberFound = numberFound + 1;
				}
			break;
			
			case '>':
				// if value is greater than input GPA value store the index
				if(database[i].GPA > gpaVal){
					locations[numberFound] = i;
					numberFound = numberFound + 1;
				}
			break;
			
			case '=':
				// if value is 1 point greater or less of the input GPA, then store the index
				if(database[i].GPA >= subtract || database[i].GPA == gpaVal || database[i].GPA <= addOn){
					locations[numberFound] = i;
					numberFound = numberFound + 1;
				}
			break;
		}
	}
	// output the appropriate results according to whether any matches were found
	if(numberFound == 0){
		cout << endl;
		cout << "Sorry, your search did not generate any results, for the comparison: " << comparison << " and GPA score: " << gpaVal << endl;
		cout << endl;
	}else {
		cout << endl;
		cout << "------ Your Search Generated the Following Results ------" << endl;
		cout << "------ Search Terms - Comparison: " << comparison << ", GPA Value: " << gpaVal << endl;
		cout << endl;
		for(int r = 0; r < numberFound; r++){
			outPartStruct(database, locations[r]);
			cout << endl;
			cout << endl;
		}
		// call the next function to allow the user to edit the records that were found
		returnType = toggleSearchMenu(database, numberFound, locations, enrolments, alphabetUpper, alphabetLower);
	}
	// return response to determine what menu to stay in
	return returnType;
}

// function is the initial decider of what to do when a name search is performed
int itemSearch(struct student database[], int & enrolments, char signals[][13], char alphabetLower[], char alphabetUpper [], bool caseSense){
	
	char searchString[100] = { '\0' };
	bool wild = false;
	bool subFormat = false;
	int year = 0;
	char session = '\0';
	char code[8] = { '\0' };
	int returnType = 0;
	
	// if there are no records then they must be imported before hand
	if(enrolments == 0){
		cout << endl;
		cout << "You are attempting a search, when no records exist. Please import your input first!!" << endl;
		cout << endl;
		enrolments = getName(database, searchString, false, signals, enrolments);
	}
	
	// if you require a case sensitive search, then you are requesting a subject search, otherwise you area just searching a name value
	if(caseSense){
		cout << endl;
		cout << "Please enter your desired subject criteria to search for. Options required options include: Year, Session and Subject Code. ";
		cout << "The wild card search can also be used by entering the '*'. Remember: Search terms are case sensitive. " << endl;
		cout << endl;
		while(subFormat == false){
			cin.getline(searchString, 100, '\n');
			subFormat = validateSubjectID(searchString, year, session ,code);
			if(subFormat == false){
				cout << "Please enter a valid subject identifier: ";
			}
		}
		
		returnType = subject(year, code, database, enrolments, alphabetLower, alphabetUpper);
		
	}else {
		cout << endl;
		cout << "Please enter your name search value: ";
		cin.getline(searchString, 100, '\n');
		returnType = searchSelect(searchString, database, enrolments, alphabetLower, alphabetUpper);
	}
	cin.clear();
	// clear errors and return the type to the calling function to assess what menu to stay in
	return returnType;
}

// function searches the entered string for a name search and decides whether it has the correct information to pass on to search
// for any matched values
int searchSelect(char searchString[], struct student database[], int & enrolments, char alphabetLower[], char alphabetUpper []){

	char speshChar[2] = { '*', ' ' };
	int alphaCount = 0;
	int wildLoc = -1;
	int wildLocCount = 0;
	bool secondWild = false;
	bool invalid = false;
	bool singleWild = false;
	int stringLength = strlen(searchString);
	int spaceCount = 0;
	int returnMenuType = 0;
	
	if(searchString[0] == '*' && stringLength == 1){
		// outputs a message notifying the entering of a wild card
		// only the single type
		cout << endl;
		cout << "The You have entered a single wild card" << endl;
		cout << endl;
		singleWild = true;
	}
	else {
		// searches the string for alternate wild card sequence, and the string itself
		// returns errors if they are found.
		for(int i = 0; i < stringLength; i++){
			for(int k = 0; k < 26; k++){
				if(searchString[i] == alphabetUpper[k] || searchString[i] == alphabetLower[k]){
					searchString[i] = alphabetLower[k];
					alphaCount = alphaCount + 1;
				}
			}
			if(searchString[i] == speshChar[0]){
				
				if(wildLocCount == 0){
					wildLoc = i;
					wildLocCount = wildLocCount + 1;
				}
				else {
					secondWild = true;
				}
				if(alphaCount > 1 || wildLoc == 0 && alphaCount > 1){
					invalid = true; 
				}
			}
			if(wildLoc == 1 && wildLocCount > 1){
				invalid = true;
			}
			if(searchString[i] == speshChar[1]){
				spaceCount = spaceCount + 1;
				if(searchString[i+1] == '\0'){
					invalid = true;
				}
			}
			if(wildLoc == 0){
				if(alphaCount > 1){
					invalid = true;
				}
			}
			if(i == stringLength - 1){
				if(wildLocCount == 0){
					if(alphaCount + spaceCount < stringLength || spaceCount > 1 || spaceCount == 0){
						invalid = true;
					}
				}
			}
		}
	}
	// function then calls appropriate versions of the searchData function depending on the type of search that has been found
	// given by the characters in the string
	if(invalid || secondWild){
		cout << endl;
		cout << "You have entered an incorrect search value. Please choose another value or select the 'n' key to re-try the search again" << endl;
		cout << endl;
		return -1;
	}
	else if(singleWild){
		// search all data records
		
		returnMenuType = searchData(searchString, 3, alphabetLower, alphabetUpper, database, enrolments);
	}
	// describes the function to call with the use of a wild card
	else if(wildLocCount == 1){
		// describes the function call with the use of the single *n value
		if(strlen(searchString) == 2) {
		
			returnMenuType = searchData(searchString, 1, alphabetLower, alphabetUpper, database, enrolments);
		}
		else {
			// describes the function call with the use of the double n*n value

			returnMenuType = searchData(searchString, 2, alphabetLower, alphabetUpper, database, enrolments);
		}
	}
	else if(wildLocCount == 0 && singleWild == false){
		// describes the function call to search a case insensitive match of the entered string
		returnMenuType = searchData(searchString, 0, alphabetLower, alphabetUpper, database, enrolments);
	}
	return returnMenuType;
}

// function outputs the results of name search
int searchData(char searchString[], int wildCard, char alphabetLower[], char alphabetUpper[], struct student database[], int & enrolments){
	
	char tempName[100];
	strcpy(tempName, searchString);
	int matches[1000];
	
	int found = 0;
	int wildSearch = 0;
	int returnType = 0;
	
	// creates a version of the name that was entered
	createVersionCase(tempName, alphabetLower, alphabetUpper, false);
	
		// calls on another function to match and store the matched values. Called function passed a boolean
		// indicating the value of a wild card and the type of wild card.
		found = searchDataNames(database, enrolments, tempName,  alphabetLower, alphabetUpper, matches, wildCard);
		
		// if there are found values, then output them.
		if(found > 0){
			cout << endl;
			cout << "------- Here are you search results for your entered for search: " <<  tempName << " -------" << endl;
			cout << endl;
			for(int i = 0; i < found; i++){
				outPartStruct(database, matches[i]);
				cout << endl;
				cout << endl;
			}
			// display the menu to the user allowing them to make changes to the found records
			returnType = toggleSearchMenu(database, found, matches, enrolments, alphabetUpper, alphabetLower);
			
			// return the selection made
			return returnType;
		
		}
		else {
			cout << endl;
			cout << "--- SORRY!!! There were no matches ---" << endl;
			cout << endl;
			return - 1;
		}
}

// creates a single case version of a string, can convert between two cases, all upper or lower
void createVersionCase(char tempName[], char alphabetLower[], char alphabetUpper[], bool convertLower) {
	
	int length = strlen(tempName);
	int space = -1;
	
	for(int i = 0; i < length; i++){
		for(int k = 0; k < 26; k++){
			if(tempName[i] == alphabetLower[k] || tempName[i] == alphabetUpper[k]){
				if(!convertLower){
					tempName[i] = alphabetUpper[k];
				}else {
					tempName[i] = alphabetLower[k];
				}
			}
		}
	}
	
	return;
}

// function searches the database for names that were entered by the user. This is a case insenstive name
// function users another function to create a case version of the searched string and the found string to match. This gives the insensitive search
int searchDataNames (struct student database[], int & enrolments, char tempName[], char alphabetLower[], char alphabetUpper[], int matches[], int WildSearch){
	
	char CurrDatabaseName[100] = { '\0' };
	int subjectCount = 0;
	int count = 0;
	int tempLength = 0;
	
	for(int i = 0; i < enrolments; i++){
	
		// performs the search based on the presence of a wild card
		
		if(WildSearch >= 0 && WildSearch <= 3){
			// copies the found name to a temp char array
			strcpy(CurrDatabaseName, database[i].name);
			
			// values are then made into a single case string. both the input and then the found
			createVersionCase(CurrDatabaseName, alphabetLower, alphabetUpper, true);
			createVersionCase(tempName, alphabetLower, alphabetUpper, true);
			// get the length of the input string minus the null character
			tempLength = strlen(CurrDatabaseName) - 1;
		}
		// switch statement is used to select the method of search to use
		switch(WildSearch){
			case 0:
				// if 0, then perform a general string string comparison
				if(strcmp(CurrDatabaseName, tempName) == 0){
					matches[count] = i;
					count = count + 1;
				}
			break;
			
			case 1:
				//wild card, match the last character to the input wild card character
				if(CurrDatabaseName[tempLength] == tempName[1]){
					matches[count] = i;
					count = count + 1;
				}
			break;
			
			case 2:
				// presence of dual char wild card.  match the first and last character to the found string
				if(CurrDatabaseName[0] ==  tempName[0] && CurrDatabaseName[tempLength] == tempName[2]){
					matches[count] = i;
					count = count + 1;
				}
			break;
			
			case 3:
				// record the matched index, and count the number of matches
				matches[count] = i;
				count = count + 1;
			break;
		}
	}
	if(count > 0){
		return count;
	}
	else {
		return -1;
	}
}

// function is passed the initial subject search values and calls on another function to perform the matching. This function
// outputs the display of the subject that was matcheds
int subject(int & year, char code[], struct student database[], int & enrolments, char alphabetLower[], char alphabetUpper []){

	int studentIndexes[1000] = { 0 };
	int subjectIndexes[1000][30];
	int subjectMatchLength[1000];
	int studentMatch = 0;
	int resultResponse = 0;
	
	// call subject to match the values to any existing records
	studentMatch = searchStudentSubject(year, code, database, enrolments, studentIndexes, subjectIndexes,  subjectMatchLength);
	
	// display the values if the returned value is greater than 0, indicating found values
	if(studentMatch > 0){
		cout << endl;
		cout << "HERE ARE YOUR LISTED MATCHES FROM THE SEARCH STRING - YEAR: " << year << ", CODE: " << code << ", SESSION: " << code[8] << endl;
		for(int i = 0; i < studentMatch; i++){
			cout << "Name: " << database[studentIndexes[i]].name << endl;
			for(int k = 0; k < subjectMatchLength[i]; k++){
				cout << "Subject Details:" << endl;
				cout << database[studentIndexes[i]].person[subjectIndexes[i][k]].code << endl;
				cout << database[studentIndexes[i]].person[subjectIndexes[i][k]].year << endl;
				cout << database[studentIndexes[i]].person[subjectIndexes[i][k]].session << endl;
				cout << database[studentIndexes[i]].GPA << endl;
			}
			cout << endl;
		}
		// if there is a response, call on the toggleSearchMenu function to give the user the ability to edit the found records
		resultResponse = toggleSearchMenu(database, studentMatch, studentIndexes, enrolments, alphabetUpper, alphabetLower);
		// returns a response value which is then returned via this function to its calling function to determine whether or not to
		// stay in the same menu
		return resultResponse;
	}
	else {
		cout << endl;
		cout << "--- SORRY!!! There were no matches ---" << endl;
		cout << endl;
		return -1;
	}
	
}


// sorts through the database structure and recors the indexes of the matches values given what the user has input
// this values stores these index in two arrays, the actual index of the student, and then has to store the index of the subject that was matched
int searchStudentSubject(int & year, char code[], struct student database[], int & enrolments, int studentIndexes[], int subjectIndexes[][30], int subjectMatchLength[]){
	
	bool confirmSubMatch = false;
	int subCounter = 0;
	int subjectIterate = 0;

	for(int i=0;i < enrolments; i++){
		for(int k = 0; k < database[i].enrolled; k++){
		
			// outs the null chraracter at the end of the subject code so that
			// an appropriate string length can be found
			
			database[i].person[k].code[7] = '\0';
			
			// indicates presence of wildcard
			if(year == -1){
				subjectIndexes[subCounter][subjectIterate] = k;
				subjectIterate = subjectIterate + 1;
				confirmSubMatch = true;
			}
			else{
				// match the values to what was input. year, session and subject code have to be matched in order to return a true result 
				if(strcmp(database[i].person[k].code, code) == 0){
					// store the subject index within the matched student record
					if(database[i].person[k].session == code[8]){
						if(database[i].person[k].year == year){
							subjectIndexes[subCounter][subjectIterate] = k;
							subjectIterate = subjectIterate + 1;
							confirmSubMatch = true;
						}
					}
				}	
			}
		}
		if(confirmSubMatch == true){
			subjectMatchLength[subCounter] = subjectIterate;
			subjectIterate = 0;
			studentIndexes[subCounter] = i;
			subCounter = subCounter + 1;
			confirmSubMatch = false;
		}
	}
	
	return subCounter;
}

// function is passed a record location in the database, deletes this value and then outputs the new information
// it also performs a swap of the values, that is, moves all proceeding values from the right of the deleted value
// one space left to cover the deleted value and decrements the number of total records
void deleteRecord(int & enrolments, struct student database[], int matches[], int & mover){
	
	char nameDelete[100] = { '\0' };
	char reset = 0;
	
	for(int i = matches[mover]; i < enrolments; i++){
		strcpy(database[i].name, nameDelete);
		database[i].id = reset;
		database[i].single.courseNum = reset;
		strcpy(database[i].single.courseName, nameDelete);
		for(int k = 0; k < database[i].enrolled; k++){
			database[i].person[k].year = reset;
			strcpy(database[i].person[k].code, nameDelete);
			database[i].person[k].credit = reset;
			database[i].person[k].subjectMark = reset;
			database[i].person[k].subStatus = '\0';
		}
		// swap all values in the current database by moving all proceeding values
		// of the deleted record, one space left
		swapVals(database, i, i+1);
	}
	// decrement the number of enrolments
	enrolments = enrolments - 1;
	
	// output the values of the updates section of the array
	for(int y= 0; y < enrolments; y++){
		outPartStruct(database, y);
	}
	return;
}
