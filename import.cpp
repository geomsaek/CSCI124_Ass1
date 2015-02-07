/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains function definitions used in the importing and exporting data
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

// function retrieves or passes the name of which to open for import
// it handles the validation of whether the importing file is of the correct format as well as record duplication
int getName(struct student database[], char supplied[], bool otherName, char signals[][13], int & enrolments){

	int counter = 0;
	int secondCount = 0;
	student secondary[1000];
	char fileName[20];
	char currentLine[300];
	bool input = false;
	int importReturn = 0;
	int importReturned = 0;
	ifstream data;
	
	// function is called in instances where the user has tried to perform an operation without
	// uploading any data. in this instance, this value is passed instead from the function that called it
	if(otherName == true){
		strcpy(fileName, supplied);
	}
	// otherwise the function will prompt the user for the name. and assess whether it exists or not
	else {
		cout <<  "Please enter the name of the file you wish to read" << endl;
		cin.getline(fileName, 20, '\n');
		input = findExist(fileName);
	}
	
	// while there is an invalid name, repeat the input process
	while(input == false){
		cout << "Please enter a enter a valid file name" << endl;
		cin.getline(fileName, 20, '\n');
		input = findExist(fileName);
	}
	
	// open the file and begin the import
	
	data.open(fileName);
	data.getline(currentLine, 300, '\n');
	
	// assess whether the file follows the correct format
	if(strcmp(currentLine, signals[0]) == 0){
		
		// handle situations where a current database exists and append accordingly
		// otherwise handle duplications in the initial upload
		if(enrolments == 0){
			importReturn = importRead(data, counter, database, signals);
			initialDoubles(counter, database);
			enrolments = counter;
		}else {
			importReturn = importRead(data, secondCount, secondary, signals);
			enrolments = secondCount;
			checkDoubles(database, secondary, secondCount, enrolments);
		}
		// output the following error responses based on bad file formats or sources that exceed the max values of the import
		switch(importReturn){
			
			case -1:
				cout << "Please ensure that each record begins with'####RECORD###'" << endl;
				enrolments = 0;
			break;
			
			case -2:
				cout << "You have entered the maximum number of subjects for a student. The import process has been stopped." << endl;
				enrolments = 0;
			break;
			
			case - 3:
				cout << "You have entered the maximum number of students into the data base the import process has been stopped." << endl;
				enrolments = 0;
			break;	
		}
	}
	else {
		cout << "Please ensure that your file to import has the correct structure" << endl;
	}
	
	// return the number of enrolments
	return enrolments;
}

// function handles the general importing of data and calls the appropriate functions to handle
// different sections to store in the database
int importRead(ifstream & data, int & counter, struct student database[], char signals[][13]){
	
	bool end = false;
	bool subjectCount = true;
	int retVal = 0;
	char currentLine[300];
	
	// validate whether the file has a starting indicator
	data.getline(currentLine, 300, '\n');
	if(strcmp(currentLine, signals[2]) == 0){
		do{
			data.getline(currentLine, 300, '\n');
			
			// ensure that the file has a starting record indicator
			if(strcmp(currentLine, signals[1]) > 0){
			
				// retrieve the general info from the file. that is lines that require just retrieving and storing
				getGeneralInfo(database, counter, data, currentLine);
				// retrieve the subjects and their values from the imported file
				subjectCount = getSubjects(database, counter,currentLine, signals, data);
				
				// if the subject indicator is false then the subjects were not of the correct format or there were too many
				if(subjectCount == false){
					retVal = -2;
					end = true;
				}
			}
			counter  = counter + 1;
			// count the records
			
		// while the file is not at the end or there are less than 1000 records
		}while(!data.eof() && counter < 1000);
		
		if(counter == 1000){
			retVal = -3;
		}
	}
	else {
		retVal = -1;
	}
	
	data.close();
	
	return retVal;
}

// retrieves the values of the subjects and stores them for a particular student record
bool getSubjects(struct student database[], int & counter, char currentLine[], char signals[][13], ifstream & data){
	
	char temp[10] = { "\0" };
	char empty[10] = { '\0' };
	bool current = false;
	int subjectCounter = 0;
	
	do {
		// gets the subject line
		data.getline(currentLine, 17, '\n');
		// ensure that the line is not that of "### RECORD ### indicating the start of another students info
		// of the end of the file has occurred
		if(strcmp(currentLine, signals[2]) == 0 || strcmp(currentLine, signals[1]) == 0){
			current = true;
		}
		else { 
			// otherwise break apart the values in the retrieved line from the file
			strncpy(temp, currentLine, 4);
			database[counter].person[subjectCounter].year = atoi(temp);
			database[counter].person[subjectCounter].session = currentLine[4];
			getSubCode(database, currentLine, counter, subjectCounter);
			subjectCounter = subjectCounter + 1;
		}

	} while(current == false);
	
	// store the number of retrieved subjects in the structure
	database[counter].enrolled = subjectCounter;
	
	strncpy(temp, "\0", 10);
	subjectCounter = 0;
	
	// return true if no error has occurred and there are less than 30 subjects
	if(database[counter].enrolled < 30){
		return true;
	}	
	else {
		return false;
	}
}

// function is passed the values to further break apart the string that contains the subject code and marking values
void getSubCode(struct student database[], char currentLine[], int counter, int subjectCounter){
	int sub = 5;
	char temp;
	char blank[10] = { '\0' };
	int mark = 0;
	int tempMark = 0;
	int numStore = 10;
	int track = 0;
	int lineLength = strlen(currentLine);
	
	for(int i = 0; i < 13; i++){
		if(i < 7){
			// retrieve the values that make up the subject code
			database[counter].person[subjectCounter].code[i] = currentLine[sub];
		}
		// retrieve the values that make up the credit value and convert these to an integer
		if(i > 6 && i < 9){
			temp =  currentLine[sub];
			database[counter].person[subjectCounter].credit = (database[counter].person[subjectCounter].credit * 10) + temp - '0';
		}
		if(i > 8 && i < 12) {
			
			if(currentLine[sub] == '\0'){
				i = 15;
			}
			else {
				// if there are unspecifed subject marks then store them in the subject status and store an indicator in the subject mark
				// indicating this
				switch(currentLine[sub]){
					case 'e':
					case 'w':
						database[counter].person[subjectCounter].subStatus = currentLine[sub];
						mark = -1;
						i = 15;
					break;
					
					default:
						// retrieve the subject mark and convert it to a number
						// with the correct denomination
						
						temp =  currentLine[sub];
						tempMark = temp - '0';
						if(track < 3){
							if(track){
								mark =  (mark * numStore) + tempMark;
								if(numStore < 10){
									numStore = numStore * 10;
								}
							}
							else {
								mark = tempMark;
							}
							track = track + 1;
						}
					break;
				}
			}
		}
		sub = sub + 1;
	}

	// store the current subject mark
	database[counter].person[subjectCounter].subjectMark = mark;
	
	return;
}

// function assesses the validity of the name of the file to export the database info to.
// it also passes a valid name to the exporting function to perform the exporting of data to the new named file
void getFileName(char exportname [], char finalExport[]){

	char textExt[] = { ".txt" };
	char dot = '.';
	bool flag = false;
	bool name = false;
	
	while(name == false){
	
		cin.getline(exportname, 20, '\n');
		flag = false;
		for(int i = 0; i < strlen(exportname); i++){
			if(exportname[i] == dot){
				flag = true;
			}
		}
		if(flag){
			cout << "Please re- enter the name without the .txt extension" << endl;
			cin.clear();
		}
		else {
			name = true;
		}
	}
	cin.clear();
	
	// pass the valid file name to the export function
	strcpy(finalExport, exportname);
	strcat(finalExport, textExt);
	
	return;
}

// function assesses whether the given name exists based on whether the name that is
// given opens a file. if it does then the it returns a false value to indicate an overwrite
bool findExist(char fileName[]){

	ifstream check;
	
	check.open(fileName);
	if(check.good()){
		check.close();
		return true;
	}
	else {
		return false;
	}
}

//retrieves the general information from a file import. by general, these strings dont need to be broken apart, just validated
void getGeneralInfo(struct student database[], int & counter, ifstream & data, char currentLine[]){

	strcpy(database[counter].name, currentLine);
	data >> database[counter].id;
	data >> database[counter].single.courseNum;
	data.clear();
	data.ignore(1,' ');
	data.getline(database[counter].single.courseName, 100, '\n');
	
	return;
}

// function assesses whether or not to export the information in the current database to an output file or not
// there has to be information before this can happen
void exportData(struct student database[], int & enrolments, char signals[][13]){
	
	char file[100];
	
	// if there arent, ask for the import first and then export
	if(enrolments == 0){
		cout << endl;
		cout << "There are no currently imported data records. Please specify the name of the file you wish to import first: ";
		enrolments = getName(database, file, false, signals, enrolments);
		calculateGPA(database, enrolments);
		moveData(database, enrolments, signals);
	}
	else {
		// if there is already information, export it
		moveData(database, enrolments, signals);
	}
	return;
}

// function prompts for the name of the output file and passes a valid name to the write function
void moveData(struct student database[], int & enrolments, char signals[][13]){
	
	char exportname[20] = { "\0" };
	char finalExport[30] = { "\0" };
	bool overWrite = true;
	char yesNo;
	bool repeatChoice = true;
	
	ofstream dataExport;
	
	// asks for the name to write to
	cout << "Please enter the name you wish the file to export to excluding the .txt extension" << endl;
	
	// assess whether it exists or not
	getFileName(exportname, finalExport);
	overWrite = findExist(finalExport);
	
	// asks to overwrite or not and prompts for another name if the user doesnt want to overwrite
	if(overWrite){
		cout << "The file you entered already exists. Do you want to overwrite it. Y/N?: ";
		while(repeatChoice == true){
			cin.clear();
			cin >> yesNo;
			if(yesNo == 'Y' || yesNo == 'y'){
				writeData(dataExport, finalExport, database, enrolments, signals);
				repeatChoice = false;
			}
			else {
				while(overWrite == true) {
					
					getFileName(exportname, finalExport);
					cout << "That name already exists, please re-enter another one: ";
					overWrite = findExist(finalExport);
				}
				
				// write the data to the file after all pre- tests are true
				writeData(dataExport, finalExport, database, enrolments, signals);
				repeatChoice = false;
			}
		}
	}
	else {
		// write the data to the file if no errors
		writeData(dataExport, finalExport, database, enrolments, signals);
	}
	
	return;
}

// function writes data to the output file in the format of the import
void writeData(ofstream & dataExport, char finalExport[], struct student database[], int & enrolments, char signals[][13]){
	
	char codeTemp[7];
	
	dataExport.open(finalExport, ios::out);
	dataExport << signals[0] << endl;
	dataExport << signals[2] << endl;
	for(int i = 0; i < enrolments; i++){
		dataExport << database[i].name << endl;
		dataExport << database[i].id << endl;
		dataExport << database[i].single.courseNum << " ";
		dataExport << database[i].single.courseName << endl;
		for(int k = 0; k < database[i].enrolled; k++){
			dataExport << database[i].person[k].year;
			dataExport << database[i].person[k].session;
			strcpy(codeTemp, database[i].person[k].code);
			dataExport << codeTemp;
			if(database[i].person[k].credit < 10){
				dataExport << 0 << database[i].person[k].credit;
			}else {
				dataExport << database[i].person[k].credit;
			}
			if(database[i].person[k].subjectMark == -1){
				dataExport << database[i].person[k].subStatus << endl;
			}else {
				dataExport << database[i].person[k].subjectMark << endl;
			}
		}
		if(i < enrolments -1){
			dataExport << "###RECORD###" << endl;
		}
	}
	dataExport << "###END###" << endl;
	dataExport.close();
	cin.clear();
	
	return;
}


// function overwrites the values of the existing database structure with that of a secondary temporary structure
void overWriteRecords(int duplicatesOriginal [], int duplicatesNew[], int dupCount, int secondCount, int  & enrolments, struct student database[], struct student secondary[]){
	
	// loop executes through until the passed duplicated count has been reached 
	
	for(int i = 0; i < dupCount; i++){
	
		// copies an existing database record with that of another value of the same equivalence.
		// does this for all instances of the current record
		
		strcpy(database[duplicatesOriginal[i]].name, secondary[duplicatesNew[i]].name);
		database[duplicatesOriginal[i]].id = secondary[duplicatesNew[i]].id;
		database[duplicatesOriginal[i]].single.courseNum = secondary[duplicatesNew[i]].single.courseNum;
		strcpy(database[duplicatesOriginal[i]].single.courseName, secondary[duplicatesNew[i]].single.courseName);
		database[duplicatesOriginal[i]].enrolled = secondary[duplicatesNew[i]].enrolled;
		for(int k = 0; k < database[duplicatesOriginal[i]].enrolled; k++){
			database[duplicatesOriginal[i]].person[k].year = secondary[duplicatesNew[i]].person[k].year;
			database[duplicatesOriginal[i]].person[k].session = secondary[duplicatesNew[i]].person[k].session;
			strcpy(database[duplicatesOriginal[i]].person[k].code, secondary[duplicatesNew[i]].person[k].code);
			database[duplicatesOriginal[i]].person[k].credit = secondary[duplicatesNew[i]].person[k].credit;
			if(secondary[duplicatesNew[i]].person[k].subjectMark == -1){
				database[duplicatesOriginal[i]].person[k].subStatus = secondary[duplicatesNew[i]].person[k].subStatus;
			}
			else {
				database[duplicatesOriginal[i]].person[k].subjectMark = secondary[duplicatesNew[i]].person[k].subjectMark;
			}
		}
	}
	
	return;
}