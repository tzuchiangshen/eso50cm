#include "myNCurses.h"
 
/**
 * Constructor
 */
myNcurses::myNcurses( void ) {
    ncols = 80;
    nrows = 24;

    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad( stdscr, TRUE );
    nodelay( stdscr, TRUE );

    clear();
    getmaxyx( stdscr, nrows, ncols );
    if( ncols < 80 || nrows < 24 ){
        endwin();
        printf( "[myNcurses::initScreen] Ventana muy pequena\n");
        printf( "[myNcurses::initScreen] ncosl=%d nrows=%d\n\n", ncols, nrows);
        printf( "[myNcurses::initScreen] Tamano minimo\n");
        printf( "[myNcurses::initScreen] ncosl=%d nrows=%d\n", 80, 24);
        printf( "[myNcurses::initScreen] Hasta la vista...\n");
    } else {
        initializePanelsData();
        for( int i = 0; i < NUMBER_PANELS; i ++ ){
            p[i] = mkpanel( pData[i].color, 
                            pData[i].y_max,  
                            pData[i].x_max, 
                            pData[i].uly, 
                            pData[i].ulx );
            fill_panel( p[i], pData[i].Nombre, pData[i].Fn );
            //set_panel_userptr( p[i], "pan0" );
        }
    }
    
    m_top_panel = 0;
    m_while = false;
    m_new_line = false;
    m_r = 2;
    m_c = 1;
}

/**
 * Destructor
 */
myNcurses::~myNcurses( void ) {
    for( int i = 0; i < NUMBER_PANELS; i ++ ){
        rmpanel(p[i]);
    }
    pflush();
    erase();
    endwin();
}

/**
 * mkpanel
 */
PANEL * myNcurses::mkpanel( int color, int rows, int cols, int tly, int tlx ){
    WINDOW * win;
    PANEL * pan = 0;
    int fg, bg;

    if ((win = newwin(rows, cols, tly, tlx)) != 0) {
        if ((pan = new_panel(win)) == 0) {
            delwin(win);
        } else if (has_colors()) {
            //fg = color;
            //bg = (color == COLOR_BLUE) ? COLOR_WHITE : COLOR_BLACK;
            bg = color;
            fg = (color == COLOR_BLUE) ? COLOR_WHITE : COLOR_BLACK;
            init_pair(color, fg, bg);
            wbkgdset(win, COLOR_PAIR(color) );
        } else {
            wbkgdset(win, A_BOLD | ' ');
        }
    }
    return pan;
}                                /* end of mkpanel */


/** 
 * fill_panel
 */
void myNcurses::fill_panel( PANEL * pan, char Titulo[], char Fn[] ){
    WINDOW * win = panel_window(pan);
    int y_max, x_max;
    //int y, x;
    int l;

    y_max= getmaxy(win);
    x_max= getmaxx(win);

    wclear( win );
    box(win, 0, 0);

    l= strlen( Titulo );
    if( l > 0 ){
        wmove(win, 0, (x_max-l-2)/2);
        wprintw(win, "%s", Titulo);
    }
    
}                                /* end of fill_panel */

/** 
 * rmpanel
 */
void myNcurses::rmpanel( PANEL * pan ) {
    WINDOW * win = panel_window( pan );
    del_panel(pan);
    delwin(win);
}                                /* end of rmpanel */

/**
 * pflush
 */
void myNcurses::pflush( void ){
    update_panels();
    doupdate();
}                                /* end of pflush */


/**
 * initializePanelsData
 */
void myNcurses::initializePanelsData( void ) {
    strcpy( pData[0].Nombre, "T C S" );
    strcpy( pData[0].Fn, "   " );
    pData[0].y_max = 3;        
    pData[0].x_max = ncols;
    pData[0].uly   = 0;        
    pData[0].ulx   = 0;
    pData[0].color = COLOR_BLUE;
    pData[0].topscr= 1;

    strcpy( pData[1].Nombre, "Input" );
    strcpy( pData[1].Fn, "OUT" );
    pData[1].y_max = 4;        pData[1].x_max = ncols;
    pData[1].uly   = nrows-4;  pData[1].ulx   = 0;
    pData[1].color = COLOR_BLUE;
    pData[1].topscr= 1;

    strcpy( pData[2].Nombre, "Time" );
    strcpy( pData[2].Fn, "TIM" );
    pData[2].y_max = 5;        
    pData[2].x_max = ncols;
    pData[2].uly   = 3;        
    pData[2].ulx   = 0;
    pData[2].color = COLOR_BLUE;
    pData[2].topscr= 1;

    strcpy( pData[3].Nombre, "Alpha" );
    strcpy( pData[3].Fn, "ENC" );
    pData[3].y_max = nrows-12; 
    pData[3].x_max = ncols;
    pData[3].uly   = 8;        
    pData[3].ulx   = 0;
    pData[3].color = COLOR_BLUE;
    pData[3].topscr= 0;


    strcpy( pData[4].Nombre, "Delta" );
    strcpy( pData[4].Fn, "ENC" );
    pData[4].y_max = nrows-12; 
    pData[4].x_max = ncols;
    pData[4].uly   = 8;        
    pData[4].ulx   = 0;
    pData[4].color = COLOR_BLUE;
    pData[4].topscr= 0;

    strcpy( pData[5].Nombre, "Position" );
    strcpy( pData[5].Fn, "POS" );
    pData[5].y_max = nrows-12; 
    pData[5].x_max = ncols;
    pData[5].uly   = 8;        
    pData[5].ulx   = 0;
    pData[5].color = COLOR_BLUE;
    pData[5].topscr= 1;
}

/**
 * DibujaPanelHora
 */

void myNcurses::DibujaPanelHora( void ) {
    WINDOW * win = panel_window(  p[pT] );
    int y_max, x_max;
    //int y, x;

    y_max= getmaxy( win );
    x_max= getmaxx( win );

    wmove( win, 2, 1 );
    whline( win, ACS_HLINE, x_max-2 );

    wmove( win, 1, 13 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 24 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 41 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 55 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 68 );
    wvline( win, ACS_VLINE, y_max-2 );

    wmove( win, 0, 13 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 24 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 55 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 68 );
    waddch( win, ACS_TTEE );

    wmove( win, 2, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 2, 13 );
    waddch( win, ACS_PLUS );
    wmove( win, 2, 24 );
    waddch( win, ACS_PLUS );
    wmove( win, 2, 41 );
    waddch( win, ACS_PLUS );
    wmove( win, 2, 55 );
    waddch( win, ACS_PLUS );
    wmove( win, 2, 68 );
    waddch( win, ACS_PLUS );
    wmove( win, 2, x_max-1 );
    waddch( win, ACS_RTEE );

    wmove( win, 4, 13 );
    waddch( win, ACS_BTEE );
    wmove( win, 4, 24 );
    waddch( win, ACS_BTEE );
    wmove( win, 4, 41 );
    waddch( win, ACS_BTEE );
    wmove( win, 4, 55 );
    waddch( win, ACS_BTEE );
    wmove( win, 4, 68 );
    waddch( win, ACS_BTEE );


    wmove( win, 1,  1); wprintw(win, "   U Date" );
    wmove( win, 1, 14); wprintw(win, "  U Time" );
    wmove( win, 1, 25); wprintw(win, "     Julian" );
    wmove( win, 1, 42); wprintw(win, "   Sideral" );
    wmove( win, 1, 56); wprintw(win, "   L Date" );
    wmove( win, 1, 69); wprintw(win, "  L Time" );
    
} 

/**
 * DibujaPanelAlpha
 */
void myNcurses::DibujaPanelAlpha( void ) {
    WINDOW * win = panel_window( p[pA] );
    int y_max, x_max;

    y_max= getmaxy( win );
    x_max= getmaxx( win );

    wmove( win, 3, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 5, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 7, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 9, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    
    wmove( win, 1,  9 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 19 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 29 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 39 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 49 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 59 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 69 );
    wvline( win, ACS_VLINE, y_max-2 );


    wmove( win, 0,  9 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 19 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 29 );
    waddch( win, ACS_TTEE );
    //wmove( win, 0, 39 );
    //waddch( win, ACS_TTEE );
    wmove( win, 0, 49 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 59 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 69 );
    waddch( win, ACS_TTEE );

    wmove( win, y_max-1, 9 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 19 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 29 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 39 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 49 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 59 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 69 );
    waddch( win, ACS_BTEE );

    wmove( win, 3, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 5, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 7, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 9, 0 );
    waddch( win, ACS_LTEE );

    wmove( win, 3, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 5, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 7, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 9, x_max-1 );
    waddch( win, ACS_RTEE );
    
    wmove( win, 3,  9 );
    waddch( win, ACS_PLUS );
    wmove( win, 5,  9 );
    waddch( win, ACS_PLUS );
    wmove( win, 7,  9 );
    waddch( win, ACS_PLUS );
    wmove( win, 9,  9 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 19 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 19 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 19 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 19 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 29 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 29 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 29 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 29 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 39 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 39 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 39 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 39 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 49 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 49 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 49 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 49 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 59 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 59 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 59 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 59 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 69 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 69 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 69 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 69 );
    waddch( win, ACS_PLUS );


    
    wmove( win, 2,  9+5 );
    wprintw(win, "0" );
    wmove( win, 2, 19+5 );
    wprintw(win, "1" );
    wmove( win, 2, 29+5 );
    wprintw(win, "2" );
    wmove( win, 2, 39+5 );
    wprintw(win, "3" );
    wmove( win, 2, 49+5 );
    wprintw(win, "4" );
    wmove( win, 2, 59+5 );
    wprintw(win, "5" );
    wmove( win, 2, 69+5 );
    wprintw(win, "-" );

    wmove( win, 4, 1 );
    wprintw(win, "MOTOR" );
    wmove( win, 6, 1 );
    wprintw(win, " WORM" );
    wmove( win, 8, 1 );
    wprintw(win, " AXIS" );
    wmove( win,10, 1 );
    wprintw(win, " - " );
    
} 
/**
 * DibujaPanelDelta
 */
void myNcurses::DibujaPanelDelta( void ) {
        WINDOW * win = panel_window( p[pD] );
    int y_max, x_max;

    y_max= getmaxy( win );
    x_max= getmaxx( win );

    wmove( win, 3, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 5, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 7, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 9, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    
    wmove( win, 1,  9 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 19 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 29 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 39 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 49 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 59 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 69 );
    wvline( win, ACS_VLINE, y_max-2 );


    wmove( win, 0,  9 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 19 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 29 );
    waddch( win, ACS_TTEE );
    //wmove( win, 0, 39 );
    //waddch( win, ACS_TTEE );
    wmove( win, 0, 49 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 59 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 69 );
    waddch( win, ACS_TTEE );

    wmove( win, y_max-1, 9 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 19 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 29 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 39 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 49 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 59 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 69 );
    waddch( win, ACS_BTEE );

    wmove( win, 3, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 5, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 7, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 9, 0 );
    waddch( win, ACS_LTEE );

    wmove( win, 3, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 5, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 7, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 9, x_max-1 );
    waddch( win, ACS_RTEE );
    
    wmove( win, 3,  9 );
    waddch( win, ACS_PLUS );
    wmove( win, 5,  9 );
    waddch( win, ACS_PLUS );
    wmove( win, 7,  9 );
    waddch( win, ACS_PLUS );
    wmove( win, 9,  9 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 19 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 19 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 19 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 19 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 29 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 29 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 29 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 29 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 39 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 39 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 39 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 39 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 49 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 49 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 49 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 49 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 59 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 59 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 59 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 59 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 69 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 69 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 69 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 69 );
    waddch( win, ACS_PLUS );


    
    wmove( win, 2,  9+5 );
    wprintw(win, "0" );
    wmove( win, 2, 19+5 );
    wprintw(win, "1" );
    wmove( win, 2, 29+5 );
    wprintw(win, "2" );
    wmove( win, 2, 39+5 );
    wprintw(win, "3" );
    wmove( win, 2, 49+5 );
    wprintw(win, "4" );
    wmove( win, 2, 59+5 );
    wprintw(win, "5" );
    wmove( win, 2, 69+5 );
    wprintw(win, "-" );

    wmove( win, 4, 1 );
    wprintw(win, "MOTOR" );
    wmove( win, 6, 1 );
    wprintw(win, " WORM" );
    wmove( win, 8, 1 );
    wprintw(win, " AXIS" );
    wmove( win,10, 1 );
    wprintw(win, " - " );
}


/**
 * DibujaPanelPosicion
 */
void myNcurses::DibujaPanelPosicion( void ) {
    WINDOW * win = panel_window( p[pP] );
    int y_max, x_max;
    //int y, x;

    y_max= getmaxy( win );
    x_max= getmaxx( win );

    wmove( win, 3, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 5, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 7, 1 );
    whline( win, ACS_HLINE, x_max-2 );
    wmove( win, 9, 1 );
    whline( win, ACS_HLINE, x_max-2 );

    wmove( win, 1, 15 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 31 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 47 );
    wvline( win, ACS_VLINE, y_max-2 );
    wmove( win, 1, 63 );
    wvline( win, ACS_VLINE, y_max-2 );


    wmove( win, 0, 15 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 31 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 47 );
    waddch( win, ACS_TTEE );
    wmove( win, 0, 63 );
    waddch( win, ACS_TTEE );

    wmove( win, y_max-1, 15 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 31 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 47 );
    waddch( win, ACS_BTEE );
    wmove( win, y_max-1, 63 );
    waddch( win, ACS_BTEE );

    wmove( win, 3, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 5, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 7, 0 );
    waddch( win, ACS_LTEE );
    wmove( win, 9, 0 );
    waddch( win, ACS_LTEE );

    wmove( win, 3, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 5, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 7, x_max-1 );
    waddch( win, ACS_RTEE );
    wmove( win, 9, x_max-1 );
    waddch( win, ACS_RTEE );

    wmove( win, 3, 15 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 15 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 15 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 15 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 31 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 31 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 31 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 31 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 47 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 47 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 47 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 47 );
    waddch( win, ACS_PLUS );

    wmove( win, 3, 63 );
    waddch( win, ACS_PLUS );
    wmove( win, 5, 63 );
    waddch( win, ACS_PLUS );
    wmove( win, 7, 63 );
    waddch( win, ACS_PLUS );
    wmove( win, 9, 63 );
    waddch( win, ACS_PLUS );

    wmove( win, 2, 16 );
    wprintw(win, "      RA" );
    wmove( win, 2, 32 );
    wprintw(win, "      DEC" );
    wmove( win, 2, 48 );
    wprintw(win, "      HA" );

    wmove( win, 4, 1 );
    wprintw(win, "CURRENT" );
    wmove( win, 6, 1 );
    wprintw(win, "TARGET" );
    wmove( win, 8, 1 );
    wprintw(win, "DIFFERENCE" );
    
} 


/**
 * ActualizaPanelPrincipal
 */
void myNcurses::ActualizaPanelPrincipal( void ) {
    WINDOW * win  = panel_window( p[0] );
    int y_max, x_max;
    //int y, x;
    
    y_max= getmaxy(win);
    x_max= getmaxx(win);

    for( int i = 1; i < 6; i++ ) {
        if( i == m_top_panel ){
            wattron( win, A_BOLD );
            mvwprintw( win, 1, 4 + (i-1)*9, "[F%d %s]", i, pData[i].Fn );
            wattroff( win, A_BOLD );
        } else {
            mvwprintw( win, 1, 4 + (i-1)*9, "[F%d %s]", i, pData[i].Fn );
        }
    }
    
}

/**
 * ActualizaPanelHora
 */
void myNcurses::ActualizaPanelHora( char * buffer ) {
    if( ! panel_hidden( p[pT] ) ) {
        WINDOW * win = panel_window( p[pT] );
        //top_panel( p[2] );
        int y, x;
        char delim[] = ";#";
        char * ptr;

        //Udate
        //strcpy( Hr[0].Nombre, "UDate" );
        x = 1;
        y = 3;
        ptr = strtok( & buffer[2], delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );


        //strcpy( Hr[1].Nombre, "UTime" );
        x = 14;
        y = 3;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //strcpy( Hr[4].Nombre, "LDate" );
        x = 56;
        y = 3;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //strcpy( Hr[5].Nombre, "LTime" );
        x = 69;
        y = 3;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //strcpy( Hr[3].Nombre, "Sideral" );
        x = 45;
        y = 3;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //strcpy( Hr[2].Nombre, "Julian" );
        x = 25;
        y = 3;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );
        wrefresh( win );
    }
}

/**
 * ActualizaPanelAlpha
 */
void myNcurses::ActualizaPanelAlpha( char * buffer ) {
    WINDOW * win;   // = panel_window(  p[pA] );
    int y, x;
    char delim[] = ";#";
    char * ptr;
   
    win = panel_window(  p[pA] );

    //  (1)  alpha motor
    y = 4;
    x = 50;
    ptr = strtok( & buffer[2], delim ); 
    wmove( win, y, x );
    wprintw( win, ptr );

    //  (2) alpha worm
    y = 6;
    x = 50;
    ptr = strtok( 0, delim ); 
    wmove( win, y, x );
    wprintw( win, ptr );
    
    //  (3) alpha axis
    y = 8;
    x = 50;
    ptr = strtok( 0, delim ); 
    wmove( win, y, x );
    wprintw( win, ptr );
    
    if( ! panel_hidden( p[pA] ) ) {
        wrefresh( win );
    }

    win = panel_window(  p[pD] );
    
    //  (4) delta motor
    y = 4;
    x = 50;//33;
    ptr = strtok( 0, delim ); 
    wmove( win, y, x );
    wprintw( win, ptr );

    //  (5) delta worm
    y = 6;
    x = 50;
    ptr = strtok( 0, delim ); 
    wmove( win, y, x );
    wprintw( win, ptr );
    
    //  (6) delta axis
    y = 8;
    x = 50;
    ptr = strtok( 0, delim ); 
    wmove( win, y, x );
    wprintw( win, ptr );
    
    if( ! panel_hidden( p[pD] ) ) {
        wrefresh( win );
    }
}

/**
 * ActualizaPanelPosicion
 */
void myNcurses::ActualizaPanelPosicion( char * buffer ) {
    if( ! panel_hidden( p[pP] ) ) {
        WINDOW * win = panel_window(  p[pP] );
        int y, x;
        char delim[] = ";#";
        char * ptr;
    
        //  (1)  m_currentRA
        //mvwprintw( win, 4, 17, "%s", Curr_Ra.string );
        y = 4;
        x = 17;
        ptr = strtok( & buffer[2], delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //  (2) m_currentHA
        y = 4;
        x = 49;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );
    
        //  (3) m_differenceRA
        y = 8;
        x = 17;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //  (4) m_differenceHA
        y = 8;
        x = 49;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //  (5) m_targetRA 
        y = 6;
        x = 17;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        
        //  (6) m_targetHA
        y = 6;
        x = 49;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        // (7)  m_currentDec 
        y = 4;
        x = 33;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //  (8) m_differenceDec
        y = 8;
        x = 33;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        //  (9) m_targetDec
        y = 6;
        x = 33;
        ptr = strtok( 0, delim ); 
        wmove( win, y, x );
        wprintw( win, ptr );

        wrefresh( win );
    }
}



/**
 * ActualizaPanelInput
 */
void myNcurses::ActualizaPanelInput( char * buffer ) {
    WINDOW * win = panel_window( p[pI] );
    
    //sprintf( buff,  "in>%s(%d)", buffer, strlen(buffer) );
    //wPrintW( win, 1, 1, buff );
    wmove( win, 1, 1 );
    wprintw( win, "rx> %s", buffer );  
    wrefresh( win );
}

/**
 * ProcesaInputTeclado
 */
int myNcurses::ProcesaInputTeclado( int ch, char * buffer ) {
    WINDOW * win = panel_window( p[pI] );
    
    wmove( win, m_r, m_c );
    
    static int    l, l_old;
    int           i;
    char          buff[256];
    
    int new_line = 0 ;
    if( 31 < ch && ch < 128 || ch == '\n' ) {
        switch( ch ){
            case '\n':
                new_line= true;
                buff[0]='\0';
                for( i= 0; i< l_old + 10; i++ ){
                    strcat( buff, " " );
                }
                //wPrintW( win, 1, 1, buff );
                wmove( win, 1, 1 );
                wprintw( win, "%s", buff );
                sprintf( buff,  "in>%s(%d)", buffer, strlen(buffer) );
                //wPrintW( win, 1, 1, buff );
                wmove( win, 1, 1 );
                wprintw( win, "%s", buff );
                pflush();
                l_old= strlen( buff );
                for( i= 1; i< 1+l; i++ ){
                    wmove( win, m_r, i );
                    waddch( win, ' ' );
                    wrefresh( win );
                }
                (m_c) = 1;
                l    = 0;
                wmove( win, m_r, m_c );
                wrefresh( win );
                break;
            default:
                buffer[l]  = ch;
                buffer[l+1]='\0';
                wdelch( win );
                winsch( win, ch );
                refresh();
                (m_c) ++;
                l++;
                wmove( win, m_r, m_c );
                wrefresh( win );
                break;
        }                       // end switch
    } else {
        switch( ch ) {
            case 4:             //KEY_LEFT
                if( l > 0 ) {
                    l--;
                    buffer[l]= 0;
                    m_c --;
                    wmove( win, m_r, m_c );
                    wdelch( win );
                    winsch( win, ' ' );
                    wrefresh( win );
                }
                break;
            case 3:             //KEY_UP
                wmove( win, 1, 1 );
                wprintw( win, "key> KEY_UP pressed", ch );  
                wrefresh( win );
                break;
            case 2:             //KEY_DOWN
                wmove( win, 1, 1 );
                wprintw( win, "key> KEY_DOWN pressed", ch );  
                wrefresh( win );
                break;  
            case 5:             //KEY_RIGHT
                wmove( win, 1, 1 );
                wprintw( win, "key> KEY_RIGHT pressed", ch );  
                wrefresh( win );
                break;
            default:
                wmove( win, 1, 1 );
                wprintw( win, "key> %d", ch );  
                wrefresh( win );
        }
    }
    return new_line;
}

/*
 * getWhile
 *
int myNcurses::getWhile( void ) {
    return m_while;
}
*/


/**
 * topPanel
 */
void myNcurses::topPanel( int i ) {
    m_top_panel = i;
    top_panel( p[m_top_panel] );
}

/**
 * showPanel
 */
void myNcurses::showPanel( int i ) {
    show_panel( p[i] );
}

/**
 * hidePanel
 */
void myNcurses::hidePanel( int i ) {
    hide_panel( p[i] );
}



/*
void myNcurses::topPanel( void ) {
    top_panel( p[m_top_panel] );
}
*/

/**
 * refreshPanel
 */
void myNcurses::refreshPanels( void ) {
    WINDOW * win;
    for( int i = 2; i < NUMBER_PANELS; i ++ ) {
        //if( pData[i].topscr ) {
            top_panel( p[i] );
            win = panel_window( p[i] );
            wrefresh( win );
        //}
    }
    
}


/**
 * bottomPanel
 */
void myNcurses:: bottomPanel( int i ) {
    m_bottom_panel = i;
    bottom_panel( p[i] );
}