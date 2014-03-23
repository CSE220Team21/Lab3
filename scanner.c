/*	Team 21 (https://github.com/CSE220Team21/Lab3)
 	Mike Mayr (MMayr15), Charlie Crow (LevenBravo)
 	18 Mar 2014
 	CSE 220: T,Th 10:30am
 //  scanner.c
 //  Lab3
 //
 //  Created by Bryce Holton.
 //  Copyright (c) 2014 Bryce Holton. All rights reserved. */

#include <stdio.h>
#include "scanner.h"
#include <stdlib.h>

/*******************
 Static functions needed for the scanner
 You need to design the proper para meter list and
 return types for functions with ???.
 ******************/
static char get_char(char* token_ptr, Token* token);
Token* get_token();
static char* skip_comment(char* token_ptr);
static char* skip_blanks(char* token_ptr);
static char* get_word(char* token_ptr, Token* token);
static char* get_number(char* token_ptr, Token* token);
static char* get_string(char* token_ptr, Token* token);
static char* get_special(char* token_ptr, Token* token);
static char* downshift_word(char* token_ptr);
static BOOLEAN is_reserved_word(char* token_ptr, Token* token);
int tolower(int c);
typedef enum 
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE,
} CharCode;

/*********************
 Static Variables for Scanner
 Must be initialized in the init_scanner function.
 *********************/
static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];  // The character table
typedef struct 
{
    char *string;
    TokenCode token_code;
} RwStruct;

const RwStruct rw_table[9][10] = 
{
    {{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,0}}, //Reserved words of size 2
    {{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,0}}, //Reserved words of size 3
    {{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,0}}, //Reserved words of size 4
    {{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,0}},  //Reserved words of size 5
    {{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,0}},  // Reserved words of size 6
    {{"program", PROGRAM},{NULL,0}}, // Reserved words of size 7
    {{"function", FUNCTION},{NULL,0}}, // Reserved words of size 8
    {{"procedure", PROCEDURE},{NULL,0}}  // Reserved words of size 9
};

void init_scanner(FILE *source_file, char source_name[], char date[]) 
{
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);
    int i;
    
    /*******************
     initialize character table, this table is useful for identifying what type of character
     we are looking at by setting our array up to be a copy the ascii table.  Since C thinks of
     a char as like an int you can use ch in get_token as an index into the table.
     *******************/
    for(i = 0; i < MAX_SOURCE_LINE_LENGTH; i++)
	{
/*===========================Letters==================================*/
	if( (i >= 65 && i < 91) || (i >= 97 && i < 123) )
		{
	    char_table[i] = LETTER;
		}
/*===========================Digits==================================*/
	else if(i >= 48 && i < 58) 
		{
	    char_table[i] = DIGIT;
		}
/*===========================Special Char==================================*/
	else
		{
		char_table[i] = SPECIAL;
		}
	}
    char_table[34]=QUOTE;
}

BOOLEAN get_source_line(char source_buffer[]) {
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
	//    char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that?
    static int line_number = 0;
    
    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL) 
	{
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else 
	{
        return (FALSE);
    }
}

Token* get_token() 
{
    char ch; //This can be the current character you are examining during scanning.
    char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
    char *token_ptr = token_string; 		//write some code to point this to the beginning of token_string: IS THIS CORRECT?
	Token *token = NULL;  //I am missing the most important variable in the function, what is it?  Hint: what should I return?
    
    ch = get_char(token_ptr, token); //returns first char after skipping spaces

	//1.  Skip past all of the blanks
    if( strcmp(&ch, " ")) 
	{
	skip_blanks( &ch );
	}
	//2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch
    CharCode c1 = char_table[ ch ];
    if( c1 == LETTER )
	{
        token_ptr = get_word(token_ptr, token); //get_word();
    }
    else if( c1 == DIGIT )
	{
        token_ptr = get_number(token_ptr, token); //get_number();
    }
    else if( c1 == QUOTE )
	{
         token_ptr = get_string(token_ptr, token); //get_string();
    }
    else
	{
	 token_ptr = get_special(token_ptr, token); //get_special();
    }
	//3.  Call the appropriate function to deal with the cases in 2.
    if(strcmp(&ch, "//")||strcmp(&ch, "/*" ))
	{
        skip_comment( &ch ); //skip_comment(); if next char is comment
    }
    return token;
}

static char get_char(char* token_ptr, Token* token) 
{
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
    char ch = *token_ptr;
    if(*token_ptr == '\0' || *token_ptr == '\n')
	{
	    ch='\0';
	}
    if(*token_ptr == EOF)
	{
        return EOF;
	}
    /* Write some code to set the character ch to the next character in the buffer */
        
    token_ptr++;
    return ch;
}
static char* skip_blanks(char* token_ptr) 
{
    while(strcmp(token_ptr, " ") == 0) //while loop for skipping forward if char is a "space"
		{
		token_ptr++;
		}
    return token_ptr;
}
static char* skip_comment(char* token_ptr) 
{
    
	//While the character is not an end brace move the pointer forward
    while(!(strcmp(token_ptr, "}"))) 
	{
		if(!(strcmp(token_ptr, "\0")) )
		{
			token_ptr++;
		}
    }
    return token_ptr;
}
static char* get_word(char* token_ptr, Token* token) 
{
    /* Write some code to Extract the word */
    char* word;
    word=token_ptr;
    strtok(word," ");

    /* Write some code to Check if the word is a reserved word.
	   If it is not a reserved word its an identifier. */
    if(!is_reserved_word(word, token)) {
        token->c1 = IDENTIFIER; //Checks for reserved word, if not -> sets the token code to identifier
    }
    token->t1=REAL_LIT; //Sets literal to real
    return word; //Returns word
}

static char* get_number(char* token_ptr, Token* token) 
{
    //print.c doesn't care what get_number returns, only get_token, an integer is much easier here
    /* Write some code to Extract the number and convert it to a literal number. */
    char *nmbr;
    nmbr= token_ptr;
    strtok(nmbr," ");
    int is_real = 0;
    int i;
    
	// scan through number string for '.' or 'e' which designates "real", not int
    for (i = 0; i < strlen(nmbr); i++) 
	{
	if(strcmp(&nmbr[i], ".") || strcmp(&nmbr[i], "e")) 
		{
		is_real = 1;
	    break;
		}
    }
    if(is_real) 
		{
		token->t1 = REAL_LIT;
		} 
	else 
		{
		token->t1 = INTEGER_LIT;
		}
    return nmbr;
}

static char* get_string(char* token_ptr, Token* token) {
    /* Write some code to Extract the string */
    char *extract_string =  NULL;
    strcpy(extract_string, token_ptr);
    strtok(extract_string, " ");    
    token->c1 = STRING;
    token->t1 = STRING_LIT;
    return extract_string;
}

static char* get_special(char* token_ptr, Token* token) 
{
    char *spcl=token_ptr;
    /* Write some code to Extract the special token.  Most are single-character
       some are double-character.  Set the token appropriately. */
        char ch = *(token->val);
	switch(ch) //creates a token for everything
	{
        case '\'': //runs when an apostrophe is encountered
            token->c1 = STRING;
            token->t1 = STRING_LIT;
            break;
        case '\"': //runs when quotes are encountered
            token->c1 = STRING;
            token->t1 = STRING_LIT;
            break;
        case '-': //runs when dash is encountered
            token->c1 = MINUS;
            token->t1 = REAL_LIT;
            break;
        case ':': //runs when colon is encountered
            if('=' == *((token->val) + 1))
			{
                token->c1 = COLONEQUAL;
                token->t1 = REAL_LIT;
            }
            else
			{
                token->c1 = COLON;
                token->t1 = REAL_LIT;
			}
            break;
        case '+': //runs when addition sign is encountered
            token->c1 = PLUS;
            token->t1 = REAL_LIT;
            break;
        case '=': //runs when equal sign is encountered
            token->c1 = EQUAL;
            token->t1 = REAL_LIT;
            break;
        case '*': //runs when asterisk is encountered
            token->c1 = STAR;
            token->t1 = REAL_LIT;
            break;
        case '(': //runs when open parenthesis is encountered
            token->c1 = LPAREN;
            token->t1 = REAL_LIT;
            break;
        case ')': //runs when closed parenthesis is encountered
            token->c1 = RPAREN;
            token->t1 = REAL_LIT;
            break;
        case '[': //runs when open bracket is encountered
            token->c1 = LBRACKET;
            token->t1 = REAL_LIT;
            break;
        case ']': //runs when closed bracket is encountered
            token->c1 = RBRACKET;
            token->t1 = REAL_LIT;
            break;
        case ';': //runs when semicolon is encountered
            token->c1 = SEMICOLON;
            token->t1 = REAL_LIT;
            break;
        case ',': //runs when comma is encountered
            token->c1 = COMMA;
            token->t1 = REAL_LIT;
            break;
        case '.': //runs when period is encountered
            if('.' == *((token->val) + 1))
			{
                token->c1 = DOTDOT;
                token->t1 = REAL_LIT;
            }
            else
			{
                token->c1 = PERIOD;
                token->t1 = REAL_LIT;
            }
            break;
        case '/': //runs when backslash is encountered
            token->c1 = SLASH;
            token->t1 = REAL_LIT;
            break;
        case '<': //runs when less-than sign is encountered
            if('=' == *((token->val) + 1))
			{
                token->c1 = LE;
                token->t1 = REAL_LIT;
            }
            else
			{
                token->c1 = LT;
                token->t1 = REAL_LIT;
            }
            break;
        case '>': //runs when greater-than sign is encountered
            if('=' == *((token->val) + 1))
			{
                token->c1 = GE;
                token->t1 = REAL_LIT;
            }
            else
			{
                token->c1 = GT;
                token->t1 = REAL_LIT;
            }
            break;
        case '!': //runs when exclamation point is encountered
            if('=' == *((token->val) + 1))
			{
                token->c1 = NE;
                token->t1 = REAL_LIT;
            }
            else
			{
                token->c1 = NOT;
                token->t1 = REAL_LIT;
            }
            break;
        case EOF: //runs when end of file is reached
            token->c1 = END_OF_FILE;
            token->t1 = REAL_LIT;
            break;
        case '&': //runs for ampersand
            token->c1 = AND;
            token->t1 = REAL_LIT;
            break;
        case '|': //runs when vertical line | is encountered
            token->c1 = OR;
            token->t1 = REAL_LIT;
            break;
        case '%': //runs when percent sign % is encountered
            token->c1 = MOD;
            token->t1 = REAL_LIT;
            break;
        case '\0': //runs when null  operator is encountered
            token->c1 = NO_TOKEN;
            token->t1 = REAL_LIT;
            break;
    }
    return spcl;
}

static char* downshift_word(char* token_ptr) 
{
    /* Make all of the characters in the incoming word lower case. */
    char *tmp = token_ptr;
    while(tmp != '\0')
	{
        *tmp = tolower(*tmp);
        tmp++;
	}
    return token_ptr;
}

static BOOLEAN is_reserved_word(char* token_ptr, Token* token) 
{
    int i;
    long j;
    char* word;
    word=token_ptr;
    strtok(word," ");
    downshift_word(word); //makes word lowercase
    j = strlen(word); //counts stringlenght of word
    if(j >= 0 || j < 9)
	{
        for(i = 0; i < 10; i++) 
		{
            if (strcmp(rw_table[j][i].string,word) == 0) 
			{
				token->c1=rw_table[j][i].token_code; //assigns the token_code of the reserved word to the token
				return TRUE;
			}
	    }
	}
    return FALSE;
}
