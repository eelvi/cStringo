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


#include "cstringo.h"

#define STR_NPOS -1


// right ( or reverse ) find, returns the idx of the first char of s1 that's in s2
// before the third argument, if no match is found it returns STR_NPOS (-1)
int rfindin( const char* s1, const char* s2, int before )
{
        if(!s1 || !s2 )
            return -1;
        int s1len=strlen(s1);
        int s2len=before;
        if(s1len>s2len || !before)
            return STR_NPOS;
        for (int i=s2len-s1len; i>(0-1); i--){
                if (strncmp(s1,s2+i,s1len)==0){
                    return i;
                }
        }
        return STR_NPOS;
}

//reverse find starting from the end of the string, returns the index or STR_NPOS.
int rfind( const char* s1, const char* s2)
{

   return rfindin( s1, s2, strlen(s2) );

}
// left (or normal) find, returns the idx of the first char of s1 that's in s2
// before the third argument, if no match is found it returns STR_NPOS (-1)
int lfindin( const char* s1, const char* s2, int before )
{
        if(!s1 || !s2)
            return -1;
        int s1len=strlen(s1);
        int s2len=before;
        if(s1len>s2len)
            return -1;
        for (int i=0; i<s2len-s1len+1; i++){
                if (strncmp(s1,s2+i,s1len)==0){
                    return i;
                }
        }
        return -1;

}

//normal find starting from the beginning of the string, returns the index or STR_NPOS.
int lfind( const char* s1, const char* s2 )
{
    return lfindin (s1, s2, strlen(s2));
}

//returns a new malloced string starting from the index begin up to and not including the index end.
// returns a pointer to the new string or NULL.
char *strcut(const char* str, int begin, int end)
{
    int len=end-begin+1;
    if (!str || begin<0 || begin>end || strlen(str)<end )
        return NULL;
    char* cutstr = malloc( len*sizeof(char) );
    strncpy(cutstr, str+begin, len );
    cutstr[len-1]='\0';
    return cutstr;

}
//copies the string until end-1 then makes null terminates the string at destination
// for example: to copy five from "fivefour" you call string_copy(dest,str,0,4); (the index of the charecter to be null)
// and you need a char[ 4 + 1 ] buffer (end-begin + 1)
int string_copy (char* dest, const char* str, int begin, int end)
{
    if(!dest || !str || begin>end )
        return 1;
    while ( *str && end-- )
        *(dest++) = *(str++);
    *dest = '\0';
    return 0;
}
//erases the charecters in the range, including the charecter at end_idx, guarentees a '\0' at the new string end
//  given the src was a proper string.
// to erase foo from    "bar foo bar" call string_erase(str, 4, 6);
//                      -012345678910"
int string_erase(char *str, int begin_idx, int end_idx)
{
    if (!str || begin_idx > end_idx )// || strlen(str)<(begin_idx+end_idx)  )
        return -1;
    char* old_beg = str+begin_idx;
    char* new_beg = str+end_idx+1;
    while ( *new_beg ){
        *(old_beg++) = *(new_beg++);
    }
    *old_beg='\0';
    
    return (int) (old_beg-str); //new length
}
//dest has to be as big as src for worst case scenarios and has to be null terminated
int delimited_section_cpy (char *dest, const char *src, const char *delim)
{
    int end=lfind(delim,src);
    if (end<STR_NPOS){
        return 1;
    }
    else if (end==STR_NPOS)
        strcpy(dest,src);
    else{
        if ( string_copy(dest, src, 0, end) != 0 )
            return 1;
    }

    return 0;
}
//rv0: failed rv[n]: n is the index of the first char of next line
//when there isn't any it returns STR_NPOS (-1)
int nextlinepos( const char* str, const char* delim)
{

    int d_idx = 0;
    if (!str || !delim || *str=='\0'  ) 
        return STR_NPOS;

    d_idx= lfind( delim, str );
    if (d_idx == STR_NPOS){
        return strlen(str);
    }

    return d_idx+strlen(delim);

}
//returns where the current line ends (when used with delim="\n" it returns the idx of \n or \0)
int lineendpos( const char* str, const char* delim)
{
    int nextline_idx = nextlinepos(str, delim);
    if (!str[nextline_idx])
        return nextline_idx;
    else
        return nextline_idx-strlen(delim);
}
//returns occurences of pat in str
//(starting at 0) without considering overlaps, when there is an error it returns STR_NPOS
int occurence_count(const char* str, const char *pat)
{
    if (!str || !pat)
        return STR_NPOS;
    int count=0;
    int pat_len = strlen(pat);

    while ( *str  ){
        str+=nextlinepos(str, pat)+pat_len;
        count++;
    }
        
return count;
}

//returns occurences of pat in str
//(starting at 1) without considering overlaps, when there is an error it returns STR_NPOS 
int sections_count(const char* str, const char *delim)
{
    int occ = occurence_count(str, delim)+1;
    occ = (occ>=0) ? occ+1 : occ;
    return occ;
}



//this function replaces the string pat with the string repl and returns a new malloced string
//with the resulting replacements, on failure it returns NULL, 
char *string_cpy_repl(const char* src, const char* pat, const char* repl){
    if (!src || !pat || !repl)
        return NULL;
    int pat_len = strlen(pat);
    int repl_len = strlen(repl);
    int old_len = strlen(src);
    int new_len = old_len+repl_delta(src,pat,repl);
    char *new_string = malloc( new_len );
    
    const char *tmp_src_ptr = src;
    char *tmp_dest_ptr = new_string;
    while ( *tmp_src_ptr ){
        if ( strncmp(tmp_src_ptr,pat,pat_len)==0 ){
            strcpy(tmp_dest_ptr,repl);
            tmp_dest_ptr+=repl_len;
            tmp_src_ptr+=pat_len;
        }
        else{
                *(tmp_dest_ptr++) = *(tmp_src_ptr++);
        }
    }
    new_string[new_len]='\0';
    return new_string;
    
}


// this functions replcaes the string pat with the string repl
// you're responsible for making sure the buffer fits, can return str_npos on failure.
// otherwise it returns the number of replacements that occured.
int str_repl( char* dest, const char* pat, const char* repl){
    int occ = occurence_count(dest, pat);
    char* tmp = string_cpy_repl(dest,pat,repl);
    if (tmp==NULL)
        return -1;
    strcpy(dest,tmp);
    free(tmp);

    return occ;    
} 
// if strlen(s1) > strlen(s2) when using str_repl function, you are responsible that there is
//  a big enough buffer, you can use this function  to show how many more (or less) 
//  bytes the string will have after replacement

int repl_delta(const char* src, const char* pat, const char* repl){
    
    int occ = occurence_count(src, pat);
    int delta = ( occ * strlen(repl) ) - ( occ * strlen(pat) );
    return delta;
}

//returns a newly malloced string with the content up to the delimter string,
//(can be used to seperate lines for example with delim="\n"), returns NULL on failure
char* section_dup(const char* src, const char* delim)
{
    if (!src || !delim || !*src)
        return NULL;
    
    int len = lineendpos(src, delim);
    //printf("%d..\n", len);
    char *buff = malloc(len);
    //if (delimited_section_cpy(buff, src, delim) !=0 )
    if (string_copy(buff, src, 0, len) !=0 )
    {   
        printf("fld");
        free(buff);
        buff=NULL;
    }

    return buff;
}

//erases the white space at the beginning and end of the string, returns the number of
//chars removed.
int str_strip( char* dest ){
    if (!dest)
        return -1;
    
    int len = strlen(dest);
    int begin=0, end=len;
    while ( end>0 && isspace(dest[end-1]) )
        end--;
    if (end != len)
        string_erase(dest, end, len-1);
    
    if ( end!=0 && isspace(dest[begin]) ){
        
        while ( isspace(dest[begin+1]) )
            begin++;
        
        string_erase(dest, 0, begin);   
    
    }
    //removed chars
    return 0+begin+(len-end);
    
}

