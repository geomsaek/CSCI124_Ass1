/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains functions used in generating the different menus of the program
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

//function displays the options of the main menu

void displayMainMenu() {
	cout << endl;
	cout << "/**************** MAIN MENU *********************/" << '\n' << endl;
	cout << "Welcome to the main menu. Please choose one of the following options to begin" << endl;
	cout << "q - Quit the program" << endl;
	cout << "i - Import data from text file" << endl;
	cout << "e - export data to text file" << endl;
	cout << "s - Display the search menu" << '\n' <<  endl;
	cout << "Please enter your selection: ";
	return;
}


// function handles the selection from the user and calls the appropriate function
void actionSelect(struct student database[], char selection, int & enrolments, char signals[][13]){
	
	char fileNone[] = { "none" };
	
	// switch statement handles valid choices
	switch(selection){
		case 'q':
			cout<< endl;
			cout << "--- You Quit the program! ---" << endl;
			cout<< endl;
		break;
		
		case 'i':
			// calls the getName function that retrieves the values from the import file
			enrolments = getName(database, fileNone, false, signals, enrolments);
			
			// calculate the GPA values after the import is complete
			calculateGPA(database, enrolments);
			
			// if there are enrolments, then output a prompt explaining this and how many records were uploaded
			if(enrolments > 0){
				displayMainMenu();
				cout << "--------- IMPORT COMPLETE ---------" << endl;
				cout << "The total number of records that were imported was: " << enrolments << endl;
				cout << endl;
			}else {
			
				// if there are no enrolments, either the file was empty or an error occurred
				// message is displayed to inform the user
				cout << "There were no records to upload in the file that was selected or there was an error in the import" << endl;
				cout << "Please ensure that the correct format was followed" << endl;
			}
			cout << '\n' << "Please specify action from the main menu you would like to make: ";
		break;
		
		case 's':
			// displays the search menu options
			displaySearchMenu(database, enrolments, signals);
		break;
		
		case 'e':
			// prompts the user of when data is exported
			cout << "--------- EXPORT DATA ---------" << endl;
			exportData(database, enrolments, signals);
			displayMainMenu();
			cout << "EXPORT COMPLETE..." << endl;
			cout << endl;
			cout << '\n' << "Please specify action from the main menu you would like to make: ";
		break;
	}
	
	
	return;
}

// displays the search menu options and calls the appropriate functions based on the user's input
void displaySearchMenu(struct student database[], int & enrolments, char signals[][13]){
	
	char searchInput;
	int length = 4;
	char validVals[4];
	validVals[0] = 'r'; 
	validVals[1] = 'n';
	validVals[2] = 's';
	validVals[3] = 'g';
	char alphabetLower[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	char alphabetUpper[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	int returnType = 0;
	
	// while the user hasn't entered the r character to return to the main menu, stay in the current menu
	while(searchInput != 'r'){
	
		cout << '\n' << "/**************** SEARCH MENU *********************/" << endl;
		cout << '\n' << "Welcome to the search menu. Please select one of the following options" << endl;
		cout << endl;
		cout << "r - return to the main menu" << endl;
		cout << "n - Name Search. (Case Insensitive). Supports the wild card * feature. Any name that starts or ends with the letter";
		cout << "desired letter will be produced" << endl;
		cout << "s - Subject Search (Case sensitive). Option returns a list of students taking the subject matching the search criteria promote the user for year, session and code";
		cout << "for each input, the user can use the wild card *" << endl;
		cout << "g - Search by GPA, the input format is a symbol <, > or = followed by a floating point number. for =, any result within 1 point of the specified GPA is considered a match";
		cout << "That is, if the user want to search for students with GPA = 5.5, it will display all students whose GPA is from 4.5 to 6.5 (inclusive)" << endl;
		cout << endl;
		cout << "Please enter your selection: ";
		
		searchInput = getUserInput(length, validVals);
		
		switch(searchInput){
	
			case 'n':
			
				// call the itemSearch function that searches for database names
				returnType = itemSearch(database, enrolments, signals, alphabetLower, alphabetUpper, false);
				// if the returntype is not 0, meaning that they have not selected to return to the main menu
				if(returnType == 0){
					searchInput = 'r';
				}
			break;
	
			case 's':
				// call the itemSearch function for subjects
				returnType = itemSearch(database, enrolments, signals, alphabetLower, alphabetUpper, true);
				if(returnType == 0){
					searchInput = 'r';
				}
			break;
	
			case 'g':
				// call the sortGPA function to handle the search for GPAs
				returnType = sortGPA(database, enrolments, signals, alphabetLower, alphabetUpper);
				if(returnType == 0){
					searchInput = 'r';
				}
			break;
		}
		// if the user has entered the r character display the main menu list and return back to the main input area
		if(searchInput == 'r'){
			displayMainMenu();
		}
		cin.clear();
	}

	return;
}

// function displays the options available when a series of search results have been returned and allows the user to move through the
// resulting record list
int toggleSearchMenu(struct student database[], int found, int matches[], int & enrolments, char alphabetUpper[], char alphabetLower[]){

	char moveBetween;
	bool select = false;
	int mover = 0;
	bool initialDisplay =true;
	
	// if the menu hasnt been displayed before then output the full options available
	if(initialDisplay == true){
		cout << endl;
		cout << "There are a total of: " << found << " results. Please use the listed keys to perform the following actions:" << endl;
		cout << endl;
		cout << "r - return to the main menu" << endl;
		cout << "s - return to the search menu" << endl;
		cout << "n - Go to the next record" << endl;
		cout << "p - Go to the previous record" << endl;
		cout << "e - edit the current record" << endl;
		cout << "d - delete the current record" << endl;
		cout << endl;
	}
	while(select == false){
		// selections are kept in a while loop, with the choices selected in a switch statement
		// until the user selects a choice that returns to a different menu, or completes an action, they will remain in this menu
	
		cout << endl;
		cout << "----- CURRENT RECORD " << mover + 1 << " -----" << endl;
		outPartStruct(database, matches[mover]);
		cout << endl;
		cout << "Please select your option display menu option, (r)eturn to search menu, (s)earch menu return, ";
		cout << "(p)revious record, (n)ext record, (e)dit current record, (d)elete current record: " << endl;
		
		// get the users value
		cin >> moveBetween;
		switch(moveBetween){
			case 'r':
				// if its r return to the main menu, 0 will return back to the calling function which will return back to the main menu
				return  0;
			break;
	
			case 's':
				// return to the search function
				return - 1;
			break;
	
			case 'n':
				// display the next item in the search result list and output the details of the current record
				// turn of the initial display of the menu
				initialDisplay =false;
				
				// if the current record is not at the end, move through the list and display the details
				if(found > 1 && mover < found-1){
					// keeps track of where the user is within the search result list
					mover = mover + 1;
					cout << endl;
					cout << "Currently Displaying Record: " << mover + 1 << " of " << found << " records" << endl;
					cout << endl;
				}else {
					// if you cant move any further then output the message
					cout << "------- You can't move to the next record, because there aren't any more to see ------- " << endl;
				}
			break;
	
			case 'p':
				// turn of the initial display of the menu
				initialDisplay =false;
				
				// displays the previous item within the search result list and outputs the details of the record
				if(found > 1 && mover > 0){
					// keeps track of where the user is within the search result list
					mover = mover - 1;
					cout << "Currently Displaying Record: " << mover + 1 << " of " << found << " records" << endl;
					cout << endl;
				}else {
					// if the user reaches the end then display a message explaining this
					cout << "-------  You have reached the start of the selection and can't move back any further -------" << endl;
				}
			break;
	
			case 'e':
				// if the user decides to edit the current record, then listed function is called
				// with the current details to edit the value
				editCurrentRecord(database, matches, mover, alphabetUpper, alphabetLower);
				calculateGPA(database, enrolments);
				select = true;
			break;
	
			case 'd':
				// if delete the current record
				deleteRecord(enrolments, database, matches, mover);
				return 0;
				select = true;
			break;
			
			default:
				// if the user enters an invalid selection clear the error and display the list
				cout << endl;
				cout << "Please enter a valid selection!" << endl;
				cin.clear();
				cin.ignore(1000,'\n');
				cout << endl;
			break;
		}
	}
}

// function edits the current record using the passed details to identify the record to edit within the database
// function iterates through all aspects of a student record and allows a user to edit each value. the user cannot enter a null value for
// any of the selections, nor can they enter the same value
void editCurrentRecord(struct student database[], int matches[], int mover, char alphabetUpper[], char alphabetLower[]){

	bool value = false;
	char valArray[4] = { 'y','Y','n','N' };
	char yesNo;
	int sectionCount = 0;
	
	while(sectionCount < 5){
	
		// main section is controlled in a while loop which outputs a different message upon the
		// iteration of a counter. according to what iteration the counter is up to and what value the
		// user enters, the choice is validated and passed on to another function, which handles what
		// section of the student record the user is to update next
		
		switch(sectionCount){
			case 0:
				cout << endl;
				cout << "Do you want to edit the current name? Y/N?: " << endl;
			break;
			
			case 1:
				// need to look at the current ID as this isnt passing the test
				cout << endl;
				cout << "Do you want to edit the current ID? Y/N?: " << endl;
			break;
			
			case 2:
				cout << endl;
				cout << "Do you want to edit the current course number? Y/N? " << endl;
			break;
			
			case 3:
				cout << endl;
				cout << "Do you want to edit the current course name? Y/N?" << endl; 
			break;
			
			case 4:
				cout << endl;
				cout << "Do you want to edit the current subject list? Y/N?: " << endl;
			break;
		}
		
		// get the user input and validate it before
		yesNo = getUserInput(4, valArray);
		
		// call on the edit function to edit the section of the record
		delegateEdit(yesNo, sectionCount, database, matches, mover, alphabetUpper, alphabetLower);
		
		// clear the selection before the next prompt.
		yesNo = '\0';
		
		// increment to the next choice
		sectionCount = sectionCount + 1;
	}
		
	return;
}

// function controls what section of the record to edit according to what iteration the calling function, editCurrentRecord is up to
void delegateEdit(char yesNo, int sectionCount, struct student database[], int matches[], int mover, char alphabetUpper[], char alphabetLower[]){
	
	int IDNumber = 0;
	
	switch(sectionCount){
		case 0:
			// edit the current name and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new name you would like to change: ";
				nameChange(database[matches[mover]].name, false, alphabetUpper, alphabetLower, 30, 0, false, 1);
				cout << "--- You changed the name to: " << database[matches[mover]].name << " ---" <<endl;
				return;
			}else {
				return;
			}
		break;
		
		case 1:
			// edit the current ID and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new ID that you would like to change: ";
				IDNumber =  numberChange(database[matches[mover]].id, 1000000, 9999999);
				database[matches[mover]].id = IDNumber;
				cout << "--- You changed the id to: " << database[matches[mover]].id << " ---" <<endl;
				cout << endl;
				IDNumber = 0;
				return;
			}else {
				return;
			}
			
		break;
		
		case 2:
			// edit the current course number and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new course number that you would like to change: ";
				IDNumber =  numberChange(database[matches[mover]].single.courseNum, 100, 9999);
				database[matches[mover]].single.courseNum = IDNumber;
				cout << "--- You changed the course number to: " << database[matches[mover]].single.courseNum << " ---" <<endl;
				cout << endl;
				IDNumber = 0;
				return;
			}else {
				return;
			}
		break;
		
		case 3:
			// edit the current course name and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new course name you would like to change: ";
				nameChange(database[matches[mover]].single.courseName , false, alphabetUpper, alphabetLower, 100, 0, true, 7);
				cout << "--- You changed the course name to: " << database[matches[mover]].single.courseName << " ---" <<endl;
				cout << endl;
				return;
			}else {
				return;
			}
		break;
		
		case 4:
			// if the user wants to edit, call the subjectChange function to edit the values associated to the subjects of the current record
			if(yesNo == 'y' || yesNo == 'Y'){
				subjectChange(database, matches, mover, alphabetUpper, alphabetLower);
			}else {
				return;
			}
		break;
	}
}

// function handles the editing of the subject values for the amount of enrolled subjects for a particular record
void subjectChange(struct student database[], int matches[], int mover, char alphabetUpper[], char alphabetLower[]){
	
	bool value = false;
	int subInfoCount = 0;
	int numOfSubs = database[matches[mover]].enrolled;
	char valArray[4] = { 'y','Y','n','N' };
	char yesNo;
	int subjectResponse = 0;
	bool markStatus = false;
	
	for(int i  = 0; i < numOfSubs; i++){
		// controlled in an inner while loop for all sections of each subject
		// the outer for loop makes this process run for the amount of enrolments for the particular record being edited
		while(subInfoCount < 6){
			// switch case handles the current iteration of the subject attribute list and outputs the message to edit
			switch(subInfoCount){
				case 0:
					cout << "Do you want to edit the current subject year? Y/N?: ";
				break;
				
				case 1:
					cout << "Do you want to edit the current subject session? Y/N?: ";
				break;
				
				case 2:
					cout << "Do you want to edit the current subject code? Y/N?: ";
				break;
				
				case 3:
					cout << "Do you want to edit the current subject credit point value? Y/N?: ";
				break;
				
				case 4:
					cout << "Do you want to edit the current subject mark? Y/N?: ";
				break;
				
				case 5:
					// if the user says no to entering a subject mark, and the value of the subject is -1
					// then that means the student has a current undefined status and the user has the option
					// to edit the status value
					// otherwise this option is unavailable
					if(subjectResponse != 1){
						cout << "Do you want to edit current subject status? Y/N?: ";
					}else {
						markStatus = true;
					}
				break;
			}
			// if the markstatus is false that is, there is a subject mark then call the next function to edit
			if(markStatus == false){
				yesNo = getUserInput(4, valArray);
				subjectResponse = subjectToEdit(yesNo, database, matches, mover, i, subInfoCount, alphabetUpper, alphabetLower);
				yesNo = '\0';
			}
			subInfoCount = subInfoCount + 1;
		}
		// reset values for next iteration
		subInfoCount = 0;
		markStatus = false;
	}
	
	return;
}


// function handles the editing of each subject value for a particular subject record belonging to a student
int subjectToEdit(char yesNo, struct student database[], int matches[], int mover, int currentSubject, int section, char alphabetUpper[], char alphabetLower[]){
	
	int numberUpdate = 0;
	bool validSingle = false;
	char choice;
	char sessionVals[3];
	sessionVals[0] = 'S';
	sessionVals[1] = 'W';
	sessionVals[2] = 'A';
	char subStatus[2];
	subStatus[0] = 'e';
	subStatus[1] = 'w';
	
	// select the case, that is the section of the current subject record you want to edit
	// and output the changed value if the user decides to change the value
	switch(section){
		case 0:
			// edit the subject year and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the subject year you wish to update: ";
				numberUpdate = numberChange(database[matches[mover]].person[currentSubject].year, 2012, 3000);
				database[matches[mover]].person[currentSubject].year = numberUpdate;
				cout << "---- You have changed the subject year to: " << database[matches[mover]].person[currentSubject].year << " ---" <<endl;
				cout << endl;
				numberUpdate = 0;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 1:
			// edit the session value for the subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				while(validSingle == false){
					cout << "Please enter the session you would like to update: ";
					cin >> choice;
					validSingle = sessionCheck(choice, sessionVals, 3);
					if(validSingle == false){
						cout << "You did not enter a correct value. Please make sure your values are upper case." << endl;
						cout << endl;
					}
				}
				validSingle = false;
				database[matches[mover]].person[currentSubject].session = choice;
				cout << "---- You have changed the session value of the subject to: " << database[matches[mover]].person[currentSubject].session << " ---" <<endl;
				cout << endl;
				choice = '\0';
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 2:
			// edit the subject code for the subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new subject code you would like to update: ";
				nameChange(database[matches[mover]].person[currentSubject].code, true, alphabetUpper, alphabetLower, 9, 3, false, 0);
				cout << "--- You changed subject " << currentSubject << "'s code to: " << database[matches[mover]].person[currentSubject].code << " ---" <<endl;
				cout << endl;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 3:
			// edit the credit point value for the current subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new subject credit point value: ";
				numberUpdate = numberChange(database[matches[mover]].person[currentSubject].credit, 5, 20);
				database[matches[mover]].person[currentSubject].credit = numberUpdate;
				numberUpdate = 0;
				cout << "---- You have changed the credit point value of the subject to: " << database[matches[mover]].person[currentSubject].credit << " ----" << endl;
				cout << endl;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 4:
			// edit the subject mark for the current subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new subject mark for this subject: ";
				numberUpdate = numberChange(database[matches[mover]].person[currentSubject].subjectMark, 0, 100);
				database[matches[mover]].person[currentSubject].subjectMark = numberUpdate;
				cout << "---- You have changed the current mark for this subject to: " << database[matches[mover]].person[currentSubject].subjectMark << " ----" << endl;
				numberUpdate = 0;
				cout << endl;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 5:
			// edit the subject status for the subject and return to the calling function, otherwise just return
			// if there a -1 then change the value and disallow the option to edit the subjects status, otherwise
			// if the user says no and the value indicates that there is a substatus value then the user must enter the substatus value
			// if the user enters a subject mark and says yes to the subject status, then output an error
			if(yesNo == 'y' || yesNo == 'Y'){	
				if(database[matches[mover]].person[currentSubject].subjectMark == -1){
					while(validSingle == false){
						cout << "Please enter the subject status you would like to update: ";
						cin >> choice;
						validSingle = sessionCheck(choice, subStatus, 2);
						if(validSingle == false){
							cout << "You did not enter a correct value. Please make sure your values are either e or w." << endl;
							cout << endl;
						}
					}
					validSingle = false;
					database[matches[mover]].person[currentSubject].subStatus = choice;
					cout << "---- You have changed the current session status for this subject to: " << database[matches[mover]].person[currentSubject].subStatus << " ----" << endl;
					choice = '\0';
					cout << endl;
				}else {
					cout << "There already is a subject mark. You can't add a subject status also!" << endl;
					cout << endl;
					return 0;
				}
				return 1;
			}else {
				return 0;
			}
		break;
	}
}
