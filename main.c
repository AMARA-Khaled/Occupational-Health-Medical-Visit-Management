#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "TPlib.h"
  
int main(){
    struct ConsTQ *TQ;
    Emp_ptr Emp;
    Emp=NULL;
    MkConsTQ(&TQ);
    menu(&Emp, TQ);
    Free_End(&Emp, TQ);
    return 0;
}