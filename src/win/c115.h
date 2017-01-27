#include	<windows.h>
#include	<commctrl.h>
#include	<shellapi.h>
#include	<richedit.h>
#include	<math.h>

#ifndef TBSTYLE_FLAT
#define TBSTYLE_FLAT  0x0800
#endif

//----------------------------------------------------------------------

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

//----------------------------------------------------------------------

#define		ED_EDIT		1000
#define		SCR_SCROLL	999

#define		CMD_NEW		10000
#define		CMD_OPEN	10001
#define		CMD_SAVEAS	10002
#define		CMD_EXIT	10003
#define		CMD_HELP	10004
#define		CMD_ABOUT	10005
#define		CMD_FONT	10006
#define		CMD_SFONT	10007
#define		CMD_GOWITH	10008

#define		CMD_SAVE	10009

#define		CMD_PLAY	10010
#define		CMD_STOP	10011

#define		CMD_SEND	10016

#define		CMD_ENG		10017
#define		CMD_RUS		10018

#define		CMD_TRAY	10020

#define		CMD_UNDO	10012
#define		CMD_CUT		10013
#define		CMD_COPY	10014
#define		CMD_PASTE	10015
#define		CMD_DEL		10021
#define		CMD_SELALL	10022

#define		CMD_COLTXT	10023
#define		CMD_COLBG	10024
#define		CMD_COLST	10025

#define		CMD_REDO	10026

#define		CMD_CLEAR	10027
#define		CMD_NEXT	10028

#define		CMD_ONTOP	10029

#define		CMD_WAVOUT	10030
#define		CMD_MIDOUT	10031

#define		CMD_VOL		10032

#define		CMD_SPKOUT	10033

#define		CMD_RTTTL	10034

#define		CMD_MIDI	10035

#define		CMD_NOKIA	10036
#define		CMD_SIEM	10037

#define		CMD_NOKIA_I	10038


#define		CMD_FILE1	11001
#define		CMD_FILE2	11002
#define		CMD_FILE3	11003
#define		CMD_FILE4	11004
#define		CMD_FILE5	11005

#define		CMD_OKBTN	1001

#define		WM_TRAY_ICON	10000

#define		STRING_MS_SANS	"MS Sans Serif"


//----------------------------------------------------------------------

BOOL IsNT();

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL AboutFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL LabelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void FontChooseFont (void);
void FontSetDefaultFont(void);
void FontSetIniFont (void);

void Beep9x(DWORD dwFreq, DWORD dwDuration);

void GetTmp(char *fn);
BOOL GetCmdFileName ( char* file );

void CenterWindow(HWND hwnd);
BOOL CreateTB(HWND hwnd);

BOOL MelodyFileSaveName(char *fn);
BOOL MelodyFileOpenName(char *fn);
void MelodyFileSave(char *fn);
BOOL MelodyFileOpen(char *fn);

DWORD WINAPI Parse( LPVOID pParam );
BOOL PlayNote(char n, BOOL s, char d);

void MidiOut (DWORD Msg);
void MidiOutChan (BYTE b1, BYTE b2, BYTE b3);
void AllNotesOff ();
void CloseDevices () ;
void OpenDevices () ;
void Midi9x(DWORD f, DWORD d);

void PCSpeak9x(DWORD f, DWORD d);

void NoteToKey(char *s);

void OpenRecent(int n);
void ReadRecent(BOOL mod);
void WriteRecent(char *f);

//----------------------------------------------------------------------


HINSTANCE	hInst;
WNDPROC		OrigLabelProc, OrigEditProc;

HBRUSH		hbrush;

CHOOSECOLOR txtcol, bgcol;
CHARFORMAT	cf;
COLORREF	acrCustClr[16];


DWORD		txtc = 0;
DWORD		bgc = 0xFFFFFF;