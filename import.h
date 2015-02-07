/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains all import and export data from filestream protoypes
Last Modified:	22 August 2013

******/

#include <iostream>
#include <fstream>

using namespace std;

// function retrieves or passes the name of which to open for import
// it handles the validation of whether the importing file is of the correct format as well as record duplication
int getName(struct student database[], char [], bool, char [][13], int &);

// function handles the general importing of data and calls the appropriate functions to handle
// different sections to store in the database
int importRead(ifstream & data, int &, struct student database[], char [][13]);

// retrieves the values of the subjects and stores them for a particular student record
bool getSubjects(struct student database[], int &, char [], char [][13], ifstream &);

// function is passed the values to further break apart the string that contains the subject code and marking values
void getSubCode(struct student database[], char currentLine[], int counter, int subjectCounter);

// function assesses the validity of the name of the file to export the database info to
void getFileName(char [], char []);

// function assesses whether the given name exists based on whether the name that is
// given opens a file. if it does then the it returns a false value to indicate an overwrite
bool findExist(char []);

//retrieves the general information from a file import. by general, these strings dont need to be broken apart, just validated
void getGeneralInfo(struct student database[], int &, ifstream &, char []);

// function assesses whether or not to export the information in the current database to an output file or not
// there has to be information before this can happen
void exportData(struct student database[], int &, char signals[][13]);

// function prompts for the name of the output file and passes a valid name to the write function
void moveData(struct student database[], int &, char[][13]);

// function writes data to the output file in the format of the import
void writeData(ofstream &, char [], struct student database[], int&, char[][13]);

// function overwrites the values of the existing database structure with that of a secondary temporary structure
void overWriteRecords(int [], int[], int, int, int & , struct student database[], struct student secondary[]);