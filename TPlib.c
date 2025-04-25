#include "TPlib.h"

//Declaration of constant string that are the Consultation Reasons
extern const char WD[]="Work Accident";
extern const char OD[]="Occupational disease";
extern const char RWV[]="Return-to-work visit";
extern const char PEV[]="Pre-employment visit";
extern const char PE[]="Periodic examination";


//Allocates memory for a single Employee Node
void Allocate_Emp(Emp_ptr *P){
    *P = (Emp_ptr)malloc(sizeof(struct EmpLL));
    (*P)->Next = NULL ;
}
/*************************************************************************
*************************************************************************/
//Frees memory of a single Employee Node
void Free_Emp(Emp_ptr *P ){
    free(*P);
    *P = NULL;
}
/*************************************************************************
*************************************************************************/
//Returns The value in the field "Next" in the Employee Node pointed by P
Emp_ptr Next_Emp(Emp_ptr P){
    return P->Next;
}
/*************************************************************************
*************************************************************************/
//Returns The values in the field "E" in the Employee Node pointed by P
struct Emp Inf_Emp(Emp_ptr P){
    return P->E;
}
/*************************************************************************
*************************************************************************/
//Puts the address that is in Q in the field "Next" in the Employee Node pointed by P
void set_Next_Emp(Emp_ptr p , Emp_ptr Q){
    p->Next = Q ;
}
/*************************************************************************
*************************************************************************/
//Puts the values that is in info in the field "E" in the Employee Node pointed by P
void set_Emp_info(Emp_ptr P, struct Emp V ){
    P->E = V ;
}
/*************************************************************************
*************************************************************************/
//Adds a Node at the end of the Linked List with the head H, and the values "info" in this Node
void Insert_End(Emp_ptr *H, struct Emp info) {
    Emp_ptr V ;
    Allocate_Emp(&V);
    set_Emp_info(V,info);
    V->Next = NULL;
    if (*H == NULL){
        *H = V;
    } else {
        Emp_ptr P = *H;
        while (Next_Emp(P) != NULL) {
            P = Next_Emp(P);
        }
        set_Next_Emp(P, V);
    }
}
/*************************************************************************
*************************************************************************/
/*Looks for the Node with the value ID in the field "EmpID" in the field "E",
Returns in P the adress of this Node and in Q the adress of the previous Node,
it also Returns in found the value "True" if the Node was found, otherwise "False"*/
void Access_ID(Emp_ptr H, char ID[9], Emp_ptr *P, Emp_ptr *Q, bool *Found) {
    *P = H;
    *Q = NULL;
    *Found = False;

    while (*P != NULL) {
        if (strcmp(ID, (*P)->E.EmpID) == 0) {
            *Found = True;
            return;
        }
        *Q = *P;
        *P = Next_Emp(*P);
    }

    *P = NULL;
}
/*************************************************************************
*************************************************************************/
//Looks Looks for the Node with the value ID in the field "EmpID" in the field "E" and removes it from the Linked List withe Head H
void Delete_ID(Emp_ptr *H , char ID[9]){
    bool found; Emp_ptr P ; Emp_ptr Q ;
    Access_ID(*H, ID, &P, &Q, &found);
    if (found){
       if (Q == NULL){
        *H = Next_Emp(P);
       }
       else {
        set_Next_Emp(Q, Next_Emp(P));
       }
       Free_Emp(&P);
    }
}

/*************************************************************************
*************************************************************************/
//Allocates memory for a single Consultation Node
void Allocate_Cons( Cons_ptr *P){
    *P = (Cons_ptr)malloc(sizeof(struct ConsLL));
    if (*P==NULL) {
        printf("Error allocating memory");
        return;
    } else{
        (*P)->Next = NULL;
    }
}
/*************************************************************************
*************************************************************************/
//Returns "True" if the Queue Q is Empty, AKA the field "Head" in Q is NULL
bool EmptyConsQ(struct ConsQ Q){
    return (Q.Head==NULL);
}
/*************************************************************************
*************************************************************************/
//Returns the number of Nodes in the Queue Q
int LengthQ(struct ConsQ Q){
    if (!EmptyConsQ(Q)){
        Cons_ptr P;
        int Cpt=0;
        P = Q.Head;
        while (P != NULL){
            P = P->Next;
            Cpt++;
        }
        return Cpt;
    }else{
        return 0;
    }
}
/*************************************************************************
*************************************************************************/
//Returns the number of Nodes with the field "Today" in the field "C" is "True"
int LengthTodayQ(struct ConsQ Q){
    if (!EmptyConsQ(Q)){
        Cons_ptr P;
        int Cpt=0;
        P = Q.Head;
        while (P != NULL){
            if (P->C.Today){
                Cpt++;
            }
            P = P->Next;
        }
        return Cpt;
    }else{
        return 0;
    }
}
/*************************************************************************
*************************************************************************/
//Adds a Node at the end of the Queue *Q, with the vaues C in the field "C"
void EnQCons(struct ConsQ *Q, struct Cons C){
    Cons_ptr P;
    Allocate_Cons(&P);
    P->C = C;
    P->Next = NULL;
    if (!EmptyConsQ(*Q)){
        Cons_ptr F;
        F=Q->Tail;
        F->Next=P;
    }else{
        Q->Head = P;
    }
    Q->Tail = P;
}
/*************************************************************************
*************************************************************************/
//Adds a Node in the corresponding Queue in *TQ, while making sure the Time for all Consultations is set correctly
void EnTQCons(struct ConsTQ *Q, struct Cons C, int N) {
    struct tm tmp;

    int queueIndex = -1;

    if (strcmp(C.ConsReason, WD) == 0) queueIndex = 0;
    else if (strcmp(C.ConsReason, OD) == 0) queueIndex = 1;
    else if (strcmp(C.ConsReason, RWV) == 0 || strcmp(C.ConsReason, PEV) == 0) queueIndex = 2;
    else if (strcmp(C.ConsReason, PE) == 0) queueIndex = 3;

    switch (queueIndex) {
        case 0:
            if (LengthTodayQ(Q->WD) >= N) {
                C.Today = False;
            } else {
                C.Today = True;
                if (!EmptyConsQ(Q->WD)) {
                    Add_45(Q->WD.Tail->C.ConsTime, &tmp);
                } else {
                    tmp.tm_hour = 8;
                    tmp.tm_min = 0;
                }
                C.ConsTime = tmp;
                Inc_time(&(Q->OD));
                Inc_time(&(Q->RWVPEV));
                Inc_time(&(Q->PE));
            }
            EnQCons(&(Q->WD), C);
            Reschedule_Last_Cons(Q);
            break;

        case 1:
            if (LengthTodayQ(Q->WD) + LengthTodayQ(Q->OD) >= N) {
                C.Today = False;
            } else {
                C.Today = True;
                if (!EmptyConsQ(Q->OD)) {
                    Add_45(Q->OD.Tail->C.ConsTime, &tmp);
                } else {
                    Add_45(Q->WD.Tail->C.ConsTime, &tmp);
                }
                C.ConsTime = tmp;
                Inc_time(&(Q->RWVPEV));
                Inc_time(&(Q->PE));
            }
            EnQCons(&(Q->OD), C);
            Reschedule_Last_Cons(Q);
            break;

        case 2:
            if (LengthTodayQ(Q->WD) + LengthTodayQ(Q->OD) + LengthTodayQ(Q->RWVPEV) >= N) {
                C.Today = False;
            } else {
                C.Today = True;
                if (!EmptyConsQ(Q->RWVPEV)) {
                    Add_45(Q->RWVPEV.Tail->C.ConsTime, &tmp);
                } else {
                    Add_45(Q->OD.Tail->C.ConsTime, &tmp);
                }
                C.ConsTime = tmp;
                Inc_time(&(Q->PE));
            }
            EnQCons(&(Q->RWVPEV), C);
            Reschedule_Last_Cons(Q);
            break;

        case 3:
            if (LengthTodayQ(Q->WD) + LengthTodayQ(Q->OD) + LengthTodayQ(Q->RWVPEV) + LengthTodayQ(Q->PE) >= N) {
                C.Today = False;
            } else {
                C.Today = True;
                if (!EmptyConsQ(Q->PE)) {
                    Add_45(Q->PE.Tail->C.ConsTime, &tmp);
                } else {
                    Add_45(Q->RWVPEV.Tail->C.ConsTime, &tmp);
                }
                C.ConsTime = tmp;
            }
            EnQCons(&(Q->PE), C);
            Reschedule_Last_Cons(Q);
            break;

        default:
            printf("Error: Unknown consultation reason: %s\n", C.ConsReason);
            break;
    }
}
/*************************************************************************
*************************************************************************/
//Removes The Element pointed by the field "Head" from the Queue *Q, while putting the values from the field "C" in *C
void DeQCons(struct ConsQ *Q, struct Cons *K){
    Cons_ptr temp;
    Cons_ptr ptemp;
    bool found;
    if (!EmptyConsQ(*Q)){
        temp=Q->Head;
        ptemp=NULL;
        found =False;
        while (!(found) && temp!=NULL){
            if (temp->C.Today){
                found = True;
                break;
            }
            ptemp=temp;
            temp=temp->Next;
        }
        if (temp!=NULL){
            *K=temp->C;
            if (ptemp==NULL){
                Q->Head=temp->Next;
            }else {
                ptemp->Next=temp->Next;
            }
            Free_Cons(&temp);
        }
    }else{
        printf ("%s \n", "Queue is empty");
    }
}
/*************************************************************************
*************************************************************************/
//Removes the first element in the priority Queue *Q, while putting the values from the field "C" in *C
void DeTQCons(struct ConsTQ *Q, struct Cons *C){
    int cpt;
    if (!EmptyConsQ(Q->WD)){
        DeQCons(&(Q->WD), C);
    } else if (!EmptyConsQ(Q->OD)){
        DeQCons(&(Q->OD), C);
    } else if (!EmptyConsQ(Q->RWVPEV)){
        DeQCons(&(Q->RWVPEV), C);
    } else if (!EmptyConsQ(Q->PE)){
        DeQCons(&(Q->PE), C);
    } else {
        printf ("%s \n", "Queue is empty");
    }
}
/*************************************************************************
*************************************************************************/
//Increments the "ConsTime" of each node in the Queue *Q by 45 minutes.
void Inc_time(struct ConsQ *Q){
    Cons_ptr P ;
    P = Q->Head ;
    struct tm tmp ;
    while(P != NULL){
        Add_45(P->C.ConsTime,&tmp);
        P->C.ConsTime = tmp ;
        P = Next_Cons(P);
    }
}
/*************************************************************************
*************************************************************************/
//Marks the last consultation in the highest-priority non-empty queue as not scheduled for today if the total queue length exceeds 10.
void Reschedule_Last_Cons(struct ConsTQ *Q) {
    struct ConsQ *selectedQueue = NULL;
    Cons_ptr last = NULL;
    int totalLength = LengthQ(Q->WD) + LengthQ(Q->OD) + LengthQ(Q->RWVPEV) + LengthQ(Q->PE);
    if (totalLength <= 10) {
        return;
    }
    if (!EmptyConsQ(Q->PE)) {
        selectedQueue = &(Q->PE);
    } else if (!EmptyConsQ(Q->RWVPEV)) {
        selectedQueue = &(Q->RWVPEV);
    } else if (!EmptyConsQ(Q->OD)) {
        selectedQueue = &(Q->OD);
    } else if (!EmptyConsQ(Q->WD)) {
        selectedQueue = &(Q->WD);
    } else {
        return;
    }

    last = selectedQueue->Tail;
    if (last == NULL) {
        return;
    }
    last->C.Today = False;
}
/*************************************************************************
*************************************************************************/
//Frees memory of a single Consultation Node
void Free_Cons(Cons_ptr *P){
    free(*P);
    *P= NULL;
}
/*************************************************************************
*************************************************************************/
//Returns The value in the field "Next" in the Node pointed by P
Cons_ptr Next_Cons(Cons_ptr P){
    return P->Next;
}
/*************************************************************************
*************************************************************************/
//Returns The value in the field "C" in the Consultation Node pointed by P
struct Cons Inf_Cons(Cons_ptr P){
    return P->C;
}
/*************************************************************************
*************************************************************************/
//Puts the address that is in Q in the field "Next" in the Consultation Node pointed by P
void set_Next_Cons(Cons_ptr P, Cons_ptr Q){
    P->Next = Q;
}
/*************************************************************************
*************************************************************************/
//Puts the values that is in info in the field "C" in the Consultation Node pointed by P
void set_Cons_info(Cons_ptr P, struct Cons info){
    P->C = info;
}
/*************************************************************************
*************************************************************************/
//Adds a Node at the end of the Linked List with the head H, and the values "info" in this Node
void Insert_End_Cons(Cons_ptr *H, struct Cons info){
    Cons_ptr V;
}
/*************************************************************************
*************************************************************************/
//Alocates memory for the Priority Queue, and initialize all the pointers to NULL
void MkConsTQ(struct ConsTQ **TQ){
    *TQ = (struct ConsTQ *)malloc(sizeof(struct ConsTQ));
    (*TQ)->WD.Head=NULL;
    (*TQ)->WD.Tail=NULL;
    (*TQ)->OD.Head=NULL;
    (*TQ)->OD.Tail=NULL;
    (*TQ)->RWVPEV.Head=NULL;
    (*TQ)->RWVPEV.Tail=NULL;
    (*TQ)->PE.Head=NULL;
    (*TQ)->PE.Tail=NULL;
}
/*************************************************************************
*************************************************************************/
//Returns a string that has in each line the Data from a field of C, Ready to be Displayed
char* ConsToStr(struct Cons C){
    char *Str = malloc(1024 * sizeof(char));
    if (Str == NULL) {
        printf("Error allocating memory for Str\n");
        return NULL;
    }
    Str[0] = '\0'; // Initialize Str as an empty string
    char temp[10];
    strcat(Str, C.EmpID);
    strcat(Str, "\n");
    strcat(Str, C.Name);
    strcat(Str, "\n");
    formatTime(temp, &(C.ConsTime));
    strcat(Str, temp);
    strcat(Str, "\n");
    strcat(Str, C.ConsReason);
    strcat(Str, "\n");
    strcat(Str, "Today: ");
    strcat(Str, C.Today ? "True" : "False");
    return Str;
}
/*************************************************************************
*************************************************************************/
//Frees memory allocated for the Employee Linked List and the Consultations Priority Queue
void Free_End(Emp_ptr *Emp, struct ConsTQ *TQ){
    Emp_ptr Emptemp;
    Cons_ptr Constemp;
    while((*Emp)!=NULL){
        Emptemp=(*Emp)->Next;
        Free_Emp(Emp);
        *Emp=Emptemp;
    }
    printf("Success\n");
    while (!EmptyConsQ(TQ->WD)){
        Constemp=TQ->WD.Head;
        TQ->WD.Head=TQ->WD.Head->Next;
        Free_Cons(&Constemp);
    }
    printf("Success\n");
    while (!EmptyConsQ(TQ->OD)){
        Constemp=TQ->OD.Head;
        TQ->OD.Head=TQ->OD.Head->Next;
        Free_Cons(&Constemp);
    }
    printf("Success\n");
    while (!EmptyConsQ(TQ->RWVPEV)){
        Constemp=TQ->RWVPEV.Head;
        TQ->RWVPEV.Head=TQ->RWVPEV.Head->Next;
        Free_Cons(&Constemp);
    }
    printf("Success\n");
    while (!EmptyConsQ(TQ->PE)){
        Constemp=TQ->PE.Head;
        TQ->PE.Head=TQ->PE.Head->Next;
        Free_Cons(&Constemp);
    }
}
/*************************************************************************
*************************************************************************/
//Frees the memory allocated by the array of strings in parsed
void free_csv_line( char **parsed ) {
    char **ptr;

    for ( ptr = parsed; *ptr; ptr++ ) {
        free( *ptr );
    }

    free( parsed );
}
/*************************************************************************
*************************************************************************/
//Counts the number of fields in a CSV-like line, handling quoted sections.
static int count_fields( const char *line ) {
    const char *ptr;
    int cnt, fQuote;

    for ( cnt = 1, fQuote = 0, ptr = line; *ptr; ptr++ ) {
        if ( fQuote ) {
            if ( *ptr == '\"' ) {
                fQuote = 0;
            }
            continue;
        }

        switch( *ptr ) {
            case '\"':
                fQuote = 1;
                continue;
            case ',':
                cnt++;
                continue;
            default:
                continue;
        }
    }

    if ( fQuote ) {
        return -1;
    }

    return cnt;
}
/*************************************************************************
*************************************************************************/
/*Extract a NULL-terminated array of strings from a given string
that has no line breaks or contains line breaks escaped by
double quotes, with each string representing a cell in the row.*/
char **parse_csv( const char *line ) {
    char **buf, **bptr, *tmp, *tptr;
    const char *ptr;
    int fieldcnt, fQuote, fEnd;

    fieldcnt = count_fields( line );

    if ( fieldcnt == -1 ) {
        return NULL;
    }

    buf = malloc( sizeof(char*) * (fieldcnt+1) );

    if ( !buf ) {
        return NULL;
    }

    tmp = malloc( strlen(line) + 1 );

    if ( !tmp ) {
        free( buf );
        return NULL;
    }

    bptr = buf;

    for ( ptr = line, fQuote = 0, *tmp = '\0', tptr = tmp, fEnd = 0; ; ptr++ ) {
        if ( fQuote ) {
            if ( !*ptr ) {
                break;
            }

            if ( *ptr == '\"' ) {
                if ( ptr[1] == '\"' ) {
                    *tptr++ = '\"';
                    ptr++;
                    continue;
                }
                fQuote = 0;
            }
            else {
                *tptr++ = *ptr;
            }

            continue;
        }

        switch( *ptr ) {
            case '\"':
                fQuote = 1;
                continue;
            case '\0':
                fEnd = 1;
            case ',':
                *tptr = '\0';
                *bptr = strdup( tmp );

                if ( !*bptr ) {
                    for ( bptr--; bptr >= buf; bptr-- ) {
                        free( *bptr );
                    }
                    free( buf );
                    free( tmp );

                    return NULL;
                }

                bptr++;
                tptr = tmp;

                if ( fEnd ) {
                    break;
                } else {
                    continue;
                }

            default:
                *tptr++ = *ptr;
                continue;
        }

        if ( fEnd ) {
            break;
        }
    }

    *bptr = NULL;
    free( tmp );
    return buf;
}
/*************************************************************************
*************************************************************************/
//Returns in *date the date that is in the string *dateStr, using DD/MM/YYYY fomat
void parseDate(const char *dateStr, struct tm *date) {
    sscanf(dateStr, "%d/%d/%d", &date->tm_mday, &date->tm_mon, &date->tm_year);

    date->tm_mon -= 1;

    date->tm_year -= 1900;
}
/*************************************************************************
*************************************************************************/
//Returns in the string *dateStr the date that is in *date, using DD/MM/YYYY fomat
void formatDate(const struct tm *date, char *dateStr) {
    sprintf(dateStr, "%02d/%02d/%d", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
}
/*************************************************************************
*************************************************************************/
//Returns in *timeStruct the time that is in the string *timeStr, using HH:MM fomat
void parseTime(const char *timeStr, struct tm *timeStruct) {
    sscanf(timeStr, "%2d:%2d", &timeStruct->tm_hour, &timeStruct->tm_min);
}

/*************************************************************************
*************************************************************************/
//Returns in the string *timeStr the time that is in *timeStruct, using HH:MM fomat
void formatTime(char *timeStr, const struct tm *timeStruct) {
    sprintf(timeStr, "%02d:%02d", timeStruct->tm_hour, timeStruct->tm_min);
}
/*************************************************************************
*************************************************************************/
//Reads the Employee Records from the file EmpRecords.txt, and transform them into a Linked List with the Head *H
void ReadEmp(Emp_ptr *H) {
    FILE *Emp = fopen("EmpRecords.txt", "r");
    if (Emp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char Temp[1024];
    Emp_ptr P = NULL;
    Emp_ptr Q = NULL;
    *H = NULL;

    while(fgets(Temp, sizeof(Temp), Emp)) {
        Allocate_Emp(&P);
        if (Q != NULL) {
            Q->Next = P;
        } else {
            *H = P;
        }

        char **Arr = parse_csv(Temp);
        if (Arr == NULL) {
            printf("Error parsing line\n");
            continue;
        }

        strcpy(P->E.EmpID, Arr[0]);
        strcpy(P->E.Name, Arr[1]);
        P->E.TNC = atoi(Arr[2]);
        parseDate(Arr[3], &(P->E.LCD));
        parseDate(Arr[4], &(P->E.RWD));
        strcpy(P->E.History[0], Arr[5]);
        strcpy(P->E.History[1], Arr[6]);
        strcpy(P->E.History[2], Arr[7]);
        strcpy(P->E.History[3], Arr[8]);
        strcpy(P->E.History[4], Arr[9]);
        free_csv_line(Arr);
        Q = P;
    }
    P=Next_Emp(*H);
    free(*H);
    *H = P;
    fclose(Emp);
}
/*************************************************************************
*************************************************************************/
//Writes the Employee Records to the file EmpRecords.txt after reading them from the Linked List with the Head H
void WriteEmp(Emp_ptr H){
    FILE *Emp = fopen("EmpRecords.txt", "w");
    if (Emp == NULL) {
        printf("Error opening file\n");
        return;
    }
    fprintf (Emp, "Employee ID,Name,Total Number of Consultations,Total Number of Consultations,Last consultation Date,Return to work Date,History,History,History,History,History\n" );
    Emp_ptr P;
    Emp_ptr Q;
    P=H;
    char D[12];
    char T[12];
    char TN[4];
    char Temp[1024];
    while (P != NULL){
        Temp[0] = '\0';
        strcat(Temp, P->E.EmpID);
        strcat(Temp, ",");
        strcat(Temp, P->E.Name);
        itoa(P->E.TNC, TN, 10);
        strcat(Temp, ",");
        strcat(Temp, TN);
        strcat(Temp, ",");
        formatDate(&P->E.LCD, D);
        formatDate(&P->E.RWD, T);
        strcat(Temp, D);
        strcat(Temp, ",");
        strcat(Temp, T);
        strcat(Temp, ",");
        strcat(Temp, P->E.History[0]);
        strcat(Temp, ",");
        strcat(Temp, P->E.History[1]);
        strcat(Temp, ",");
        strcat(Temp, P->E.History[2]);
        strcat(Temp, ",");
        strcat(Temp, P->E.History[3]);
        strcat(Temp, ",");
        strcat(Temp, P->E.History[4]);
        fprintf(Emp, Temp);
        P=Next_Emp(P);
    }
    fclose(Emp);
}
/*************************************************************************
*************************************************************************/
/**Reads the information from the terminal to be added in the field "E" in the Record pointed by H,
while making sure it has a unique ID comparing to all Nodes in the Linked List W*/
void Read_info(Emp_ptr H, Emp_ptr W) {
    Textecolor(15);
    printf("**New Employee Record**\n");

    char tempID[20];
    char ID[9];  // Final valid ID
    bool found = True;
    Emp_ptr P, Q;

    // Read and validate ID
    do {
        printf("Employee ID (up to 8 characters): ");
        scanf(" %8s", tempID); // Limit input to 8 characters max
        strncpy(ID, tempID, 8);
        ID[8] = '\0'; // Null-terminate
        Access_ID(W, ID, &P, &Q, &found);
        if (found) {
            printf("This ID is already taken. Please enter a unique ID.\n");
        }
    } while (found);

    strncpy(H->E.EmpID, ID, sizeof(H->E.EmpID) - 1);
    H->E.EmpID[sizeof(H->E.EmpID) - 1] = '\0'; // Ensure null-termination

    // Read Name
    printf("Employee Name: ");
    scanf(" %[^\n]", H->E.Name);

    // Initialize data
    H->E.TNC = 0;
    printf("Enter the last consultation date (DD/MM/YYYY format): ");
    char datestr[12];
    scanf("%s", datestr);
    parseDate(datestr, &H->E.LCD);

    H->E.RWD.tm_year = 0;
    H->E.RWD.tm_mday = 1;
    H->E.RWD.tm_mon = 0;

    for (int i = 0; i < 5; i++) {
        strcpy(H->E.History[i], "None");
    }

    printf("Press ENTER to return to the menu...");
    getchar();
    getchar(); // To consume newline left in the buffer
}
/*************************************************************************
*************************************************************************/
//Adds a Node to the Linked List with the Head H, after reading it Data from the terminal
void Add_Emp(Emp_ptr *H){
    Emp_ptr New_Emp;
    Allocate_Emp(&New_Emp);
    Read_info(New_Emp, *H);
    Insert_End(H, New_Emp->E);
}
/*************************************************************************
*************************************************************************/
//Allows the user to chose an "Add-able" Consultation Reason and specify the Empolyee to have the Consultaion
void Read_Cons_info(struct Cons *C, Emp_ptr Emp){
    Textecolor(15);
    HisChoice2(C->ConsReason);
    if (strcmp(C->ConsReason, PEV)!=0){
        printf("**New Appointement**\n");
        printf("Employee ID: ");
        char temp[20];
        scanf(" %[^\n]", temp);
        bool found= True;
        Emp_ptr P;
        Emp_ptr Q;
        again: while (strlen(temp)>8){
            printf("The ID is too long, \nplease Type in an Employee ID that is up to 8 characters\n");
            scanf(" %[^\n]", temp);
        }
        char ID[9];
        strncpy(ID, temp, 8);
        ID[8] = '\n';
        Access_ID(Emp, temp, &P, &Q, &found);
        while (!found){
            printf("Unfound ID, \nplease Type in an Existing ID\n");
            scanf(" %[^\n]", temp);
            goto again;
        }
        ID[strcspn(ID, "\n")] = '\0';
        snprintf(C->EmpID, sizeof(C->EmpID), "%s", ID);
        strcpy(C->Name, P->E.Name);
    }else{
        strcpy(C->EmpID, "NotYet");
        strcpy(C->Name, "Not Yet");
    }
}
/*************************************************************************
*************************************************************************/
//Reads the Consultations Records from the file Consultations.txt, and transform them into a Priority Queue *TQ
void ReadCons (struct ConsTQ *TQ){
    FILE *Cons = fopen("Consultations.txt", "r" );
    if (Cons == NULL){
        printf("error openning the file");
    }
    else{
        char line[1000];
        fgets(line, sizeof(line), Cons);
        while (fgets(line, sizeof(line), Cons) != NULL) {
            char **cons_info = parse_csv(line);
            if (cons_info == NULL) {
                printf("Error parsing line\n");
                continue;
            }
            Cons_ptr newCons;
            Allocate_Cons(&newCons);
            strcpy(newCons->C.EmpID, cons_info[0]);
            strcpy(newCons->C.Name, cons_info[1]);
            parseTime(cons_info[2], &newCons->C.ConsTime);
            strcpy(newCons->C.ConsReason, cons_info[3]);
            newCons->C.ConsReason[strcspn(newCons->C.ConsReason, "\n")] = '\0';
            newCons->C.Today=False;
            EnTQCons(TQ, newCons->C, 10);
            for (int i = 0; cons_info[i] != NULL; i++) {
                free(cons_info[i]);
            }
            free(cons_info);
        }
    WW: fclose(Cons);
}
}
/*************************************************************************
*************************************************************************/
//Writes the Consultations Records to the file Consultations.txt after reading them from the Priority Queue Q
void WriteCons(struct ConsTQ Q) {

    FILE *file = fopen("Consultations.txt", "w");
    if (file == NULL) {
        printf(" Error opening Consultation.txt for writing!\n");
        return;
    }
    fprintf(file,"Employee ID,Employee Name,Consultation Time,Consultation Reason\n");
    const char *queueNames[] = {"Work Accident", "Occupational Disease", "Return-to-Work / Pre-employment", "Periodic Examination"};

    struct ConsQ queues[] = {Q.WD, Q.OD, Q.RWVPEV, Q.PE};

    for (int i = 0; i < 4; i++) {
        Cons_ptr temp = queues[i].Head;

        while (temp != NULL) {
            char timeStr[10];
            sprintf(timeStr, "%02d:%02d", temp->C.ConsTime.tm_hour, temp->C.ConsTime.tm_min);

            fprintf(file, "%s,%s,%s,%s\n", temp->C.EmpID, temp->C.Name, timeStr, temp->C.ConsReason);

            temp = temp->Next;
        }
    }

    fclose(file);
}
/*************************************************************************
*************************************************************************/
//A function that reads data from Emprecord for a schedueled appointment
void Read_sch_cons(struct Emp E ,const char str[] , struct Cons *C) {
    strcpy(C->EmpID,E.EmpID);
    strcpy(C->Name,E.Name);
    if(strcmp(RWV,str)==0){
      strcpy(C->ConsReason,RWV) ;
    }
    else if(strcmp(PE,str)== 0 ){
        strcpy(C->ConsReason,PE);
    }
    C->Today = FALSE ;
}
/*************************************************************************
*************************************************************************/
//Add a consultation into Consultation.txt without overwriting the content
void Add_Cons_txt(struct Cons C, FILE *file) {
    if (file == NULL) {
        printf(" Error openning file\n");
        return;
    }

    char str[10];
    sprintf(str, "%02dH%02d", C.ConsTime.tm_hour, C.ConsTime.tm_min);

    fprintf(file, "%s,%s,%s,%s\n", C.EmpID, C.Name, str, C.ConsReason);
}
/*************************************************************************
*************************************************************************/
//Prepares the Priority Queue to be saved in the file, by adding the Return-to-work visits and Periodic examinations
void End_Day_Update(Emp_ptr E,struct ConsTQ Q ,Emp_ptr *H){
    WriteEmp(E);
    WriteCons(Q);
    Emp_ptr P = *H ;

    struct tm Cons_Time ;

    Cons_Time.tm_hour = 8 ;

    Cons_Time.tm_min = 30 ;

    struct Cons C ;

     struct tm Tomorrow ;

                Next_Day(&Tomorrow);
             FILE *file = fopen("Consultations.txt", "a");
    if (file == NULL) {
        printf(" Error opening Consultations.txt\n");
        return;
    }
        while(P != NULL){
            if((P->E.RWD.tm_year == Tomorrow.tm_year)&& (P->E.RWD.tm_mon == Tomorrow.tm_mon)&&(P->E.RWD.tm_mday == Tomorrow.tm_mday)){
                Read_sch_cons(P->E , RWV , &C);
                C.ConsTime = Cons_Time ;
                Add_Cons_txt(C,file);
                Add_45(C.ConsTime,&Cons_Time);
            }
             P = Next_Emp(P);
        }
        P = *H ;
        struct tm tmp;
        while(P != NULL){
            Next_Year(P->E.LCD,&tmp);
            if((tmp.tm_year == Tomorrow.tm_year)&&(tmp.tm_mon == Tomorrow.tm_mon)&&(tmp.tm_mday == Tomorrow.tm_mday)){
                Read_sch_cons(P->E , PE , &C);
                C.ConsTime = Cons_Time ;
                Add_Cons_txt(C,file);
                Add_45(C.ConsTime , &Cons_Time);
            }
             P = Next_Emp(P);
        }
        fclose(file);
}

/*************************************************************************
*************************************************************************/
//A function to get the next consultation time
void Add_45(struct tm cons_t, struct tm *Ncons_t) {
    *Ncons_t = cons_t;
    Ncons_t->tm_min += 45;
    if (Ncons_t->tm_min >= 60) {
        Ncons_t->tm_hour += Ncons_t->tm_min / 60;
        Ncons_t->tm_min %= 60;
    }
    if (Ncons_t->tm_hour >= 24) {
        Ncons_t->tm_hour %= 24;
    }
}
/*************************************************************************
*************************************************************************/
//A function that reurns the next year of a given date
void Next_Year(struct tm Date , struct tm *Next ){
    Date.tm_year = Date.tm_year + 1 ;
    int Day = Date.tm_mday ;
    Date.tm_hour = 0;
    Date.tm_min = 0;
    Date.tm_sec = 0;
    mktime(&Date);
    Next->tm_year = Date.tm_year ;
    Next->tm_mday = Day ;
    Next->tm_mon = Date.tm_mon ;
    if((Day == 29 )&& (Next->tm_mday == 28 )){
        Next->tm_mday = 1 ;
        Next->tm_mon = 2 ;
    }
}
/*************************************************************************
*************************************************************************/
//Convert tm format date into readable format
void Convert_Date(struct tm *Date){
    Date->tm_mon = Date->tm_mon + 1  ;
     Date->tm_year = Date->tm_year + 1900;
 }
/*************************************************************************
*************************************************************************/
//Returns the date of tomorrow
void Next_Day(struct tm *NextDay){
    time_t current_day ;
    time(&current_day);
    *NextDay = *localtime(&current_day);
    NextDay->tm_mday ++  ;
    NextDay->tm_hour = 0;
    NextDay->tm_min = 0;
    NextDay->tm_sec = 0;
    mktime(NextDay);
}
/*************************************************************************
*************************************************************************/
//Shifts all the Consultation history of an Employee "*E" and adds the Reason str as the most recent
void Update_History(struct Emp *E ,char str[]){
    int i  ;
    for(i = 4 ; i > 0 ; i--){
       strcpy(E->History[i] , E->History[i-1]);
    }
    strcpy(E->History[0],str);
}
/*************************************************************************
**************************************************************************/
//Moves the console cursor to the specified (x, y) position, used in the user interface
void gotoxy(int x, int y) {
  COORD c;
  c.X = x;
  c.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*************************************************************************
*************************************************************************/
//Shows a loading that takes 1.8 seconds, used in the user interface effects
void Loading()
{
  system("cls");
  Textecolor(0);
  gotoxy(10, 15);
  printf("\t\t\t|||                                                 10%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t|||||                                               20%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t||||||||||||||||                                    30%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t|||||||||||||||||||||||||                           40%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t|||||||||||||||||||||||||||||||                     60%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t||||||||||||||||||||||||||||||||||||||||            80%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t||||||||||||||||||||||||||||||||||||||||||||||||||  100%% ");
  system("cls");
}
/*************************************************************************
*************************************************************************/
//Shows a loading that takes 0.6 seconds, used in the user interface effects
void QLoading()
{
  system("cls");
  Textecolor(0);
  gotoxy(10, 15);
  printf("\t\t\t|||                                                 10%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t||||||||||||||||||||||||||||||                     50%%  ");
  Sleep(300);
  system(";cls");
  gotoxy(10, 15);
  printf("\t\t\t||||||||||||||||||||||||||||||||||||||||||||||||||  100%% ");
  system("cls");
  Textecolor(15);
}
/*************************************************************************
*************************************************************************/
//Changes the text color in the console, used in the user interface
void Textecolor(int ForgC)
{
  WORD wColor;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;


  if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
  {
    wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
    SetConsoleTextAttribute(hStdOut, wColor);
  }
  return;
}
/*************************************************************************
*************************************************************************/
//Outputs on the screen all the Employee Records that are in the Linked List with the head H
void ShowEmpLL(struct EmpLL* H){
    Emp_ptr P;
    P=H;
    while(P != NULL) {
        printf("Employee ID: %s\n", P->E.EmpID);
        printf("%s\n", P->E.Name);
        printf("Total Number of Consultations: %d\n", P->E.TNC);
        printf("Last Consultation Date: %02d/%02d/%d\n", P->E.LCD.tm_mday, P->E.LCD.tm_mon+1, P->E.LCD.tm_year+1900);
        if (P->E.RWD.tm_year!=0){
        printf("Return To Work Date: %02d/%02d/%d\n", P->E.RWD.tm_mday, P->E.RWD.tm_mon+1, P->E.RWD.tm_year+1900);}
        printf("History:\n");
        for (int i = 0; i < 5; i++) {
            printf("%s\n", P->E.History[i]);
        }
        printf("*****************************\n");
        P = P->Next;
    }
}
/*************************************************************************
*************************************************************************/
//Outputs on the screen all the Consultation Records that are in the Priority Queue TQ
void ShowConsLL(struct ConsTQ TQ){
    Cons_ptr P;
    char temp[7];
    P = TQ.WD.Head;
    Textecolor(1);
    printf("Today:\n\n");
    Textecolor(15);
    while (P != NULL){
        if (P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            formatTime(temp ,&(P->C.ConsTime));
            printf("%s\n", temp);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.OD.Head;
    while (P != NULL){
        if (P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            formatTime(temp ,&(P->C.ConsTime));
            printf("%s\n", temp);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.RWVPEV.Head;
    while (P != NULL){
        if (P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            formatTime(temp ,&(P->C.ConsTime));
            printf("%s\n", temp);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.PE.Head;
    while (P != NULL){
        if (P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            formatTime(temp ,&(P->C.ConsTime));
            printf("%s\n", temp);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.WD.Head;
    Textecolor(1);
    printf("Tomorrow:\n\n");
    Textecolor(15);
    while (P != NULL){
        if (!P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.OD.Head;
    while (P != NULL){
        if (!P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.RWVPEV.Head;
    while (P != NULL){
        if (!P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }
    P = TQ.PE.Head;
    while (P != NULL){
        if (!P->C.Today){
            printf ("%s\n", P->C.EmpID);
            printf("%s\n",P->C.Name);
            printf("%s\n", P->C.ConsReason);
            printf("******************\n");
        }
        P = Next_Cons(P);
    }

}
/*************************************************************************
*************************************************************************/
//Returns in the string S an "Add-able" Consultation Reason chosen by the user
void HisChoice2(char S[21]) {
    system("cls");
   message tab[15];
   strcpy(tab[0].message,PEV);
   strcpy(tab[0].flichaR,"->");
   strcpy(tab[0].flichaL,"<-");
   strcpy(tab[1].message,OD);
   strcpy(tab[1].flichaR,"");
   strcpy(tab[1].flichaL,"");
   strcpy(tab[2].message,WD);
   strcpy(tab[2].flichaR,"");
   strcpy(tab[2].flichaL,"");
   int i;
   int c;
   int choix=0;
   la:
   Textecolor(1);
   printf("\n\n\n");
   printf("\t\t\t\t               Choose Consultaion Reason               \t\t\t\t\n");
   printf("\t\t\t\t                        ------                         \t\t\t\t\n");
   printf("\t\t\t\t*******************************************************\t\t\t\t\n");
   printf("\t\t\t\t                                                       \t\t\t\t\n");
   for (i=0;i<3;i++)
   {
   printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
   printf("\t\t\t\t\t\t\n");
   }
   printf("\t\t\t\t                                                               \t\t\t\t\n");
   printf("\t\t\t\t*******************************************************\t\t\t\t\n");

   c=getch();
   switch(c){
   case 72:
       strcpy(tab[choix].flichaR,"");
       strcpy(tab[choix].flichaL,"");
       if (choix==0) choix=2;
       else choix--;
       strcpy(tab[choix].flichaR,"->");
       strcpy(tab[choix].flichaL,"<-");
       break;
   case 80:
       strcpy(tab[choix].flichaR,"");
       strcpy(tab[choix].flichaL,"");
       if (choix==2) choix=0;
       else choix++;
       strcpy(tab[choix].flichaR,"->");
       strcpy(tab[choix].flichaL,"<-");
       break;
   case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
       c=c-49;
       goto end;
   case 13:
       c=choix;
       goto end;
   default: break;
   }
   system("cls");
   goto la;
   end:
   QLoading();
   switch(c) {
       case 0:
            strcpy(S, PEV);
           break;
       case 1:
            strcpy(S, OD);
           break;
       case 2:
            strcpy(S, WD);
           break;
   }
}
/*************************************************************************
*************************************************************************/
//Returns in the string S a Consultation Reason chosen by the user
void HisChoice(char S[21]) {
    system("cls");
   message tab[15];
   strcpy(tab[0].message,PE);
   strcpy(tab[0].flichaR,"->");
   strcpy(tab[0].flichaL,"<-");
   strcpy(tab[1].message,PEV);
   strcpy(tab[1].flichaR,"");
   strcpy(tab[1].flichaL,"");
   strcpy(tab[2].message,RWV);
   strcpy(tab[2].flichaR,"");
   strcpy(tab[2].flichaL,"");
   strcpy(tab[3].message,OD);
   strcpy(tab[3].flichaR,"");
   strcpy(tab[3].flichaL,"");
   strcpy(tab[4].message,WD);
   strcpy(tab[4].flichaR,"");
   strcpy(tab[4].flichaL,"");
   int i;
   int c;
   int choix=0;
   la:
   Textecolor(1);
   printf("\n\n\n");
   printf("\t\t\t\t       Choose Employee Consultaion History Update       \t\t\t\t\n");
   printf("\t\t\t\t                        ------                         \t\t\t\t\n");
   printf("\t\t\t\t*******************************************************\t\t\t\t\n");
   printf("\t\t\t\t                                                       \t\t\t\t\n");
   printf("\t\t\t\t         Consultation History:                    \t\t\t\t\n");
   printf("\t\t\t\t                                                       \t\t\t\t\n");
   for (i=0;i<6;i++)
   {
   printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
   printf("\t\t\t\t\t\t\n");
   }
   printf("\t\t\t\t                                                               \t\t\t\t\n");
   printf("\t\t\t\t*******************************************************\t\t\t\t\n");

   c=getch();
   switch(c){
   case 72:
       strcpy(tab[choix].flichaR,"");
       strcpy(tab[choix].flichaL,"");
       if (choix==0) choix=4;
       else choix--;
       strcpy(tab[choix].flichaR,"->");
       strcpy(tab[choix].flichaL,"<-");
       break;
   case 80:
       strcpy(tab[choix].flichaR,"");
       strcpy(tab[choix].flichaL,"");
       if (choix==4) choix=0;
       else choix++;
       strcpy(tab[choix].flichaR,"->");
       strcpy(tab[choix].flichaL,"<-");
       break;
   case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
       c=c-49;
       goto end;
   case 13:
       c=choix;
       goto end;
   default: break;
   }
   system("cls");
   goto la;
   end:
   QLoading();
   switch(c) {
       case 0:
            strcpy(S, PE);
           break;
       case 1:
            strcpy(S, PEV);
           break;
       case 2:
            strcpy(S, RWV);
           break;
       case 3:
            strcpy(S, OD);
           break;
       case 4:
            strcpy(S, WD);
           break;
   }
}

/*************************************************************************
*************************************************************************/
/**Allows the user to chose a scheduled Consultation from the Priority Queue TQ,
Returns in *P the address to the Node of this Consultation,
and in Q the adress to the previous Node of this Consultation,
All in an easy to use interface*/
void ChoseConsmenu(struct ConsTQ TQ, Cons_ptr *P, Cons_ptr *Q) {
    system("cls");
    if (!EmptyConsQ(TQ.WD) || !EmptyConsQ(TQ.OD) || !EmptyConsQ(TQ.RWVPEV) || !EmptyConsQ(TQ.PE)){
        int TL=LengthQ(TQ.WD) + LengthQ(TQ.OD) + LengthQ(TQ.RWVPEV) + LengthQ(TQ.PE);
        message tab[TL+2];
        Cons_ptr PT;
        PT =TQ.WD.Head;
        int Waw=1;
        while (PT==NULL){
            switch (Waw){
                case 1:
                    PT=TQ.OD.Head;
                    break;
                case 2:
                PT=TQ.RWVPEV.Head;
                    break;
                case 3:
                PT=TQ.PE.Head;
                    break;
            }
            Waw++;
        }
        int cpt=2;
        strcpy(tab[1].message, ConsToStr(PT->C) );
        strcpy(tab[1].flichaR,"->");
        strcpy(tab[1].flichaL,"<-");
        Waw=1;
        getthem: PT=PT->Next;
        while (PT==NULL){
            switch (Waw){
                case 1:
                    PT=TQ.OD.Head;
                    break;
                case 2:
                PT=TQ.RWVPEV.Head;
                    break;
                case 3:
                PT=TQ.PE.Head;
                    break;
                case 4:
                    goto prela;
                    break;
            }
            Waw++;
        }
        strcpy(tab[cpt].message, ConsToStr(PT->C));
        strcpy(tab[cpt].flichaR,"");
        strcpy(tab[cpt].flichaL,"");
        cpt++;
        goto getthem;
        prela: strcpy(tab[0].message,"Next Page");
        strcpy(tab[0].flichaR,"");
        strcpy(tab[0].flichaL,"");
        int i;
        int c;
        cpt=1;
        int choix=1;
        la:
        Textecolor(1);
        printf("\n\n\n");
        printf("     Choose a Consultation                \t\t\t\t\n");
        printf("           ------                         \t\t\t\t\n");
        printf("*****************************\t\t\t\t\n");
        printf("\t\t\t\t                                                       \t\t\t\t\n");
        if ((TL-cpt)>3) {
            for (i=cpt;i<cpt+4;i++)
            {
                printf("%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
                printf("\n*****************************\n");
            }
            printf("\t\t\t\t                                                               \t\t\t\t\n");
            printf("%s %s %s",tab[0].flichaR,tab[0].message,tab[0].flichaL);
            printf("\n*****************************\n");
            c=getch();
            switch(c){
            case 72:
                strcpy(tab[choix].flichaR,"");
                strcpy(tab[choix].flichaL,"");
                if (choix==0) choix=cpt+3;
                else if (choix==cpt) choix = 0;
                else choix--;
                strcpy(tab[choix].flichaR,"->");
                strcpy(tab[choix].flichaL,"<-");
                break;
            case 80:
                strcpy(tab[choix].flichaR,"");
                strcpy(tab[choix].flichaL,"");
                if (choix==cpt+3) choix=0;
                else if (choix==0) choix = cpt;
                else choix++;
                strcpy(tab[choix].flichaR,"->");
                strcpy(tab[choix].flichaL,"<-");
                break;
            case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
                c=c-49;
                goto end;
            case 13:
                c=choix;
                goto end;
            default: break;
            }
        }else{
            for (i=cpt;i<=TL;i++)
            {
                printf("%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
                printf("\n*****************************\n");
            }
            c=getch();
            switch(c){
            case 72:
                strcpy(tab[choix].flichaR,"");
                strcpy(tab[choix].flichaL,"");
                if (choix==cpt) choix=TL;
                else choix--;
                strcpy(tab[choix].flichaR,"->");
                strcpy(tab[choix].flichaL,"<-");
                break;
            case 80:
                strcpy(tab[choix].flichaR,"");
                strcpy(tab[choix].flichaL,"");
                if (choix==TL) choix=cpt;
                else choix++;
                strcpy(tab[choix].flichaR,"->");
                strcpy(tab[choix].flichaL,"<-");
                break;
            case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
                c=c-49;
                goto end;
            case 13:
                c=choix % 4;
                goto end;
            default: break;
            }
        }

        system("cls");
        goto la;
        end:
        QLoading();
        int NT;
        switch(c) {
            case 0:
                strcpy(tab[choix].flichaR,"");
                strcpy(tab[choix].flichaL,"");
                cpt = cpt + 4;
                choix = cpt;
                strcpy(tab[choix].flichaR,"->");
                strcpy(tab[choix].flichaL,"<-");
                goto la;
                break;
            case 1:
                NT=cpt;
                *P=TQ.WD.Head;
                *Q=NULL;
                int Waw=1;
                for (i=1; i<=NT-1; i++){
                    while (*P==NULL){
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                    *Q=*P;
                    *P=(*P)->Next;
                    while (*P==NULL){
                        *Q=NULL;
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                }
                break;
            case 2:
                NT=cpt+1;
                printf("Success1\n");
                *P=TQ.WD.Head;
                *Q=NULL;
                Waw=1;
                printf("Success2\n");
                for (i=1; i<=NT-1; i++){
                    while (*P==NULL){
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                    *Q=*P;
                    *P=(*P)->Next;
                    while (*P==NULL){
                        *Q=NULL;
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                }
                break;
            case 3:
                NT=cpt+2;
                *P=TQ.WD.Head;
                *Q=NULL;
                Waw=1;
                for (i=1; i<=NT-1; i++){
                    while (*P==NULL){
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                    *Q=*P;
                    *P=(*P)->Next;
                    while (*P==NULL){
                        *Q=NULL;
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                }
                break;
            case 4:
                NT=cpt+3;
                *P=TQ.WD.Head;
                *Q=NULL;
                Waw=1;
                for (i=1; i<=NT-1; i++){
                    while (*P==NULL){
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                    *Q=*P;
                    *P=(*P)->Next;
                    while (*P==NULL){
                        *Q=NULL;
                        switch (Waw){
                            case 1:
                                *P=TQ.OD.Head;
                                break;
                            case 2:
                                *P=TQ.RWVPEV.Head;
                                break;
                            case 3:
                                *P=TQ.PE.Head;
                                break;
                        }
                        Waw++;
                    }
                }
                break;
   }}else{
    printf("No Consultations yet");
   }
}


/*************************************************************************
*************************************************************************/
/**Allows the user to chose a Consultation Reason,
to be used in the Employee Record update,
All in an easy to use interface*/
void HisUpdmenu(Emp_ptr P) {
    system("cls");
   message tab[15];
   strcpy(tab[0].message,P->E.History[0]);
   strcpy(tab[0].flichaR,"->");
   strcpy(tab[0].flichaL,"<-");
   strcpy(tab[1].message,P->E.History[1]);
   strcpy(tab[1].flichaR,"");
   strcpy(tab[1].flichaL,"");
   strcpy(tab[2].message,P->E.History[2]);
   strcpy(tab[2].flichaR,"");
   strcpy(tab[2].flichaL,"");
   strcpy(tab[3].message,P->E.History[3]);
   strcpy(tab[3].flichaR,"");
   strcpy(tab[3].flichaL,"");
   strcpy(tab[4].message,P->E.History[4]);
   strcpy(tab[4].flichaR,"");
   strcpy(tab[4].flichaL,"");
   strcpy(tab[5].message,"     Exit Update     ");
   strcpy(tab[5].flichaR,"");
   strcpy(tab[5].flichaL,"");
   int i;
   int c;
   int choix=0;
   la:
   Textecolor(1);
   printf("\n\n\n");
   printf("\t\t\t\t          Employee Consultaion History Update          \t\t\t\t\n");
   printf("\t\t\t\t                        ------                         \t\t\t\t\n");
   printf("\t\t\t\t     Updating Employee:         %s            \t\t\t\t\n", P->E.Name );
   printf("\t\t\t\t*******************************************************\t\t\t\t\n");
   printf("\t\t\t\t                                                       \t\t\t\t\n");
   printf("\t\t\t\t         Consultation History:                    \t\t\t\t\n");
   for (i=0;i<6;i++)
   {
   printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
   printf("\t\t\t\t\t\t\n");
   }
   printf("\t\t\t\t                                                               \t\t\t\t\n");
   printf("\t\t\t\t*******************************************************\t\t\t\t\n");

   c=getch();
   switch(c){
   case 72:
       strcpy(tab[choix].flichaR,"");
       strcpy(tab[choix].flichaL,"");
       if (choix==0) choix=5;
       else choix--;
       strcpy(tab[choix].flichaR,"->");
       strcpy(tab[choix].flichaL,"<-");
       break;
   case 80:
       strcpy(tab[choix].flichaR,"");
       strcpy(tab[choix].flichaL,"");
       if (choix==5) choix=0;
       else choix++;
       strcpy(tab[choix].flichaR,"->");
       strcpy(tab[choix].flichaL,"<-");
       break;
   case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
       c=c-49;
       goto end;
   case 13:
       c=choix;
       goto end;
   default: break;
   }
   system("cls");
   goto la;
   end:
   QLoading();
   switch(c) {
    case 0:
        HisChoice(P->E.History[0]);
        break;
    case 1:
        HisChoice(P->E.History[1]);
    break;
    case 2:
        HisChoice(P->E.History[2]);
    break;
    case 3:
        HisChoice(P->E.History[3]);
    break;
    case 4:
        HisChoice(P->E.History[4]);
    break;
    case 5:
        break;
   }
}

/*************************************************************************
*************************************************************************/
//Allows the user to update an Employee Record, All in an easy to use interface
void Update_Emp(Emp_ptr H, char ID[9]) {
    bool found;
    Emp_ptr P;
    Emp_ptr Q;
    Access_ID(H, ID, &P, &Q, &found);
    printf("done");
    if (!found) {
        printf("Employee with ID %s not found.\n", ID);
        return;
    }
    system("cls");
    message tab[15];
    strcpy(tab[0].message,"         Change Name         ");
    strcpy(tab[0].flichaR,"->");
    strcpy(tab[0].flichaL,"<-");
    strcpy(tab[1].message,"  Update Total Consultations ");
    strcpy(tab[1].flichaR,"");
    strcpy(tab[1].flichaL,"");
    strcpy(tab[2].message,"Modify Last Consultation Date");
    strcpy(tab[2].flichaR,"");
    strcpy(tab[2].flichaL,"");
    strcpy(tab[3].message,"  Update Return-to-Work Date ");
    strcpy(tab[3].flichaR,"");
    strcpy(tab[3].flichaL,"");
    strcpy(tab[4].message,"     Edit History Records    ");
    strcpy(tab[4].flichaR,"");
    strcpy(tab[4].flichaL,"");
    strcpy(tab[5].message,"         Exit Update         ");
    strcpy(tab[5].flichaR,"");
    strcpy(tab[5].flichaL,"");
    int i;
    int c;
    int choix=0;
    la:
    Textecolor(1);
    printf("\n\n\n");
    printf("\t\t\t\t                    Employee Record Update                     \t\t\t\t\n");
    printf("\t\t\t\t                            ------                             \t\t\t\t\n");
    printf("\t\t\t\t**Updating Employee:         %s               **\t\t\t\t\n", P->E.Name );
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    for (i=0;i<6;i++)
    {
    printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
    printf("\t\t\t\t\t\t\n");
    }
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");

    c=getch();
    switch(c){
    case 72:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==0) choix=5;
        else choix--;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 80:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==5) choix=0;
        else choix++;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
        c=c-49;
        goto end;
    case 13:
        c=choix;
        goto end;
    default: break;
    }
    system("cls");
    goto la;
    end:
    QLoading();
    switch(c) {
        case 0:
            printf("Enter new name: ");
            scanf(" %[^\n]", &P->E.Name);
            break;
        case 1:
            printf("Enter new total number of consultations: ");
            scanf("%d", &P->E.TNC);
            break;
        case 2:
            printf("Enter new last consultation date (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &P->E.LCD.tm_mday, &P->E.LCD.tm_mon, &P->E.LCD.tm_year);
            P->E.LCD.tm_mon -= 1;
            P->E.LCD.tm_year -= 1900;
            break;
        case 3:
            printf("Enter new return-to-work date (DD/MM/YYYY) or 0 if none: ");
            int day, month, year;
            scanf("%d", &day);
            if (day != 0) {
                scanf("/%d/%d", &month, &year);
                P->E.RWD.tm_mday = day;
                P->E.RWD.tm_mon = month - 1;
                P->E.RWD.tm_year = year - 1900;
            } else {
                P->E.RWD.tm_mday = 0;
            }
            break;
        case 4:
            HisUpdmenu(P);
            break;
        case 5:
            break;
    }
 }
/*************************************************************************
*************************************************************************/
//Shows a page with the credits and the affeliation
void Fassada(){
    system("cls");
    Textecolor(1);
    printf("\t\t\t\t%ccole nationale sup%crieur d'informatique \n",130,130);
    Textecolor(1);
    printf("\t\t\t  .----------------.  .----------------.  .----------------.\n");
    printf("\t\t\t | .--------------. || .--------------. || .--------------. |\n");
    printf("\t\t\t | |  _________   | || |    _______   | || |     _____    | |\n");
    printf("\t\t\t | | |_   ___  |  | || |   /  ___  |  | || |    |_   _|   | |\n");
    printf("\t\t\t | |   | |_  \\_|  | || |  |  (__ \\_|  | || |      | |     | |\n");
    printf("\t\t\t | |   |  _|  _   | || |   \'.___`-.   | || |      | |     | |\n");
    printf("\t\t\t | |  _| |___/ |  | || |  |`\\____) |  | || |     _| |_    | |\n");
    printf("\t\t\t | | |_________|  | || |  |_______.\'  | || |    |_____|   | |\n");
    printf("\t\t\t | |              | || |              | || |              | |\n");
    printf("\t\t\t | \'--------------\' || \'--------------\' || \'--------------\' |\n");
    printf("\t\t\t  \'----------------\'  \'----------------\'  \'----------------\'\n");
    Sleep(200);
    Textecolor(10);
    printf("\t\t\t\tCPI - 1%cre Ann%ce - Ann%ce Universitaire 2024/2025 \n", 138, 130, 130);
    printf("\t\t\t ____________________________________________________________\n");
    printf("\t\t\t|                                                            |\n");
    printf("\t\t\t| Designed & Coded By : AMARA KHALED WALID                   |\n");
    printf("\t\t\t|                                  &                         |\n");
    printf("\t\t\t|                             AZZOUZ ZAKI                    |\n");
    printf("\t\t\t|                                                            |\n");
    printf("\t\t\t|                                                            |\n");
    printf("\t\t\t|         \tSECTION : C           GROUPE: 12             |\n");
    printf("\t\t\t|                      \t  TP1 ALSDD                          |\n");
    printf("\t\t\t|      \tOccupational Health Medical Visit Management         |\n");
    printf("\t\t\t|                                                            |\n");
    printf("\t\t\t|                                                            |\n");
    printf("\t\t\t| Supervised by: Ms. CHADER ASMA                             |\n");
    printf("\t\t\t|____________________________________________________________|\n\n\n");
    Textecolor(20);
    printf("\t\t\t _____________________________________________________________\n");
    printf("\t\t\t| Note!! Press the 'ENTER' key to access the                  |\n");
    printf("\t\t\t| You can switch between options using the Up and Down keys.  |\n");
    printf("\t\t\t|____________________________________________________________ |\n");
    getchar();
}
/*************************************************************************
*************************************************************************/
//Allows the user to use the Consultations Records management Menu, in an easy to use interface
void Consmenu(struct ConsTQ *TQ, Emp_ptr *Emp, int *N){
    start: system("cls");

    message tab[15];
    strcpy(tab[0].message,"     Add a new Appointement     ");
    strcpy(tab[0].flichaR,"->");
    strcpy(tab[0].flichaL,"<-");
    strcpy(tab[1].message,"   Reschedule an Appointement   ");
    strcpy(tab[1].flichaR,"");
    strcpy(tab[1].flichaL,"");
    strcpy(tab[2].message,"   Close The Next Appointement  ");
    strcpy(tab[2].flichaR,"");
    strcpy(tab[2].flichaL,"");
    strcpy(tab[3].message,"     Cancel an Appointement     ");
    strcpy(tab[3].flichaR,"");
    strcpy(tab[3].flichaL,"");
    strcpy(tab[4].message,"       Retrun To main menu      ");
    strcpy(tab[4].flichaR,"");
    strcpy(tab[4].flichaL,"");
    int i;
    int c;
    int choix=0;
    la:
    Textecolor(1);
    printf("\n\n\n");
    printf("\t\t\t\t                 Appointement Record Management                \t\t\t\t\n");
    printf("\t\t\t\t                            ------                             \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    for (i=0;i<5;i++)
    {
    printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
    printf("\t\t\t\t\t\t\n");
    }
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");

    c=getch();
    switch(c){
    case 72:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==0) choix=4;
        else choix--;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 80:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==4) choix=0;
        else choix++;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
        c=c-49;
        goto end;
    case 13:
        c=choix;
        goto end;
    default: break;
    }
    system("cls");
    goto la;
    end:
    QLoading();
    char tempID[20];
    char ID[9];
    Cons_ptr P;
    Cons_ptr Q;
    Emp_ptr PED;
    Emp_ptr QE;
    bool found;
    bool gethem;
    struct Cons K;
    int cpt;
    int A7sseb=0;
    Textecolor(15);
    switch(c) {
        case 0:
            Read_Cons_info(&K, *Emp);
            EnTQCons(TQ, K, *N);
            if (strcmp(K.ConsReason, WD)==0){
                cpt=2;
                P=TQ->OD.Head;
                Q=TQ->WD.Tail;
            }else if (strcmp(K.ConsReason, OD)==0){
                cpt=3;
                Q=TQ->OD.Tail;
                P=TQ->RWVPEV.Head;
            }else if (strcmp(K.ConsReason, RWV)==0 || strcmp(K.ConsReason, PEV)==0){
                cpt=4;
                P=TQ->PE.Head;
                Q=TQ->RWVPEV.Tail;
            }else if (strcmp(K.ConsReason, PE)==0){
                cpt=5;
            }
            if (Q->C.Today) {
                while (P != NULL || cpt != 5){
                    A7sseb++;
                    if (P==NULL){
                        switch (cpt){
                            case 1:
                            P=TQ->OD.Head;
                            cpt=2;
                            break;
                            case 2:
                            P=TQ->RWVPEV.Head;
                            cpt=3;
                            break;
                            case 3:
                            P=TQ->PE.Head;
                            cpt=4;
                            break;
                        }
                    }
                    if (!P->C.Today && A7sseb!=1){
                        Q->C.Today=False;
                        break;
                    }
                    Q=P;
                    P=P->Next;
                }
            }
            printf("Appointement Added Successfully.\n");
            printf("Press a Key to return...");
            getchar();
            getchar();
            break;
        case 1:
            ChoseConsmenu(*TQ, &P, &Q);
            Q=P;
            if (P->C.Today){
                if (strcmp(P->C.ConsReason, WD)==0){
                    cpt=1;
                }else if (strcmp(P->C.ConsReason, OD)==0){
                    cpt=2;
                }else if (strcmp(P->C.ConsReason, RWV)==0 || strcmp(P->C.ConsReason, PEV)==0){
                    cpt=3;
                }else if (strcmp(P->C.ConsReason, PE)==0){
                    cpt=4;
                }
                while (P != NULL || cpt != 4){
                    if (P==NULL){
                        switch (cpt){
                            case 1:
                            P=TQ->OD.Head;
                            cpt=2;
                            break;
                            case 2:
                            P=TQ->RWVPEV.Head;
                            cpt=3;
                            break;
                            case 3:
                            P=TQ->PE.Head;
                            cpt=4;
                            break;
                        }
                    }
                    if (!P->C.Today){
                        P->C.Today=True;
                        break;
                    }
                    P=P->Next;
                }
            }
            Q->C.Today=False;
            break;
        case 2:
            if (*N>0){
                DeTQCons(TQ, &K);
                printf("Closed Appointement: \n");
                printf("%s\n", ConsToStr(K));

                if (strcmp(K.ConsReason, PEV)==0){
                    Add_Emp(Emp);
                    QE=*Emp;
                    PED=NULL;
                    while (QE != NULL){
                        PED=QE;
                        QE=QE->Next;
                        found=True;
                    }
                }else{
                strncpy(ID, K.EmpID, 8);
                ID[8] = '\0';
                Access_ID(*Emp, ID, &PED, &QE, &found);
                }
                if (found){
                    Update_History(&(PED->E), K.ConsReason);
                    PED->E.TNC++;
                    time_t current_time;
                    time(&current_time);
                    struct tm *local_time = localtime(&current_time);
                    PED->E.LCD.tm_mday = local_time->tm_mday;
                    PED->E.LCD.tm_mon = local_time->tm_mon;
                    PED->E.LCD.tm_year = local_time->tm_year;
                }
                *N--;
            }else{
                printf("No Appointements Left Today\n");
            }
                getchar();
            break;
        case 3:
            ChoseConsmenu(*TQ, &P, &Q);
            if (Q==NULL){
                if (P==TQ->WD.Head){
                    TQ->WD.Head=P->Next;
                }else if (P==TQ->OD.Head){
                    TQ->WD.Head=P->Next;
                }else if (P==TQ->RWVPEV.Head){
                    TQ->WD.Head=P->Next;
                }else if (P==TQ->PE.Head){
                    TQ->WD.Head=P->Next;
                }
            }else {
                set_Next_Cons(Q, P->Next);
            }
            Q=P;
            printf("%s\n", P->C.ConsReason);
            if (strcmp(P->C.ConsReason, WD)==0){
                cpt=1;
            }else if (strcmp(P->C.ConsReason, OD)==0){
                cpt=2;
            }else if (strcmp(P->C.ConsReason, RWV)==0 || strcmp(P->C.ConsReason, PEV)==0){
                cpt=3;
            }else if (strcmp(P->C.ConsReason, PE)==0){
                cpt=4;
            }
            while (P != NULL || cpt != 4){
                if (P==NULL){
                    switch (cpt){
                        case 1:
                        P=TQ->OD.Head;
                        cpt=2;
                        break;
                        case 2:
                        P=TQ->RWVPEV.Head;
                        cpt=3;
                        break;
                        case 3:
                        P=TQ->PE.Head;
                        cpt=4;
                        break;
                    }
                }
                printf("%s\n", ConsToStr(P->C));
                if (!P->C.Today){
                    P->C.Today=True;
                    break;
                }
                P=P->Next;
            }
            printf("Canceled Appointement: \n");
            printf("%s\n", ConsToStr(Q->C));
            printf("Press a key to continue...");
            getchar();
            Free_Cons(&Q);
            break;
        case 4:
            goto ConsEnd;
            break;
    }
    goto start;
    ConsEnd: system("cls");
}
/*************************************************************************
*************************************************************************/
//Allows the user to use the Employee Records management Menu, in an easy to use interface
void Empmenu(Emp_ptr *Emp){
    startE: system("cls");
    message tab[15];
    strcpy(tab[0].message,"Show the List Employees Records");
    strcpy(tab[0].flichaR,"->");
    strcpy(tab[0].flichaL,"<-");
    strcpy(tab[1].message,"      Add Employee Record      ");
    strcpy(tab[1].flichaR,"");
    strcpy(tab[1].flichaL,"");
    strcpy(tab[2].message,"     Update Employee Record    ");
    strcpy(tab[2].flichaR,"");
    strcpy(tab[2].flichaL,"");
    strcpy(tab[3].message,"     Delete Employee Record    ");
    strcpy(tab[3].flichaR,"");
    strcpy(tab[3].flichaL,"");
    strcpy(tab[4].message,"      Retrun To main menu      ");
    strcpy(tab[4].flichaR,"");
    strcpy(tab[4].flichaL,"");
    int i;
    int c;
    int choix=0;
    la:
    Textecolor(1);
    printf("\n\n\n");
    printf("\t\t\t\t                   Employee Record Management                  \t\t\t\t\n");
    printf("\t\t\t\t                            ------                             \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    for (i=0;i<5;i++)
    {
    printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
    printf("\t\t\t\t\t\t\n");
    }
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");

    c=getch();
    switch(c){
    case 72:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==0) choix=4;
        else choix--;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 80:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==4) choix=0;
        else choix++;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
        c=c-49;
        goto end;
    case 13:
        c=choix;
        goto end;
    default: break;
    }
    system("cls");
    goto la;
    end:
    QLoading();
    char tempID[20];
    char ID[9];
    Emp_ptr P;
    Emp_ptr Q;
    bool found;
    switch(c) {
        case 0:
            Textecolor(15);
            ShowEmpLL(*Emp);
            printf("Press ENTER To return to the menu...");
            getchar();
            break;
        case 1:
            Add_Emp(Emp);
            getchar();
            break;
        case 2:
            Textecolor(15);
            printf("Type in the ID of the Employee to update the Record: \n");
            scanf(" %[^\n]", tempID);
            again: while (strlen(tempID) > 8) {
                printf("The ID is too long, \nplease Type in an Employee ID that is up to 8 characters\n");
                scanf(" %[^\n]", tempID);
            }
            strncpy(ID, tempID, 8);
            ID[8] = '\0';
            Access_ID(*Emp,ID, &P, &Q, &found);
            while (!found){
                printf("Employee ID Not Found, Type in the right ID: \n");
                scanf(" %[^\n]", tempID);
                goto again;
            }
            Update_Emp(*Emp, ID);
            break;
        case 3:
            Textecolor(15);
            printf("Type in the ID of the Employee you want to Delete: \n");
            scanf(" %[^\n]", tempID);
    again2: while (strlen(tempID) > 8) {
                printf("The ID is too long, \nplease Type in an Employee ID that is up to 8 characters\n");
                scanf(" %[^\n]", tempID);
            }
            strncpy(ID, tempID, 8);
            ID[8] = '\0';
            Access_ID(*Emp, tempID, &P, &Q, &found);
            while (!found){
                printf("Employee ID Not Found, Type in the right ID: \n");
                scanf(" %[^\n]", tempID);
                goto again2;
            }
            Delete_ID(Emp, ID);
            break;
        case 4:
            goto EmpEnd;
            break;
    }
    goto startE;
    EmpEnd: system("cls");
}

/*************************************************************************
*************************************************************************/
//An easy to use interface that allows the user to explore and use all the available functionalities
void menu(Emp_ptr *Emp, struct ConsTQ *TQ){
    Loading();
    Fassada();
    int YetToday=10;
    bool Read=False;
    start: system("cls");
    message tab[15];
    strcpy(tab[0].message,"  Show Scheduled appointments  ");
    strcpy(tab[0].flichaR,"->");
    strcpy(tab[0].flichaL,"<-");
    strcpy(tab[1].message,"      Manage appointments      ");
    strcpy(tab[1].flichaR,"");
    strcpy(tab[1].flichaL,"");
    strcpy(tab[2].message,"    Manage Employee Records    ");
    strcpy(tab[2].flichaR,"");
    strcpy(tab[2].flichaL,"");
    strcpy(tab[3].message,"   Save The Data To the Files  ");
    strcpy(tab[3].flichaR,"");
    strcpy(tab[3].flichaL,"");
    strcpy(tab[4].message,"  Save and Update To the Files ");
    strcpy(tab[4].flichaR,"");
    strcpy(tab[4].flichaL,"");
    strcpy(tab[5].message,"              Exit             ");
    strcpy(tab[5].flichaR,"");
    strcpy(tab[5].flichaL,"");
    int i;
    int c;
    int choix=0;
    la:
    Textecolor(3);
    printf("\n\n\n");
    printf("\t\t\t\t                             MENU                              \t\t\t\t\n");
    printf("\t\t\t\t                            ------                             \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    for (i=0;i<6;i++)
    {
    printf("\t\t\t\t\t\t%s %s %s",tab[i].flichaR,tab[i].message,tab[i].flichaL);
    printf("\t\t\t\t\t\t\n");
    }
    printf("\t\t\t\t                                                               \t\t\t\t\n");
    printf("\t\t\t\t***************************************************************\t\t\t\t\n");

    c=getch();
    switch(c){
    case 72:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==0) choix=5;
        else choix--;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 80:
        strcpy(tab[choix].flichaR,"");
        strcpy(tab[choix].flichaL,"");
        if (choix==5) choix=0;
        else choix++;
        strcpy(tab[choix].flichaR,"->");
        strcpy(tab[choix].flichaL,"<-");
        break;
    case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
        c=c-49;
        goto end;
    case 13:
        c=choix;
        goto end;
    default: break;
    }
    system("cls");
    goto la;
    end:
    QLoading();
    Textecolor(15);
    switch(c) {
        case 0:
            if (!Read){
                ReadEmp(Emp);
                ReadCons(TQ);
                Read=True;
            }
            if (EmptyConsQ(TQ->WD) && EmptyConsQ(TQ->OD) && EmptyConsQ(TQ->RWVPEV) && EmptyConsQ(TQ->PE)){
                printf("No Appointements Yet.\n");
            }else{
            ShowConsLL(*TQ);
            }
            printf("Press a key to continue...\n");
            getchar();
            break;
        case 1:
            if (!Read){
                ReadEmp(Emp);
                ReadCons(TQ);
                Read=True;
            }
            Consmenu(TQ, Emp, &YetToday);
            break;
        case 2:
            if (!Read){
            ReadEmp(Emp);
            ReadCons(TQ);
            Read=True;
            }
            Empmenu(Emp);
        break;
        case 3:
            printf("Saving Data...\n");
            if (!Read){
                ReadEmp(Emp);
                ReadCons(TQ);
                Read=True;
                }
            WriteEmp(*Emp);
            WriteCons(*TQ);
            printf("Done.\nPress a key to return...");
            while ((getchar()) != '\n');
            getchar();
        break;
        case 4:
            printf("Updating Data...\n");
            if (!Read){
                ReadEmp(Emp);
                ReadCons(TQ);
                Read=True;
                }
            End_Day_Update(*Emp,*TQ, Emp);
            printf("Saving Data...\n");
            WriteEmp(*Emp);
            YetToday=10;
            Read=False;
            printf("Files Updated Successfully.\n");
            printf("Press a key to continue...\n");
            getchar();
            break;
        case 5:
            goto RealEnd;
        break;

    }
    goto start;
    RealEnd:
    system("cls");
}

