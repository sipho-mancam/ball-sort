#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

#define BUTTON_1 100
#define RED 10
#define GREEN 20
#define BLUE 30
#define YELLOW 40

struct colorSTR
{
    int r, g, b;
};
typedef struct colorSTR colorSTR;

class ballSTR
{
private:
    colorSTR myColor;
    int valid, num_balls, current_index, colType;

public:
    //methods here...
    ballSTR(int r, int g, int b, int c_type)
    {
         myColor.r = r;
         myColor.g = g;
         myColor.b = b;
         colType = c_type;
    }
    ballSTR(){}
    colorSTR getColor(){return myColor;}
    int getColorType(){return colType;}
    void setColor(int r, int g, int b)
    {
        myColor.r = r;
        myColor.g = g;
        myColor.b = b;
    }
    void setColorType(int c_Type){colType = c_Type;}
    void setNumBalls(int i){num_balls = i;}
    int getNumBalls(){return num_balls;}
    void setValid(int val){valid = val;}
    int getValid(){return valid;}
    void setCurrentIndex(int c_i){current_index = c_i;}
    int getCurrentIndex(){return current_index;}
    ballSTR *next;
    ballSTR *prev;
};

struct columnSTR
{
    ballSTR *ballHead=NULL;

    int myID;
    int xPos, yPos, toggle=0;
    struct columnSTR *next, *prev;
};
typedef struct columnSTR columnSTR;
typedef columnSTR *Node;

typedef ballSTR* ballNode;

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
Node* insertElement(Node *head, columnSTR column);
Node searchID(Node *head, int id);
int insertBall(ballNode *ballHead, ballSTR *data);
ballNode searchBall(Node head, int current_index);
void init(HWND, Node *head, int);
Node initColums(Node* head);
void deleteColumns(Node *head);
void DrawColumn(HWND hwnd, Node head);
void DrawCircle(HWND hwnd, int xPos, int yPos, colorSTR color, HPEN hPen);
int onClick(Node currentPtr, int index, HWND hwnd);
int num_ball_in_column(Node currentPtr);
void onMove(HWND hwnd, int xPos, Node currentPtr);
int checkFull(Node head, int*);

TCHAR szClassName[ ] = _T("Semester Project");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */


    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Semester Project 2"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           700,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND but_1, textWind;
    static Node head = NULL, currentPtr=NULL, prevPtr=NULL;
    static unsigned int toggle=0, xPos, yPos, colTab[6], tempID, tempVar, colType;
    static int b_index, nob, complete=0;
    static ballNode currentBall=NULL;
    static HPEN hPen_black;
    wchar_t tempText[100];
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE:
            but_1 = CreateWindowW(L"Button", L"Start", WS_CHILD | WS_VISIBLE, 500, 30, 70, 30, hwnd, (HMENU)BUTTON_1,NULL, NULL);
            textWind = CreateWindowW(L"Static", L"", WS_CHILD| WS_VISIBLE, 400, 65, 250, 20, hwnd, (HMENU)0, NULL, NULL);
            hPen_black = CreatePen(PS_SOLID, 2, RGB(0,0,0));
            break;
        case WM_COMMAND:
            if(HIWORD(wParam)==BN_CLICKED)
            {
                switch(LOWORD(wParam))
                {
                case BUTTON_1:
                    init(hwnd, &head, toggle);
                    if(!toggle){toggle = 1;}

                    currentPtr = head;
                    for(int i=0; i<6; i++)
                    {
                        colTab[i] = currentPtr->xPos-15;
                        currentPtr = currentPtr->next;

                    }
                    break;
                }
            }
            break;
        case WM_LBUTTONDOWN:
            if(toggle)
            {
                xPos = GET_X_LPARAM(lParam);
                yPos = GET_Y_LPARAM(lParam);

                if(xPos > colTab[0] && xPos<colTab[0]+30 && yPos>50 && yPos<170)tempID = 1;
                else if(xPos > colTab[1] && xPos<colTab[1]+30 && yPos>45 && yPos<=170)tempID = 2;
                else if(xPos > colTab[2] && xPos<colTab[2]+30 && yPos>45 && yPos<=170)tempID = 3;
                else if(xPos > colTab[3] && xPos<colTab[3]+30 && yPos>45 && yPos<=170)tempID = 4;
                else if(xPos > colTab[4] && xPos<colTab[4]+30 && yPos>45 && yPos<=170)tempID = 5;
                else if(xPos > colTab[5] && xPos<colTab[5]+30 && yPos>45 && yPos<=170)tempID = 6;
                else tempID =100;

                currentPtr = searchID(&head, tempID);
                b_index = 10;
                tempVar = currentPtr->yPos+10;

                cout<<"yPos: "<<yPos<<" Temp Var: "<<tempVar<<" TempVar - 20: "<<tempVar-120<<endl;
                if(currentPtr!=NULL)currentBall = currentPtr->ballHead;
                if(tempID!=100)
                {
                    if(yPos<=tempVar && yPos>tempVar-20)b_index = 0; // last ball
                    else if(yPos<=tempVar-20 && yPos>tempVar-40)b_index = 1;
                    else if(yPos<=tempVar-40 && yPos>tempVar-60)b_index = 2;
                    else if(yPos<=tempVar-60 && yPos>tempVar-80)b_index = 3;
                    else if(yPos<=tempVar-80 && yPos>tempVar-100)b_index = 4;
                    else if(yPos<=tempVar-100 && yPos>tempVar-120)b_index = 5;
                    else b_index= 10;

                }
                if(b_index!= 10)
                {
                    cout<<b_index<<endl;
                    onClick(currentPtr, b_index, hwnd);
                    currentBall = searchBall(currentPtr, b_index);
                    prevPtr = currentPtr;
                }

            }
            break;

        case WM_MOUSEMOVE:
            if(toggle )
            {
                xPos = GET_X_LPARAM(lParam);
                yPos = GET_Y_LPARAM(lParam);

                if(xPos > colTab[0] && xPos<colTab[0]+30 && yPos>50 && yPos<170)tempID = 1;
                else if(xPos > colTab[1] && xPos<colTab[1]+30 && yPos>45 && yPos<=170)tempID = 2;
                else if(xPos > colTab[2] && xPos<colTab[2]+30 && yPos>45 && yPos<=170)tempID = 3;
                else if(xPos > colTab[3] && xPos<colTab[3]+30 && yPos>45 && yPos<=170)tempID = 4;
                else if(xPos > colTab[4] && xPos<colTab[4]+30 && yPos>45 && yPos<=170)tempID = 5;
                else if(xPos > colTab[5] && xPos<colTab[5]+30 && yPos>45 && yPos<=170)tempID = 6;
                //else tempID =100;
                if(currentBall!=NULL)
                {
                    if(currentBall->getValid()==1)
                    {
                        currentPtr = searchID(&head, tempID);
                        if(wParam & MK_LBUTTON )
                        {
                            if(currentBall!=NULL && currentBall->getValid())
                            {
                                onMove(hwnd, xPos, currentPtr);
                                for(int i=0; i<currentBall->getNumBalls(); i++)
                                DrawCircle(hwnd, xPos, yPos - i*20, currentBall->getColor(), hPen_black);
                            }
                            else
                            {
                                onMove(hwnd, xPos, currentPtr);
                            }
                        }
                    }
                }

            }
            break;
        case WM_LBUTTONUP:
            if(toggle)
            {

                nob = num_ball_in_column(currentPtr);

                ballNode tempBall;


                if(currentBall!=NULL)
                {

                    if(currentBall->getNumBalls()<= (6 - nob))
                    {
                        if(currentPtr->ballHead!=NULL)
                        {
                            if(currentBall->getColorType() == searchBall(currentPtr, nob-1)->getColorType())
                            {
                                tempBall = currentBall;

                                for(int i=0; i<currentBall->getNumBalls(); i++)
                                {
                                    insertBall(&currentPtr->ballHead, tempBall);

                                }
                                DrawColumn(hwnd, currentPtr);
                                colType = checkFull(currentPtr, &complete);

                                // delete objects after recreating them somewhere else
                                if(currentBall!=NULL)
                                {
                                    int s = currentBall->getNumBalls();

                                    currentBall = searchBall(prevPtr, num_ball_in_column(prevPtr)-1);
                                    for(int j=0; j<s; j++)
                                    {
                                        tempBall = currentBall->prev;
                                        delete currentBall;
                                        currentBall = tempBall;
                                        if(currentBall!=NULL)currentBall->next = NULL;
                                        else prevPtr->ballHead = NULL;
                                    }
                                }
                                DrawColumn(hwnd, prevPtr);
                            }
                            else
                            {
                                DrawColumn(hwnd, currentPtr);
                                DrawColumn(hwnd,prevPtr);
                            }
                        }
                        else
                        {
                            tempBall = currentBall;
                            for(int i=0; i<currentBall->getNumBalls(); i++)
                            {
                                //tempBall->setCurrentIndex(i);
                                insertBall(&currentPtr->ballHead, tempBall);

                            }
                            DrawColumn(hwnd, currentPtr);
                            colType = checkFull(currentPtr, &complete);
                            if(currentBall!=NULL)
                            {
                                int s = currentBall->getNumBalls();
                                currentBall = searchBall(prevPtr, num_ball_in_column(prevPtr)-1);
                                for(int j=0; j<s; j++)
                                {
                                    tempBall = currentBall->prev;
                                    delete currentBall;
                                    currentBall = tempBall;
                                    if(currentBall!=NULL)currentBall->next = NULL;
                                    else prevPtr->ballHead=NULL;
                                }
                            }
                            DrawColumn(hwnd, prevPtr);
                        }
                    }
                    else
                    {
                        DrawColumn(hwnd, currentPtr);
                        DrawColumn(hwnd, prevPtr);
                    }
                }
                onMove(hwnd, xPos,currentPtr);

                if(currentPtr->toggle)
                {
                    switch(colType)
                    {
                    case RED:
                        swprintf(tempText, L"Red is completed. completed: %d", complete);
                        break;
                    case GREEN:
                        swprintf(tempText, L"Green is completed.completed: %d", complete);
                        break;
                    case BLUE:
                        swprintf(tempText, L"Blue is completed. completed: %d", complete);
                        break;
                    case YELLOW:
                        swprintf(tempText, L"Yellow is completed. completed: %d", complete);
                        break;

                    }

                    if(complete==4)swprintf(tempText, L"Congratulations !!! you won!");
                    SetWindowTextW(textWind, tempText);
                }
            }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

Node* insertElement(Node *head, columnSTR column)
{
    Node newNode = (Node)malloc(sizeof(columnSTR));
    if(newNode==NULL)
    {
        puts("Couldn't create memory....");
        return NULL;
    }
    else
    {
        Node currentPtr = *(head);
        Node prevPtr = NULL;

        *(newNode) = column;

        //go to the last no to assign the new data to.
        while(currentPtr != NULL)
        {
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if(prevPtr == NULL)
        {
            (*head) = newNode;
            (*head)->prev = NULL;
            (*head)->next = NULL;
        }
        else
        {
            currentPtr = newNode;
            prevPtr->next = currentPtr;
            currentPtr->prev = prevPtr;
            currentPtr->next = NULL;
        }
    }
    return head;

}

Node searchID(Node *head, int id)
{
    Node currentPtr = *(head);

    while(currentPtr!=NULL)
    {
        if(currentPtr->myID != id)currentPtr = currentPtr->next;
        else break;
    }

    if(currentPtr != NULL)return currentPtr;
    else return *head;
}

int insertBall(ballNode *ballHead, ballSTR *data)
{
    ballNode newData = new ballSTR;
    int i=0;
    if(newData == NULL)return -1;
    else {
            ballNode currentBall = *ballHead;
            ballNode prevBall = NULL;
            *newData = *data;

        while(currentBall!=NULL)
        {
            i++;
            prevBall = currentBall;
            currentBall = currentBall->next;

        }
        if(prevBall == NULL)
        {
            *ballHead = newData;
            (*ballHead)->next =NULL;
            (*ballHead)->prev = NULL;
            (*ballHead)->setCurrentIndex(i);
            return 1;
        }
        else {
                currentBall = newData;
                currentBall->setCurrentIndex(i);
                currentBall->next = NULL;
                currentBall->prev = prevBall;
                prevBall->next = currentBall;
                return 1;
        }

    }

}

ballNode searchBall(Node head, int current_index)
{
    Node currentPtr = head;
    ballNode currentBall = currentPtr->ballHead;
    if(currentBall != NULL)
    {
        while(currentBall->getCurrentIndex() != current_index)
        {
            currentBall = currentBall->next;
            //if we at the end of the list, break out and spit out NULL
            if(currentBall == NULL)
            {
                return NULL;
            }
            //else keep searching
        }
        return currentBall;
    }
    else
        return NULL;
}

void init(HWND hwnd, Node *head, int toggle)
{
    HPEN hPen;

    //generate random numbers to randomly assign colors to balls....
    srand(time(NULL));

    int b_color, r=0, g=0, b=0, y=0, k=0;

    // receive the head of the list after being initialized
    Node currentPtr;
    if(!toggle){currentPtr = initColums(head);} // initialize columns only once...p
    else currentPtr = *head;
    ballSTR ball(0, 0, 0, 0);
    ballNode currentball = NULL;


    //column for the ball
    for(int i=0; i<4;i++)
    {
        if(toggle)
        {
            k = num_ball_in_column(currentPtr);
        }
        else k=6;
        currentball = currentPtr->ballHead;
        for(int j=0; j<k; j++)
        {
            RAND:
                b_color = 1+rand()%4;
                switch(b_color)
                {
                case 1:
                    if(r==6)goto RAND;
                    break;
                case 2:
                    if(g==6)goto RAND;
                    break;
                case 3:
                    if(b==6)goto RAND;
                    break;
                case 4:
                    if(y==6)goto RAND;
                    break;
                }



            switch(b_color)
            {
            case 1: // red
                ball.setColor(255, 0, 0);
                ball.setColorType(RED);
                r++;
                break;
            case 2: // green
                ball.setColorType(GREEN);
                ball.setColor(0, 255, 0);
                g++;
                break;
            case 3: // blue
                ball.setColor(0, 0, 255);
                ball.setColorType(BLUE);
                b++;
                break;
            case 4: //yellow
                ball.setColor(255, 255, 0);
                ball.setColorType(YELLOW);
                y++;
                break;
            }

            ball.setCurrentIndex(j);

            if(!toggle)insertBall(&currentPtr->ballHead, &ball);
            else
            {
                //for(int i=0; i<1 && currentball!=NULL; i++)

                    currentball->setColor(ball.getColor().r,ball.getColor().g,ball.getColor().b);
                    currentball->setColorType(ball.getColorType());
                    currentball->setCurrentIndex(ball.getCurrentIndex());
                    currentball = currentball->next;

            }
        }
        DrawColumn(hwnd, currentPtr);
        currentPtr = currentPtr->next;
    }
    DrawColumn(hwnd, currentPtr);
    DrawColumn(hwnd, currentPtr->next);
}

Node initColums(Node* head)
{
    columnSTR column;
    int k=0;

    for(int j=0; j<12; j++)
    {
        if(j%2 == 0)//Draw rectangle,
        {
            k++;
            column.myID = k;
            column.xPos = 40+j*30+15;
            column.yPos = 170-10;
            head = insertElement(head, column);
        }
    }
    return *head;
}

int onClick(Node currentPtr, int index, HWND hwnd)
{
    //check if the selected ball is valid...
    static int colorType, i;
    ballNode currentball, tempCurrentBall;

    //Get the current ball select from the index
    currentball = searchBall(currentPtr, index);
    if(currentball!=NULL)
    {
        colorType = currentball->getColorType();
        i = index;
        HPEN hPen;
        tempCurrentBall = currentball;
        if(index < num_ball_in_column(currentPtr)-1)
        {
            while(index <num_ball_in_column(currentPtr))
            {
                if(tempCurrentBall->getColorType() == colorType)
                {
                    //if it's the top ball in the column set valid
                    if(index == num_ball_in_column(currentPtr)-1){
                            currentball->setValid(1);
                            index++;
                    }
                    else
                    {
                        tempCurrentBall = tempCurrentBall->next;
                        index++;
                    }
                }
                else
                {
                    currentball->setValid(0);
                    return 0;

                }
            }
        }
        else currentball->setValid(1);
        // Select the number of balls selected...

        index = i;

        hPen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
        if(currentball->getValid())
        {
            currentball->setNumBalls(num_ball_in_column(currentPtr)-i);

            int j=index;
            while(j < num_ball_in_column(currentPtr))
            {

                DrawCircle(hwnd, currentPtr->xPos, currentPtr->yPos - j*20, currentball->getColor(), hPen);
                j++;
            }
            DeleteObject(hPen);
            return 1;
        }
        DeleteObject(hPen);
        return 0;
    }
    else
        return 0;
}

void DrawColumn(HWND hwnd, Node head)
{
    Node currentPtr = head;
    ballNode currentBall = currentPtr->ballHead;

    HDC hdc;
    RECT myRect;
    HPEN hPen;
    hdc = GetDC(hwnd);
    HGDIOBJ defObj;
    HBRUSH myBrush = CreateSolidBrush(RGB(170, 170, 170));
    hPen = CreatePen(PS_NULL, 1, RGB(0,0,0));

    defObj = SelectObject(hdc, myBrush);
    SelectObject(hdc, hPen);
    Rectangle(hdc,currentPtr->xPos-15,50, currentPtr->xPos+15, currentPtr->yPos+10);

    DeleteObject(hPen);
    if(currentBall!=NULL)
    {
        while(currentBall!=NULL)
        {
            hPen = CreatePen(PS_SOLID, 1, RGB(currentBall->getColor().r,currentBall->getColor().g,currentBall->getColor().b));
            DrawCircle(hwnd, currentPtr->xPos, currentPtr->yPos - currentBall->getCurrentIndex()*20, currentBall->getColor(), hPen);
            DeleteObject(hPen);
            currentBall = currentBall->next;
        }
    }
    SelectObject(hdc, defObj);
    ReleaseDC(hwnd, hdc);
    DeleteObject(myBrush);

}
void DrawCircle(HWND hwnd, int xPos, int yPos, colorSTR color, HPEN hPen)
{
    HDC hdc;
    HBRUSH myBrush;
    HGDIOBJ defObj;

    hdc = GetDC(hwnd);

    myBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));

    defObj = SelectObject(hdc, myBrush);
    SelectObject(hdc, hPen);

    Ellipse(hdc, xPos-10, yPos -10, xPos+10, yPos+10);

    SelectObject(hdc, defObj);
    ReleaseDC(hwnd, hdc);

    DeleteObject(hPen);
    DeleteObject(myBrush);
}

void onMove(HWND hwnd, int xPos, Node currentPtr)
{
    RECT myRect;
    HBRUSH myBrush = CreateSolidBrush(RGB(240, 240, 240));
    HDC hdc = GetDC(hwnd);
    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0,0,0));
    HGDIOBJ defObj;

    defObj = SelectObject(hdc, hPen);
    hPen = (HPEN)SelectObject(hdc, myBrush);

    // if we are inside the column, only print the column
    if(xPos>currentPtr->xPos-20&&xPos<currentPtr->xPos+20)
    {
        Rectangle(hdc,currentPtr->xPos-15, 0, currentPtr->xPos+15, 500);
        DrawColumn(hwnd, currentPtr);
    }
    if(xPos<currentPtr->xPos || xPos>=currentPtr->xPos+45)
    {
        Rectangle(hdc,currentPtr->xPos-15-30, 0, currentPtr->xPos+15, 500);
        DrawColumn(hwnd, currentPtr);
    }
    if(xPos>currentPtr->xPos || xPos<=currentPtr->xPos-44)
    {
        Rectangle(hdc, currentPtr->xPos-15, 0, currentPtr->xPos+15+30, 500);
        DrawColumn(hwnd, currentPtr);
    }
    SelectObject(hdc, defObj);
    ReleaseDC(hwnd, hdc);
    DeleteObject(hPen);
    DeleteObject(myBrush);
}


int num_ball_in_column(Node currentPtr)
{
    int i=0;

    ballNode currentball = currentPtr->ballHead;

    while(currentball!=NULL)
    {
        currentball = currentball->next;
        ++i;
    }

    return i;
}

int checkFull(Node head, int *complete)
{
    Node currentPtr = head;
    ballNode currentBall = currentPtr->ballHead;
    int i=0;

    while(currentBall!=NULL && currentBall->next!=NULL)
    {
        if(currentBall->getColorType() == currentBall->next->getColorType())
        {
            i++;
        }
        currentBall = currentBall->next;
    }
    if(i==5)
    {
        currentPtr->toggle=1;
        *complete += 1;
        return currentPtr->ballHead->getColorType();
    }
    else{
        currentPtr->toggle = 0;
        return 0;
    }
}
