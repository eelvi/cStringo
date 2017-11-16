/*
    cStringo,  a collection of mostly independant string manipulation
	functions written in c.

    Copyright (C) 2017  github.com/eelvi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// when a string isn't found or a general error occured this value is returned.
#define STR_NPOS -1
    
    
    //Functions that return new malloced strings require you to free the strings after using them.
    //Some functions such as str_repl() require you to make sure you have a large enough buffer.
    


// right ( or reverse ) find, returns the idx of the first char of s1 that's in s2
// before the third argument, if no match is found it returns STR_NPOS (-1)
int rfindin( const char* s1, const char* s2, int before );

//reverse find starting from the end of the string, returns the index or STR_NPOS.
int rfind( const char* s1, const char* s2);

// left (or normal) find, returns the idx of the first char of s1 that's in s2
// before the third argument, if no match is found it returns STR_NPOS (-1)
int lfindin( const char* s1, const char* s2, int before );

//normal find starting from the beginning of the string, returns the index or STR_NPOS.
int lfind( const char* s1, const char* s2 );


//returns a new malloced string starting from the index begin up to and not including the index end.
// returns a pointer to the new string or NULL.
char *strcut(const char* str, int begin, int end);

//copies the string until end-1 then makes null terminates the string at destination
// for example: to copy five from "fivefour" you call string_copy(dest,str,0,4); (the index of the charecter to be null)
// and you need a char[ 4 + 1 ] buffer (end-begin + 1)
int string_copy (char* dest, const char* str, int begin, int end);

//dest has to be as big as src for worst case scenarios and has to be null terminated
// common use case: delimited_section_cpy(dest,src, "\n) or (,,"\r\n")
int delimited_section_cpy (char *dest, const char *src, const char *delim);

//returns the next char after the delimiter, when there isn't any
//it returns returns zero on failure.
int nextlinepos( const char* str, const char* delim);

//returns where the current line ends (when used with delim="\n" it returns the idx of \n or the string length)
//when there are no delimiters found
int lineendpos( const char* str, const char* delim);

//returns occurences of pat in str
//(starting at 0) without considering overlaps, when there is an error it returns STR_NPOS
int occurence_count(const char* str, const char *pat);

//returns occurences of pat in str
//(starting at 1) without considering overlaps, when there is an error it returns STR_NPOS 
int sections_count(const char* str, const char *delim);

//returns a newly malloced string with the content up to the delimter string,
//(can be used to seperate lines for example with delim="\n"), returns NULL on failure
char* section_dup(const char* src, const char* delim);

//erases the charecters in the range, including the charecter at end_idx, guarentees a '\0' at the new string end
//  given the src was a proper string.
// to erase foo from    "bar foo bar" you'd call string_erase(str, 4, 6);
//                      -012345678910"
int string_erase(char *str, int begin_idx, int end_idx);

//this function replaces the string pat with the string repl and returns a new malloced string
//with the resulting replacements, on failure it returns NULL, 
char *string_cpy_repl(const char* src, const char* pat, const char* repl);

// this functions replcaes the string pat with the string repl
// you're responsible for making sure the buffer fits, can return str_npos on failure.
// otherwise it returns the number of replacements that occured.
int str_repl( char* dest, const char* pat, const char* repl);
    
// if strlen(s1) > strlen(s2) when using str_repl function, you are responsible that there is
//  a big enough buffer, you can use this function  to show how many more (or less) 
//  bytes the string will have after replacement
int repl_delta(const char* src, const char* pat, const char* repl);


//erases the white space at the beginning and end of the string, returns the number of
//chars removed.
int str_strip( char* dest );

