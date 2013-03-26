#ifndef _MYNCURSES_H_
#define _MYNCURSES_H_

#include <ncurses.h>
#include <panel.h>

#include <stdio.h>

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "degUtil.h"

#define NUMBER_PANELS   6
#define pI              1
#define pT              2
#define pA              3
#define pD              4
#define pP              5


struct PanelData {
  char Nombre[16];
  char Fn[8];
  int  uly, ulx;
  int  y_max, x_max;
  int  color;
  int  topscr;
};

class myNcurses {
    public:
        myNcurses( void );
        ~myNcurses( void );
        
        PANEL * mkpanel(int color, int rows, int cols, int tly, int tlx);
        void rmpanel( PANEL * pan );
        void fill_panel( PANEL * pan, char Titulo[], char Fn[] );
        void pflush( void );
        void initializePanelsData( void );
        
        void DibujaPanelHora( void ); 
        void DibujaPanelAlpha( void );
        void DibujaPanelDelta( void );
        void DibujaPanelPosicion( void );
        
        int ProcesaInputTeclado( int ch, char * buffer );
        void ActualizaPanelPrincipal( void );
        void ActualizaPanelHora( char * buffer );
        void ActualizaPanelAlpha( char * buffer );
        void ActualizaPanelPosicion( char * buffer );
        void ActualizaPanelInput( char * buffer );
        
        
        void refreshPanels( void );
        void topPanel( int );
        void showPanel( int );
        void hidePanel( int );
        void bottomPanel( int );

        
    private:
        
        PANEL * p[NUMBER_PANELS];
        struct PanelData pData[NUMBER_PANELS];
        
        int m_top_panel;
        int m_top_panel_old;
        int m_bottom_panel;
        int m_bottom_panel_old;
        int m_while;
        int m_new_line;
        int nrows, ncols;
        int m_r, m_c;
        
};

#endif //_MYNCURSES_H_




