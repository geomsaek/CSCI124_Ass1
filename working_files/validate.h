/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 1
Description:	Contains function prototypes used validation
Last Modified:	24 August 2013

******/

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include<cstdlib>

using namespace std;

// function checks the input values where the user needs to answer a yes or no question and returns
// boolean value if the value matches or not
bool checkInput(char);

//function gets the user input and checks against a set of values and returns the valid
// character. validation occurs within this function
char getUserInput(int , char[]);

// function takes a char string, breaks it apart and tests whether it is valid or not for a subject search
bool validateSubjectID(char[], int &, char &, char[]);

// function checks, whether the correct session input has been inserted
// takes a user value, an array to match the values against and the length of the match value array
// returns a boolean based on whether the string is valid or not
bool sessionCheck(char, char [], int);

// validates a passed string that is used in the editing the values of a student record. can be passed booleans and limit values
// to indicate lengths that need to be met, to include extra punctuation and special characters in a valid string and also numbers
// returns a boolean regarding whether or not the passed value is valid or not and ready to store
bool validateString(char[], int, bool, char[], char[], char[], int, bool, int);

//function handles the changing of a string value, but calls on the validateString function to perform the actual validation checking.
// this function will prompt the user until a valid string is entered based on the specifications that are passed to it
// takes the string to be changed, whether it needs to include numbers, a max length of the string, the amount of numbers allowed, any
//extra punctuation symbols and the number of spaces allowed
void nameChange(char [], bool, char [], char [], int, int, bool, int);

// function checkes whether a number is within a certain range. takes the variable to store the
// valid number in and the number range values. function returns the valid number
int numberChange(int, int, int);
