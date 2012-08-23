
/*******************************************************************
** HighlightCodeV3.2 software by yzfy(雨中飞燕) http://yzfy.org/c **
*******************************************************************/
#include "stdafx.h"
#define MAX_PLY 4
#define VERSION "1.0"

#define SIZE_X 9
#define SIZE_Y 10
#define BOARD_SIZE SIZE_X*SIZE_Y

#define MOVE_STACK 4096
#define HIST_STACK 50

#define EMPTY 7
#define DARK 0
#define LIGHT 1

#define PAWN 0
#define BISHOP 1
#define ELEPHAN 2
#define KNIGHT 3
#define CANNON 4
#define ROOK 5
#define KING 6

#define INFINITY 20000
#define NORMAL 0
#define SELECT 1

typedef struct
{
    int from, dest;
} move;

typedef struct
{
    move m;
} gen_rec;

typedef struct
{
    move m;
    int capture;
} hist_rec;


/*
VSCCP - Very Simple Chinese Chess Program
Written by Pham Hong Nguyen
Faculty of Information Technology - Vietnam National University, Hanoi
Office: 334 Nguyen Trai street, Thanh Xuan District, Hanoi, Vietnam
Home page: http://www.Geocities.com/SiliconValley/Grid/6544/
Email : phhnguyen@yahoo.com

Version:
-> 1.0 - basic
1.1 - add opening book
1.2 - improve eval function
1.3 - improve alphabeta search by adding iterative deepening,
quiescence, principal variation, history, killer heuristics
1.4 - add new rules and interface for tournament between 2 programs
A tournament can be controled by ROCC (Referee of Chinese Chess)
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
//#include "defs10.h"


/* the board representation && the initial board state */
char color[BOARD_SIZE]=
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 0, 7, 7, 7, 7, 7, 0, 7,
    0, 7, 0, 7, 0, 7, 0, 7, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    1, 7, 1, 7, 1, 7, 1, 7, 1,
    7, 1, 7, 7, 7, 7, 7, 1, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    1, 1, 1, 1, 1, 1, 1, 1, 1
};

char piece[BOARD_SIZE]=
{
    5, 3, 2, 1, 6, 1, 2, 3, 5,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 4, 7, 7, 7, 7, 7, 4, 7,
    0, 7, 0, 7, 0, 7, 0, 7, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    0, 7, 0, 7, 0, 7, 0, 7, 0,
    7, 4, 7, 7, 7, 7, 7, 4, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7,
    5, 3, 2, 1, 6, 1, 2, 3, 5
};

/* For getting information */
unsigned long nodecount, brandtotal = 0, gencount = 0;
char ply, side, xside, computerside;
move newmove;
gen_rec gen_dat[MOVE_STACK];
short gen_begin[HIST_STACK], gen_end[HIST_STACK];
hist_rec hist_dat[HIST_STACK];
short hdp;

volatile unsigned long *systicks=(unsigned long *)0x46C; /* Clock counter */
unsigned long tickstart, tickend;

void clrscr()
{
    system("cls");
}
int textcolor(short iColor)
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    GetConsoleScreenBufferInfo(hd, &csbInfo);
    return SetConsoleTextAttribute(hd, iColor|(csbInfo.wAttributes&~0xF));
}

int textbackground(short iColor)
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    GetConsoleScreenBufferInfo(hd, &csbInfo);
    return SetConsoleTextAttribute(hd, (iColor<<4)|(csbInfo.wAttributes&~0xF0));
}

int gotoxy(int x, int y)
{
    COORD cd;
    cd.X = x;cd.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cd);
}

/***** INTERFACE *****/
void MoveTo(short x, short y)
{
    gotoxy (5*x+1, 2*y+2);
}

void DrawCell(short pos, short mode)
{
    //char piece_char[] = "PBENCRK+"; /*"TSVMPXT+"; /* the piece letters */
    char piece_char[] = "兵仕象马炮车将+"; /*"TSVMPXT+"; /* the piece letters */

    if (color[pos]==DARK) textcolor(13);
    else if (color[pos]==LIGHT) textcolor(14); else textcolor(7);
    if (mode==NORMAL) textbackground(0); else textbackground(1);
    MoveTo(pos%9, pos/9);
    printf("%c", piece_char[piece[pos]<<1]);
    if(piece[pos]<7)
    {
        printf("%c", piece_char[(piece[pos]<<1)+1]);
    }
}


void DrawBoard(void)
{
    int i;
    textcolor(7); textbackground(0); clrscr(); gotoxy(1,1);
    printf("\n");
    printf(" +----+----+----+----+----+----+----+----+ 9\n");
    printf(" |    |    |    |  \\ | /  |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 8\n");
    printf(" |    |    |    |  / | \\  |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 7\n");
    printf(" |    |    |    |    |    |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 6\n");
    printf(" |    |    |    |    |    |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 5\n");
    printf(" |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 4\n");
    printf(" |    |    |    |    |    |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 3\n");
    printf(" |    |    |    |    |    |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 2\n");
    printf(" |    |    |    |  \\ | /  |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 1\n");
    printf(" |    |    |    |  / | \\  |    |    |    |\n");
    printf(" +----+----+----+----+----+----+----+----+ 0\n");
    printf(" A    B    C    D    E    F    G    H    I\n");
    gotoxy(1,25); printf("Up, Down, Right, Left: cursor move Enter or Space: select ESC: escape");
    gotoxy(48,1); printf("VERY SIMPLE CHINESE CHESS PROGRAM");
    gotoxy(50,2); printf("(C) Pham Hong Nguyen. Ver %s", VERSION);

    for (i=0; i<BOARD_SIZE; i++) DrawCell(i, NORMAL);
}


void Gen(void);

short GetHumanMove(void)
{
    static short x = 4, y = 5;
    short ch, from, t, i, selecting = 0;

    Gen(); /* for check legal move only */

    while (1)
    {
        MoveTo (x, y);
        ch = (short)getch();
        switch (ch)
        {
        case 13:
        case 32: /* Enter or Spacebar */
            t = x + y*SIZE_X;
            if (!selecting)
            {
                if (color[t]==side)
                {
                    selecting = 1; from = t; DrawCell(t, SELECT);
                }
            }
            else
            {
                if (t != from) DrawCell(from, NORMAL);
                if (color[t]==side)
                {
                    from = t; DrawCell(t, SELECT);
                }
                else

                {
                    newmove.from = from; newmove.dest = t;
                    for (i=gen_begin[ply]; i<gen_end[ply]; i++)
                        if (gen_dat[i].m.from==newmove.from && gen_dat[i].m.dest==newmove.dest) return 0;
                    DrawCell(from, SELECT);
                }
            }
            break;

        case 27: return 1; /* ESC */

        case 0xE0:
            ch = (short)getch();
            switch (ch)
            {
            case 75: if (x) x--; else x = SIZE_X-1; break; /* LEFT */
            case 77: if (x<SIZE_X-1) x++; else x = 0; break; /* RIGHT */
            case 72: if (y) y--; else y = SIZE_Y-1; break; /* UP */
            case 80: if (y<SIZE_Y-1) y++; else y = 0; break; /* DOWN */
            }
            break;
        } /* switch */
    } /* while */
}


/**** MOVE GENERATE ****/
short offset[7][8] =
{{-1, 1,13, 0, 0, 0, 0, 0}, /* PAWN {for DARK side} */
    {-12,-14,12,14,0,0,0,0}, /* BISHOP */
    {-28,-24,24,28, 0, 0, 0, 0 }, /* ELEPHAN */
    {-11,-15,-25,-27,11,15,25,27}, /* KNIGHT */
    {-1, 1,-13,13, 0, 0, 0, 0}, /* CANNON */
    {-1, 1,-13,13, 0, 0, 0, 0}, /* ROOK */
    {-1, 1,-13,13, 0, 0, 0, 0}
}; /* KING */

short mailbox182[182] =
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,
    -1,-1, 9,10,11,12,13,14,15,16,17,-1,-1,
    -1,-1,18,19,20,21,22,23,24,25,26,-1,-1,
    -1,-1,27,28,29,30,31,32,33,34,35,-1,-1,
    -1,-1,36,37,38,39,40,41,42,43,44,-1,-1,
    -1,-1,45,46,47,48,49,50,51,52,53,-1,-1,
    -1,-1,54,55,56,57,58,59,60,61,62,-1,-1,
    -1,-1,63,64,65,66,67,68,69,70,71,-1,-1,
    -1,-1,72,73,74,75,76,77,78,79,80,-1,-1,
    -1,-1,81,82,83,84,85,86,87,88,89,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
    };

short mailbox90[90] =
    {28, 29, 30, 31, 32, 33, 34, 35, 36,
    41, 42, 43, 44, 45, 46, 47, 48, 49,
    54, 55, 56, 57, 58, 59, 60, 61, 62,
    67, 68, 69, 70, 71, 72, 73, 74, 75,
    80, 81, 82, 83, 84, 85, 86, 87, 88,
    93, 94, 95, 96, 97, 98, 99,100,101,
    106, 107,108,109,110,111,112,113,114,
    119, 120,121,122,123,124,125,126,127,
    132, 133,134,135,136,137,138,139,140,
    145, 146,147,148,149,150,151,152,153
    };

short legalposition[90] =
    {1, 1, 5, 3, 3, 3, 5, 1, 1,
    1, 1, 1, 3, 3, 3, 1, 1, 1,
    5, 1, 1, 3, 7, 3, 1, 1, 5,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    9, 1,13, 1, 9, 1,13, 1, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9
    };

short maskpiece[7] = {8, 2, 4, 1, 1, 1, 2},
    knightcheck[8] = {1,-1,-9,-9,-1,1,9,9},
    elephancheck[8] = {-10,-8,8,10,0,0,0,0},
    kingpalace[9] = {3,4,5,12,13,14,21,22,23};

void InitGen(void)
{
    gen_begin[0] = 0; ply = 0; hdp = 0;
}

short KingFace(short from, short dest)
{
    short i, k, t, r = 0;
    i = from % SIZE_X;
    if (i>=3 && i<=5 && piece[dest]!=KING)
    {
        t = piece[dest]; piece[dest] = piece[from]; piece[from] = EMPTY;
        i = 0;
        for (k=kingpalace[i]; piece[k]!=KING; k++) ;
        for (k += SIZE_X; k<BOARD_SIZE && piece[k]==EMPTY; k += SIZE_X);
        if (piece[k]==KING) r = 1;
        piece[from] = piece[dest]; piece[dest] = t;
    }
    return r;
}

void Gen_push(short from, short dest)
{
    if (!KingFace(from, dest))
    {
        gen_dat[gen_end[ply]].m.from = from;
        gen_dat[gen_end[ply]].m.dest = dest;
        gen_end[ply]++;
    }
}


void Gen(void)
{
    short i, j, k, n, p, x, y, t, fcannon;

    gen_end[ply] = gen_begin[ply];

    for (i=0; i < BOARD_SIZE; i++)
        if (color[i]==side)
        {
            p = piece[i];
            for (j=0; j<8; j++)
            {
                if (!offset[p][j]) break;
                x = mailbox90[i]; fcannon = 0;
                if (p==ROOK || p==CANNON) n = 9; else n = 1;
                for (k=0; k<n; k++)
                {
                    if (p==PAWN && side==LIGHT) x -= offset[p][j]; else x += offset[p][j];

                    y = mailbox182[x];
                    if (side == DARK) t = y; else t = 89-y;
                    if (y==-1 || (legalposition[t] & maskpiece[p])==0) break;
                    if (!fcannon)
                    {
                        if (color[y]!=side)
                            switch (p)
                            {
                            case KNIGHT: if (color[i+knightcheck[j]]==EMPTY) Gen_push(i, y); break;
                            case ELEPHAN:if (color[i+elephancheck[j]]==EMPTY) Gen_push(i, y); break;
                            case CANNON: if (color[y]==EMPTY) Gen_push(i, y); break;
                            default: Gen_push(i, y);
                            }
                        if (color[y]!=EMPTY) { if (p==CANNON) fcannon++; else break; }
                    }
                    else   /* CANNON switch */
                    {
                        if (color[y] != EMPTY)
                        {
                            if (color[y]==xside) Gen_push(i, y);
                            break;
                        }
                    }
                } /* for k */
            } /* for j */
        }

    gen_end[ply+1] = gen_end[ply]; gen_begin[ply+1] = gen_end[ply];
    brandtotal += gen_end[ply] - gen_begin[ply]; gencount++;
}


/***** MOVE *****/
short MakeMove(move m)
{
    short from, dest, p;
    nodecount++;
    from = m.from; dest = m.dest;
    hist_dat[hdp].m = m; hist_dat[hdp].capture = p = piece[dest];
    piece[dest] = piece[from]; piece[from] = EMPTY;
    color[dest] = color[from]; color[from] = EMPTY;
    hdp++; ply++; side = xside; xside = 1-xside;
    return p == KING;
}


void UnMakeMove(void)
{
    short from, dest;
    hdp--; ply--; side = xside; xside = 1-xside;
    from = hist_dat[hdp].m.from; dest = hist_dat[hdp].m.dest;
    piece[from] = piece[dest]; color[from] = color[dest];
    piece[dest] = hist_dat[hdp].capture;
    if (piece[dest] == EMPTY) color[dest] = EMPTY; else color[dest] = xside;
}


short UpdateNewMove(void)
{
    short from, dest, p;
    from = newmove.from; dest = newmove.dest; p = piece[dest];
    piece[dest] = piece[from]; piece[from] = EMPTY;
    color[dest] = color[from]; color[from] = EMPTY;
    DrawCell(from, NORMAL); DrawCell(dest, NORMAL);
    return p == KING;
}


/***** EVALUATE *****/
short Eval(void)
{
    static short piecevalue[7] = {10, 20, 20, 40, 45, 90, 1000};
    short i, s = 0;
    for (i=0; i<BOARD_SIZE; i++)
        if (color[i]==side) s += piecevalue[piece[i]];
        else if (color[i]==xside) s -= piecevalue[piece[i]];
    return s;
}


/***** SEARCH *****/
/* Search game tree by alpha-beta algorith */
short AlphaBeta(short alpha, short beta, short depth)
{
    short i, value, best;

    if (!depth) return Eval();

    Gen();
    best = -INFINITY;

    for (i=gen_begin[ply]; i<gen_end[ply] && best<beta; i++)
    {
        if (best > alpha) alpha = best;

        if (MakeMove(gen_dat[i].m)) value = 1000-ply;
        else value = -AlphaBeta(-beta, -alpha, depth-1);
        UnMakeMove();

        if (value > best)
        {
            best = value; if (!ply) newmove = gen_dat[i].m;
        }
    }

    return best;
}


/***** THINK *****/
/* Call AlphaBeta short && display some information */
void ComputerThink(void)
{
    short best;
    tickstart = clock(); nodecount = 0;
    best = AlphaBeta(-INFINITY, INFINITY, MAX_PLY);
    /* Display some information */
    tickend = clock(); textcolor(7);
    gotoxy(50, 4); printf("Depth : %d", MAX_PLY);
    gotoxy(50, 5); printf("Node total : %ld ", nodecount);
    gotoxy(50, 6); printf("Brand factor : %.2f ", (float)brandtotal/gencount);
    gotoxy(50, 7); printf("Time (second) : %.2f ", (tickend-tickstart)/1000.0);
    gotoxy(50, 8); printf("Nodes per second : %ld ", (long)(nodecount*1000.0/(tickend-tickstart+1)));
    gotoxy(50, 9); printf("Score : %d ", best);
    gotoxy(50,11); printf("Computer move : %c%d%c%d ",
            (char)(newmove.from%SIZE_X+65),
            (short)(SIZE_X-newmove.from/SIZE_X),
            (char)(newmove.dest%SIZE_X+65),
            (short)(SIZE_X-newmove.dest/SIZE_X));
}


/***** MAIN BLOCK *****/
int main(void)
{
    InitGen(); DrawBoard(); side = LIGHT; xside = DARK; computerside = DARK;

    do
    {
        if (side == computerside) ComputerThink();
        else if (GetHumanMove()) break;
        side = xside; xside = 1-xside;
    }
    while (!UpdateNewMove());
}


