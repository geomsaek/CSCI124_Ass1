/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains function prototypes used in database program
Last Modified:	22 August 2013

******/

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include<cstdlib>

using namespace std;

struct subList {
	int year;
	char session;
	char code[7];
	int credit;
	int subjectMark;
	char subStatus;
};

struct numName {
	int courseNum;
	char courseName[100];
};

// contains the information uploaded through the text file. This is linked to other structures which store
// additional information such as the subjects enrolled in and the course number and name
struct student {
	char name[30];
	int id;
	numName single;
	int enrolled;
	subList person[30];
	float GPA;
};

// function checks the presence of doubles during the initial load of a text file. it is passed the enrolments
// variable which will monitor the number of students that are uploaded and affected if there are duplicates
// it is also passed an array of the student database which will store this information
void initialDoubles(int &, struct student database[]);

// function will output a section of the structure, i.e the records pertaining to a single student
void outPartStruct(struct student question[], int sectionDisplay);

//record will assess the presence of doubles during a second upload of students. it will compare
// what is stored to what is being uploaded
void checkDoubles(struct student database[], struct student secondary[], int, int &);

// function will output information when a duplicate record is encountered and react according to the users choice
// this is accomplished by using a second structure to store the temporary struct values before uploading
// and marking which ones to append to the existing structure
char doubleOutNotify(char, bool, struct student database[], int, int, struct student secondary[]);

// function will swap information between two different records in a structure that is already holding information
void swapVals(struct student database[], int, int);

// function will append new values to the existing database. Is called when the user wants to upload additional records
// after an initial upload
void appendVal(struct student database[], int, int, struct student secondary[]);

// function calculates the GPA score for a given student
void calculateGPA(struct student database[], int);

// function sorts the GPA scores according to the search values input by the user
// input of the search criteria is validated during this function
int sortGPA(struct student database[], int &, char [][13], char[], char[]);

// function is passed on valid search terms and outputs found results
int findGPArecords(float, char, struct student database[], int &, char[], char[]);

// performs the search functionality for the subject code or name search
int itemSearch(struct student database[], int &, char[][13], char [], char[], bool);

// function records the matched records with the input and stores the indexes and count of these matches to be displayed
int searchSelect (char [], struct student database[], int &, char[], char[]);

//outputs the search results recorded by the searchSelect function and displays the editing and viewing menu
int searchData(char [], int, char[], char [], struct student database[], int & enrolments);

// creates an lower case version of a string. used for insensitive searches. to remove upper case characters
void createVersionCase(char [], char [], char [], bool);

// searches the current database for a name match
int searchDataNames (struct student database[], int &, char[], char [], char [], int [], int);

// searches the database for a subject year and session search. performs error checking before passing the data on to another
// function to perform and record the details of this search
int subject(int &, char[], struct student database[], int &, char[], char[]);

// is called by the subject function and passed the criteria and performs the results of a subject search and stores the resultss
int searchStudentSubject(int &, char [], struct student database[], int &, int [], int [][30], int[]);

// deletes a selected record
void deleteRecord(int &, struct student database[], int [], int &);
