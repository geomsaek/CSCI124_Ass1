/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains function prototypes used in generating the different menus of the program
Last Modified:	22 August 2013

******/

#include <iostream>
#include <fstream>

using namespace std;

//function displays the options of the main menu
void displayMainMenu();

// function handles the selection from the user and calls the appropriate function
void actionSelect(struct student database[], char, int &, char[][13]);

// displays the search menu options and calls the appropriate functions based on the user's input
void displaySearchMenu(struct student database[], int & enrolments, char signals[][13]);

// function displays the options available when a series of search results have been returned and allows the user to move through the
// resulting record list
int toggleSearchMenu(struct student database[], int, int [], int &, char [], char[]);

// function edits the current record using the passed details to identify the record to edit within the database
// function iterates through all aspects of a student record and allows a user to edit each value. the user cannot enter a null value for
// any of the selections, nor can they enter the same value
void editCurrentRecord(struct student database[], int[], int, char[], char[]);

// function controls what section of the record to edit according to what iteration the calling function, editCurrentRecord is up to
void delegateEdit(char, int, struct student database[], int [], int, char [], char []);

// function handles the editing of the subject values for the amount of enrolled subjects for a particular record
void subjectChange(struct student database[], int [], int, char[], char[]);

// function handles the editing of each subject value for a particular subject record belonging to a student
int subjectToEdit(char, struct student database[], int[], int, int, int, char[], char[]);