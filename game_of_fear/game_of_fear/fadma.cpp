#include <windows.h>
#include <locale>
#include <stdio.h>
#include <iostream>
#include "fadma.h"
using namespace std;																				// пока не нажата ни одна из клавишь 0, 1, Enter, ничего не делать, после чего,

inline double rad_ (double degree) {return degree * Pi / 180;}			// перевод угла из градусов в радианы
inline double deg_ (double radian) {return radian * 180 / Pi;}			// перевод угла из радиан в градусы

char *Str (char *Str_, char *fmt, ...) {
	va_list ap;
	char *p, temp [500], *out = Str_, format[20], *str;
	int ival, ind;
	double dval;
	va_start (ap, fmt);
	for (p = fmt; *p; ++p) {
		if (*p != '%') {
			*out++ = *p;
			continue;
		}
		ind = 0;
		while (isdigit(*++p))
			ind = ind * 10 + *p - '0';
		switch (*p) {
		case 'd':
			ival = va_arg (ap, int);
			if (ind)
				sprintf_s <20> (format, "%%%dd", ind);
			else
				strcpy_s <20> (format, "%d");
			sprintf_s <500> (temp, format, ival);
			for (char *t = temp; *t; )
				*out++ = *t++;
			break;
		case 'f':
			dval = va_arg (ap, double);
			if (ind)
				sprintf_s <20> (format, "%%%df", ind);
			else
				strcpy_s <20> (format, "%f");
			sprintf_s <500> (temp, format, dval);
			for (char *t = temp; *t; )
				*out++ = *t++;
			break;
		case 's':
			str = va_arg (ap, char *);
			if (ind)
				sprintf_s <20> (format, "%%%ds", ind);
			else
				strcpy_s <20> (format, "%s");
			sprintf_s <500> (temp, format, str);
			for (char *t = temp; *t; )
				*out++ = *t++;
			break;
		case 'c':
			*out++ = va_arg (ap, char);
			break;
		}
	}
	*out = 0;
	va_end (ap);
	return Str_;
}

HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);

Point scrn;

int Where::x() {
	_CONSOLE_SCREEN_BUFFER_INFO p;
	GetConsoleScreenBufferInfo (hCons,&p);
	return p.dwCursorPosition.X;
}
int Where::y() {
	_CONSOLE_SCREEN_BUFFER_INFO p;
	GetConsoleScreenBufferInfo (hCons,&p);
	return p.dwCursorPosition.Y;
}
Point Where::p() {
	_CONSOLE_SCREEN_BUFFER_INFO p;
	GetConsoleScreenBufferInfo (hCons,&p);
	return p.dwCursorPosition;
}

void gotoxy(int x_pos, int y_pos) {
	COORD scrn;
	scrn.X = x_pos; scrn.Y = y_pos;
	SetConsoleCursorPosition(hCons, scrn);
}

Point operator + (const Point &p0, const Point &p1) {
	Point p(p0);
	p.x += p1.x; 
	p.y += p1.y;
	return p;
}
Point operator - (const Point &p0, const Point &p1) {
	Point p(p0);
	p.x -= p1.x; 
	p.y -= p1.y;
	return p;
}
Point operator * (const Point &p, const int i){
	return Point(p.x*i, p.y*i);
}
Point operator * (const int i,const Point &p) {
	return p*i;
}
bool operator == (const Point p0, const Point p1) {
	return p0.x == p1.x && p0.y == p1.y;
}
bool operator != (const Point p0, const Point p1) {
	return p0.x != p1.x || p0.y != p1.y;
}

void getscreensize (Point &rdp) {
	tagPOINT p;
	GetCursorPos (&p);
	int fl = butdwn(VK_LBUTTON);
	if (fl)
		mouse_event (MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, ~(~long(0) << 16), ~(~long(0) << 16), 0, 0);
	GetCursorPos (&rdp);
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, coord_ (p.x, rdp.x), coord_ (p.y, rdp.y), 0, 0);
	if (fl)
		mouse_event (MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}
void click (Point P, bool LEFT) {
	if (P.x > scrn.x || P.y > scrn.y)
		return;
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, coord_ (P.x, scrn.x), coord_ (P.y, scrn.y), 0, 0);
	mouse_event (LEFT ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event (LEFT ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}
inline void click (int x, int y, bool LEFT) {
	click (Point (x, y), LEFT);
}
void line (Point A, Point B, bool LEFT) {
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, coord_ (A.x, scrn.x), coord_ (A.y, scrn.y), 0, 0);
	mouse_event (LEFT ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, coord_ (B.x, scrn.x), coord_ (B.y, scrn.y), 0, 0);
	mouse_event (LEFT ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}
void line (int x1, int y1, int x2, int y2, bool LEFT) {
	line (Point (x1, y1), Point (x2, y2), LEFT);
}
// вывод текста без "разрыва" слов
void print_l(const char *text, int indent, int width) {
	char *str = new char[strlen (text)+1], *space = new char[indent+1];
	char *p = str;
	const char *t = text;
	while (*t)
		*p++ = *t++;
	*p = 0;
	space[indent] = 0;
	for (p = space; *p; )
		*p++ = ' ';
	p = str, t = str;
	wc_ (*p);
	for (int fs = width; *p;) {
		if (*++p > 0 && isspace (*p) || *p == 0)
			if (fs + (t - p) > 0) {
				fs -= p-t-1;
				while (t < p-1)
					wc_ (*++t);
			} else {
				wc_ ('\n');
				ws_ (space);
				fs = width + t - p;
				++t;
				while (t < p-1)
					wc_ (*++t);
			}
	}
	delete []str;
	delete []space;
}
void error__(char *text) {
	char s[400] = "ERROR: ";
	strcat_s <400> (s, text);
	strcat_s <400> (s, ".");
	print_l (s);
}

inline bool isruss(int Char)
{return Char > 191 && Char < 256 || Char == 168 || Char == 184; }

bool readallc (int (*f) (int), bool while_f, FILE *File, char *string_) {
	if(string_) {
		while((f(*string_ = getc(File))==0) == !while_f)
			if(*string_++ == EOF) {
				*(string_ - 1) = 0;
				return 0;
			}
		ungetc(*string_, File);
		*string_ = 0;
	}
	else {
		int c;
		while((f(c = getc(File)) == 0) == !while_f)
			if(c == EOF)
				return 0;
		ungetc(c, File);
	}
	return 1;
}

string tolower (string s) {
	for (string::iterator p = s.begin(), end = s.end (); p < end; ++p)
		*p = tolower (*p);
	return s;
}

string toupper (string s) {
	for (string::iterator p = s.begin(), end = s.end (); p < end; ++p)
		*p = toupper (*p);
	return s;
}

void printintstr(int *StringOfInt, FILE *File, bool torus) {
	if(torus) {
		int c, co;
		while(*StringOfInt) {
			if((c = *StringOfInt++) > 223)
				if(c < 240)
					co = c-64;
				else
					co = c-16;
			else
				co = c - 64;
			if(co == 120)
				co = 241;
			else if(co == 104)
				co = 240;
			putc(co, File);
		}
		return;
	}
	while(*StringOfInt)
		putc(*StringOfInt++,File);
}

template <class T>
void binout (T number, bool endline,int Number_of_digits) {
	int n = sizeof(T) * 8;
	T mask = T(1) << (n-1);
	if(Number_of_digits > -1)
		number <<= n - Number_of_digits;
	else
		Number_of_digits = n;
	for (int i = 0; i<Number_of_digits; ++i, number <<= 1)
		cout << int(bool(number & mask));
	if (endline)
		wc_ ('\n');
}
inline int setcolor (int color) {
	SetConsoleTextAttribute (hCons, color);
	return color;
}
int setcolor (const char *color) {
	int c = to_color (color);
	setcolor (c);
	return c;
}
int to_color (const char *color) {
	int c = 0;
	bool fl = 1;
	for (const char *p = color; *p; ++p)
		switch (tolower(*p)) {
		case '|':
			fl = 0;
			break;
		case 'i':
			c |= (8) << 4*fl;
			break;
		case 'r':
			c |= (4) << 4*fl;
			break;
		case 'g':
			c |= (2) << 4*fl;
			break;
		case 'b':
			c |= (1) << 4*fl;
			break;
		default: {
			char s[200];
			error (Str(s, "символ %c не допускается в строке, задающей цвет текста в функции setcolor", *p));
		}
	}
	return c;
}

int MENU_Color = 0;
int menu (string title, int num_of_options, ...) {
	va_list ap;
	va_start (ap, num_of_options);
	char s[800], str [800];
	char **opt = new char* [num_of_options];
	int *y = new int[num_of_options], temp;
	mod cur_opt (num_of_options);
	int color = MENU_Color ? MENU_Color : (rand () % 6) + 1;
	system ("cls");
	setcolor (8 | color);
	color <<= 4;
	print_l (("\n    " + title + "\n\n").c_str ());
	setcolor (0x7);
	int ind = 6;
	for (int i = 0; i < num_of_options; ++i) {
		y[i] = Where::y ();
		strcpy_s <800> (s, va_arg (ap, char *));
		opt[i] = new char[temp = strlen (s) +1];
		strcpy_s (opt[i], temp, s);
		printf ("   ");
		print_l (Str (str, "%s\n", opt[i]), ind, 79-ind);
	}
	bool ret = 0;
	setpressbut (187);
	timer t;
	UINT wt[2] = {500, 50};
	bool lt = 0;
	while (1) {
		gotoxy (0, y[cur_opt]);
		setcolor (15 | color);
		printf (" %c ",char(16));
		print_l (Str (str, "%s %c", opt[cur_opt], char(17)), ind, 79-ind);
		gotoxy (79,24);
		setcolor (0x7);
		waitbutup (VK_RETURN);
		t.reset ();
		waitbutup (VK_UP)
			if(t.get_time () > wt[lt]) {
				lt = 1;
				break;
			}
		waitbutup (VK_DOWN)
			if(t.get_time () > wt[lt]) {
				lt = 1;
				break;
			}
		if (t.get_time () < wt[lt])
			lt = 0;
		waitbutdwn (VK_RETURN) {
			FOR_ (i, '1', '1' + min (9, num_of_options))
				if (butdwn (i)) {
					waitbutup (i);
					cur_opt = i - '1';
					ret = 1;
					break;
				}
			if (butdwn (VK_UP) || butdwn (VK_DOWN) || ret)
				break;
			if (butdwn (VK_ESCAPE))
				exit (0);
		}
		if (butdwn (VK_UP))
			setpressbut (187);
		if (butdwn (VK_RETURN)) {
			cin.ignore (100, '=');
			break;
		}
		if (ret) {
			setpressbut (189);
			setpressbut (VK_RETURN);
			cin.ignore (100, '-');
			break;
		}
		gotoxy (0, y[cur_opt]);
		printf ("   ");
		print_l (Str (str, "%s  ", opt[cur_opt]), ind, 79-ind);
		if (butdwn (VK_UP))
			--cur_opt;
		else
			++cur_opt;
	}
	system ("cls");
	FOR (i, num_of_options)
		delete []opt[i];
	delete []opt;
	delete []y;
	gotoxy(0, 0);
	va_end (ap);
	return int(cur_opt)+1;
}
#define _CRT_SECURE_NO_WARNINGS
int getval (int *var,char * text, bool clearScreen) {
	if (clearScreen) 
		clrscr;
	setpressbut(VK_RETURN);
	cin.clear ();
	cin >> *var;
	return *var;
}