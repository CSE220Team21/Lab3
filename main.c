//
//	Team 21 (https://github.com/CSE220Team21/Lab3) 
//	Mike Mayr (MMayr15), Charlie Crow (LevenBravo)
//	3/19/14
//	CSE 220: T/Th 10:30am
//
//  main.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include <stdio.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

FILE *init_lister(const char *name, char source_file_name[], char dte[]);
void quit_scanner(FILE *src_file, Token *list);
void add_token_to_list(Token *list, Token *new_token);

int main(int argc, const char * argv[])
{
    Token *token;
    Token *token_list; //This needs to be implemented as a linked list in scanner.h.
    char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file = init_lister(argv[1], source_name, date);
    init_scanner(source_file, source_name, date);
    
    do
    {
        token = get_token();
        add_token_to_list(token_list, token);
        print_token(token);
    }
    while (token != NULL);  //checks token if == to null exits
    // use NULL when reffering to NULL

    
    quit_scanner(source_file, token_list); //exits wile loop
    return 0;
}
void add_token_to_list(Token *list, Token *new_token)
{
    
    //ADD END OF LIST
    /*
    void push(node_t * head, int val) 
    {
    node_t * current = head;
    while (current->next != NULL) 
    {
        current = current->next;
    }

    // now we can add a new variable
    current->next = malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
    } 
    
    recode this to match your inputs
    *list and *new_token
    */
    
    
    
    
    // Add new_token to the list knowing that list is a linked list.
    
}
void quit_scanner(FILE *src_file, Token *list)
{
    //write code to free all of the memory for the token list
    free(*list)
    free(list)
    //free the pointer then free the actual
    //may need to loop through if list is a list of pointers // don't mem leak
     /*while( NULL != it ) 
     {
        node * tmp = it;
        it = it->next;
        free(tmp);
      }
      recode this to match the incoming inputs .... turd
    */
    
    fclose(src_file);
}
FILE *init_lister(const char *name, char source_file_name[], char dte[])
{
    time_t timer;
    FILE *file;
    
    strcpy(source_file_name, name);
    file = fopen(source_file_name, "r");
    time(&timer);
    strcpy(dte, asctime(localtime(&timer)));
    return file;
}
