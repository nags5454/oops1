#include <cstdio>
#include<iostream>
#include<conio.h>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <dos.h>

typedef struct
{
    /*a structure to hold details of vehicle
     *parked at a particular cell in the parking lot.*/

    char vehicle_no[11];
    int flag;
    //int token;

}cell;

cell parking[8][8]; //The virtual parking space simulating an actual parking space with 100X100 parking slots
int availability=0; //This variable checks for the availability of the overall parking space.
int token_count=0;  //A counter to generate token numbers in a systematic and ascending manner

typedef struct
{
    /*a structure dedicated to store the positions
     *of each cell easing the time taken to access the
     *particular cell involved*/
    int x,y;
}position;

typedef struct detail_node
{
    /*a structure dedicated to store the details like vehicle number, flag to see if the vehicle was previously logged out,
     *in time, out time, position as to where a vehicle is parked, it's priority class
     *and the corresponding token number allotted.*
     **
     *this structure shall be used in various pursuits like printing a token to a
     *customer during check-in, printing of receipt during checkout,to maintain a
     *track record of the vehicles that had parked in the parking lot so far.*/
    char vno[11];
    time_t in;
    time_t out;
    position p;
    int token;
    int flag;
    struct detail_node *next;
}detail_node;

//Function to halt the program for a specified amount of time(in milliseconds)
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

/*a function to insert a node at the end of a linked list
 *wherein each node is of the type detail_node.*/
detail_node *insert(detail_node *first,detail_node *newspace)
{
    detail_node *temp;
    //newspace->next=NULL;
    if(first==NULL)
        first=newspace;
    else
    {
        temp=first;
        while(temp->next!=NULL)
            temp=temp->next;
        temp->next=newspace;
    }
    return first;
}

/*a function to search for a customer detail based on the token number entered
 *if the match is found, the function returns the address of the node wherein
 *the said customer details are stored. Otherwise, returns NULL.*/
detail_node *search(detail_node *first,int key_token)
{
    if(first==NULL)
        return NULL;
    while(first!=NULL&&first->token!=key_token)
        first=first->next;
    return first;
}

/*this function allots names to classes involved as:
 *1=Business
 *2=Premium
 *3=Economy
 *and returns the corresponding strings to the calling function.*/
/*char *allot_name_to_class(int a)
{
    /*A function to allot names to priority classes.
    /*char a1[]="BUSINESS";
    char a2[]="PREMIUM";
    char a3[]="ECONOMY";
    switch(a)
    {
        case 1: return "BUSINESS";
        case 2: return "PREMIUM";
        case 3: return "ECONOMY";
    }
}*/
//A function to display a error in a blinking fashion
void display_error_message(char a[])
{
    int i=0;
    system("cls");
    while(i<10)
    {
        //delay(150);
        system("cls");
        delay(100);
        printf("%s",a);
        ++i;
        delay(100);
    }
}
/*A function to display details of the Linked List in order.*/
void display(detail_node *first)
{
    char in[40],out[40];
    if(first==NULL)
        display_error_message("NO VEHICULAR ACTIVITY DETECTED RECENTLY");
    else
    {
        system("cls");
        printf("Vehicle Number\t\t\tIn Time\t\t\t\tOut Time\tToken\tX\tY");
        while(first!=NULL)
        {
            strcpy(in,ctime(&first->in));
            in[strlen(in)-1]='\0';

            strcpy(out,ctime(&first->out));
            out[strlen(out)-1]='\0';

            printf("\n%-11s\t\t%s\t%s\t%-10s\t%d\t%d\t%d",first->vno,in,out,first->token,first->p.x,first->p.y);
            first=first->next;
        }
        getchar();
    }
}


/*a function to assign current time to a record of
 *an outgoing vehicle.*/
void assign_out_time(detail_node *check_out)
{
    time_t now=time(0);
    check_out->out=now;
}

/*a function to allot parking space to an incoming vehicle based on the priority number
 *and after checking the availability of the corresponding class to admit new vehicles.
 *this function then returns the exact position as to wherein the incoming vehicle needs
 *to be parked.*/
position allot()
{
    int i,j;
    position pos;

   for(i=0;i<8;++i)
        {
            for(j=0;j<8;++j)
            {
                if(parking[i][j].flag==0)
                {
                        pos.x=i;
                        pos.y=j;
                        parking[i][j].flag=1;
                        return pos;
                }
            }
        }
        pos.x=pos.y=-1;
        return pos;
    }
}

/*a function to generate an admission token to an incoming vehicle based on
 *priority chosen.
 *this function prints the token with various parameters like position assigned,
 *in time, vehicle no and token number included in it.
 *this function makes a call to allot() function to allot parking space.*/
detail_node *token_generator(detail_node *first,detail_node *new_customer)
{
    new_customer->p=allot();
    new_customer->token=++token_count;
    //parking[new_customer->p.x][new_customer->p.y].token=token_count;
    new_customer->in=time(0);
    new_customer->next=NULL;
    new_customer->flag=1;

    first=insert(first,new_customer);//insert the node onto the linked list

    system("cls");

    printf("\t    SIT PARKING");
    printf("\nVehicle Number\t:\t%s",new_customer->vno);
    printf("\nCheck In\t:\t%s",ctime(&new_customer->in));
   // printf("Priority\t:\t%s",allot_name_to_class(new_customer->priority));
    printf("\nCost(Min. 1 Hour+Hourly Charges):");
    printf("INR 75+60");
    printf("\nPosition\t:\t%d,%d",new_customer->p.x,new_customer->p.y);
    printf("\n\t  TOKEN NUMBER:%d",new_customer->token);
    availability++;

    return first;
}

/*a function to generate the final receipt of cost incurred on a customer based on
 *time his/her vehicle had been parked in the said parking space.
 *this function further calls the assign_out_time() and later on updates the linked list.*/
detail_node *receipt_generator(detail_node *first,int tno)
{
    detail_node *temp=NULL;
    temp=search(first,tno);
    if(temp==NULL)
        display_error_message("!!!ILLEGAL ENTRY!!!");
    else if(temp->flag==0)
        display_error_message("Vehicle already checked out");
    else
    {
        parking[temp->p.x][temp->p.y].flag=0;
        //parking[temp->p.x][temp->p.y].token=0;
        //temp->out=time(0);
        int time=difftime(temp->in,temp->out)/3600;
        double cost;
        if(time<1)
            cost=75;
        else
            cost=75+time*60;
        temp->flag=0;
        system("cls");
        printf("\t      RECEIPT");
        printf("\nVehicle Number\t:%s",temp->vno);
        printf("\nCheck In\t:%s",ctime(&temp->in));
        assign_out_time(temp);
        printf("Check Out\t:%s",ctime(&temp->out));
       // printf("Priority\t:%s",allot_name_to_class(temp->priority));
        printf("\nPosition\t:%d,%d",temp->p.x,temp->p.y);
        printf("\nCost\t\t:%lf",cost);
        printf("\n\t   TOKEN NUMBER:%d",temp->token);
        printf("\n\t   !!!THANK YOU!!!");
        delay(3500);
        --availability;
    }
    return first;
}

/*a function to check whether every vehicle has been checked out
 *before logging out of the administrator*/
int check_clearance()
{
    int i,j;
    for(i=0;i<8;++i)
    {
        for(j=0;j<8;++j)
        {
            if(parking[i][j].flag==1)
                return 1;
        }
    }
    return 0;
}
/*this function stores all the linked list content into a file upon logging out
void on_exit(detail_node *first)
{
    FILE *fp;
    detail_node *temp1=first,*temp2;
    time_t now=time(0);
    char in[35],out[35];
    fp=fopen("ParkingDetails.txt","a+");
    if(fp==NULL)
        printf("\nFILE NOT CREATED");
    //fprintf(fp,"%s",ctime(&now));
    if(first==NULL)
    {
        fprintf(fp,"%s",ctime(&now));
        fprintf(fp,"No Vehicular Activity Today");
        display_error_message("NO VEHICULAR ACTIVITY DETECTED RECENTLY");
    }
    else
    {
        system("cls");
        printf("Successfully Logged Out");
        int i=0;
        fprintf(fp,"%s",ctime(&now));
        fprintf(fp,"Serial\tVehicle Number\t\t\tIn Time\t\t\t\tOut Time\t\tPriority\tToken\tX\tY");
        while(first!=NULL)
        {
            strcpy(in,ctime(&first->in));
            in[strlen(in)-1]='\0';

            strcpy(out,ctime(&first->out));
            out[strlen(out)-1]='\0';


            fprintf(fp,"\n%-6d\t%-12s\t\t%s\t%s\t%-10s\t%d\t%d\t%d",++i,first->vno,in,out,allot_name_to_class(first->priority),first->token,first->p.x,first->p.y);
            first=first->next;
        }
        fprintf(fp,"\n\n");

        while(temp1!=NULL)
        {
            temp2=temp1;
            temp1=temp1->next;
            free(temp2);
        }
    }
}*/

/*a function to display the parking space and whether a vehicle is parked in
 at a cell or not by indicating 1 for vehicle parked and 0 for vehicle not parked*/
void display_space()
{
    int i,j;
    printf("\t\t     SIT PARKING LOT\n");
    printf("ENTRANCE\t\t\t\t\t     EXIT\n");
    printf("|  |_________________________________________________|  |\n\n");
    for(i=0;i<8;++i)
    {
        for(j=0;j<8;++j)
        {
            printf("%d\t",parking[i][j].flag);
        }
        printf("\n");
    }
    printf("_________________________________________________________");
}

/*a function to see whether a vehicle has previously checked in and hasn't checked out
 *and still intends to check in again*/
int background_search(detail_node *first,char vno[])
{
    if(first==NULL)
        return 0;
    while(first!=NULL)
    {
        if(!strcmp(first->vno,vno)&&(first->flag!=0))
            return 1;
        first=first->next;
    }
    return 0;
}


void main()
{
            //declarations
            int choice;
            int chc;
            int admin_choice;
            int prior;
            int bootup_password;
            char vehicle_no[11];
            detail_node *first=NULL,*temp;
            int tkno,password;
            //declarations for main function
    system("cls");
    printf("\t\t\tSIT PARKING LOT");
    printf("\nWELCOME TO BOOT UP.");
    printf("\nENTER ADMINISTRATOR BOOT UP PASSWORD:");
    scanf("%d",&bootup_password);
    if(bootup_password!=1234)
    {
        display_error_message("!!!INVALID PASSWORD!!!");
        exit(0);
    }

    while(1)
    {

        system("cls");
        printf("\t\t\tSIT PARKING LOT\n");
        printf("1.Check-In\n2.Check-Out\n3.Administrator Log In\n");
        scanf("%d",&choice);
        system("cls");
        switch(choice)
        {
        case 1:
            //This option checks for whether the parking space is available or not
            //And accordingly generates a token for the customer based on his
            //Priority choices of the customer.
            if(availability>=64)
                printf("\nSORRY NO ADMISSIONS ALLOWED");
            else
            {
                system("cls");
                printf("Want location details?(1=Yes\\0=No)");
                scanf("%d",&chc);
                if(chc)
                {
                    system("cls");
                    display_space();
                    printf("\nPRESS ENTER");
                    getchar();
                    //getchar();
                    //system("cls");
                }
                else
                    system("cls");
                //system("cls");
                temp=(detail_node*)malloc(sizeof(detail_node));
                getchar();
                system("cls");
                //if(token_count)
                  //  getchar();
                printf("Enter Vehicle Number:");
                gets(vehicle_no);
                strupr(vehicle_no);//changes
                if(background_search(first,vehicle_no))
                    display_error_message("Vehicle already checked in.");
                else
                {
                        strcpy(temp->vno,vehicle_no);
                        first=token_generator(first,temp);
                        delay(3500);
                }

            }
            break;
        case 2:
            //This option lets the customer enter his token number
            //and get his receipt generated.
            system("cls");
            printf("Enter Token Number:");
            scanf("%d",&tkno);
            receipt_generator(first,tkno);
            break;
        case 3://Administrator log in option.
            system("cls");
            printf("1.View Register.\n2.Log Out\n3.Display Parking Space\nOption Selected:");
            scanf("%d",&admin_choice);
            if(admin_choice<1||admin_choice>3)
                display_error_message("!!!Invalid Choice Admin!!!");
            else
            {
                printf("\nPASSWORD:");
                scanf("%d",&password);
                if(password==1234)
                {
                    switch(admin_choice)
                    {

                        case 1:
                            //This choice gives access to administer privileges.
                            //i.i, The Administer can access and see the linked so created.
                            //But before accessing this choice, the Admin is supposed to enter
                            //a preset password
                            {
                                display(first);
                                getchar();
                                //getchar();
                            }
                        break;
                    case 2:
                        //This is again is an exclusive option for the admin.
                        //He can terminate the program thereby sending all the details
                        //Stored on the linked list created so far into a file.s
                        if(check_clearance(first))
                        {
                            system("cls");
                            display_error_message("CUSTOMER STILL INSIDE! PLEASE CLEAR!");
                        }
                        else
                        {
                            on_exit(first);
                            exit(0);
                        }
                    break;
                    case 3://display of parking space
                            system("cls");
                            display_space();
                            printf("\n\nPRESS ENTER");
                            getchar();
                            getchar();
                    break;
                    default://invalid choice default
                        system("cls");
                        display_error_message("!!!INVALID CHOICE ADMIN!!!");
                        //delay(2000);
                }

            }
            else
                    display_error_message("Administrator Access Denied");
            }
            break;

        default:
            system("cls");
            display_error_message("!!!INVALID CHOICE!!!");
        }

    }

}
