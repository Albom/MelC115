#include "all.h"


//----------------------------------------------------------------------

BOOL		NT = TRUE; //Windows NT - TRUE; 9x - FALSE

char		tempfile[MAX_PATH];

HWND		hWnd, hToolBar, hEdit, hStatus;
HANDLE		hT; //thread handle
HMENU		hMenu, hRecent;
HCURSOR		hCurHand;
LOGFONT		logfont;
HACCEL		hAccel;

WIN32_FIND_DATA find;
NOTIFYICONDATA notdata;

char		buf[256]; //text buffer
char		fname[MAX_PATH]; //file name
char		inifile[MAX_PATH];
int			O=4; //octave

int			starts, ends;

BOOL		gowith = TRUE;
BOOL		eng = TRUE;
BOOL		tray = FALSE;

int format = 0;

char watch[2], ontop[2];

char szfilter1[] = { "Melody files (*.mel)\0*.mel\0All files (*.*)\0*.*\0"};
char szfilter2[] = { "Файлы мелодий (*.mel)\0*.mel\0Все файлы (*.*)\0*.*\0"};

char szfilter3[] = { "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0"};
char szfilter4[] = { "Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0"};

char szfilter5[] = { "MIDI files (*.mid)\0*.mid\0All files (*.*)\0*.*\0"};
char szfilter6[] = { "Файлы MIDI (*.mid)\0*.mid\0Все файлы (*.*)\0*.*\0"};


char szAppTitle[] = { "Mel C115 v4.6" };


//----------------------------------------------------------------------

void char2str(char c, char *s)
{

*s = c;
*(s+1) = 0;

}

//----------------------------------------------------------------------

void dMoto2RTTTL(char m, char *r)
{

switch(m)
	{

	case '0':
		lstrcpy(r, "1");
		break;

	case '1':
		lstrcpy(r, "2");
		break;

	case '2':
		lstrcpy(r, "4");
		break;

	case '3':
		lstrcpy(r, "8");
		break;

	case '4':
		lstrcpy(r, "16");
		break;

	case '5':
		lstrcpy(r, "32");
		break;



	};

}

//----------------------------------------------------------------------
void dRTTTL2Moto(char m, char *r)
{

switch(m)
	{

	case '0':
		lstrcpy(r, "1");
		break;

	case '1':
		lstrcpy(r, "2");
		break;

	case '2':
		lstrcpy(r, "4");
		break;

	case '3':
		lstrcpy(r, "8");
		break;

	case '4':
		lstrcpy(r, "16");
		break;

	case '5':
		lstrcpy(r, "32");
		break;



	};

}

//----------------------------------------------------------------------

void dMoto2Siemens(char m, char *r)
{

switch(m)
	{

	case '0':
		lstrcpy(r, "(1/1)");
		break;

	case '1':
		lstrcpy(r, "(1/2)");
		break;

	case '2':
		lstrcpy(r, "(1/4)");
		break;

	case '3':
		lstrcpy(r, "(1/8)");
		break;

	case '4':
		lstrcpy(r, "(1/16)");
		break;

	case '5':
		lstrcpy(r, "(1/32)");
		break;



	};

}

//----------------------------------------------------------------------

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


//----------------------------------------------------------------------

void ExportRTTTL()
{

int i;
char ct;
char o[2];

char b[1024];
char t[16];

char fn[MAX_PATH];

OPENFILENAME ofn ;

HANDLE	f;
DWORD	r;

SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
if ( 0 == lstrlen(buf) ) 
	return;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "txt";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter3;
else ofn.lpstrFilter = szfilter4;

if ( !GetSaveFileName(&ofn) )
	return;


o[0]='1';
o[1]=0;


b[0] = 0;

lstrcpy(b, "C115:d=4,o=2,b=125:");


for (i = 0; i < lstrlen(buf); i++)
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
					lstrcat(b, t);
					char2str(buf[i+1], t);
					lstrcat(b, t);
					lstrcat(b, "#");
					lstrcat(b, o);
					lstrcat(b, ", ");
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
					lstrcat(b, t);
					char2str(ct, t);
					lstrcat(b, t);
					lstrcat(b, "#");
					lstrcat(b, o);
					lstrcat(b, ", ");
					i+=2;
					break;


			default: 
					dMoto2RTTTL(buf[i+1], t);
					lstrcat(b, t);
					if (buf[i] == 'r')
						lstrcat(b, "p");
					else
						{
						char2str(buf[i], t);
						lstrcat(b, t);
						lstrcat(b, o);
						}
					lstrcat(b, ", ");
					i++;
					break;
			};
		}

	}

b[lstrlen(b)-2]=0;

f = CreateFile(fn, GENERIC_WRITE, 0, NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

WriteFile (f, b, lstrlen(b), &r, NULL);
CloseHandle(f);
}

//----------------------------------------------------------------------

void ExportNokia()
{

int i;
char ct;
char o[2];

char b[1024];
char t[16];

char fn[MAX_PATH];

OPENFILENAME ofn ;

HANDLE	f;
DWORD	r;

SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
if ( 0 == lstrlen(buf) ) 
	return;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "txt";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter3;
else ofn.lpstrFilter = szfilter4;

if ( !GetSaveFileName(&ofn) )
	return;


o[0]='1';
o[1]=0;

b[0] = 0;

for (i = 0; i < lstrlen(buf); i++)
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
					lstrcat(b, t);
					lstrcat(b, "#");
					char2str(buf[i+1], t);
					lstrcat(b, t);
					lstrcat(b, o);
					lstrcat(b, " ");
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
					lstrcat(b, t);
					lstrcat(b, "#");
					char2str(ct, t);
					lstrcat(b, t);
					lstrcat(b, o);
					lstrcat(b, " ");
					i+=2;
					break;


			default: 
					dMoto2RTTTL(buf[i+1], t);
					lstrcat(b, t);
					if (buf[i] == 'r')
						lstrcat(b, "-");
					else
						{
						char2str(buf[i], t);
						lstrcat(b, t);
						lstrcat(b, o);
						}
					lstrcat(b, " ");
					i++;
					break;
			};
		}

	}

b[lstrlen(b)-1]=0;

f = CreateFile(fn, GENERIC_WRITE, 0, NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

WriteFile (f, b, lstrlen(b), &r, NULL);
CloseHandle(f);
}

//----------------------------------------------------------------------

void ExportSiemens()
{

int i;
char ct;
char o[2];

char b[1024];
char t[16];

char fn[MAX_PATH];

OPENFILENAME ofn ;

HANDLE	f;
DWORD	r;

SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
if ( 0 == lstrlen(buf) ) 
	return;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "txt";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter3;
else ofn.lpstrFilter = szfilter4;

if ( !GetSaveFileName(&ofn) )
	return;


o[0]='1';
o[1]=0;

b[0] = 0;

for (i = 0; i < lstrlen(buf); i++)
	{


	if (buf[i] == '*')
		{
		switch (buf[++i])
			{
			case '3': *o = '1'; break;
			case '4': *o = '2'; break;
			case '5': *o = '3'; break;
			case '6': *o = '4'; break;
			default: return;
			};
		continue;
		}

	else
		{
		switch ( buf[i] )
			{
			case '#': 
					char2str(buf[i+1]-32, t);
					lstrcat(b, t);
					lstrcat(b, "is");
					lstrcat(b, o);
					dMoto2Siemens(buf[i+2], t);
					lstrcat(b, t);
					lstrcat(b, " ");
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

					char2str(ct-32, t);
					lstrcat(b, t);
					lstrcat(b, "is");
					lstrcat(b, o);
					dMoto2Siemens(buf[i+2], t);
					lstrcat(b, t);
					lstrcat(b, " ");
					i+=2;
					break;


			default: 
					if (buf[i] == 'r')
						lstrcat(b, "P");
					else
						{
						char2str(buf[i]-32, t);
						lstrcat(b, t);
						lstrcat(b, o);
						}
					dMoto2Siemens(buf[i+1], t);
					lstrcat(b, t);

					lstrcat(b, " ");
					i++;
					break;
			};
		}

	}

b[lstrlen(b)-1]=0;

f = CreateFile(fn, GENERIC_WRITE, 0, NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

WriteFile (f, b, lstrlen(b), &r, NULL);
CloseHandle(f);
}


//----------------------------------------------------------------------


BOOL IsNT()
{

OSVERSIONINFO ver;

ver.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);

GetVersionEx(&ver);

if (ver.dwPlatformId == VER_PLATFORM_WIN32_NT) 
	return TRUE;
else return FALSE;

}

//----------------------------------------------------------------------

void ImportNokia()
{

char fn[MAX_PATH];
char _fn[MAX_PATH];

char b[4*1024];
char _b[4*1024];

char t[16];
char _t[16];

int i;

char _o; // octave

char tmp[256];

DWORD	len; //file's length

HANDLE	f; //file's descriptor
DWORD	r;
WIN32_FIND_DATA FindFile;

OPENFILENAME ofn ;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "txt";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter3;
else ofn.lpstrFilter = szfilter4;

if ( !GetOpenFileName(&ofn) )
	return;


if ( *(fn+1) != ':' ) 
	{
	GetCurrentDirectory(MAX_PATH, _fn);
	lstrcat(_fn, "\\");
	lstrcat(_fn, fn);
	lstrcpy(fn, _fn);
	}


FindFirstFile(fn, &FindFile);
len = FindFile.nFileSizeLow;
if ( 4*1024 < len )
	return;

memset(_b, 0, 4*1024);
memset( b, 0, 4*1024);

f = CreateFile( fn, GENERIC_READ, 0, NULL, 
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

ReadFile (f, _b, len, &r, NULL);
CloseHandle(f);

wsprintf (b, "       %s       ", _b);

i = 4;
_o = '4';
tmp[0] = 0;

while ( i < len )
	{

	*t = 0;
	if ( 255 < lstrlen (tmp) )
		break;

	switch ( b[i] )
		{

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
			if ( _o != b[i+1]+3 )
				{
				wsprintf(_t, "*%c", _o=b[i+1]+3);
				lstrcat(t, _t);
				}

			if (b[i]<97)
				b[i]+=32;

			if (b[i] == 'h')
				b[i] = 'b';

			if (b[i-1] == '#')
				lstrcat(t, "#");

			wsprintf(_t, "%c", b[i]);
			lstrcat(t, _t);

			switch (b[i-1])
				{
				case '.':
						if (( '2' == b[i-2] )&& ( '3' == b[i-3] ))
							lstrcat(t, "4r5");
						else 
							switch (b[i-2])
								{
								case '1': lstrcat(t, "0r0r1"); break;
								case '2': lstrcat(t, "0r1"); break;
								case '4': lstrcat(t, "1r2"); break;
								case '8': lstrcat(t, "2r3"); break;
								case '6': lstrcat(t, "3r4"); break;
								};//switch (b[i-2])
						break;

				case '#':
						if (b[i-2] == '.')
							{
							if (( '2' == b[i-3] )&& ( '3' == b[i-4] ))
								lstrcat(t, "4r5");
							else 
								switch (b[i-3])
									{
									case '1': lstrcat(t, "0r0r1"); break;
									case '2': lstrcat(t, "0r1"); break;
									case '4': lstrcat(t, "1r2"); break;
									case '8': lstrcat(t, "2r3"); break;
									case '6': lstrcat(t, "3r4"); break;
									};//switch (b[i-2])
							}
						else 
							{
							if (( '2' == b[i-2] )&& ( '3' == b[i-3] ))
								lstrcat(t, "4");
							else 
								switch (b[i-2])
									{
									case '1': lstrcat(t, "0r0"); break;
									case '2': lstrcat(t, "0"); break;
									case '4': lstrcat(t, "1"); break;
									case '8': lstrcat(t, "2"); break;
									case '6': lstrcat(t, "3"); break;
									};//switch (b[i-1])
							}

					break;

				default:
					if (( '2' == b[i-1] )&& ( '3' == b[i-2] ))
						lstrcat(t, "4");
					else 
						switch (b[i-1])
							{
							case '1': lstrcat(t, "0r0"); break;
							case '2': lstrcat(t, "0"); break;
							case '4': lstrcat(t, "1"); break;
							case '8': lstrcat(t, "2"); break;
							case '6': lstrcat(t, "3"); break;
							};//switch (b[i-1])
				}//switch (b[i-1])

			break;

		case '-':
				lstrcat(t, "r");
				if (( '2' == b[i-1] )&& ( '3' == b[i-2] ))
					lstrcat(t, "4");
				else 
					switch (b[i-1])
						{
						case '1': lstrcat(t, "0r0"); break;
						case '2': lstrcat(t, "0"); break;
						case '4': lstrcat(t, "1"); break;
						case '8': lstrcat(t, "2"); break;
						case '6': lstrcat(t, "3"); break;
						};

			break;

		default: 
			break;

		};

	lstrcat (tmp, t);
	i++;

	}

PlaySound(NULL, NULL, SND_PURGE);
AllNotesOff();
TerminateThread(hT, 0);
	
memset(fname, 0, MAX_PATH);
SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)NULL);

SendMessage(hEdit, WM_SETTEXT, (WPARAM) 0, (LPARAM) tmp);
}


//----------------------------------------------------------------------

void PCSpeak9x(DWORD f, DWORD d)
{
/*
__asm
{

in al, 0x61
push ax
;cli

}
*/
}

//----------------------------------------------------------------------

DWORD Swap32(DWORD x)
{

_asm{
mov EAX, x
bswap EAX
}

}

//----------------------------------------------------------------------


WORD Swap16(WORD x)
{

_asm {
	mov AX, x
  xchg ah,al
}

}


//----------------------------------------------------------------------

unsigned int WriteVarLen(unsigned long value, char *b)
{
unsigned int len = 0;
DWORD	buffer = value&0x7F;


while((value>>=7)>0)
	{
	buffer<<=8,
	buffer|=0x80;
	buffer+=(value&0x7F);
	}
	
for(;;)
	{
	*(b+len) = char (buffer);
	len++;
	if(buffer&0x80)
		buffer>>=8;
	else
		break;
    }

return len;
}


//----------------------------------------------------------------------

void add(char *d, char *s, unsigned int pos, unsigned int len)
{
memcpy (d+pos, s, len);
}


//----------------------------------------------------------------------


void ExportMidi()
{

HANDLE f;
DWORD r, dw;
WORD w;
int i = 0;
DWORD pos = 0;

int o = 1;

char b[1024*16];
char t[512];
char _do[3];
char _end[]={char(0xff), char(0x2f), char(0x00)};


char fn[MAX_PATH];

OPENFILENAME ofn ;

SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
if (lstrlen(buf)==0) return;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "mid";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter5;
else ofn.lpstrFilter = szfilter6;

if ( !GetSaveFileName(&ofn) )
	return;


_do[0] = char (0x90);
_do[2] = char (0x60);

f = CreateFile(fn, GENERIC_WRITE, 0, NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

WriteFile (f, "MThd", 4, &r, NULL);

dw = Swap32(6);
WriteFile (f, &dw, 4, &r, NULL);

w = Swap16(1);
WriteFile (f, &w, 2, &r, NULL);
WriteFile (f, &w, 2, &r, NULL);

w = Swap16(0x0060);
WriteFile (f, &w, 2, &r, NULL);

WriteFile (f, "MTrk", 4, &r, NULL);

w = WriteVarLen(0, t);
add(b, t, pos, w);
pos+=w;


for (i = 0; i < lstrlen(buf); i++)
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
							_do[0] = (char)0x90;
							_do[1] = char (49+12*o);
							break;

						case 'd':
							_do[0] = (char)0x90;
							_do[1] = char (51+12*o);
							break;

						case 'f':
							_do[0] = (char)0x90;
							_do[1] = char (54+12*o);
							break;

						case 'g':
							_do[0] = (char)0x90;
							_do[1] = char (56+12*o);
							break;

						case 'a':
							_do[0] = (char)0x90;
							_do[1] = char (58+12*o);
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
							_do[0] = (char)0x90;
							_do[1] = char (49+12*o);
							break;

						case 'e':
							_do[0] = (char)0x90;
							_do[1] = char (51+12*o);
							break;

						case 'g':
							_do[0] = (char)0x90;
							_do[1] = char (54+12*o);
							break;

						case 'a':
							_do[0] = (char)0x90;
							_do[1] = char (56+12*o);
							break;

						case 'b':
							_do[0] = (char)0x90;
							_do[1] = char (58+12*o);
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
							_do[0] = (char)0x90;
							_do[1] = char (48+12*o);
							break;

						case 'd':
							_do[0] = (char)0x90;
							_do[1] = char (50+12*o);
							break;

						case 'e':
							_do[0] = (char)0x90;
							_do[1] = char (52+12*o);
							break;

						case 'f':
							_do[0] = (char)0x90;
							_do[1] = char (53+12*o);
							break;

						case 'g':
							_do[0] = (char)0x90;
							_do[1] = char (55+12*o);
							break;

						case 'a':
							_do[0] = (char)0x90;
							_do[1] = char (57+12*o);
							break;

						case 'b':
							_do[0] = (char)0x90;
							_do[1] = char (59+12*o);
							break;

						case 'r':
							_do[0] = (char)0x80;
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
WriteFile (f, &dw, 4, &r, NULL);
WriteFile (f, b, pos, &r, NULL);

CloseHandle(f);


}

//----------------------------------------------------------------------

HMIDIOUT Out;
UINT Chan;

//----------------------------------------------------------------------

void MidiOut (DWORD Msg) 
{

if (Out)
	midiOutShortMsg (Out, Msg);

}

//----------------------------------------------------------------------

void MidiOutChan (BYTE b1, BYTE b2, BYTE b3) 
{

MidiOut ((((b3 << 8) | b2) << 8) | (b1 | BYTE (Chan)));

}

//----------------------------------------------------------------------

void AllNotesOff () 
{

MidiOutChan (0xB0, 123, 0);

}

//----------------------------------------------------------------------

void CloseDevices () 
{

if (Out) 
	{
	midiOutReset(Out);
	AllNotesOff ();
	Sleep(10);
	midiOutClose(Out);
	Out = NULL;
	}

}

//----------------------------------------------------------------------

void OpenDevices () 

{
Out = NULL;
midiOutOpen (&Out,  - 1, 0, 0, 0);
}


//----------------------------------------------------------------------

void Midi9x(DWORD f, DWORD d)
{

char x;

switch (f)
	{

	case 262: x=0; break;
	case 294: x=2; break;
	case 330: x=4; break;
	case 350: x=5; break;
	case 392: x=7; break;
	case 440: x=9; break;
	case 494: x=11; break;

	case 277: x=1; break;
	case 311: x=3; break;
	case 370: x=6; break;
	case 415: x=8; break;
	case 466: x=10; break;

	};


MidiOutChan(0x90, BYTE ((48+x+12*(O-3))), 100);
Sleep(2*d);
}




//----------------------------------------------------------------------


void CenterWindow(HWND hwnd)
{

RECT rc;

GetWindowRect(hwnd, &rc);
MoveWindow(hwnd, GetSystemMetrics(SM_CXSCREEN)/2-(rc.right-rc.left)/2,
GetSystemMetrics(SM_CYSCREEN)/2-(rc.bottom-rc.top)/2,(rc.right-rc.left),(rc.bottom-rc.top), TRUE);

}


//----------------------------------------------------------------------

void GetTmp(char *fn)
{

*fn = 0;
GetTempPath(255, fn);
lstrcat (fn, "melc21note.wav");

}


//----------------------------------------------------------------------


void FontChooseFont ()
{

CHOOSEFONT	cf;

cf.lStructSize	= sizeof (CHOOSEFONT);
cf.hwndOwner	= hWnd;
cf.hDC		= NULL;
cf.lpLogFont	= &logfont;
cf.iPointSize	= 0;
cf.Flags		= CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
cf.rgbColors	= 0L;
cf.lCustData	= 0L;
cf.lpfnHook		= NULL;
cf.lpTemplateName	= NULL;
cf.hInstance	= NULL;
cf.lpszStyle	= NULL;
cf.nFontType	= 0;
cf.nSizeMin		= 0;
cf.nSizeMax		= 0;

if ( !ChooseFont (&cf) ) return;

SendMessage(hEdit, WM_SETFONT, (WPARAM)(HFONT) CreateFontIndirect (&logfont), 0);

InvalidateRect(hEdit, NULL, TRUE);

if ( DRIVE_FIXED == GetDriveType(NULL)  )
	{
	WritePrivateProfileString( "View", "FontFace", logfont.lfFaceName, inifile );
	WritePrivateProfileStruct( "View", "FontInfo", &logfont, sizeof(logfont), inifile);
	}

}

//----------------------------------------------------------------------


void FontSetDefaultFont()
{

memset(&logfont, 0, sizeof(logfont));

logfont.lfHeight = -48;
lstrcpy (logfont.lfFaceName, "Arial");

SendMessage(hEdit, WM_SETFONT, (WPARAM)(HFONT) CreateFontIndirect(&logfont), 0);


InvalidateRect(hEdit, NULL, TRUE);


if ( DRIVE_FIXED == GetDriveType(NULL) )
	{
	WritePrivateProfileString( "View", "FontFace", "Arial", inifile );
	WritePrivateProfileStruct( "View", "FontInfo", &logfont, sizeof(logfont), inifile);
	}
}


//----------------------------------------------------------------------


void FontSetIniFont ()
{

GetPrivateProfileStruct( "View", "FontInfo", &logfont, sizeof(logfont), inifile);
GetPrivateProfileString( "View", "FontFace", "Arial", logfont.lfFaceName, LF_FACESIZE, inifile );

SendMessage(hEdit, WM_SETFONT, (WPARAM)(HFONT) CreateFontIndirect(&logfont), 0);


InvalidateRect(hEdit, NULL, TRUE);


}


//----------------------------------------------------------------------


BOOL CreateTB(HWND hwnd)
{
	TBADDBITMAP tbab;
	TBBUTTON tbb[5];

memset(tbb,0,sizeof tbb);

// Play
tbb[0].iBitmap = 0;
tbb[0].idCommand = CMD_PLAY;
tbb[0].fsState = TBSTATE_ENABLED;
tbb[0].fsStyle = TBSTYLE_BUTTON;

// Stop
tbb[1].iBitmap = 1;
tbb[1].idCommand = CMD_STOP;
tbb[1].fsState = TBSTATE_ENABLED;
tbb[1].fsStyle = TBSTYLE_BUTTON;

//Separator
tbb[2].iBitmap = 0;
tbb[2].idCommand = 0;
tbb[2].fsState = TBSTATE_ENABLED;
tbb[2].fsStyle = TBSTYLE_SEP;

//Clear
tbb[3].iBitmap = 2;
tbb[3].idCommand = CMD_CLEAR;
tbb[3].fsState = TBSTATE_ENABLED;
tbb[3].fsStyle = TBSTYLE_BUTTON;

//Next
tbb[4].iBitmap = 3;
tbb[4].idCommand = CMD_NEXT;
tbb[4].fsState = TBSTATE_ENABLED;
tbb[1].fsStyle = TBSTYLE_BUTTON;



hToolBar = CreateToolbarEx(hwnd, WS_CHILD|WS_BORDER|WS_VISIBLE|TBSTYLE_TOOLTIPS|TBSTYLE_FLAT,
							0, 2, hInst, 
							101, 
							tbb, 
							5, //num of buttons
							16, 16, 16, 16, sizeof(TBBUTTON));

if ( NULL == hToolBar ) 
	return FALSE;

tbab.hInst = HINST_COMMCTRL;
tbab.nID   = IDB_STD_SMALL_COLOR;
SendMessage(hToolBar, 
			TB_ADDBITMAP, 
			(WPARAM) 2,(LPARAM) &tbab);

ShowWindow(hToolBar,SW_SHOW);

return TRUE;
}


//----------------------------------------------------------------------


BOOL MelodyFileOpenName(char *fn)
{

OPENFILENAME ofn ;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "mel";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter1;
else ofn.lpstrFilter = szfilter2;

return GetOpenFileName(&ofn);
}


//----------------------------------------------------------------------

BOOL MelodyFileSaveName(char *fn)
{

OPENFILENAME ofn ;

*fn = 0;

memset(&ofn,0,sizeof(OPENFILENAME));
ofn.lStructSize		= sizeof (OPENFILENAME);
ofn.hwndOwner		= hWnd;
ofn.hInstance		= NULL;
ofn.lpstrCustomFilter	= NULL;
ofn.nMaxCustFilter	= 0;
ofn.nFilterIndex	= 0;
ofn.lpstrFile		= fn;
ofn.nMaxFile		= _MAX_PATH;
ofn.lpstrFileTitle	= NULL;
ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT;
ofn.lpstrInitialDir	= NULL;
ofn.lpstrTitle		= NULL;
ofn.Flags			= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT;
ofn.nFileOffset		= 0;
ofn.nFileExtension	= 0;
ofn.lpstrDefExt		= "mel";
ofn.lCustData		= 0L;
ofn.lpfnHook		= NULL;
ofn.lpTemplateName	= NULL;

if (eng)
	ofn.lpstrFilter = szfilter1;
else ofn.lpstrFilter = szfilter2;

return GetSaveFileName(&ofn);
}

//----------------------------------------------------------------------

void MelodyFileSave(char *fn)
{

char	tmp1[256], tmp2[256];
HANDLE	f;
DWORD	r;

SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) tmp2);

memset(tmp1, 0, 256);
lstrcpy(tmp1, tmp2);

f = CreateFile(fn, GENERIC_WRITE, 0, NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

WriteFile (f, tmp1, 256, &r, NULL);
CloseHandle(f);

}

//----------------------------------------------------------------------

BOOL MelodyFileOpen(char *fn)
{

char	tmp[256];
char	_fn[MAX_PATH];
HANDLE	f;
DWORD	r;
WIN32_FIND_DATA FindFile;


if ( *(fn+1) != ':' ) 
	{
	GetCurrentDirectory(MAX_PATH, _fn);
	lstrcat(_fn, "\\");
	lstrcat(_fn, fn);
	lstrcpy(fn, _fn);
	}


FindFirstFile(fn, &FindFile);
if ( FindFile.nFileSizeLow != 256 ) return FALSE;


f = CreateFile( fn, GENERIC_READ, 0, NULL, 
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

ReadFile (f, tmp, 256, &r, NULL);
CloseHandle(f);


SendMessage(hEdit, WM_SETTEXT, (WPARAM) 0, (LPARAM) tmp);

PlaySound(NULL, NULL, SND_PURGE);
TerminateThread(hT, 0);

WriteRecent(fn);

return TRUE;
}


//----------------------------------------------------------------------


BOOL GetCmdFileName ( char* file )
{

int numArgs, i, j;
LPSTR tmp;
char end;


j = 0;

CommandLineToArgvW( GetCommandLineW(), &numArgs);
if (numArgs != 1)
	{
	tmp = GetCommandLineA();

	for (i=1; i<lstrlen(tmp); i++)
	{
		if ( *(tmp+i) == '\"') break;

	}

	i++;

	if ( *(tmp+i+1) == '\"') 
		{
		end = '\"';
		i+=2;
		}
	else 
		{
		end = ' ';
		i++;
		}

	for (i; i<lstrlen(tmp); i++)
	{
		if (*(tmp+i) == end) break;
		*(file+j) = *(tmp+i);
		j++;
	}

	*(file+j)=0;

	return TRUE;
	}

return FALSE;
}


//----------------------------------------------------------------------
void NoteToKey(char *s)
{

switch (*s)
	{

	case 'a':
		lstrcpy(s, "2");
		break;

	case 'b': 
		lstrcpy(s, "22");
		break;

	case 'c': 
		lstrcpy(s, "222");
		break;

	case 'd': 
		lstrcpy(s, "3");
		break;

	case 'e': 
		lstrcpy(s, "33");
		break;

	case 'f': 
		lstrcpy(s, "333");
		break;

	case 'g': 
		lstrcpy(s, "4");
		break;

	case '1':
		lstrcpy(s, "1");
		break;

	case '2': 
		lstrcpy(s, "2222");
		break;

	case '3': 
		lstrcpy(s, "3333");
		break;

	case '4': 
		lstrcpy(s, "44");
		break;

	case '5': 
		lstrcpy(s, "5");
		break;

	case '6': 
		lstrcpy(s, "6");
		break;

	case '7': 
		lstrcpy(s, "77");
		break;

	case '8': 
		lstrcpy(s, "8");
		break;

	case '0': 
		lstrcpy(s, "0");
		break;

	case '#':
		lstrcpy(s, "000");
		break;

	case '&': 
		lstrcpy(s, "0000");
		break;

	case 'r': 
		lstrcpy(s, "7");
		break;

	case '*': 
		lstrcpy(s, "00");
		break;

	default:
		*s= '\0';
	};

}

//----------------------------------------------------------------------


BOOL AboutFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

switch (msg)
	{
	case WM_INITDIALOG:
			hCurHand = LoadCursor(hInst, "HAND");
			OrigLabelProc=(WNDPROC)SetWindowLong(GetDlgItem(hwnd, 100), GWL_WNDPROC, (LONG)LabelProc);
			CenterWindow(hwnd);
			SendDlgItemMessage(hwnd, 101, WM_SETFONT,(WPARAM)(HFONT)CreateFont(-24, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Arial"), 0);
			SetDlgItemText(hwnd, 101, szAppTitle);

			if (eng)
				{
				SetWindowText(hwnd, "About");
				SetDlgItemText(hwnd, 102, "© 2007 Oleksandr Bogomaz");
				}

		//	SetDlgItemText(hwnd, 105, "Z540958548096\r\nE800407922706\r\nR733778429919\r\nU619452519173");
			SetFocus(GetDlgItem(hwnd, 101));
			break;


	case WM_CLOSE:
			EndDialog(hwnd, 0);
			return 1;


	}

return 0;
}



//----------------------------------------------------------------------




BOOL LabelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

RECT rc;

switch (msg)
	{
	case WM_PAINT:
			{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd,(LPPAINTSTRUCT)&ps);
			SetTextColor(hdc, RGB(0,0,255));
			SetBkColor(hdc, GetSysColor(COLOR_MENU));
			SelectObject(hdc, CreateFont(-14, 0, 0, 0, FW_REGULAR, 0, 1, 0, 0, 0, 0, 0, 0, STRING_MS_SANS));
			SetTextAlign(hdc, TA_RIGHT);
			GetClientRect(hwnd, &rc);
			TextOut(hdc, rc.right, 0, "mailto: albom85@yandex.ru", 25);
			EndPaint(hwnd, (LPPAINTSTRUCT)&ps);
			break;
			}

	case WM_SETCURSOR:
			{
			SetCursor(hCurHand);
			break;
			}


	case WM_LBUTTONDOWN:
			{
			ShellExecute( NULL, "open", "mailto:albom85@yandex.ru",NULL, NULL, SW_SHOWNORMAL);
			break;
			}

	default:
			return CallWindowProc ( OrigLabelProc, hwnd, msg, wParam, lParam );

}

return 0;
}


//----------------------------------------------------------------------


BOOL EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

DWORD t;
POINT p;

switch (msg)
	{

	case WM_RBUTTONDOWN:
		GetCursorPos(&p);
		SetForegroundWindow(hWnd);
		TrackPopupMenu(GetSubMenu(hMenu, 1), TPM_LEFTALIGN, p.x, p.y, 0, hWnd, NULL);
		break;

	case WM_KEYUP:
			if ( wParam == VK_SPACE)
				{
				PlaySound(NULL, NULL, SND_PURGE);
				TerminateThread(hT, 0);
				SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
				hT = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Parse, 
								NULL, 
								THREAD_TERMINATE,
								&t);
				break;
				}

			if ( wParam == VK_ESCAPE)
				{
				PlaySound(NULL, NULL, SND_PURGE);
				TerminateThread(hT, 0);
				break;
				}

			break;


	case WM_CHAR:
		switch ((int) wParam)
			{
			case ' ':
			case VK_ESCAPE:
				break;

			case  8:
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'r':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'R':
			case '#':
			case '&':
			case '*':
				CallWindowProc ( OrigEditProc, hwnd, msg, wParam, lParam );
				break;

			default:
				MessageBeep(0);
				return TRUE;
						
			}
		
		return TRUE;
		

	default:
			return CallWindowProc ( OrigEditProc, hwnd, msg, wParam, lParam );

	}

return 0;
}


//----------------------------------------------------------------------

void Beep9x(DWORD f, DWORD d)
{

int i;

char table[32*1024];
char head[] = {	0x52, 0x49, 0x46, 0x46, 0x32, 0, 0, 0, 0x57, 0x41, 0x56, 0x45, 0x66,
				0x6D, 0x74, 0x20, 0x12, 0, 0, 0, 1, 0, 1, 0, 0x22, 0x56, 0, 0, 0x44,
				char(0xAC), 0, 0, 2, 0, 0x10, 0, 0, 0, 0x66, 0x61, 0x63, 0x74, 4, 0, 0, 0, 
				0, 0, 0, 0, 0x64, 0x61, 0x74, 0x61};
HANDLE	hf;
DWORD	r;
UINT	len;

PlaySound(NULL, NULL, SND_PURGE);

len=40*d;


for (i=0; i<32*1024; i++)
	table[i] = char ( sin(f*(2*M_PI)*((float)i)/(32*1024))*100 );

hf = CreateFile(tempfile, GENERIC_WRITE, 0, NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


WriteFile (hf, head, sizeof(head), &r, NULL);
WriteFile (hf, &len, 4, &r, NULL);
WriteFile (hf, table, 32*1024, &r, NULL);
CloseHandle(hf);



PlaySound(tempfile, NULL, SND_SYNC);
PlaySound(NULL, NULL, SND_PURGE);

DeleteFile(tempfile);


}


//----------------------------------------------------------------------


DWORD WINAPI Parse( LPVOID pParam )
{

int i;
char ct;

starts = 0;
ends = 0;

O = 2;
for (i = 0; i < lstrlen(buf); i++)
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
					ends=i+3;
					PlayNote(buf[++i], TRUE, buf[i+1]);
					break;
			

			case '&':
					ends=i+3;
					ct = buf[++i];
					switch (ct)
						{
						case 'b': ct = 'a'; break;
						case 'd': ct = 'c'; break;
						case 'e': ct = 'd'; break;
						case 'g': ct = 'f'; break;
						case 'a': ct = 'g'; break;

						default: return -1;
						};
					PlayNote(ct, TRUE, buf[i+1]);
					break;


			default: 
					ends=i+2;
					PlayNote(buf[i], FALSE, buf[i+1]);
					break;
			}
		}

	}

return 0;
}


//----------------------------------------------------------------------

BOOL PlayNote(char n, //note
		 BOOL s, //sharp
		 char d //delay
		 )
{

DWORD f = 0; //freq
DWORD dd = 0; //duration

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


		default: return FALSE;
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

		default: return FALSE;

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
			if (gowith)
				SendMessage(hEdit, EM_SETSEL, starts, ends);
			Sleep ( dd );
			return TRUE;
			break;

		default: return FALSE;

		};

if (gowith)
	SendMessage(hEdit, EM_SETSEL, starts, ends);

switch (format)
	{
	case 0:	Beep9x( O*f, dd); break;
	case 1: Midi9x(f, dd); break;
	case 2: if (NT) Beep(O*f, dd); break;
	};

return TRUE;
}


//----------------------------------------------------------------------

void OpenRecent(int n)
{
char t1[MAX_PATH];
char t2[128];

itoa(n, t2, 10);
GetPrivateProfileString( "Open", t2, NULL, t1, MAX_PATH, inifile );

lstrcpy (fname, t1);

if ( MelodyFileOpen(t1) )
	SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)t1);

else WritePrivateProfileString( "Open", t2, "", inifile );
ReadRecent(TRUE);
}

//----------------------------------------------------------------------

void WriteRecent(char *f)
{

char t1[MAX_PATH];
char t2[128];
int i;

for (i=0; i<5; i++)
	{
	itoa(i, t2, 10);
	GetPrivateProfileString( "Open", t2, NULL, t1, MAX_PATH, inifile );
	if ( 0 == lstrcmp (f, t1) )
		return;
	}

for (i=0; i<4; i++)
	{
	itoa(4-i-1, t2, 10);
	GetPrivateProfileString( "Open", t2, NULL, t1, MAX_PATH, inifile );
	itoa(4-i, t2, 10);
	WritePrivateProfileString( "Open", t2, t1, inifile );
	}

WritePrivateProfileString( "Open", "0", f, inifile );
ReadRecent(TRUE);
}

//----------------------------------------------------------------------

void ReadRecent(BOOL mod)
{

DestroyMenu(hRecent);

if (mod)
	DeleteMenu(GetSubMenu(hMenu, 0), 2, MF_BYPOSITION);

hRecent = CreatePopupMenu();

unsigned int i;
char t1[MAX_PATH];
char n[3];

memset (t1, 0, MAX_PATH);

for (i=0; i<5; i++)
	{
	itoa(i, n, 10);
	GetPrivateProfileString( "Open", n, NULL, t1, MAX_PATH, inifile );
	if (*t1)
		AppendMenu(hRecent, MF_STRING, CMD_FILE1+i, t1);

	}

if (eng)
	InsertMenu(GetSubMenu(hMenu, 0), 2, MF_BYPOSITION|MF_STRING|MF_POPUP, 
				(int)hRecent,"Recent Files");
else 
	InsertMenu(GetSubMenu(hMenu, 0), 2, MF_BYPOSITION|MF_STRING|MF_POPUP, 
				(int)hRecent,"Открыть снова");

}

//----------------------------------------------------------------------


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
      LPSTR lpszCmdLine, int nCmdShow)
{

MSG msg;
WNDCLASS wc;


InitCommonControls();
LoadLibrary("riched32.dll"); 

hInst = hInstance;

memset(&wc,0,sizeof(wc));
wc.lpszClassName = "c155dlg";
wc.style = 0;
wc.lpfnWndProc = (WNDPROC) MainWndProc;
wc.cbClsExtra = wc.cbWndExtra = 0;
wc.hInstance = hInst;
wc.hIcon = LoadIcon(hInst, "C115ICON");
wc.hCursor = LoadCursor((HINSTANCE) NULL, IDC_ARROW);
wc.lpszMenuName = NULL;
wc.hbrBackground = (HBRUSH) COLOR_WINDOW;

RegisterClass(&wc);

memset(&wc,0,sizeof(wc));
wc.lpfnWndProc = DefDlgProc;
wc.cbWndExtra = DLGWINDOWEXTRA;
wc.lpszMenuName = NULL;
wc.hInstance = hInst;
wc.hIcon = LoadIcon(hInst, "C115ICON");
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
wc.lpszClassName = "aboutdlg";
RegisterClass(&wc);

GetTmp(tempfile);

hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, "c155dlg", szAppTitle,
					WS_OVERLAPPEDWINDOW, 
					0, 0, 550, 175, 
					(HWND) NULL, (HMENU) NULL, hInst, (LPVOID) NULL);

ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);

hAccel = LoadAccelerators (hInst, "KEYS");

while (GetMessage(&msg, (HWND) NULL, 0, 0))
		{
		if (!TranslateAccelerator (hWnd, hAccel, &msg))
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}
		}

return msg.wParam;

}


//----------------------------------------------------------------------


LRESULT WINAPI MainWndProc( 
      HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

DWORD t;

LPTOOLTIPTEXT lpttt;
LPNMHDR lpnmhdr;

char tmp1[256], tmp2[2048];

int parts[3];

switch (msg)
	{

	case WM_CREATE:
			hWnd = hwnd;

			NT = IsNT();

			memset (&notdata, 0, sizeof(notdata) );
			notdata.cbSize = sizeof(notdata);
			notdata.hWnd = hWnd;
			notdata.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			notdata.uCallbackMessage = WM_TRAY_ICON;
			notdata.hIcon = LoadIcon(hInst, "ZC115ICON");
			lstrcpyn(notdata.szTip, szAppTitle, sizeof(notdata.szTip));

			CreateTB(hWnd);

			hEdit = CreateWindow("RichEdit20A", "",
					WS_CHILD|WS_VISIBLE|ES_LEFT|ES_LOWERCASE|ES_AUTOHSCROLL|WS_HSCROLL|ES_DISABLENOSCROLL, 
					0, 0, 0, 0, 
					(HWND) hwnd, (HMENU) NULL, hInst, (LPVOID) NULL);

			hStatus = CreateWindow("msctls_statusbar32", "",
					WS_CHILD|WS_VISIBLE, 
					0, 0, 0, 0, 
					(HWND) hwnd, (HMENU) NULL, hInst, (LPVOID) NULL);

			OrigEditProc=(WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (LONG)EditProc);
			SendMessage (hEdit, EM_LIMITTEXT, 256, 0L) ;

			GetCurrentDirectory(MAX_PATH-1, inifile);
			lstrcat (inifile, "\\melc115.ini");

			memset (&find, 0, sizeof(find) );

			if ( FindFirstFile(inifile, &find) == INVALID_HANDLE_VALUE)
				{

				eng = TRUE;
				gowith = TRUE;
				tray = FALSE;

				SetMenu( hWnd, LoadMenu(hInst, "MENU_ENG") );
				CheckMenuItem(GetMenu(hWnd), CMD_RUS, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), CMD_ENG, MF_CHECKED);

				FontSetDefaultFont();

				CheckMenuItem(GetMenu(hWnd), CMD_GOWITH, MF_CHECKED);

				if ( DRIVE_FIXED == GetDriveType(NULL) )
					{
					WritePrivateProfileString( "View", "Watch", "1", inifile );
					WritePrivateProfileString( "View", "Tray", "0", inifile );
					WritePrivateProfileString( "Lang", "Lang", "0", inifile );
					WritePrivateProfileString( "View", "OnTop", "0", inifile );

					WritePrivateProfileStruct( "View", "TextColor", &txtc, sizeof(txtc), inifile);
					WritePrivateProfileStruct( "View", "BgColor", &bgc, sizeof(bgc), inifile);

					WritePrivateProfileString( "Sound", "Type", "0", inifile );

					WritePrivateProfileString( "Open", "0", "", inifile );
					WritePrivateProfileString( "Open", "1", "", inifile );
					WritePrivateProfileString( "Open", "2", "", inifile );
					WritePrivateProfileString( "Open", "3", "", inifile );
					WritePrivateProfileString( "Open", "4", "", inifile );
					}

				}
			else
				{
				FontSetIniFont();

				GetPrivateProfileString( "View", "Watch", NULL, watch, 2,inifile );
				if (*watch == '0')
					{
					gowith = FALSE;
					CheckMenuItem(GetMenu(hWnd), CMD_GOWITH, MF_UNCHECKED);
					}
				else CheckMenuItem(GetMenu(hWnd), CMD_GOWITH, MF_UNCHECKED);

				GetPrivateProfileString( "Lang", "Lang", NULL, watch, 2,inifile );
				if (*watch == '0')
					{
					eng = TRUE;
					SetMenu( hWnd, LoadMenu(hInst, "MENU_ENG") );
					CheckMenuItem(GetMenu(hWnd), CMD_RUS, MF_UNCHECKED);
					CheckMenuItem(GetMenu(hWnd), CMD_ENG, MF_CHECKED);
					}
				else
					{
					eng = FALSE;
					SetMenu( hWnd, LoadMenu(hInst, "MENU_RUS") );
					CheckMenuItem(GetMenu(hWnd), CMD_RUS, MF_CHECKED);
					CheckMenuItem(GetMenu(hWnd), CMD_ENG, MF_UNCHECKED);
					}
				
				hMenu = GetMenu(hWnd);

				GetPrivateProfileString( "View", "OnTop", NULL, ontop, 2,inifile );
				if (*ontop == '1')
					{
					CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_CHECKED);
					SetWindowPos(hWnd, HWND_TOPMOST	, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
					}
				else CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_UNCHECKED);
			
				GetPrivateProfileStruct( "View", "TextColor", &txtc, sizeof(txtc), inifile);
				GetPrivateProfileStruct( "View", "BgColor", &bgc, sizeof(txtc), inifile);
				memset (&cf, 0, sizeof(cf));
				cf.cbSize = sizeof(cf);
				cf.crTextColor = ( DWORD ) txtc;
				cf.dwMask = CFM_COLOR;
				SendMessage(hEdit, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
				SendMessage (hEdit, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)(DWORD)bgc);

				GetPrivateProfileString( "View", "Tray", NULL, watch, 2,inifile );
				if (*watch == '0')
					{
					tray = FALSE;	
					CheckMenuItem(GetMenu(hWnd), CMD_TRAY, MF_UNCHECKED);
					}
				else
					{
					tray = TRUE;
					Shell_NotifyIcon(NIM_ADD, &notdata);
					CheckMenuItem(GetMenu(hWnd), CMD_TRAY, MF_CHECKED);
					}
				}

				GetPrivateProfileString( "Sound", "Type", NULL, watch, 2,inifile );

				switch (*watch)
					{
					case '0':
						format = 0;
						CheckMenuItem(GetMenu(hWnd), CMD_WAVOUT, MF_CHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_MIDOUT, MF_UNCHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_SPKOUT, MF_UNCHECKED);
						break;

					case '1':
						format = 1;
						CheckMenuItem(GetMenu(hWnd), CMD_MIDOUT, MF_CHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_WAVOUT, MF_UNCHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_SPKOUT, MF_UNCHECKED);
						break;

					case '2':
						format = 2;
						CheckMenuItem(GetMenu(hWnd), CMD_SPKOUT, MF_CHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_MIDOUT, MF_UNCHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_WAVOUT, MF_UNCHECKED);
						break;
					};

			ReadRecent(FALSE);

			if ( GetCmdFileName (fname) )
				{
				if ( MelodyFileOpen(fname) )
					SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)fname);
				}



			CenterWindow(hWnd);
			
			OpenDevices ();
			AllNotesOff();
			break;


	case WM_SIZE:
			SendMessage(hToolBar,msg,wParam,lParam);
			MoveWindow (hStatus, 3, HIWORD(lParam)-19, LOWORD(lParam)-6, 18, 1);
			MoveWindow (hEdit, 0, 29, LOWORD(lParam), HIWORD(lParam)-50, 1);
			if (tray)
				if ( IsIconic(hWnd) ) ShowWindow(hWnd, SW_HIDE);

			parts[0]=LOWORD(lParam)-150;
			parts[1]=LOWORD(lParam)-100;
			parts[2]=-1;
			SendMessage(hStatus, SB_SETPARTS, (WPARAM) 3, (LPARAM) (LPINT) parts);
			break;


	case WM_MENUSELECT:
		SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
		if ( ! *buf )
			{
			EnableMenuItem(hMenu, CMD_SEND, MF_GRAYED);
			EnableMenuItem(hMenu, CMD_RTTTL, MF_GRAYED);
			EnableMenuItem(hMenu, CMD_MIDI, MF_GRAYED);
			EnableMenuItem(hMenu, CMD_NOKIA, MF_GRAYED);
			EnableMenuItem(hMenu, CMD_SIEM, MF_GRAYED);
			}
		else
			{
			EnableMenuItem(hMenu, CMD_SEND, MF_ENABLED);
			EnableMenuItem(hMenu, CMD_RTTTL, MF_ENABLED);
			EnableMenuItem(hMenu, CMD_MIDI, MF_ENABLED);
			EnableMenuItem(hMenu, CMD_NOKIA, MF_ENABLED);
			EnableMenuItem(hMenu, CMD_SIEM, MF_ENABLED);
			}

		if (! NT)
			EnableMenuItem(hMenu, CMD_SPKOUT, MF_GRAYED);
		else EnableMenuItem(hMenu, CMD_SPKOUT, MF_ENABLED);

		break;


	case WM_SETFOCUS:
		SetFocus (hEdit) ;
		break ;


	case WM_NOTIFY:
		lpnmhdr = (LPNMHDR) lParam;
		if (lpnmhdr -> code == TTN_NEEDTEXT)
			{
			
			lpttt = (LPTOOLTIPTEXT) lParam;
			lpttt->hinst = hInst;

			switch (lpttt->hdr.idFrom)
				{
				case CMD_PLAY:
					if (eng)
						lpttt->lpszText = "Play (space)";
					else lpttt->lpszText = "Играть (пробел)";
					break;

				case CMD_STOP:
					if (eng)
						lpttt->lpszText = "Stop (esc)";
					else lpttt->lpszText = "Стоп (esc)";
					break;

				case CMD_CLEAR:
					if (eng)
						lpttt->lpszText = "Unmark";
					else lpttt->lpszText = "Снять выделение";
					break;

				case CMD_NEXT:
					if (eng)
						lpttt->lpszText = "Next symbol";
					else lpttt->lpszText = "Следующий символ";
					break;


				};

		}
		break;


	case WM_TRAY_ICON:
			if ( (LOWORD(lParam) == WM_LBUTTONUP) || 
				 (LOWORD(lParam) == WM_RBUTTONUP) )
				if ( ! IsWindowVisible(hWnd) ) 
					{
					ShowWindow(hWnd, SW_SHOW);
					ShowWindow(hWnd, SW_SHOWNORMAL);
					SetForegroundWindow(hWnd);
					}
				else
					ShowWindow(hWnd, SW_HIDE);
			break;


	case WM_DESTROY:
			CloseDevices();
			Shell_NotifyIcon(NIM_DELETE, &notdata);
			PostQuitMessage(0);
			break;


	case WM_DROPFILES:
			DragQueryFile( (HDROP) wParam, 0, fname, MAX_PATH);
			MelodyFileOpen(fname);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)fname);
			break;


	case WM_COMMAND:
			switch (LOWORD (wParam) )
			{


				case CMD_SEND:
					SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) tmp1);

					if ( *tmp1 )
						{
						wsprintf (tmp2, "mailto:albom85@yandex.ru?subject=melc115&body=%s", tmp1);

						ShellExecute( NULL, "open", tmp2,
									NULL, NULL, SW_SHOWNORMAL);
						}
					break;

				case CMD_UNDO:
					SendMessage(hEdit, EM_UNDO, 0, 0);
					break;

				case CMD_REDO:
					SendMessage(hEdit, EM_REDO, 0, 0);
					break;

				case CMD_COPY:
					SendMessage(hEdit, WM_COPY, 0, 0);
					break;

				case CMD_PASTE:
					SendMessage(hEdit, WM_PASTE, 0, 0);
					break;

				case CMD_CUT:
					SendMessage(hEdit, WM_CUT, 0, 0);
					break;
		
				case CMD_DEL:
					SendMessage(hEdit, WM_CLEAR, 0, 0);
					break;

				case CMD_SELALL:
					SendMessage(hEdit, EM_SETSEL, 0, -1);
					break;

				case CMD_PLAY:
					PlaySound(NULL, NULL, SND_PURGE);
					AllNotesOff();
					TerminateThread(hT, 0);
					SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) buf);
					hT = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Parse, 
								NULL, 
								THREAD_TERMINATE,
								&t);
					break;

				case CMD_STOP:
					PlaySound(NULL, NULL, SND_PURGE);
					AllNotesOff();
					TerminateThread(hT, 0);
					break;

				case CMD_NEW:
					PlaySound(NULL, NULL, SND_PURGE);
					AllNotesOff();
					TerminateThread(hT, 0);

					memset(fname, 0, MAX_PATH);
					SendMessage(hEdit, WM_SETTEXT, (WPARAM) 0, (LPARAM) NULL);
					SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)NULL);
					break;

				case CMD_OPEN:
					if ( MelodyFileOpenName(fname) )
						{
						if ( MelodyFileOpen(fname) )
							SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)fname);
						}
					break;

				case CMD_SAVE:
					if ( *fname )
						{
						MelodyFileSave(fname);
						SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)fname);
						break;
						}

				case CMD_SAVEAS:
					if ( MelodyFileSaveName(fname) )
						{
						MelodyFileSave(fname);
						SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)fname);
						}
					break;

				case CMD_EXIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;

				case CMD_HELP:
					ShellExecute( NULL, "open", "./help/help.htm",
									NULL, NULL, SW_SHOWNORMAL);
					break;

				case CMD_ABOUT:
						DialogBox(hInst, "ABOUTDLG", hWnd, (DLGPROC) AboutFunc);
					break;

				case CMD_FONT:
					FontChooseFont();
					cf.crTextColor = txtc;
					SendMessage(hEdit, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
					break;

				case CMD_SFONT:
					FontSetDefaultFont();
					cf.crTextColor = txtc;
					SendMessage(hEdit, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
					break;

				case CMD_COLTXT:
					memset (&txtcol, 0, sizeof(txtcol));
					txtcol.hInstance = (HWND) hInst;
					txtcol.lStructSize = sizeof (txtcol);
					txtcol.hwndOwner = hWnd;
					txtcol.Flags = CC_RGBINIT;
					txtcol.lpCustColors = (LPDWORD) acrCustClr;
					if ( ChooseColor(&txtcol) )
						{
						memset (&cf, 0, sizeof(cf));
						cf.cbSize = sizeof(cf);
						cf.crTextColor = txtc = (DWORD) txtcol.rgbResult;
						cf.dwMask = CFM_COLOR;
						SendMessage(hEdit, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileStruct( "View", "TextColor", &txtc, sizeof(txtc), inifile);
						}

					break;


				case CMD_COLBG:
					memset (&bgcol, 0, sizeof(bgcol));
					bgcol.hInstance = (HWND)hInst;
					bgcol.lStructSize = sizeof (bgcol);
					bgcol.hwndOwner = hWnd;
					bgcol.Flags = CC_RGBINIT;
					bgcol.lpCustColors = (LPDWORD) acrCustClr;
					if ( ChooseColor(&bgcol) )
						{
						bgc = (DWORD) bgcol.rgbResult;
						SendMessage ( hEdit, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)bgcol.rgbResult);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileStruct( "View", "BgColor", &bgc, sizeof(bgc), inifile);
						}
					break;


				case CMD_COLST:
						bgc = (DWORD) 0xFFFFFF;
						SendMessage (hEdit, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)bgc);
						txtc = cf.crTextColor = ( DWORD ) 0;
						SendMessage(hEdit, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							{
							WritePrivateProfileStruct( "View", "TextColor", &txtc, sizeof(txtc), inifile);
							WritePrivateProfileStruct( "View", "BgColor", &bgc, sizeof(bgc), inifile);
							}
						break;

				case CMD_GOWITH:
					SendMessage(hEdit, EM_SETSEL, 0, 0);
					if (gowith)
						{
						gowith = FALSE;
						CheckMenuItem(GetMenu(hWnd), CMD_GOWITH, MF_UNCHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "View", "Watch", "0", inifile );
						}
						else 
						{
						gowith = TRUE;
						CheckMenuItem(GetMenu(hWnd), CMD_GOWITH, MF_CHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "View", "Watch", "1", inifile );

						}
					break;


				case CMD_TRAY:
						if (tray)
						{
						tray = FALSE;
						CheckMenuItem(GetMenu(hWnd), CMD_TRAY, MF_UNCHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "View", "Tray", "0", inifile );
						Shell_NotifyIcon(NIM_DELETE, &notdata);
						}
						else
						{
						tray = TRUE;
						CheckMenuItem(GetMenu(hWnd), CMD_TRAY, MF_CHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "View", "Tray", "1", inifile );
						Shell_NotifyIcon(NIM_ADD, &notdata);
						}
					break;


				case CMD_ENG:
					eng = TRUE;
					SetMenu( hWnd, LoadMenu(hInst, "MENU_ENG") );
					hMenu = GetMenu(hWnd);
					CheckMenuItem( hMenu, CMD_RUS, MF_UNCHECKED);
					CheckMenuItem( hMenu, CMD_ENG, MF_CHECKED);

					if ( DRIVE_FIXED == GetDriveType(NULL) )
						WritePrivateProfileString( "Lang", "Lang", "0", inifile );

					if (gowith)
						CheckMenuItem( hMenu, CMD_GOWITH, MF_CHECKED);
					else 
						CheckMenuItem( hMenu, CMD_GOWITH, MF_UNCHECKED);

					if (tray)
						CheckMenuItem( hMenu, CMD_TRAY, MF_CHECKED);
					else 
						CheckMenuItem( hMenu, CMD_TRAY, MF_UNCHECKED);

					if (*ontop == '1')
						CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_CHECKED);
					else
						CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_UNCHECKED);

					ReadRecent(FALSE);
					break;

				case CMD_RUS:
					eng = FALSE;
					SetMenu( hWnd, LoadMenu(hInst, "MENU_RUS") );
					hMenu = GetMenu(hWnd);
					CheckMenuItem( hMenu, CMD_RUS, MF_CHECKED);
					CheckMenuItem( hMenu, CMD_ENG, MF_UNCHECKED);
					
					if ( DRIVE_FIXED == GetDriveType(NULL) )
						WritePrivateProfileString( "Lang", "Lang", "1", inifile );

					if (gowith)
						CheckMenuItem( hMenu, CMD_GOWITH, MF_CHECKED);
					else 
						CheckMenuItem( hMenu, CMD_GOWITH, MF_UNCHECKED);

					if (tray)
						CheckMenuItem( hMenu, CMD_TRAY, MF_CHECKED);
					else 
						CheckMenuItem( hMenu, CMD_TRAY, MF_UNCHECKED);

					if (*ontop == '1')
						CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_CHECKED);
					else
						CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_UNCHECKED);

					ReadRecent(FALSE);
					break;

				case CMD_SITE:
					ShellExecute( NULL, "open", "http://albom06.boom.ru",NULL, NULL, SW_SHOWNORMAL);
					break;

				case CMD_NEXT:
					PlaySound(NULL, NULL, SND_PURGE);
					TerminateThread(hT, 0);
					SendMessage(hEdit, EM_GETSEL, (WPARAM)&starts, 
									(LPARAM) (LPDWORD)&ends);
					
					SendMessage(hEdit, WM_GETTEXT, (WPARAM) 255, (LPARAM) tmp1);

					if ( ( (starts==ends)&&ends ) || !*tmp1)
						break;

					SendMessage(hEdit, EM_SETSEL, 0, ++ends);

					tmp2[1] = '\0';
					tmp2[0] = *(tmp1+ends-1);

					SendMessage(hStatus, SB_SETTEXT, (WPARAM)1, (LPARAM) tmp2 );

					NoteToKey(tmp2);

					SendMessage(hStatus, SB_SETTEXT, (WPARAM)2, (LPARAM) tmp2 );
					break;

				case CMD_CLEAR:
					PlaySound(NULL, NULL, SND_PURGE);
					TerminateThread(hT, 0);

					SendMessage(hEdit, EM_SETSEL, 0, 0);
					SendMessage(hStatus, SB_SETTEXT, (WPARAM)1, (LPARAM) NULL );
					SendMessage(hStatus, SB_SETTEXT, (WPARAM)2, (LPARAM) NULL );
					break;

				case CMD_ONTOP:
						if (*ontop == '1')
						{
						*ontop = '0';
						CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_UNCHECKED);
						SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "View", "OnTop", "0", inifile );
						}
						else
						{
						*ontop = '1';
						CheckMenuItem(GetMenu(hWnd), CMD_ONTOP, MF_CHECKED);
						SetWindowPos(hWnd, HWND_TOPMOST	, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "View", "OnTop", "1", inifile );
						}
					break;


				case CMD_WAVOUT:
						AllNotesOff();
						PlaySound(NULL, NULL, SND_PURGE);
						TerminateThread(hT, 0);
						format = 0;
						CheckMenuItem(GetMenu(hWnd), CMD_MIDOUT, MF_UNCHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_WAVOUT, MF_CHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_SPKOUT, MF_UNCHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "Sound", "Type", "0", inifile );
						SendMessage(hEdit, EM_SETSEL, 0, 0);
						break;


				case CMD_MIDOUT:
						AllNotesOff();
						PlaySound(NULL, NULL, SND_PURGE);
						TerminateThread(hT, 0);
						format = 1;
						CheckMenuItem(GetMenu(hWnd), CMD_MIDOUT, MF_CHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_WAVOUT, MF_UNCHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_SPKOUT, MF_UNCHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "Sound", "Type", "1", inifile );
						SendMessage(hEdit, EM_SETSEL, 0, 0);
						break;


				case CMD_SPKOUT:
						AllNotesOff();
						PlaySound(NULL, NULL, SND_PURGE);
						TerminateThread(hT, 0);
						format = 2;
						CheckMenuItem(GetMenu(hWnd), CMD_SPKOUT, MF_CHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_MIDOUT, MF_UNCHECKED);
						CheckMenuItem(GetMenu(hWnd), CMD_WAVOUT, MF_UNCHECKED);
						if ( DRIVE_FIXED == GetDriveType(NULL) )
							WritePrivateProfileString( "Sound", "Type", "2", inifile );
						SendMessage(hEdit, EM_SETSEL, 0, 0);
						break;


				case CMD_VOL:
						ShellExecute( NULL, "open", "sndvol32.exe",
									NULL, NULL, SW_SHOWNORMAL);
						break;


				case CMD_RTTTL:
						ExportRTTTL();
						break;

				case CMD_MIDI:
						ExportMidi();
						break;

				case CMD_NOKIA:
						ExportNokia();
						break;

				case CMD_SIEM:
						ExportSiemens();
						break;

				case CMD_NOKIA_I:
						ImportNokia();
						break;


				case CMD_FILE1:
				case CMD_FILE2:
				case CMD_FILE3:
				case CMD_FILE4:
				case CMD_FILE5:
						OpenRecent (LOWORD (wParam) - CMD_FILE1);
						break;


			}
			break;


	default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
    }


return 0L;
}


//----------------------------------------------------------------------