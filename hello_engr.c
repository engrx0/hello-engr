#include <ncurses.h>

void print_border(int start_row, int start_col, int height, int width, int delay);

struct window_t
{
    int xo;
    int yo;
    int width;
    int height;
};

int main()
{
    initscr();
    curs_set(0);
    // cbreak();
    // noecho();

    int main_height = 24;
    int main_width = 80;
    print_border(0, 0, main_height, main_width, 10);
    print_border(2, 10, 4, 60, 10);
    print_border(8, 5, 15, 70, 10);


    getch();

    endwin(); // close ncurses

    return 0;
}

void print_border(int start_row, int start_col, int height, int width, int delay)
{
    int end_row = start_row + height;
    int end_col = start_col + width;

    // Draw top-left corner
    move(start_row, start_col);
    addch(ACS_ULCORNER);
    napms(delay);
    refresh();

    // Draw top horizontal line
    for (int col = start_col + 1; col < end_col; col++)
    {
        move(start_row, col);
        addch(ACS_HLINE);
        napms(delay);
        refresh();
    }

    // Draw top-right corner
    move(start_row, end_col);
    addch(ACS_URCORNER);
    napms(delay);
    refresh();

    // Draw right vertical line
    for (int row = start_row + 1; row < end_row; row++)
    {
        move(row, end_col);
        addch(ACS_VLINE);
        napms(delay);
        refresh();
    }

    // Draw bottom-right corner
    move(end_row, end_col);
    addch(ACS_LRCORNER);
    napms(delay);
    refresh();

    // Draw bottom horizontal line
    for (int col = end_col - 1; col > start_col; col--)
    {
        move(end_row, col);
        addch(ACS_HLINE);
        napms(delay);
        refresh();
    }

    // Draw bottom-left corner
    move(end_row, start_col);
    addch(ACS_LLCORNER);
    napms(delay);
    refresh();

    // Draw left vertical line
    for (int row = end_row - 1; row > start_row; row--)
    {
        move(row, start_col);
        addch(ACS_VLINE);
        napms(delay);
        refresh();
    }
}
