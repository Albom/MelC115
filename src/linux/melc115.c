
#include "melc115.h"

//====================================================

void Sleep ( unsigned int d )
{
/*
struct timeval value;

if (d <= 0)
	return;

value.tv_usec = d % 1000;
value.tv_sec = d / 1000;

(void) select (1, 0, 0, 0, &value);
*/
BeepLin(0, d);
}

//====================================================

void dMoto2RTTTL(char m, char *r)
{

switch(m)
	{

	case '0':
		strcpy(r, "1");
		break;

	case '1':
		strcpy(r, "2");
		break;

	case '2':
		strcpy(r, "4");
		break;

	case '3':
		strcpy(r, "8");
		break;

	case '4':
		strcpy(r, "16");
		break;

	case '5':
		strcpy(r, "32");
		break;

	};

}

//====================================================

unsigned int dMoto2Midi(char m)
{

switch(m)
	{

	case '0':
		return 280;

	case '1':
		return 140;

	case '2':
		return 70;

	case '3':
		return 35;

	case '4':
		return 18;

	};

return 0;
}


//====================================================

void add(char *d, char *s, unsigned int pos, unsigned int len)
{
memcpy (d+pos, s, len);
}

//====================================================

unsigned int Swap32(unsigned int x)
{

__asm__ __volatile(
"movl x, %eax\n", 
"bswap %eax"
);

}

//====================================================

unsigned short Swap16(unsigned short x)
{

__asm__ __volatile(
"movw x, %ax\n",
"xchg %al, %ah"
);

}

//====================================================

void ExportMidi(GtkWidget *widget, gpointer user_data)
{

GtkWidget *file_selector = (GtkWidget *)user_data;
const gchar *selected_filename;
char midi_fn[MAXPATHLEN];//output file's name
int outf; //output file's descriptor

unsigned int dw;
unsigned short w;
int i = 0;
unsigned int pos = 0;
int o = 1;
char b[1024*16];
char t[512];
char _do[3];
char _end[]={(char)0xff, (char)0x2f, (char)0x00};

const gchar *txt;

txt = gtk_entry_get_text(GTK_ENTRY(hEdit));
strcpy(buf, txt);

_do[0] = (char) 0x90;
_do[2] = (char) 0x60;

selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
strcpy(midi_fn, selected_filename);

outf = open (midi_fn, O_WRONLY|O_CREAT);
write (outf, "MThd", 4);
dw = 0x06000000;//Swap32(6);
write (outf, &dw, 4);
w = Swap16(1);
write (outf, &w, 2);
write (outf, &w, 2);
w = Swap16(0x0060);
write (outf, &w, 2);
write (outf, "MTrk", 4);
w = WriteVarLen(0, t);
add(b, t, pos, w);
pos+=w;

for (i = 0; i < strlen(buf); i++)
	{


	if (buf[i] == '*')
		{
		switch (buf[++i])
			{
			case '3': o = 0; break;
			case '4': o = 1; break;
			case '5': o = 2; break;
			case '6': o = 3; break;
			default: return;
			};
		continue;
		}

	else
		{
		switch ( buf[i] )
			{
			case '#': 
					switch (buf[i+1])
						{
						case 'c':
							_do[0] = (char) 0x90;
							_do[1] = (char) (49+12*o);
							break;

						case 'd':
							_do[0] = (char) 0x90;
							_do[1] = (char) (51+12*o);
							break;

						case 'f':
							_do[0] = (char) 0x90;
							_do[1] = (char) (54+12*o);
							break;

						case 'g':
							_do[0] = (char) 0x90;
							_do[1] = (char) (56+12*o);
							break;

						case 'a':
							_do[0] = (char) 0x90;
							_do[1] = (char) (58+12*o);
							break;

						};

					add(b, _do, pos, 3);
					pos+=3;

					w = WriteVarLen(dMoto2Midi(buf[i+2]), t);
					add(b, t, pos, w);
					pos+=w;
					i+=2;
					break;


			case '&':
					switch (buf[i+1])
						{
						case 'd':
							_do[0] = (char) 0x90;
							_do[1] = (char) (49+12*o);
							break;

						case 'e':
							_do[0] = (char) 0x90;
							_do[1] = (char) (51+12*o);
							break;

						case 'g':
							_do[0] = (char) 0x90;
							_do[1] = (char) (54+12*o);
							break;

						case 'a':
							_do[0] = (char) 0x90;
							_do[1] = (char) (56+12*o);
							break;

						case 'b':
							_do[0] = (char) 0x90;
							_do[1] = (char) (58+12*o);
							break;
						};

					add(b, _do, pos, 3);
					pos+=3;

					w = WriteVarLen(dMoto2Midi(buf[i+2]), t);
					add(b, t, pos, w);
					pos+=w;
					i+=2;
					break;


			default: 
					switch (buf[i])
						{
						case 'c':
							_do[0] = (char) 0x90;
							_do[1] = (char) (48+12*o);
							break;

						case 'd':
							_do[0] = (char) 0x90;
							_do[1] = (char) (50+12*o);
							break;

						case 'e':
							_do[0] = (char) 0x90;
							_do[1] = (char) (52+12*o);
							break;

						case 'f':
							_do[0] = (char) 0x90;
							_do[1] = (char) (53+12*o);
							break;

						case 'g':
							_do[0] = (char) 0x90;
							_do[1] = (char) (55+12*o);
							break;

						case 'a':
							_do[0] = (char) 0x90;
							_do[1] = (char) (57+12*o);
							break;

						case 'b':
							_do[0] = (char) 0x90;
							_do[1] = (char) (59+12*o);
							break;

						case 'r':
							_do[0] = (char) 0x80;
							break;

						};

					add(b, _do, pos, 3);
					pos+=3;

					w = WriteVarLen(dMoto2Midi(buf[i+1]), t);
					add(b, t, pos, w);
					pos+=w;
					i++;
					break;
			};
		}

	}

w = WriteVarLen(0, t);
add(b, t, pos, w);
pos+=w;

add(b, _end, pos, 3);
pos+=3;

dw = Swap32(pos);
write (outf,  &dw, 4);
write (outf,  b, pos);

close(outf); 

}
//====================================================

unsigned int WriteVarLen(unsigned long value, char *b)
{

unsigned int len = 0;
unsigned int buffer = value&0x7F;


while((value>>=7)>0)
	{
	buffer<<=8,
	buffer|=0x80;
	buffer+=(value&0x7F);
	}
	
for(;;)
	{
	*(b+len) = (char) buffer;
	len++;
	if(buffer&0x80)
		buffer>>=8;
	else
		break;
    }

return len;
}

//====================================================

void BeepLin(unsigned int f, unsigned int d)
{

//d/=2;
#define FD 10

char *b;
unsigned int i;
float t;
double x;

b = (char *) malloc(d*FD);
//if (b == NULL) g_printf("Error!");

for (i=0; i<d*FD; i++)
	{
	t =  sin((float)f*(2*M_PI)*((float)i)/(8000))*2 ;
	b[i] = (char) t;
	}

add(wbuf, b, wpos, d*FD);

wpos+=d*FD;

free(b);

}

//====================================================

int Parse()
{

int i;
char ct;

O = 2;
for (i = 0; i < strlen(buf); i++)
	{
	

	if (buf[i] == '*')
		{
		switch (buf[++i])
			{
			case '3': O=1; break;
			case '4': O=2; break;
			case '5': O=4; break;
			case '6': O=8; break;
			case '7': O=16; break;

			default: return -1;
			};
		continue;
		}

	else
		{
		switch ( buf[i] )
			{
			case '#': 
					PlayNote(buf[++i], 1, buf[i+2]);
					i++;
					break;
			

			case '&':
					ct = buf[++i];

					switch (ct)
						{
						case 'b': ct = 'a'; break;
						case 'd': ct = 'c'; break;
						case 'e': ct = 'd'; break;
						case 'g': ct = 'f'; break;
						case 'a': ct = 'g'; break;
						};

					PlayNote(ct, 1, buf[i+1]);
					i++;
					break;


			default: 
					PlayNote(buf[i], 0, buf[i+1]);
					i++;
					break;
			}
		}

	}

return 0;
}

//----------------------------------------------------------------------

int PlayNote(char n, //note
		 int s, //sharp
		 char d //delay
		 )
{

unsigned int f = 0; //freq
unsigned int dd = 0; //duration

switch (d)
		{

		case '0': 
			dd = 720;
			break;

		case '1': 
			dd = 360;
			break;

		case '2': 
			dd = 180;
			break;

		case '3': 
			dd = 90;
			break;

		case '4': 
			dd = 44;
			break;

		case '5': 
			dd = 22;
			break;


		default: return 1;
		};

if (s)
	{
	switch (n)
		{

		case 'c':
			f = 277;
			break;

		case 'd':
			f = 311;
			break;

		case 'f':
			f = 370;
			break;

		case 'g':
			f = 415;
			break;

		case 'a':
			f = 466;
			break;

		default: return 1;

		};
	}
else
	switch (n)
		{

		case 'c':
			f = 262;
			break;

		case 'd':
			f = 294;
			break;

		case 'e':
			f = 330;
			break;

		case 'f':
			f = 350;
			break;

		case 'g':
			f = 392;
			break;

		case 'a':
			f = 440;
			break;

		case 'b':
			f = 494;
			break;

		case 'r':
			Sleep ( dd );
			return 0;
			break;

		default: return 1;

		};

BeepLin(O*f, dd);
return 0;
}

//====================================================

void char2str(char c, char *s)
{

*s = c;
*(s+1) = 0;

}

//====================================================

void ExportRTTTL(GtkWidget *widget, gpointer user_data)
{

GtkWidget *file_selector = (GtkWidget *)user_data;
const gchar *selected_filename;
char rtttl_fn[MAXPATHLEN];//output file's name
int outf; //output file's descriptor

int i;
char ct;
char o[2];

char b[1024];
char t[16];

const gchar *txt;

txt = gtk_entry_get_text(GTK_ENTRY(hEdit));
strcpy(buf, txt);

o[0]='1';
o[1]=0;


strcpy(b, "C115:d=4,o=2,b=125:");

for (i = 0; i < strlen(buf); i++)
	{


	if (buf[i] == '*')
		{
		switch (buf[++i])
			{
			case '3': *o = '0'; break;
			case '4': *o = '1'; break;
			case '5': *o = '2'; break;
			case '6': *o = '3'; break;
			default: return;
			};
		continue;
		}

	else
		{
		switch ( buf[i] )
			{
			case '#': 
					dMoto2RTTTL(buf[i+2], t);
					strcat(b, t);
					char2str(buf[i+1], t);
					strcat(b, t);
					strcat(b, "#");
					strcat(b, o);
					strcat(b, ", ");
					i+=2;
					break;


			case '&':
					ct = buf[i+1];

					switch (ct)
						{
						case 'b': ct = 'a'; break;
						case 'd': ct = 'c'; break;
						case 'e': ct = 'd'; break;
						case 'g': ct = 'f'; break;
						case 'a': ct = 'g'; break;
						};

					dMoto2RTTTL(buf[i+2], t);
					strcat(b, t);
					char2str(ct, t);
					strcat(b, t);
					strcat(b, "#");
					strcat(b, o);
					strcat(b, ", ");
					i+=2;
					break;


			default: 
					dMoto2RTTTL(buf[i+1], t);
					strcat(b, t);
					if (buf[i] == 'r')
						strcat(b, "p");
					else
						{
						char2str(buf[i], t);
						strcat(b, t);
						strcat(b, o);
						}
					strcat(b, ", ");
					i++;
					break;
			};
		}

	}

b[strlen(b)-2]=0;

selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
strcpy(rtttl_fn, selected_filename);

outf = open (rtttl_fn, O_WRONLY|O_CREAT);
write (outf,  b, strlen(b));
close(outf); 

}

//====================================================

void SaveTextFileName()
{
GtkWidget *file_selector;

file_selector = gtk_file_selection_new ("Save melody in RTTTL to a text file.");

g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                     "clicked",
                     G_CALLBACK ( ExportRTTTL ),
                     (gpointer) file_selector);

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             (gpointer) file_selector); 

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->cancel_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             (gpointer) file_selector); 

gtk_widget_show (file_selector);
}

//====================================================

void SaveMidiFileName()
{
GtkWidget *file_selector;

file_selector = gtk_file_selection_new ("Save melody to a MIDI file.");

g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                     "clicked",
                     G_CALLBACK ( ExportMidi ),
                     (gpointer) file_selector);

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             (gpointer) file_selector); 

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->cancel_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             (gpointer) file_selector); 

gtk_widget_show (file_selector);
}


//====================================================

void set_window_icon(GtkWidget *gtk_wnd, gchar **xpm_data)
{
GdkPixmap *pix;
GdkBitmap *mask;

gtk_widget_realize(gtk_wnd);
pix = gdk_pixmap_create_from_xpm_d (gtk_wnd->window, &mask, NULL, xpm_data);
gdk_window_set_icon(gtk_wnd->window, 0, pix, mask);
}

//====================================================

void OpenMelFile (GtkWidget *widget, gpointer user_data) 
{

GtkWidget *file_selector = (GtkWidget *)user_data;
const gchar *selected_filename;
int inf; //input file's descriptor

*fn = '\0';

selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));

strcpy(fn, selected_filename);

inf = open (fn, O_RDONLY);
read (inf,  buf, 256);
close(inf);

gtk_entry_set_text(GTK_ENTRY(hEdit), buf);

gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);
gtk_statusbar_push(GTK_STATUSBAR(statusbar), 1, fn);
}

//====================================================

gint delete_event( GtkWidget *widget,
                   GdkEvent  *event,
                   gpointer   data )
{
    gtk_main_quit ();
    return(FALSE);
}


//====================================================

void NewMelFile()
{
memset(fn, '\0', MAXPATHLEN);
memset(buf, 0, 256);
gtk_entry_set_text(GTK_ENTRY(hEdit), "");
gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);
gtk_statusbar_push(GTK_STATUSBAR(statusbar), 1, "");
}


//====================================================

void Play()
{
int outf;
const gchar *txt;

txt = gtk_entry_get_text(GTK_ENTRY(hEdit));
strcpy(buf, txt);

wpos = 0;
Parse();

outf = open ("/dev/dsp", O_WRONLY);
//outf = open ("data", O_WRONLY|O_CREAT);
write (outf,  wbuf, wpos);
close(outf);

}

//====================================================

void Stop()
{
}

//====================================================

void About()
{
GtkWidget *dialog = NULL;
GtkWidget *about; 
GtkWidget *img;
GtkWidget *box; 
GdkPixmap *pix;
GdkBitmap *mask;

pix = gdk_pixmap_create_from_xpm_d (hWnd->window, &mask, NULL, c115_xpm);

dialog = gtk_dialog_new_with_buttons(szAppName,
					GTK_WINDOW(hWnd),
					GTK_DIALOG_MODAL,
					GTK_STOCK_CLOSE,
					GTK_RESPONSE_ACCEPT, NULL); 

gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);

g_signal_connect_swapped (GTK_OBJECT (dialog), 
                             "response", 
                             G_CALLBACK (gtk_widget_destroy),
                             GTK_OBJECT (dialog));

about = gtk_label_new(szAbout);
gtk_label_set_justify (GTK_LABEL(about), GTK_JUSTIFY_RIGHT);
gtk_widget_show (about);

box = gtk_hbox_new (FALSE, 0);
gtk_widget_show (box);

gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), box, FALSE, FALSE, 0); 

img = gtk_image_new_from_pixmap(pix, mask);
gtk_widget_show (img);
gtk_container_add (GTK_CONTAINER (box), img);

gtk_container_add (GTK_CONTAINER (box), about);
gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
gtk_widget_set_usize(dialog, 300, 200);
gtk_widget_show_all (dialog);

}

//====================================================

void OpenMelFileName()
{
GtkWidget *file_selector;

file_selector = gtk_file_selection_new ("Open a melody file.");

gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (file_selector));

g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                     "clicked",
                     G_CALLBACK ( OpenMelFile ),
                     (gpointer) file_selector);

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             (gpointer) file_selector); 

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->cancel_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             (gpointer) file_selector); 

gtk_widget_show (file_selector);

}

//====================================================

void SaveMelFile(GtkWidget *widget, gpointer user_data)
{

GtkWidget *file_selector = (GtkWidget *)user_data;
const gchar *selected_filename;
int outf; //output file's descriptor
const gchar *txt;

txt = gtk_entry_get_text(GTK_ENTRY(hEdit));
strcpy(buf, txt);

*fn = '\0';

selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));

strcpy(fn, selected_filename);

outf = open (fn, O_WRONLY|O_CREAT);
write (outf,  buf, 256);
close(outf); 

gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);
gtk_statusbar_push(GTK_STATUSBAR(statusbar), 1, fn);

}
//====================================================

void SaveMelFileName()
{
GtkWidget *file_selector;

file_selector = gtk_file_selection_new ("Save a melody file.");

g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                     "clicked",
                     G_CALLBACK ( SaveMelFile ),
                     (gpointer) file_selector);

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             (gpointer) file_selector); 

g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->cancel_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             (gpointer) file_selector); 

gtk_widget_show (file_selector);

}

//====================================================


int main(int argc,char **argv) { 

GtkWidget *vbox; //vertical box 

GtkWidget *menu;
GtkWidget *export_menu;
GtkWidget *menuitem;
GtkWidget *menubar;


GtkWidget *new_item;
GtkWidget *open_item;
GtkWidget *save_item;
GtkWidget *about_item;
GtkWidget *exit_item;
GtkWidget *export_item;
GtkWidget *rtttl_item;
GtkWidget *midi_item;
GtkWidget *separator;

GtkWidget *toolbar;
GtkWidget *play_icon;
GtkWidget *stop_icon;
GtkWidget *play_btn;
GtkWidget *stop_btn;

PangoFontDescription *font;

GdkPixmap *pix;
GdkBitmap *mask;

GtkWidget *scrollbar;

gtk_init(&argc, &argv); 


hWnd = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
gtk_widget_set_usize(hWnd, 500, 130);
gtk_window_set_title (GTK_WINDOW (hWnd), szAppName );
set_window_icon(hWnd, icon_xpm);

gtk_signal_connect (GTK_OBJECT (hWnd), "delete_event",
                     GTK_SIGNAL_FUNC (delete_event), NULL);

gtk_window_set_position (GTK_WINDOW (hWnd), GTK_WIN_POS_CENTER);
gtk_window_set_resizable (GTK_WINDOW (hWnd), FALSE);

hEdit = gtk_entry_new();
gtk_widget_set_usize(hEdit, 500, 50);
gtk_widget_show(hEdit);
gtk_editable_set_editable(GTK_EDITABLE(hEdit), TRUE);
gtk_entry_set_max_length(GTK_ENTRY(hEdit), 256);

font = pango_font_description_from_string(FONT);
gtk_widget_modify_font(GTK_WIDGET(hEdit), font); 

scrollbar = gtk_hscrollbar_new(0);
gtk_widget_show (scrollbar);

vbox = gtk_vbox_new (FALSE, 0);
gtk_widget_show (vbox);

gtk_container_add (GTK_CONTAINER (hWnd), vbox); 



toolbar = gtk_toolbar_new();
gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar), GTK_ORIENTATION_HORIZONTAL);
gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
gtk_widget_ref (toolbar);
gtk_widget_show (toolbar);

pix = gdk_pixmap_create_from_xpm_d (hWnd->window, &mask, NULL, play_xpm);
play_icon = gtk_pixmap_new (pix, mask);
pix = gdk_pixmap_create_from_xpm_d (hWnd->window, &mask, NULL, stop_xpm);
stop_icon = gtk_pixmap_new (pix, mask);

play_btn = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                  GTK_TOOLBAR_CHILD_BUTTON,
                  NULL,
                  NULL/*"Play"*/,
                  "Play", NULL,
                  play_icon, NULL, NULL);
gtk_signal_connect (GTK_OBJECT (play_btn), "button_release_event",
			  GTK_SIGNAL_FUNC (Play), 0); 

  
stop_btn = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                  GTK_TOOLBAR_CHILD_BUTTON,
                  NULL,
                  NULL/*"Stop"*/,
                  "Stop", NULL,
                  stop_icon, NULL, NULL);
gtk_signal_connect (GTK_OBJECT (stop_btn), "button_release_event",
			  GTK_SIGNAL_FUNC (Stop), 0); 


menubar = gtk_menu_bar_new ();

menu = gtk_menu_new ();

menuitem = gtk_menu_item_new_with_label ("File");
gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);
gtk_widget_show (menuitem);

new_item = gtk_menu_item_new_with_label ("New");
gtk_menu_shell_append (GTK_MENU_SHELL (menu), new_item);      
gtk_widget_show (new_item);
gtk_signal_connect (GTK_OBJECT (new_item), "activate",
			  GTK_SIGNAL_FUNC (NewMelFile), 0); 

open_item = gtk_menu_item_new_with_label ("Open ...");
gtk_menu_shell_append (GTK_MENU_SHELL (menu), open_item);      
gtk_widget_show (open_item);
gtk_signal_connect (GTK_OBJECT (open_item), "activate",
			  GTK_SIGNAL_FUNC (OpenMelFileName), 0); 

save_item = gtk_menu_item_new_with_label ("Save ...");
gtk_menu_shell_append (GTK_MENU_SHELL (menu), save_item);      
gtk_widget_show (save_item);
gtk_signal_connect (GTK_OBJECT (save_item), "activate",
			  GTK_SIGNAL_FUNC (SaveMelFileName), 0); 

export_menu = gtk_menu_new ();
rtttl_item = gtk_menu_item_new_with_label ("RTTTL ...");
gtk_menu_shell_append (GTK_MENU_SHELL (export_menu), rtttl_item);      
gtk_widget_show (rtttl_item);
gtk_signal_connect (GTK_OBJECT (rtttl_item), "activate",
			  GTK_SIGNAL_FUNC (SaveTextFileName), 0); 


midi_item = gtk_menu_item_new_with_label ("MIDI ...");
gtk_menu_shell_append (GTK_MENU_SHELL (export_menu), midi_item);      
gtk_widget_show (midi_item);
gtk_signal_connect (GTK_OBJECT (midi_item), "activate",
			  GTK_SIGNAL_FUNC (SaveMidiFileName), 0); 


export_item = gtk_menu_item_new_with_label ("Export");
gtk_menu_shell_append (GTK_MENU_SHELL (menu), export_item);      
gtk_menu_item_set_submenu (export_item, export_menu);      
gtk_widget_show (export_item);


separator = gtk_separator_menu_item_new();
gtk_widget_show (separator);
gtk_menu_shell_append (GTK_MENU_SHELL (menu), separator); 

exit_item = gtk_menu_item_new_with_label ("Quit");
gtk_menu_shell_append (GTK_MENU_SHELL (menu), exit_item);      
gtk_widget_show (exit_item);
gtk_signal_connect (GTK_OBJECT (exit_item), "activate",
			  GTK_SIGNAL_FUNC (delete_event), 0); 


menu = gtk_menu_new ();

menuitem = gtk_menu_item_new_with_label ("Help");
gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
gtk_menu_item_set_right_justified (GTK_MENU_ITEM (menuitem), TRUE);
gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);
gtk_widget_show (menuitem);

about_item = gtk_menu_item_new_with_label ("About ...");
gtk_menu_shell_append (GTK_MENU_SHELL (menu), about_item);      
gtk_widget_show (about_item);
gtk_signal_connect (GTK_OBJECT (about_item), "activate",
			  GTK_SIGNAL_FUNC (About), 0); 


gtk_widget_show (menuitem);
gtk_widget_show(menubar);

statusbar = gtk_statusbar_new ();
gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR(statusbar), FALSE);
gtk_widget_show(statusbar);

gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (vbox), toolbar, FALSE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (vbox), hEdit, FALSE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (vbox), scrollbar, FALSE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (vbox), statusbar, FALSE, FALSE, 0);

gtk_widget_show(hWnd);
gtk_window_set_focus (GTK_WINDOW(hWnd), hEdit);

memset(buf, 0, 256);

gtk_main();

return(0);
} 

//====================================================

