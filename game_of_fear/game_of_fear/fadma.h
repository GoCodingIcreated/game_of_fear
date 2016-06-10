#pragma once
#define VERSION		1.051
#include <windows.h>
#include <locale>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#define butdwn(vk_button)		(GetAsyncKeyState(vk_button) & 1<<15)			// если клавиша vk_button нажата, эквивалентна единице, в противном случае - нулю
#define butup(vk_button)		(!butdwn(vk_button))							// если клавиша vk_button нажата, эквивалентна нулю, в противном случае - единице
#define waitbutdwn(vk_button)	while(butup(vk_button))							// ожидание нажатия клавиши vk_button
#define waitbutup(vk_button)	while(butdwn(vk_button))						// пока клвавиша нажата, ничего не делать
#define waitbutdwn_(vk_button)	while(butup(vk_button))Sleep(10)					// ожидание нажатия клавиши vk_button
#define waitbutup_(vk_button)	while(butdwn(vk_button))Sleep(10)						// пока клвавиша нажата, ничего не делать

// эмуляция нажатия клавиши
#define setpressbut(vk_button)	{keybd_event(vk_button, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );keybd_event(vk_button, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );}
#define setpressbut_plus(vk_button,middle) {keybd_event(vk_button, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );middle; keybd_event(vk_button, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );}
#define wpressbut(vk_button,erase)	{waitbutup(vk_button); waitbutdwn(vk_button); waitbutup(vk_button); if(erase)setpressbut (VK_BACK);}

#define pause_					{waitbutup(VK_RETURN); waitbutdwn(VK_RETURN); waitbutup(VK_RETURN);}	
																				// для перехода на следующий этап исполнения требует нажатия клавиши Enter
#define exit_					{waitbutup('0');waitbutup('1');waitbutup(0x0D);ws_("\nExit?");while(butup('0') && butup('1') && butup(0x0D)); if(butdwn('1') || butdwn(0x0D)) exit(0); wc_(10);}
																				// пока не нажата ни одна из клавишь 0, 1, Enter, ничего не делать, после чего,
																				// в случае нажатия 1 или Enter, осуществляется выход из программы

#define wi_(integer)			fprintf(stderr,"%lld ",(__int64)(integer))		// Вывод целого числа
#define wip_(integer)			{wi_(integer); pause_;}							// Вывод целого числа с ожиданием нажатия Enter после этого
#define wiel_(integer)			{wi_(integer); wc_(10);}						// Вывод целого числа и символа конца строки после него
#define wielp_(integer)			{wiel_(integer); pause_}						// Вывод целого числа, символа конца строки с ожиданием нажатия Enter после этого
#define wie_(integer)			{wi_(integer); exit_}							// Вывод целого числа и, если пользователь выберет выход, завершение работы программы

#define wd_(double_)			fprintf(stderr,"%Lg ",(long double)(double_))	// Вывод числа вещественного типа
#define wdp_(double_)			{wd_(double_); pause_}							// Аналогично с wi_
#define wdel_(double_)			{wd_(double_); wc_(10);}
#define wdelp_(double_)			{wdel_(double_); pause_;}
#define wde_(double_)			{wd_(double_); exit_}

#define ws_(string_)			fprintf(stderr,"%s",string_)					// Вывод строки
#define wsp_(string_)			{ws_(string_); pause_ }							// Аналогично с wi_
#define wsel_(string_)			{ws_(string_); wc_(10);}
#define wselp_(string_)			{wsel_(string_); pause_}
#define wse_(string_)			{ws_(string_); exit_}

#define wc_(char_)				putc(char_,stderr)								// Вывод символа
#define wcp_(char_)				{wc_(char_); pause_}							// Аналогично с wi_
#define wcel_(char_)			{wc_(char_); wc_(10);}
#define wcelp_(char_)			{wcel_(char_); pause_}
#define wce_(char_)				{wc_(char_); exit_}

#define Pi						3.1415926535									// константы
#define e_						2.7182818284
#define log_(Base, LogValue)	(log (double (LogValue)) / log (double (Base))) // логарифмы
#define lb_(LogValue)			log_ (2, LogValue)						// двоичный
#define lg_(LogValue)			log_ (10, LogValue)						// десятичный
#define ln_(LogValue)			log_ (e_, LogValue)						// натуральный
inline double rad_ (double degree);			// перевод угла из градусов в радианы
inline double deg_ (double radian);		// перевод угла из радиан в градусы

#define _FOR_(I,INITVALUE,N_)	for (I = (INITVALUE); I < (N_); ++I)		// часто используемые конструкции
#define _FOR(I,N_)				_FOR_ (I, 0, N_)							// с использованием цикла for
#define forstl(p,stl)			for (auto p = (stl).begin (), end = (stl).end (); p != end; ++p)
#define forstl_p(p,stl)			for (auto p : (stl))

#define FOR_(I,INITVALUE,N_)	for (int I = (INITVALUE), variable_N = (N_); I < variable_N; ++I)
#define FOR(I,N_)				FOR_ (I, 0, N_)
#define For(N_)					FOR (count_variable_for_loop_, N_)
#define _n_						<< ' ' <<
#define SWITCH(Var)				auto cpy_for_switch = Var; if (false) {}
#define CASE(Value)				else if (cpy_for_switch == Value)
#define CASE2(Value, Value2)	else if (cpy_for_switch == Value || cpy_for_switch == Value2)
#define DEFAULT					else if (1)

#define Max(a,b)				(((a)<(b))?(b):(a))
#define Min(a,b)				(((a)>(b))?(b):(a))

// Очищение экрана
#define clrscr					{system("cls"); gotoxy(0,0);}

char *Str (char *Str_, char *fmt, ...);

extern HANDLE hCons;

// Класс точка
class Point:public POINT {
public:
	Point (int X = 0, int Y = 0) {x = X; y = Y;}
	Point (COORD c) {x = c.X, y = c.Y;}
};
extern Point scrn;

namespace Where {
	int x();	// текущая координата x курсора в консоли
	int y();	// текущая координата y курсора в консоли
	Point p();	// возвращает точку с координатами, совпадающими с координатами курсора в консоли
}
void gotoxy(int x_pos, int y_pos);	// перевод курсора в консоли в точку с координатами x, y
Point operator + (const Point &p0, const Point &p1);
Point operator - (const Point &p0, const Point &p1);
Point operator * (const Point &p, const int i);
Point operator * (const int i,const Point &p);
bool operator == (const Point p0, const Point p1);
bool operator != (const Point p0, const Point p1);
#define coord_(coordinate, max_coord_value)		((coordinate) * (~(~long(0) << 16)) / (max_coord_value))

void getscreensize (Point &rdp); // заносит в scrn разрешение экрана
void click (Point P, bool LEFT = 1);	// производит щелчок мыши (левая кнопка мыши, если LEFT == true)
inline void click (int x, int y, bool LEFT = 1);
void line (Point A, Point B, bool LEFT = 1);	// зажимает кнопку мыши в точке A, затем перемещает курсор в точу B и отпускает кнопку мыши (левая кнопка мыши, если LEFT == true)
void line (int x1, int y1, int x2, int y2, bool LEFT = 1);
// вывод текста без "разрыва" слов
void print_l(const char *text, int indent = 0, int width = 79);
template <class T>
class er_check{
public:
	T data;
	bool error_;
	er_check () : error_ (0) {}
};
#define error(text)	cout << "\n\nERROR : " + string (text) + "\n"
#define error_(text) {error__(text); retval.error = 1; return retval;}
void error__(char *text);

inline bool isruss(int Char); // проверяет, является ли символ буквой русского алфавита

 // читает все символы из файла File и заносит их в строку string_ (если string_ != NULL), до тех пор, пока функция f не вернет !while_f (на вход ей подаются считываемые символы)
bool readallc (int (*f) (int), bool while_f, FILE *File = stdin, char *string_ = NULL);
string tolower (string s); // Ко всем символам строки применяется функция tolower

string toupper (string s); // Ко всем символам строки применяется функция toupper

void printintstr(int *StringOfInt, FILE *File = stdout, bool torus = 0);	// вывод строки, представленной как массив из переменных типа int

// класс вычетов по модулю n
class mod {
	UINT val;
	UINT n;
public:
	mod (UINT N, int value = 0): n(N), val (value % N) {if (val < 0) val += n;}
	void set_N (UINT N) {n = N; val %= N;}
	mod& operator = (int i) {return *this = mod (n,i);}
	mod& operator ++ () {val = (val+1)%n; return *this;}
	mod operator ++ (int) {mod D(*this); val = (val+1)%n; return D;}
	mod& operator -- () {val = (val+n-1) % n; return *this;}
	mod operator -- (int) {mod D(*this); val = (val+n-1) % n; return D;}
	mod operator + (int a) {return mod (n, val+a);}
	mod operator - (int a) {return mod (n, val-a);}
	mod & operator += (int a) { return *this = *this + a; }
	mod & operator -= (int a) { return *this = *this - a; }
	operator int () { return val; }
};

#define MAX_NUMBER_OF_DIGITS -1
// вывод двоичного представления объекта number с символом конца строки на конце, если endline == true
template <class T>
void binout (T number, bool endline = true,int Number_of_digits = MAX_NUMBER_OF_DIGITS);
inline int setcolor (int color);	// задать цвет выводимых в консоль символов с помощью числа
int setcolor (const char *color);	// задать цвет выводимых в консоль символов с помощью строки
int to_color (const char *color);	
#pragma comment(lib, "winmm.lib")
#include <MMSystem.h>

// таймер
class timer {
	ULONG time, ptime;
	bool stopped;
public:
	timer () {reset ();}
	void reset () {	stopped = 0; time = timeGetTime (); }	// сбросить время
	void stop () { ptime = timeGetTime (); stopped = 1;}	// приостановить таймер
	void restart () { if(stopped) {stopped = 0; time += timeGetTime() - ptime;}	// возобновить таймер
		else error("таймер не был остановлен перед вызовом restart");
	}
	ULONG get_time () {return stopped ? ptime - time : timeGetTime () - time;} // получить время
	void show_time () { wiel_ (get_time ());}		// вывод времени в stderr
};

extern int MENU_Color;
// реализует диалог с пользователем через консоль, в котором выводится строка title и num_of_options вариантов ответа
// (строки, содержащие варианты ответа передаются в функцию, начиная с 3-го параметра). Возвращаемым значением является номер выбранного пункта меню
int menu (string title, int num_of_options, ...);	

//template <class T>
int getval (int *var,char * text, bool clearScreen); // чистит экран, если clearScreen == true, выводит text и считывает в *var значение. Возвращает *var.