#ifndef TPLIB_H_INCLUDED
#define TPLIB_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
typedef int bool;
#define True 1
#define False 0
struct Emp {   //The Structure used for a single Employee Record
    char EmpID[10];
    char Name[30];
    int TNC ;
    struct tm LCD;
    struct tm RWD;
    char History[5][21];

};


struct EmpLL {  //The structure used for a node of a single Employee Record in the Linked List
    struct Emp E ;
    struct EmpLL* Next;
};

typedef struct EmpLL* Emp_ptr ;

typedef struct elements{ //The structure used in the Menu to display the choices
    char message[1024];
    char flichaR[4];
    char flichaL[4];
    }message;
//Declaration of constant string that are the Consultation Reasons
extern const char WD[];
extern const char OD[];
extern const char RWV[];
extern const char PEV[];
extern const char PE[];

struct Cons {  //The structure used for a single Consultation Record
    char EmpID[10];
    char Name[30];
    struct tm ConsTime;
    char ConsReason[21];
    bool Today;
};

struct ConsLL { //The structure used for a node of a single Consultation in the Queue
    struct Cons C;
    struct ConsLL* Next;
};

typedef struct ConsLL* Cons_ptr;

struct ConsQ {  //The structure used for a single Queue
    Cons_ptr Head;
    Cons_ptr Tail;
};

struct ConsTQ {  //The structure used for The priority Queue, it has 4 Sub-Queues
    struct ConsQ WD;
    struct ConsQ OD;
    struct ConsQ RWVPEV;
    struct ConsQ PE;
};

//Allocates memory for a single Employee Node
void Allocate_Emp(Emp_ptr *P);

//Frees memory of a single Employee Node
void Free_Emp(Emp_ptr *P );

//Returns The value in the field "Next" in the Employee Node pointed by P
Emp_ptr Next_Emp(Emp_ptr P);

//Returns The values in the field "E" in the Employee Node pointed by P
struct Emp Inf_Emp(Emp_ptr P);

//Puts the address that is in Q in the field "Next" in the Employee Node pointed by P
void set_Next_Emp(Emp_ptr p , Emp_ptr Q);

//Puts the values that is in info in the field "E" in the Employee Node pointed by P
void set_Emp_info(Emp_ptr P, struct Emp info );

//Adds a Node at the end of the Linked List with the head H, and the values "info" in this Node
void Insert_End(Emp_ptr *H, struct Emp info);

/*Looks for the Node with the value ID in the field "EmpID" in the field "E",
Returns in P the adress of this Node and in Q the adress of the previous Node,
it also Returns in found the value "True" if the Node was found, otherwise "False"*/
void Access_ID(Emp_ptr H ,char ID[9],Emp_ptr *P , Emp_ptr *Q ,bool *Found);

//Looks Looks for the Node with the value ID in the field "EmpID" in the field "E" and removes it from the Linked List withe Head H
void Delete_ID(Emp_ptr *H , char ID[9]);

//Allocates memory for a single Consultation Node
void Allocate_Cons( Cons_ptr *P);

//Returns "True" if the Queue Q is Empty, AKA the field "Head" in Q is NULL
bool EmptyConsQ(struct ConsQ Q);

//Returns the number of Nodes in the Queue Q
int LengthQ(struct ConsQ Q);

//Returns the number of Nodes with the field "Today" in the field "C" is "True"
int LengthTodayQ(struct ConsQ Q);

//Adds a Node at the end of the Queue *Q, with the vaues C in the field "C"
void EnQCons(struct ConsQ *Q, struct Cons C);

//Adds a Node in the corresponding Queue in *TQ, while making sure the Time for all Consultations is set correctly
void EnTQCons (struct ConsTQ *TQ, struct Cons C, int N);

//Removes The Element pointed by the field "Head" from the Queue *Q, while putting the values from the field "C" in *C
void DeQCons(struct ConsQ *Q, struct Cons *C);

//Removes the first element in the priority Queue *Q, while putting the values from the field "C" in *C
void DeTQCons(struct ConsTQ *Q, struct Cons *C);

//Increments the "ConsTime" of each node in the Queue *Q by 45 minutes.
void Inc_time(struct ConsQ *Q);

//Marks the last consultation in the highest-priority non-empty queue as not scheduled for today if the total queue length exceeds 10.
void Reschedule_Last_Cons(struct ConsTQ *Q);

//Frees memory of a single Consultation Node
void Free_Cons(Cons_ptr *P);

//Returns The value in the field "Next" in the Node pointed by P
Cons_ptr Next_Cons(Cons_ptr P);

//Returns The value in the field "C" in the Consultation Node pointed by P
struct Cons Inf_Cons(Cons_ptr P);

//Puts the address that is in Q in the field "Next" in the Consultation Node pointed by P
void set_Next_Cons(Cons_ptr P, Cons_ptr Q);

//Puts the values that is in info in the field "C" in the Consultation Node pointed by P
void set_Cons_info(Cons_ptr P, struct Cons info);

//Adds a Node at the end of the Linked List with the head H, and the values "info" in this Node
void Insert_End_Cons(Cons_ptr *H, struct Cons info);

//Alocates memory for the Priority Queue, and initialize all the pointers to NULL
void MkConsTQ(struct ConsTQ **TQ);

//Returns a string that has in each line the Data from a field of C, Ready to be Displayed
char* ConsToStr(struct Cons C);

//Frees memory allocated for the Employee Linked List and the Consultations Priority Queue
void Free_End(Emp_ptr *Emp, struct ConsTQ *TQ);

//Frees the memory allocated by the array of strings in parsed
void free_csv_line( char **parsed );

//Counts the number of fields in a CSV-like line, handling quoted sections.
static int count_fields( const char *line );

/*Extract a NULL-terminated array of strings from a given string
that has no line breaks or contains line breaks escaped by
double quotes, with each string representing a cell in the row.*/
char **parse_csv( const char *line );

//Returns in *date the date that is in the string *dateStr, using DD/MM/YYYY fomat
void parseDate(const char *dateStr, struct tm *date);

//Returns in the string *dateStr the date that is in *date, using DD/MM/YYYY fomat
void formatDate(const struct tm *date, char *dateStr);

//Returns in *timeStruct the time that is in the string *timeStr, using HH:MM fomat
void parseTime(const char *timeStr, struct tm *timeStruct);

//Returns in the string *timeStr the time that is in *timeStruct, using HH:MM fomat
void formatTime(char *timeStr, const struct tm *timeStruct);

//Reads the Employee Records from the file EmpRecords.txt, and transform them into a Linked List with the Head *H
void ReadEmp(Emp_ptr *H);

//Writes the Employee Records to the file EmpRecords.txt after reading them from the Linked List with the Head H
void WriteEmp(Emp_ptr H);

/**Reads the information from the terminal to be added in the field "E" in the Record pointed by H,
while making sure it has a unique ID comparing to all Nodes in the Linked List W*/
void Read_info(Emp_ptr H, Emp_ptr W);

//Adds a Node to the Linked List with the Head H, after reading it Data from the terminal
void Add_Emp(Emp_ptr *H);

//Allows the user to chose an "Add-able" Consultation Reason and specify the Empolyee to have the Consultaion
void Read_Cons_info(struct Cons *C, Emp_ptr Emp);

//Reads the Consultations Records from the file Consultations.txt, and transform them into a Priority Queue *TQ
void ReadCons (struct ConsTQ *TQ);

//Writes the Consultations Records to the file Consultations.txt after reading them from the Priority Queue Q
void WriteCons(struct ConsTQ Q);

//A function that reads data from Emprecord for a schedueled appointment
void Read_sch_cons(struct Emp E ,const char str[] , struct Cons *C);

//Add a consultation into Consultation.txt without overwriting the content
void Add_Cons_txt(struct Cons C , FILE *file);

//Prepares the Priority Queue to be saved in the file, by adding the Return-to-work visits and Periodic examinations
void End_Day_Update(Emp_ptr E,struct ConsTQ Q ,Emp_ptr *H);

//A function to get the next consultation time
void Add_45(struct tm cons_t, struct tm *Ncons_t);

//A function that reurns the next year of a given date
void Next_Year(struct tm Date , struct tm *Next );

//Convert tm format date into readable format
void Convert_Date(struct tm *Date);

//Returns the date of tomorrow
void Next_Day(struct tm *NextDay);

//Shifts all the Consultation history of an Employee "*E" and adds the Reason str as the most recent
void Update_History(struct Emp *E ,char str[]);

// Moves the console cursor to the specified (x, y) position, used in the user interface
void gotoxy(int x, int y);

//Shows a loading that takes 1.8 seconds, used in the user interface effects
void Loading();

//Shows a loading that takes 0.6 seconds, used in the user interface effects
void QLoading();

//Changes the text color in the console, used in the user interface
void Textecolor(int ForgC);

//Outputs on the screen all the Employee Records that are in the Linked List with the head H
void ShowEmpLL(struct EmpLL* H);

//Outputs on the screen all the Consultation Records that are in the Priority Queue TQ
void ShowConsLL(struct ConsTQ TQ);

//Returns in the string S an "Add-able" Consultation Reason chosen by the user
void HisChoice2(char S[21]);

//Returns in the string S a Consultation Reason chosen by the user
void HisChoice(char S[21]);

/**Allows the user to chose a scheduled Consultation from the Priority Queue TQ,
Returns in *P the address to the Node of this Consultation,
and in Q the adress to the previous Node of this Consultation,
All in an easy to use interface*/
void ChoseConsmenu(struct ConsTQ TQ, Cons_ptr *P, Cons_ptr *Q);

/**Allows the user to chose a Consultation Reason,
to be used in the Employee Record update,
All in an easy to use interface*/
void HisUpdmenu(Emp_ptr P);

//Allows the user to update an Employee Record, All in an easy to use interface
void Update_Emp(Emp_ptr H, char ID[9]);

//Shows a page with the credits and the affeliation
void Fassada();

//Allows the user to use the Consultations Records management Menu, in an easy to use interface
void Consmenu(struct ConsTQ *TQ, Emp_ptr *Emp, int *N);

//Allows the user to use the Employee Records management Menu, in an easy to use interface
void Empmenu(Emp_ptr *Emp);

//An easy to use interface that allows the user to explore and use all the available functionalities
void menu(Emp_ptr *Emp, struct ConsTQ *TQ);


#endif // TPLIB_H_INCLUDED
