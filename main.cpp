#include <iostream>
#include <ncurses.h>
#include <time.h>

const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char NO_ONE = 'N';

class TicTacToe
{
private:
    bool invalidpos;
    char *grid;
    char turn; // Who's turn is it now
    void InitGraphics()
    {
        // Using ncurses lib.
        initscr(); // init curses mode (allocs mem for present window, stdscr)
        cbreak();  // read character without having to press ENTER (disable line buffering)
    }
    void TearDownGraphics() { endwin(); /* end curses mode. Failure to do so will result in weird terminal behaviour.*/ }
    void DrawBoard()
    {
        clear(); // Clear screen.
        addstr("Welcome to TIC TAC TOE\n\n");

        // Print grid
        printw("\t%c | %c | %c\n", grid[0], grid[1], grid[2]);
        printw("\t---------\n");
        printw("\t%c | %c | %c\n", grid[3], grid[4], grid[5]);
        printw("\t---------\n");
        printw("\t%c | %c | %c\n", grid[6], grid[7], grid[8]);
        addch('\n');
    }
    int AskNumber()
    {
        int inp = GetNumber();
        while (Invalid(inp))
        {
            addstr(" /!\\ Number must be between 1-9 AND that square must not be occupied already. Try again.\n");
            inp = GetNumber();
        }
        return inp;
    }
    int GetNumber()
    {
        printw("%c, where will you move? (1-9): ", turn);
        refresh(); // Draw.
        int number = getch() - '0';
        return number - 1; // 0-based array
    }
    bool Invalid(int pos)
    {
        return pos < 0 || pos > 8 || grid[pos] != EMPTY;
    }
    void HumanMove(int pos)
    {
        grid[pos] = X;
    }
    void MachineMove(int pos)
    {
        grid[pos] = O;
    }
    void SwapTurns()
    {
        turn = turn == X ? O : X;
    }
    char Winner()
    {
        // all possible Winning Rows
        const int WR[8][3] = {{0, 1, 2},
                              {3, 4, 5},
                              {6, 7, 8},
                              {0, 3, 6},
                              {1, 4, 7},
                              {2, 5, 8},
                              {0, 4, 8},
                              {2, 4, 6}};

        for (int i = 0; i < 8; i++)
            if (grid[WR[i][0]] == X && grid[WR[i][1]] == X && grid[WR[i][2]] == X || grid[WR[i][0]] == O && grid[WR[i][1]] == O && grid[WR[i][2]] == O)
                // we have a winner: return who
                return WR[i][0];

        return NO_ONE;
    }
    void AnnounceWinner()
    {
        printw("\n\n%c wins!!!", turn);
        refresh();
    }
    char PlayGame()
    {
        InitBoard();
        DrawBoard();
        while (Winner() == NO_ONE)
        {
            int position = AskNumber();
            if (turn == X)
                HumanMove(position);
            else
                MachineMove(position);

            DrawBoard();
            SwapTurns();
        }

        AnnounceWinner();

        return AskForAnotherGame();
    }
    char AskForAnotherGame()
    {
        addstr("\n\nPress 'q' to quit. Press anything else to play again.");
        refresh();
        return getch();
    }
    void InitBoard()
    {
        for (int i = 0; i < 9; i++)
            grid[i] = EMPTY;
    }

public:
    TicTacToe()
    {
        InitGraphics();

        // Create grid and initialise it
        grid = new char[9];

        // Randomise whether human or machine starts
        srand(time(NULL));
        turn = rand() % 2 == 0 ? X : O;
    }
    ~TicTacToe()
    {
        delete grid;
        TearDownGraphics();
    }
    void Run()
    {
        int input;
        do
        {
            input = PlayGame();
        } while (input != 'q' && input != 'Q');
    }
};

int main()
{
    TicTacToe game;
    game.Run();
}