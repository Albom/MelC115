#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <sys/param.h>
#include <sys/select.h> 
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "c115.xpm"
#include "icon.xpm"
#include "play.xpm"
#include "stop.xpm"

//====================================================

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

//====================================================

char szAppName[]={"Mel C115 v0.2/4.5"}; //application name (title)
char szAbout[]={"\nMel C115\nv0.2/4.5\n\nauthor: Oleksandr Bogomaz\ne-mail: albom85@yandex.ru\n"};
char fn[MAXPATHLEN]; //file's name
char FONT[]={"Mono 32"}; //edit box's font

char buf[256]; //melody buffer
int O=4; //octave

unsigned int wpos;
char wbuf[512*1024];

//====================================================

GtkWidget   *hWnd;  //main window
GtkWidget   *hEdit; //main edit control
GtkWidget   *statusbar; //statusbar

//====================================================

void set_window_icon(GtkWidget *gtk_wnd, gchar **xpm_data);
void OpenMelFile(GtkWidget *widget, gpointer user_data);
gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data);
void NewMelFile();
void About();
void OpenMelFileName();
void SaveMelFile(GtkWidget *widget, gpointer user_data);
void SaveMelFileName();

//====================================================

void dMoto2RTTTL(char m, char *r);
unsigned int dMoto2Midi(char m);
void add(char *d, char *s, unsigned int pos, unsigned int len);
int Parse();
int PlayNote(char n, int s, char d);
void Sleep( unsigned int d );
void BeepLin(unsigned int f, unsigned int d);
void ExportRTTTL();
void char2str(char c, char *s);
unsigned short Swap16(unsigned short x);
unsigned int Swap32(unsigned int x);
unsigned int WriteVarLen(unsigned long value, char *b);

//====================================================
