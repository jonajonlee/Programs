#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string>

using namespace std;


//change SIZE for difficulty. Must be even number >= 6.
const int SIZE = 12;

void setPosition(int num, char **ARR, char ch, int n)
{
    int posX, posY;

    int counter = 0;
    while(counter != num)
    {
        posX = rand()%n+1;
        posY = rand()%n+1;
        if(ARR[posX][posY]=='-')
        {
            ARR[posX][posY] = ch;
            counter++;
        }
    }
}

void checkNear(bool &wum, bool &pitt, bool &gol, bool &bow, int &x, int &y, char **ARR)
{
    cout << "DIALOGUE" << endl << endl;

    if(wum)
        cout << "Hero: What is that horrid stench?" << endl;
    if(pitt)
        cout << "Hero: There's an arid breeze about..." << endl;
    if(gol)
        cout << "Hero: My treasure-senses are tingling!" << endl;
    if(bow)
        cout << "Hero: There's a weapon nearby. At least I hope so." << endl;

    cout << "-----------------------------------------------------" << endl;
    cout << "MAP" << endl << endl;

    wum = false;
    pitt = false;
    gol = false;
    bow = false;
}

void printArray(char **ARR, int n)
{
    for(int i=1; i<n; i++)
    {
        for(int j=1; j<n; j++)
            cout << ARR[i][j];
        cout << endl;
    }
}

void setStart(char **ARR, int &x, int &y, int n)
{
    x=n-2;
    y=n/2+1;
    ARR[x][y] = '@';
}

void view(int x, int y, char **ARR, bool &wum, bool &pitt, bool &gol, bool &bow)
{
    string vue = "0";
    char l;


    for(int i=x-1; i<x+2; i++)
    {
        for(int j=y-1;j<y+2;j++)
        {
            l = ARR[i][j];

            if(l=='W')
                wum=true;
            if(l=='O')
                pitt=true;
            if(l=='$')
                gol=true;
            if(l=='}')
                bow=true;

            if(l=='#')
                vue=vue+string("#");
            else if(l=='@')
                vue=vue+string("@");
            else if(l=='.')
                vue=vue+string(".");
            else
                vue=vue+string("?");
        }
        vue=vue+string("\n");
    }
    checkNear(wum,pitt,gol,bow,x,y,ARR);
    for(int i=1;i<vue.length();i++)
        cout << vue[i];
    cout << endl;
}

void status(bool wep, int gp)
{

    cout << "-----------------------------------------------------" << endl;
    cout << "STATUS" << endl << endl;
    cout << "Gold: " << gp << endl;
    if(wep==true)
        cout << "Weapon: Bow/Arrow" << endl;
    else
        cout << "Weapon: None"<< endl;
    cout << "-----------------------------------------------------" << endl;
}

void changeDir(char dir, char **ARR, int &x, int &y)
{
    if(dir=='n'||dir=='N')
        x=x-1;
    else if(dir=='s'||dir=='S')
        x=x+1;
    else if(dir=='w'||dir=='W')
        y=y-1;
    else if(dir=='e'||dir=='E')
        y=y+1;
}

void move(char **ARR, int &x, int &y, int &gp, bool &wep, bool &alive, char dir)
{
    char v;

    if(dir=='n'||dir=='N')
        v = ARR[x-1][y];
    else if(dir=='s'||dir=='S')
        v = ARR[x+1][y];
    else if(dir=='w'||dir=='W')
        v = ARR[x][y-1];
    else if(dir=='e'||dir=='E')
        v = ARR[x][y+1];

    cout << "UPDATES" << endl << endl;

    switch(v)
    {
        case '#':
            cout << "There's a wall there, you fool" << endl;
            break;
        case 'W':
            cout << "Oh no! A wumpus!" << endl;
            if(wep==true)
                cout << "You shot it's eyes out. That sucker's dead!" << endl;
            else
            {
                cout << "All that's left of you are bones and hair" << endl;
                alive = false;
                break;
            }
            ARR[x][y] = '.';
            changeDir(dir,ARR,x,y);
            break;
        case '$':
            cout << "You found some gold!" << endl;
            gp=gp+500;
            ARR[x][y] = '.';
            changeDir(dir,ARR,x,y);
            break;
        case '}':
            cout << "You found a weapon!" << endl;
            wep = true;
            ARR[x][y] = '.';
            changeDir(dir,ARR,x,y);
            break;
        case 'O':
            cout << "You just sparta'd yourself into a pit" << endl;
            alive = false;
            break;
        case '-':
            ARR[x][y] = '.';
            changeDir(dir,ARR,x,y);
            break;
        case '.':
            ARR[x][y] = '.';
            changeDir(dir,ARR,x,y);
            break;

    }
    cout << "-----------------------------------------------------" << endl;
    ARR[x][y] = '@';
}
bool checkGMax(int gp,int gMax)
{
    if(gp==gMax)
        return true;
    return false;
}



int main()
{
    int x,y;
    int n = SIZE+1;
    int wumpus = (SIZE-2)*(SIZE-2)*.2;                    //20% wumpus
    int pit = (SIZE-2)*(SIZE-2)*.05;                      //5% pit
    int gold = (SIZE-2)*(SIZE-2)*.15;                     //15% gold
    int weapon = (SIZE-2)*(SIZE-2)*.15;                   //15% weapon

    int gMax = gold*500;
    int gp = 0;                                           //gold count
    bool wep = false;                                     //weapon status
    bool alive = true;                                    //if not alive, end game
    bool winner = false;
    char dir;
    bool wum=false,pitt=false,gol=false,bow=false;


    //create array of pointers
    char *ARR[n];
    for(int i=0; i<n; i++)
        ARR[i] = new char[n];

    //change to 2-d Array
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            ARR[i][j] = '-';

    //create edges
    for(int i=1; i<n; i++)
        for(int j=1; j<n; j++)
            if(i==1 || i==n-1 || j==1 || j==n-1)    //Add borders #
                ARR[i][j] = '#';

    setStart(ARR,x,y,n);
    setPosition(wumpus, ARR, 'W',n);
    setPosition(pit, ARR, 'O',n);
    setPosition(gold, ARR, '$',n);
    setPosition(weapon, ARR, '}',n);

/*  Prints out the cheat key
    cout << "****************" << endl;
    cout << "Cheat Key" << endl;
    printArray(ARR,n);
    cout << "****************" << endl;
*/
    cout << endl;
    cout << "WUMPUS CAVE TREASURE HUNT" << endl << endl;
    cout << "OBJECTIVE:                                                    " << endl;
    cout << "You are a hero looking for treasure. Unarmed, You made your   " << endl;
    cout << "way to the dangerous Wumpus Cave. Inside, it is filled with   " << endl;
    cout << "powerful beasts ready to eat anything and anyone.             " << endl;
    cout << "Are you up for an adventure?                                  " << endl;
    cout << endl;
    cout << "INSTRUCTIONS:                                                 " << endl;
    cout << "N - move north, S - move south, W - move west, E - move east  " << endl;
    cout << "(Controls are NOT case-sensitive)                             " << endl;
    cout << "Click enter after your movement selection.                    " << endl;

    cout << endl;

    cout << "The adventure begins now!" << endl << endl;

    view(x,y, ARR, wum,pitt,gol,bow);
    status(wep,gp);

    while(true)
    {
        cout << "Enter your next move (N, S, E, W)     Your Move: ";
        cin >> dir;
        cout << endl << "*****************************************************" << endl << endl;

        move(ARR,x,y,gp,wep,alive,dir);
        if(alive==false)
            break;
        if(checkGMax(gp,gMax))
        {
            winner=true;
            break;
        }
        view(x,y,ARR,wum,pitt,gol,bow);
        status(wep,gp);

    }

    if(winner)
    {
        cout << "*************" << endl;
        cout << "   WINNER!" << endl;
        cout << " Looted Cave" << endl;
        cout << " Gold = " << gp << endl;
        cout << "*************" << endl;

        return 0;
    }

    cout << endl;
    cout << "*************" << endl;
    cout << "  GAME OVER" << endl;
    cout << "  Gold: " <<  gp << endl;
    cout << "*************" << endl;

    return 0;
}
