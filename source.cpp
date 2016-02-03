#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <time.h> // get current time (used in random function)
#include <fstream>
using namespace std;

void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setTextColor(int textColor, int backColor = 0)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAttribute = backColor << 4 | textColor;
	SetConsoleTextAttribute(consoleHandle, colorAttribute);
}

void clear() // Handmade function :D
{
	gotoxy(0, 0);
	for (int i = 0; i < 30; i++)
		cout << "                                                                                ";
	gotoxy(0, 0);
}
int random(int start, int end) // Handmade function :D
{
	time_t seconds;
	seconds = time(NULL);
	return seconds % (end - start + 1) + start;
}
const string key = "PooACEres";
void encrypt(string &text)
{

	int lh = text.length(), lk = key.length();
	for (int i = 0, j = 0; i < lh; i++, j++)
	{
		if (j>lk)
			j = 0;

		if (text[i] + key[j] > 126)
			text[i] += key[j] - 95;
		else
			text[i] += key[j];
	}

}
void decrypt(string &hash, int start, int end)
{
	int  lk = key.length();
	for (int i = start, j = 0; i < end; i++, j++)
	{
		if (j>lk)
			j = 0;

		if (hash[i] - key[j] < 32)
			hash[i] = hash[i] - key[j] + 95;
		else
			hash[i] -= key[j];
	}
}
string int2string(int numb)
{
	if (numb == 0)
		return "0";

	string out;
	int counter, x;
	for (counter = 0, x = numb; x != 0; x /= 10, counter++);

	out.resize(counter);

	for (int j = counter - 1; j >= 0; j--, numb /= 10)
		out[j] = (numb % 10) + 48;

	return out;

}
int string2int(string str)
{
	int out = 0;
	for (int i = 0; i < str.length(); i++)
	{
		out *= 10;
		out += str[i] - 48;
	}
	return out;
}

bool turn = random(0, 1);
int winner;
bool looser;


struct place
{
	bool fill;
	char v;// v = view
	COORD pos, posl, posr, posu, posd;

	int value;

	void goleft()
	{
		gotoxy(posl.X, posl.Y);
	}
	void goright()
	{
		gotoxy(posr.X, posr.Y);
	}
	void goup()
	{
		gotoxy(posu.X, posu.Y);
	}
	void godown()
	{
		gotoxy(posd.X, posd.Y);
	}

};
place a[3][3];

struct person
{
	string name;
	int tscore;//total score
	int score; // each game score
	int wins;
	int loses;
	int draws;
	char ch;
	int moves;
};
int gamesplaye = 0;
person player[2];

struct node
{
	person load;
	node *next;
};
node *first = NULL;
node *old = NULL;
node *old2 = NULL;

node *search(string name)
{
	node *p;
	p = first;
	while (p != NULL)
	{
		if (p->load.name == name)
			return p;

		p = p->next;
	}
	return p;
}

void sort(node *p)
{
	node *nx, *pr;
	if (first == NULL)
	{
		first = p;
		first->next = NULL;
		return;
	}
	else
	{

		nx = first;
		pr = NULL;
		float avgp = p->load.tscore / (p->load.wins + p->load.loses + p->load.draws), avgn;
		while (nx != NULL)
		{
			avgn = nx->load.tscore / (nx->load.wins + nx->load.loses + nx->load.draws);
			if (avgp > avgn)
			{
				if (pr == NULL)
				{
					p->next = first;
					first = p;
				}
				else
				{
					pr->next = p;
					p->next = nx;
				}
				return;

			}

			pr = nx;
			nx = nx->next;
		}
	}

	pr->next = p;
	p->next = NULL;
}
void rip(node *cur)
{
	if (first == cur)
	{
		first = cur->next;
		sort(cur);
		return;
	}


	node *pr = NULL, *nx = first;

	while (nx != NULL)
	{

		if (cur->load.name == nx->load.name)
		{
			pr->next = cur->next;
			sort(cur);
			return;
		}
		pr = nx;
		nx = nx->next;
	}
}
void creat_node(string item)
{
	node *p = new node;
	int i;

	int l;
	for (l = 0; item[l] != (char)-109; l++);
	p->load.name.resize(l);
	for (i = 0; item[i] != (char)-109; i++)
		p->load.name[i] = item[i];

	p->load.score = 0;
	p->load.tscore = 0;

	for (++i; item[i] != (char)-109; i++)
		p->load.tscore = p->load.tscore * 10 + (item[i] - 48);

	p->load.wins = 0;
	for (++i; item[i] != (char)-109; i++)
		p->load.wins = p->load.wins * 10 + item[i] - 48;

	p->load.loses = 0;
	for (++i; item[i] != (char)-109; i++)
		p->load.loses = p->load.loses * 10 + item[i] - 48;

	p->load.draws = 0;
	for (++i; item[i] != (char)-109; i++)
		p->load.draws = p->load.draws * 10 + item[i] - 48;

	sort(p);
}
void load()
{

	ifstream in;
	in.open("info.txt");

	if (in.is_open())
	{
		string loaded;
		getline(in, loaded);

		string item;
		item.resize(1000);

		int start = 0;
		int loadedln = loaded.length();

		for (int i = 0, j = 0; i < loadedln; i++)
		{
			if (loaded[i] != char(-108))
			{
				if (loaded[i] != char(-109))
				{
					item[j] = loaded[i];
					j++;
				}
				else
				{

					decrypt(item, start, j);
					start = j + 1;
					item[j] = loaded[i];
					j++;
				}
			}
			else
			{

				creat_node(item);
				item = "";
				item.resize(1000);
				j = 0;
				start = 0;
			}
		}
		in.close();
	}


}
void save()
{
	ofstream out;
	out.open("info.txt");
	if (out.is_open())
	{

		int i = 0;

		while (first != NULL)
		{
			i++;

			node *dlt = first;
			encrypt(first->load.name);
			out << first->load.name << (char)147;

			string str0 = int2string(first->load.tscore);
			encrypt(str0);
			out << str0 << (char)147;

			string str = int2string(first->load.wins);
			encrypt(str);
			out << str << (char)147;

			string str1 = int2string(first->load.loses);
			encrypt(str1);
			out << str1 << (char)147;

			string str2 = int2string(first->load.draws);
			encrypt(str2);

			out << str2 << (char)147 << (char)148;

			first = first->next;
			delete dlt;
		}
		out.close();
	}
	return;
}


struct ij
{
	int i, j, value;
};

int whichischoosen2 = 0;
bool isdetailson = 0;
bool easyorhard = 1;
bool singleormulti = 0;

int whichischoosen = 0;

void splash_screen();
void menu();
void paint_menu();
void getinfo();
void map();
void mul_game();
bool game_is_running();
void endmenu();
void paintendmenu();
void singleget();
void singlemap();
void singlegame();
void singlehard();
void printscore();
int findvalue(int, int);
void singleeasy();
int main()
{
	splash_screen();
	load();
	clear();
	menu();
	gotoxy(0, 22);
	return 0;
}
void set_vals()
{
	setTextColor(0, 15);
	clear();

	a[0][0].value = 1; // for AI 
	a[0][1].value = 0;
	a[0][2].value = 1;

	a[1][0].value = 0;
	a[1][1].value = 2;
	a[1][2].value = 0;

	a[2][0].value = 1;
	a[2][1].value = 0;
	a[2][2].value = 1;


	a[0][0].pos.X = 36;
	a[0][0].pos.Y = 9;

	a[0][1].pos.X = 40;
	a[0][1].pos.Y = 9;

	a[0][2].pos.X = 44;
	a[0][2].pos.Y = 9;

	a[1][0].pos.X = 36;
	a[1][0].pos.Y = 13;

	a[1][1].pos.X = 40;
	a[1][1].pos.Y = 13;

	a[1][2].pos.X = 44;
	a[1][2].pos.Y = 13;

	a[2][0].pos.X = 36;
	a[2][0].pos.Y = 17;

	a[2][1].pos.X = 40;
	a[2][1].pos.Y = 17;

	a[2][2].pos.X = 44;
	a[2][2].pos.Y = 17;


	a[0][0].posl = a[0][2].pos;
	a[0][0].posr = a[0][1].pos;
	a[0][0].posu = a[2][0].pos;
	a[0][0].posd = a[1][0].pos;

	a[0][1].posl = a[0][0].pos;
	a[0][1].posr = a[0][2].pos;
	a[0][1].posu = a[2][1].pos;
	a[0][1].posd = a[1][1].pos;

	a[0][2].posl = a[0][1].pos;
	a[0][2].posr = a[0][0].pos;
	a[0][2].posu = a[2][2].pos;
	a[0][2].posd = a[1][2].pos;

	a[1][0].posl = a[1][2].pos;
	a[1][0].posr = a[1][1].pos;
	a[1][0].posu = a[0][0].pos;
	a[1][0].posd = a[2][0].pos;

	a[1][1].posl = a[1][0].pos;
	a[1][1].posr = a[1][2].pos;
	a[1][1].posu = a[0][1].pos;
	a[1][1].posd = a[2][1].pos;

	a[1][2].posl = a[1][1].pos;
	a[1][2].posr = a[1][0].pos;
	a[1][2].posu = a[0][2].pos;
	a[1][2].posd = a[2][2].pos;

	a[2][0].posl = a[2][2].pos;
	a[2][0].posr = a[2][1].pos;
	a[2][0].posu = a[1][0].pos;
	a[2][0].posd = a[0][0].pos;

	a[2][1].posl = a[2][0].pos;
	a[2][1].posr = a[2][2].pos;
	a[2][1].posu = a[1][1].pos;
	a[2][1].posd = a[0][1].pos;

	a[2][2].posl = a[2][1].pos;
	a[2][2].posr = a[2][0].pos;
	a[2][2].posu = a[1][2].pos;
	a[2][2].posd = a[0][2].pos;

	for (int i = 0; i<3; i++)
	for (int j = 0; j<3; j++)
		a[i][j].v = ' ';


	player[0].moves = 0;
	player[1].moves = 0;
}
void splash_screen()
{
	setTextColor(14, 0); // white

	gotoxy(35, 9);
	cout << " " << 'T' << "   " << 'I' << "   " << 'C';

	gotoxy(35, 13);
	cout << " " << 'T' << "   " << 'A' << "   " << 'C';

	gotoxy(35, 17);
	cout << " " << 'T' << "   " << 'O' << "   " << 'E';

	setTextColor(15, 0); // red

	int k = 0;
	int z = 2;
	int t = 100;

	for (int i = 40, j = 8; i<46; i++)
	{
		gotoxy(i, j);
		cout << (char)248;
		t = t - z;
		Sleep(t);
	}

	for (int i = 46, j = 8; j<19; j++)
	{
		gotoxy(i, j);
		cout << (char)248;
		t = t - z;
		Sleep(t);
	}

	for (int i = 46, j = 19; i>34; i--)
	{
		gotoxy(i, j);
		cout << (char)248;
		t = t - z;
		Sleep(t);
	}

	for (int i = 34, j = 19; j>7; j--)
	{
		gotoxy(i, j);
		cout << (char)248;
		t = t - z;
		Sleep(t);
	}

	for (int i = 35, j = 8; i<40; i++)
	{
		gotoxy(i, j);
		cout << (char)248;
		t = t - z;
		Sleep(t);
	}

	for (int i = 40, j = 8; i<46; i++)
	{
		gotoxy(i, j);
		cout << ' ';
		t = t + z;
		Sleep(t);
	}

	for (int i = 46, j = 8; j<19; j++)
	{
		gotoxy(i, j);
		cout << ' ';
		t = t + z;
		Sleep(t);
	}

	for (int i = 46, j = 19; i>34; i--)
	{
		gotoxy(i, j);
		cout << ' ';
		t = t + z;
		Sleep(t);
	}

	for (int i = 34, j = 19; j>7; j--)
	{
		gotoxy(i, j);
		cout << ' ';
		t = t + z;
		Sleep(t);
	}

	for (int i = 35, j = 8; i<40; i++)
	{
		gotoxy(i, j);
		cout << ' ';
		t = t + z;
		Sleep(t);
	}



}
void menu()
{
	set_vals();
	paint_menu();

	while (true)
	{
		switch (_getch())
		{
		case 80://go down
			if (whichischoosen == 3)
				whichischoosen = 0;
			else if (whichischoosen == 4 || whichischoosen == 5)
			{
				clear();
				whichischoosen = 1;
			}

			else
				whichischoosen++;

			paint_menu();
			break;
		case 72: // up
			if (whichischoosen == 0)
				whichischoosen = 3;
			else if (whichischoosen == 4 || whichischoosen == 5)
			{
				whichischoosen = 0;
				clear();
			}

			else
				whichischoosen--;

			paint_menu();
			break;
		case 75: //left
			if (whichischoosen == 4)
				whichischoosen = 5;
			else if (whichischoosen == 5)
				whichischoosen = 4;

			paint_menu();
			break;
		case 77://right
			if (whichischoosen == 4)
				whichischoosen = 5;
			else if (whichischoosen == 5)
				whichischoosen = 4;
			//else // same as 13

			paint_menu();
			break;
		case 13: // ENTER PRESSED
			if (whichischoosen == 0) // single player
			{
				whichischoosen = 4;
				singleormulti = 0;
				paint_menu();
			}
			else if (whichischoosen == 1) // multi-player
			{
				getinfo();
				map();
				singleormulti = 1;
				mul_game();
				exit(0);
			}
			else if (whichischoosen == 2) // scores
			{
				clear();
				printscore();
			}
			else if (whichischoosen == 3) // exit
			{
				save();
				exit(0);
			}

			else if (whichischoosen == 4) // easy
			{
				clear();
				easyorhard = 0;
				singleget();
				singlemap();
				singlegame();
				return;
			}
			else if (whichischoosen == 5) // hard
			{
				clear();
				easyorhard = 1;
				singleget();
				singlemap();
				singlegame();
				return;
			}

			break;
		}

	}


}
void paint_menu()
{
	switch (whichischoosen)
	{
	case 0:
		setTextColor(0, 15);
		gotoxy(35, 1);
		cout << ".: Menu :.";
		gotoxy(35, 2);
		cout << "__________";

		gotoxy(33, 4);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Single Player";
		setTextColor(0, 15); // white back
		gotoxy(33, 5);
		cout << " Multi Player   ";
		gotoxy(33, 6);
		cout << " Scores   ";
		gotoxy(33, 7);
		cout << " Exit   ";
		break;
	case 1:
		setTextColor(0, 15);
		gotoxy(35, 1);
		cout << ".: Menu :.";
		gotoxy(35, 2);
		cout << "__________";

		gotoxy(33, 4);
		cout << " Single Player   ";
		gotoxy(33, 5);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Multi Player";
		setTextColor(0, 15); // white back
		gotoxy(33, 6);
		cout << " Scores   ";
		gotoxy(33, 7);
		cout << " Exit   ";
		break;
	case 2:
		setTextColor(0, 15);
		gotoxy(35, 1);
		cout << ".: Menu :.";
		gotoxy(35, 2);
		cout << "__________";

		gotoxy(33, 4);

		cout << " Single Player   ";
		gotoxy(33, 5);
		cout << " Multi Player   ";
		gotoxy(33, 6);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Scores";
		setTextColor(0, 15); // white back
		gotoxy(33, 7);
		cout << " Exit   ";
		break;
	case 3:
		setTextColor(0, 15);
		gotoxy(35, 1);
		cout << ".: Menu :.";
		gotoxy(35, 2);
		cout << "__________";

		gotoxy(33, 4);
		cout << " Single Player   ";
		gotoxy(33, 5);
		cout << " Multi Player   ";
		gotoxy(33, 6);
		cout << " Scores   ";
		gotoxy(33, 7);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Exit";
		setTextColor(0, 15);
		break;
	case 4:
		setTextColor(0, 15);
		gotoxy(35, 1);
		cout << ".: Menu :.";
		gotoxy(35, 2);
		cout << "__________";

		gotoxy(33, 4);
		cout << " Single Player   ";

		gotoxy(33, 5);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Easy";
		setTextColor(0, 15); // white back
		cout << "    Hard         ";
		gotoxy(33, 6);
		cout << " Multi Player   ";
		gotoxy(33, 7);
		cout << " Scores   ";
		gotoxy(33, 8);
		cout << " Exit   ";

		break;
	case 5:
		setTextColor(0, 15);
		gotoxy(35, 1);
		cout << ".: Menu :.";
		gotoxy(35, 2);
		cout << "__________";

		gotoxy(33, 4);
		cout << " Single Player   ";

		gotoxy(33, 5);
		cout << " Easy    ";
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Hard";
		setTextColor(0, 15); // white back
		cout << "      ";
		gotoxy(33, 6);
		cout << " Multi Player   ";
		gotoxy(33, 7);
		cout << " Scores   ";
		gotoxy(33, 8);
		cout << " Exit   ";
		break;
	}
}
void getinfo()
{
	clear();
	gotoxy(32, 1);
	cout << ".::Information::.";
	gotoxy(32, 2);
	cout << "_________________";
	gotoxy(36, 4);
	cout << "Player 1 ";
	gotoxy(33, 5);
	cout << "Name : ";

	if (player[0].name == "")
		cin.ignore();

	getline(cin, player[0].name);


	if (player[0].name.length() > 34)
	{
		player[0].name[34] = '.';
		player[0].name[33] = '.';
		player[0].name[32] = '.';
		string hash;
		hash.resize(35);
		for (int i = 0; i< 35; i++)
			hash[i] = player[0].name[i];

		player[0].name = hash;

	}

	old = search(player[0].name);
	if (old != NULL)
	{
		player[0] = old->load;
	}


	gotoxy(33, 6);
	cout << "X or O : ";
	do
	{
		player[0].ch = getchar();
		if (player[0].ch != 'X' && player[0].ch != 'x' && player[0].ch != 'O' && player[0].ch != 'o')
		{
			gotoxy(33, 7);
			cout << "                                        ";
			gotoxy(33, 7);
			cout << "Retry : ";
		}


	} while (player[0].ch != 'X' && player[0].ch != 'x' && player[0].ch != 'O' && player[0].ch != 'o');

	if (player[0].ch == 'X' || player[0].ch == 'x')
	{
		player[0].ch = 'X';
		player[1].ch = 'O';
	}
	else
	{
		player[0].ch = 'O';
		player[1].ch = 'X';
	}

	clear();

	gotoxy(32, 1);
	cout << ".::Information::.";
	gotoxy(32, 2);
	cout << "_________________";
	gotoxy(36, 3);
	cout << "Player 2 ";
	gotoxy(33, 4);
	cout << "Name : ";
	cin.ignore();
	getline(cin, player[1].name);

	if (player[1].name.length() > 34)
	{
		player[1].name[34] = '.';
		player[1].name[33] = '.';
		player[1].name[32] = '.';
		string hash;
		hash.resize(35);
		for (int i = 0; i< 35; i++)
			hash[i] = player[1].name[i];

		player[1].name = hash;

	}

	old2 = search(player[1].name);
	if (old2 != NULL)
	{
		player[1] = old2->load;
	}

	clear();
}
void map()
{

	if (!turn)
	{

		gotoxy(32, 1);
		cout << ".::Information::.";
		gotoxy(32, 2);
		cout << "_________________";



		gotoxy(40 - ((player[0].name.length() + player[1].name.length() + 12) / 2), 3);

		setTextColor(12, 15);//red
		cout << player[0].name << " (" << player[0].ch << ") ";
		setTextColor(0, 15);//black
		cout << "Vs ";
		cout << player[1].name << " (" << player[1].ch << ')' << endl;
	}
	else
	{
		gotoxy(32, 1);
		cout << ".::Information::.";
		gotoxy(32, 2);
		cout << "_________________";



		gotoxy(40 - ((player[0].name.length() + player[1].name.length() + 12) / 2), 3);


		cout << player[0].name << " (" << player[0].ch << ") ";
		cout << "Vs ";
		setTextColor(12, 15);//red
		cout << player[1].name << " (" << player[1].ch << ')' << endl;
		setTextColor(0, 15);//black
	}


	gotoxy(35, 8);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";
	gotoxy(35, 9);

	setTextColor(12, 15); // red
	cout << " " << a[0][0].v << " ";
	setTextColor(0, 15); // black

	cout << (char)186 << " ";
	setTextColor(12, 15); // red
	cout << a[0][1].v;
	setTextColor(0, 15); // black

	cout << " " << (char)186 << " ";
	setTextColor(12, 15); // red
	cout << a[0][2].v << " ";
	setTextColor(0, 15); // black

	gotoxy(35, 10);

	cout << "   " << (char)186 << "   " << (char)186 << "   ";
	gotoxy(35, 11);

	cout << (char)205 << (char)205 << (char)205 << (char)206 << (char)205 << (char)205 << (char)205
		<< (char)206 << (char)205 << (char)205 << (char)205;

	gotoxy(35, 12);

	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 13);

	setTextColor(12, 15); // red
	cout << " " << a[1][0].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[1][1].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[1][2].v << " ";
	setTextColor(0, 15); // black

	gotoxy(35, 14);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 15);
	cout << (char)205 << (char)205 << (char)205 << (char)206 << (char)205 << (char)205 << (char)205
		<< (char)206 << (char)205 << (char)205 << (char)205;

	gotoxy(35, 16);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 17);
	cout << " ";

	setTextColor(12, 15); // red
	cout << a[2][0].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[2][1].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[2][2].v << " ";
	setTextColor(0, 15); // black
	gotoxy(35, 18);
	cout << "   " << (char)186 << "   " << (char)186 << "   " << endl << endl;

}
void mul_game()
{
	gotoxy(40, 13);
	int p[2] = { 1, 1 };//pointer

	int step = 0;
	while (game_is_running() && step < 9)
	{
		switch (_getch())
		{

		case  75:
			a[p[0]][p[1]].goleft();
			p[0] = (a[p[0]][p[1]].posl.Y - 8) / 4;
			p[1] = (a[p[0]][p[1]].posl.X - 33) / 4;
			break;

		case 77:
			a[p[0]][p[1]].goright();
			p[0] = (a[p[0]][p[1]].posr.Y - 8) / 4;
			p[1] = (a[p[0]][p[1]].posr.X - 33) / 4;
			break;

		case 72:
			a[p[0]][p[1]].goup();
			p[0] = (a[p[0]][p[1]].posu.Y - 8) / 4;
			p[1] = (a[p[0]][p[1]].posu.X - 33) / 4;
			break;

		case 80:

			a[p[0]][p[1]].godown();
			p[0] = (a[p[0]][p[1]].posd.Y - 8) / 4;
			p[1] = (a[p[0]][p[1]].posd.X - 33) / 4;
			break;
		case 13:
			if (a[p[0]][p[1]].v == ' ')
			{
				a[p[0]][p[1]].v = player[turn].ch;


				p[0] = 1;
				p[1] = 1;

				if (turn)
				{
					player[turn].moves++;
					turn = 0;
				}
				else
				{
					player[turn].moves++;
					turn = 1;
				}


				map();
				gotoxy(40, 13);
				step++;
			}

			break;
		}
	}
	if (step == 9 && game_is_running())
	{
		winner = 2;
		player[0].score = 100;
		player[0].tscore += 100;
		player[0].draws += 1;

		player[1].score = 100;
		player[1].tscore += 100;
		player[1].draws += 1;
	}
	else if (player[winner].moves == 3)
	{
		player[winner].wins += 1;
		player[winner].score = 700;
		player[winner].tscore += 700;


		player[looser].loses += 1;
		player[looser].score = 0;
		player[looser].tscore += 0;
	}
	else if (player[looser].moves > 3)
	{
		player[winner].wins += 1;
		player[winner].score = 500;
		player[winner].tscore += 500;


		player[looser].loses += 1;
		player[looser].score = 0;
		player[looser].tscore += 0;
	}
	clear();
	endmenu();
}
bool game_is_running()
{
	char dooz[3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			dooz[j] = a[i][j].v;


		if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[0].ch)
		{
			winner = 0;
			looser = 1;
			return 0; // end mul-game
		}
		else if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[1].ch)
		{
			winner = 1;
			looser = 0;
			return 0; // end mul-game
		}

	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			dooz[j] = a[j][i].v;

		if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[0].ch)
		{
			winner = 0;
			looser = 1;
			return 0; // end mul-game
		}
		else if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[1].ch)
		{
			winner = 1;
			looser = 0;
			return 0; // end mul-game
		}
	}

	for (int i = 0; i < 3; i++)
		dooz[i] = a[i][i].v;

	if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[0].ch)
	{
		winner = 0;
		looser = 1;
		return 0; // end mul-game
	}
	else if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[1].ch)
	{
		winner = 1;
		looser = 0;
		return 0; // end mul-game
	}

	for (int i = 0, j = 2; i < 3; i++, j--)
		dooz[j] = a[i][j].v;

	if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[0].ch)
	{
		winner = 0;
		looser = 1;
		return 0; // end mul-game
	}
	else if (dooz[0] == dooz[1] && dooz[1] == dooz[2] && dooz[2] == player[1].ch)
	{
		winner = 1;
		looser = 0;
		return 0; // end mul-game
	}

	return 1;
}
void endmenu()
{

	paintendmenu();
	while (1)
	{
		switch (_getch())
		{
		case 80://down
			if (whichischoosen2 == 2)
				whichischoosen2 = 0;
			else
				whichischoosen2++;
			paintendmenu();
			break;
		case 72: //up
			if (whichischoosen2 == 0)
				whichischoosen2 = 2;
			else
				whichischoosen2--;
			paintendmenu();
			break;
		case 13: // enter
			if (whichischoosen2 == 0) // details
			{
				if (isdetailson)
				{
					isdetailson = 0;
					clear();
				}

				else
					isdetailson = 1;
				paintendmenu();
			}
			else if (whichischoosen2 == 1) // continue
			{
				isdetailson = 0;

				set_vals();
				singlemap();
				if (singleormulti)
					mul_game();
				else
					singlegame();
			}
			else if (whichischoosen2 == 2) // main menu
			{
				isdetailson = 0;
				clear();

				if (old == NULL)
				{
					node *test = new node;
					test->load = player[0];
					sort(test);
				}
				else
				{
					old->load = player[0];
					rip(old);
				}


				if (singleormulti)
				{

					if (old2 == NULL)
					{
						node *test2 = new node;
						test2->load = player[1];
						sort(test2);
					}
					else
					{
						old2->load = player[1];
						rip(old2);
					}

				}

				player[0].wins = 0;
				player[0].draws = 0;
				player[0].loses = 0;
				player[0].tscore = 0;

				player[1].wins = 0;
				player[1].draws = 0;
				player[1].loses = 0;
				player[1].tscore = 0;
				menu();
			}
			break;
		}
	}

}
void paintendmenu()
{
	if (winner == 2)
	{

		gotoxy(34, 1);
		cout << ".:: Draw ::.";
		gotoxy(34, 2);
		cout << "____________";
	}
	else
	{
		int lown = player[winner].name.length(); // lown = lenghth of winner name
		gotoxy(34 - lown / 2, 1);
		cout << ".:: " << player[winner].name << " Won ::.";
		gotoxy(34 - lown / 2, 2);
		for (int i = 0; i < lown + 12; i++)
			cout << '_';
	}



	gotoxy(35, 8);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";
	gotoxy(35, 9);

	setTextColor(12, 15); // red
	cout << " " << a[0][0].v << " ";
	setTextColor(0, 15); // black

	cout << (char)186 << " ";
	setTextColor(12, 15); // red
	cout << a[0][1].v;
	setTextColor(0, 15); // black

	cout << " " << (char)186 << " ";
	setTextColor(12, 15); // red
	cout << a[0][2].v << " ";
	setTextColor(0, 15); // black

	gotoxy(35, 10);

	cout << "   " << (char)186 << "   " << (char)186 << "   ";
	gotoxy(35, 11);

	cout << (char)205 << (char)205 << (char)205 << (char)206 << (char)205 << (char)205 << (char)205
		<< (char)206 << (char)205 << (char)205 << (char)205;

	gotoxy(35, 12);

	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 13);

	setTextColor(12, 15); // red
	cout << " " << a[1][0].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[1][1].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[1][2].v << " ";
	setTextColor(0, 15); // black

	gotoxy(35, 14);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 15);
	cout << (char)205 << (char)205 << (char)205 << (char)206 << (char)205 << (char)205 << (char)205
		<< (char)206 << (char)205 << (char)205 << (char)205;

	gotoxy(35, 16);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 17);
	cout << " ";

	setTextColor(12, 15); // red
	cout << a[2][0].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[2][1].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[2][2].v << " ";
	setTextColor(0, 15); // black
	gotoxy(35, 18);
	cout << "   " << (char)186 << "   " << (char)186 << "   " << endl << endl;

	//details
	int lfn = player[0].name.length(), lsn = player[1].name.length(); // first and second players name
	int hash = player[0].score;
	int nd = 0; // number of players scores digits

	// left side
	gotoxy(18 - lfn / 2, 7);
	setTextColor(12, 15); // red
	cout << player[0].name;
	setTextColor(0, 15); // black


	if (lfn < 5)
		lfn = 5;
	gotoxy(18 - lfn / 2, 8);
	for (int i = 0; i < lfn; i++)
		cout << '_';

	while (hash != 0)
	{
		hash /= 10;
		nd++;
	}
	gotoxy(17 - (nd + 4) / 2, 10);
	cout << player[0].score << " pts";


	// right side
	gotoxy(63 - lsn / 2, 7);

	setTextColor(12, 15); // red
	cout << player[1].name;
	setTextColor(0, 15); // black

	if (lsn < 5)
		lsn = 5;
	gotoxy(63 - lsn / 2, 8);
	for (int i = 0; i < lsn; i++)
		cout << '_';

	nd = 0;
	hash = player[1].score;
	while (hash != 0)
	{
		hash /= 10;
		nd++;
	}
	gotoxy(63 - (nd + 4) / 2, 10);
	cout << player[1].score << " pts";

	if (isdetailson)
	{
		const int stime = 200; // sleep time

		//wins left
		nd = 0;
		hash = player[0].wins;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(18 - (nd + 6) / 2, 12);
		cout << "Wins: " << player[0].wins;
		//wins right
		nd = 0;
		hash = player[1].wins;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(63 - (nd + 6) / 2, 12);
		cout << "Wins: " << player[1].wins;
		// draws left
		Sleep(stime);
		nd = 0;
		hash = player[0].draws;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(18 - (nd + 7) / 2, 14);
		cout << "Draws: " << player[0].draws;

		// draws right
		nd = 0;
		hash = player[1].draws;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(63 - (nd + 7) / 2, 14);
		cout << "Draws: " << player[1].draws;
		// loses left
		Sleep(stime);
		nd = 0;
		hash = player[0].loses;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(18 - (nd + 7) / 2, 16);
		cout << "Loses: " << player[0].loses;

		// loses right
		nd = 0;
		hash = player[1].loses;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(63 - (nd + 7) / 2, 16);
		cout << "Loses: " << player[1].loses;
		// overall score left
		Sleep(stime);
		nd = 0;
		hash = player[0].tscore;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(18 - (nd + 19) / 2, 18);
		cout << "Overall Score: " << player[0].tscore << " pts";

		// overall score right
		nd = 0;
		hash = player[1].tscore;
		while (hash != 0)
		{
			hash /= 10;
			nd++;
		}

		gotoxy(63 - (nd + 19) / 2, 18);
		cout << "Overall Score: " << player[1].tscore << " pts";
	}

	switch (whichischoosen2)
	{
	case 0: // details
		gotoxy(35, 4);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Details";
		setTextColor(0, 15); // white back
		gotoxy(32, 5);
		cout << "    Continue      ";
		gotoxy(32, 6);
		cout << "    Main Menu      ";
		break;
	case 1: // continue
		gotoxy(33, 4);
		cout << "    Details        ";
		gotoxy(34, 5);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Continue";
		setTextColor(0, 15); // white back
		gotoxy(32, 6);
		cout << "    Main Menu      ";
		break;
	case 2: // main menu
		gotoxy(33, 4);
		cout << "    Details        ";
		gotoxy(32, 5);
		cout << "    Continue         ";
		gotoxy(34, 6);
		cout << (char)175 << ' ';
		setTextColor(0, 12); // red back
		cout << "Main Menu";
		setTextColor(0, 15); // white back
		break;
	}

	gotoxy(0, 0);

}
void singleget()
{

	clear();
	gotoxy(32, 1);
	cout << ".::Information::.";
	gotoxy(32, 2);
	cout << "_________________";
	gotoxy(36, 4);
	cout << "Player 1 ";
	gotoxy(33, 5);
	cout << "Name : ";

	if (player[0].name != "")
		cin.ignore();

	getline(cin, player[0].name);

	if (player[0].name.length() > 34)
	{
		player[0].name[34] = '.';
		player[0].name[33] = '.';
		player[0].name[32] = '.';
		string hash;
		hash.resize(35);
		for (int i = 0; i < 35; i++)
			hash[i] = player[0].name[i];

		player[0].name = hash;

	}


	old = search(player[0].name);
	if (old != NULL)
	{
		player[0] = old->load;
	}

	gotoxy(33, 6);
	cout << "X or O : ";
	do
	{
		player[0].ch = getchar();
		if (player[0].ch != 'X' && player[0].ch != 'x' && player[0].ch != 'O' && player[0].ch != 'o')
		{
			gotoxy(33, 7);
			cout << "                                        ";
			gotoxy(33, 7);
			cout << "Retry : ";
		}


	} while (player[0].ch != 'X' && player[0].ch != 'x' && player[0].ch != 'O' && player[0].ch != 'o');

	if (player[0].ch == 'X' || player[0].ch == 'x')
	{
		player[0].ch = 'X';
		player[1].ch = 'O';
	}
	else
	{
		player[0].ch = 'O';
		player[1].ch = 'X';
	}

	clear();

	if (easyorhard)
		player[1].name = "AI (HARD)";
	else
		player[1].name = "AI (EASY)";
}
void singlemap()
{
	gotoxy(32, 1);
	cout << ".::Information::.";
	gotoxy(32, 2);
	cout << "_________________";
	gotoxy(40 - ((player[0].name.length() + player[1].name.length() + 12) / 2), 3);
	setTextColor(12, 15);//red
	cout << player[0].name << " (" << player[0].ch << ") ";
	setTextColor(0, 15);//black
	cout << "Vs ";
	cout << player[1].name << " (" << player[1].ch << ')' << endl;


	gotoxy(35, 8);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";
	gotoxy(35, 9);

	setTextColor(12, 15); // red
	cout << " " << a[0][0].v << " ";
	setTextColor(0, 15); // black

	cout << (char)186 << " ";
	setTextColor(12, 15); // red
	cout << a[0][1].v;
	setTextColor(0, 15); // black

	cout << " " << (char)186 << " ";
	setTextColor(12, 15); // red
	cout << a[0][2].v << " ";
	setTextColor(0, 15); // black

	gotoxy(35, 10);

	cout << "   " << (char)186 << "   " << (char)186 << "   ";
	gotoxy(35, 11);

	cout << (char)205 << (char)205 << (char)205 << (char)206 << (char)205 << (char)205 << (char)205
		<< (char)206 << (char)205 << (char)205 << (char)205;

	gotoxy(35, 12);

	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 13);

	setTextColor(12, 15); // red
	cout << " " << a[1][0].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[1][1].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[1][2].v << " ";
	setTextColor(0, 15); // black

	gotoxy(35, 14);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 15);
	cout << (char)205 << (char)205 << (char)205 << (char)206 << (char)205 << (char)205 << (char)205
		<< (char)206 << (char)205 << (char)205 << (char)205;

	gotoxy(35, 16);
	cout << "   " << (char)186 << "   " << (char)186 << "   ";

	gotoxy(35, 17);
	cout << " ";

	setTextColor(12, 15); // red
	cout << a[2][0].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[2][1].v;
	setTextColor(0, 15); // black
	cout << " " << (char)186 << " ";

	setTextColor(12, 15); // red
	cout << a[2][2].v << " ";
	setTextColor(0, 15); // black
	gotoxy(35, 18);
	cout << "   " << (char)186 << "   " << (char)186 << "   " << endl << endl;

}
void singlegame()
{
	gotoxy(40, 13);
	int p[2] = { 1, 1 };//pointer

	int step = 0;
	while (game_is_running() && step < 9)
	{
		if (turn)
		{
			if (easyorhard)
				singlehard();
			else
				singleeasy();

			player[1].moves++;
			turn = 0;
			singlemap();

			gotoxy(40, 13);
			p[0] = 1;
			p[1] = 1;

			step++;
		}
		else
		{
			switch (_getch())

			{

			case  75:
				a[p[0]][p[1]].goleft();
				p[0] = (a[p[0]][p[1]].posl.Y - 8) / 4;
				p[1] = (a[p[0]][p[1]].posl.X - 33) / 4;
				break;

			case 77:
				a[p[0]][p[1]].goright();
				p[0] = (a[p[0]][p[1]].posr.Y - 8) / 4;
				p[1] = (a[p[0]][p[1]].posr.X - 33) / 4;
				break;

			case 72:
				a[p[0]][p[1]].goup();
				p[0] = (a[p[0]][p[1]].posu.Y - 8) / 4;
				p[1] = (a[p[0]][p[1]].posu.X - 33) / 4;
				break;

			case 80:

				a[p[0]][p[1]].godown();
				p[0] = (a[p[0]][p[1]].posd.Y - 8) / 4;
				p[1] = (a[p[0]][p[1]].posd.X - 33) / 4;
				break;
			case 13:
				if (a[p[0]][p[1]].v == ' ')
				{
					a[p[0]][p[1]].v = player[turn].ch;


					p[0] = 1;
					p[1] = 1;

					player[0].moves++;
					turn = 1;



					singlemap();
					gotoxy(40, 13);
					step++;
				}

				break;
			}
		}

	}

	if (step == 9 && game_is_running())
	{
		winner = 2;
		player[0].score = 100;
		player[0].tscore += 100;
		player[0].draws += 1;

		player[1].score = 100;
		player[1].tscore += 100;
		player[1].draws += 1;
	}
	else if (player[winner].moves == 3)
	{
		player[winner].wins += 1;
		player[winner].score = 700;
		player[winner].tscore += 700;


		player[looser].loses += 1;
		player[looser].score = 0;
		player[looser].tscore += 0;
	}
	else if (player[winner].moves >= 4)
	{
		player[winner].wins += 1;
		player[winner].score = 500;
		player[winner].tscore += 500;


		player[looser].loses += 1;
		player[looser].score = 0;
		player[looser].tscore += 0;
	}
	clear();
	endmenu();
}
void singlehard()
{
	ij max;
	max.i = 0;
	max.j = 0;
	max.value = -1;

	int found;
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	if (a[i][j].v == ' ')
	{
		found = findvalue(i, j);
		if (max.value < found + a[i][j].value)
		{
			max.value = found + a[i][j].value;
			max.i = i;
			max.j = j;
		}

	}

	a[max.i][max.j].v = player[1].ch;
}
int findvalue(int i, int j)
{
	int value = 0;
	char dooz[3];

	int counter0 = 0, counter1 = 0;
	if (i + j == 2 || i == j && j != 1)
	{
		for (int k = 0; k < 3; k++)
		{
			dooz[k] = a[i][k].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;
		}

		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;


		counter0 = 0;
		counter1 = 0;

		for (int k = 0; k < 3; k++)
		{
			dooz[k] = a[k][j].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}


		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;


		if (i == j)
		{
			counter0 = 0;
			counter1 = 0;

			for (int k = 0; k < 3; k++)
			{
				dooz[k] = a[k][k].v;

				if (dooz[k] == player[0].ch)
					counter0++;
				else if (dooz[k] == player[1].ch)
					counter1++;

			}


			if (counter0 == 0 && counter1 == 1)
				value += 3;
			else if (counter0 == 2)
				value += 5;
			else if (counter1 == 2)
				value += 7;

		}
		else
		{
			counter0 = 0;
			counter1 = 0;

			for (int k = 0, m = 2; k < 3; m--, k++)
			{
				dooz[k] = a[m][k].v;

				if (dooz[k] == player[0].ch)
					counter0++;
				else if (dooz[k] == player[1].ch)
					counter1++;

			}

			if (counter0 == 0 && counter1 == 1)
				value += 3;
			else if (counter0 == 2)
				value += 5;
			else if (counter1 == 2)
				value += 7;

		}

	}
	else if (i == 1 && j == 1)
	{
		counter0 = 0;
		counter1 = 0;

		for (int k = 0; k < 3; k++)
		{
			dooz[k] = a[i][k].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}

		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;


		counter0 = 0;
		counter1 = 0;

		for (int k = 0; k < 3; k++)
		{
			dooz[k] = a[k][j].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}

		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;


		counter0 = 0;
		counter1 = 0;

		for (int k = 0; k < 3; k++)
		{
			dooz[k] = a[k][k].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}


		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;


		counter0 = 0;
		counter1 = 0;

		for (int k = 0, m = 2; k < 3; m--, k++)
		{
			dooz[k] = a[m][k].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}

		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;

	}
	else
	{
		counter0 = 0;
		counter1 = 0;

		for (int k = 0; k < 3; k++)
		{
			dooz[k] = a[i][k].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}

		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;

		counter0 = 0;
		counter1 = 0;

		for (int k = 0; k < 3; k++)
		{

			dooz[k] = a[k][j].v;

			if (dooz[k] == player[0].ch)
				counter0++;
			else if (dooz[k] == player[1].ch)
				counter1++;

		}

		if (counter0 == 0 && counter1 == 1)
			value += 3;
		else if (counter0 == 2)
			value += 5;
		else if (counter1 == 2)
			value += 7;
		// exception
		if ((a[0][0].v == player[0].ch && a[2][2].v == player[0].ch && a[1][1].v == player[1].ch) || (a[0][2].v == player[0].ch && a[2][0].v == player[0].ch && a[1][1].v == player[1].ch))
			value += 2;
	}


	return value;
}
void singleeasy()
{
	ij max;
	max.i = 0;
	max.j = 0;
	max.value = -1;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (max.value < a[i][j].value && a[i][j].v == ' ')
			{
				max.value = a[i][j].value;
				max.i = i;
				max.j = j;
			}
		}
	}


	a[max.i][max.j].v = player[1].ch;
}
void printscore()
{
	save();
	load();
	gotoxy(32, 1);
	cout << ".::Top Scores::.";
	gotoxy(31, 2);
	for (int i = 0; i < 18; i++)
		cout << (char)196;
	gotoxy(34, 3);
	cout << "Score / Games";

	node *worm = first;
	for (int i = 0; i < 10 && worm != NULL; i++)
	{
		if (i < 3)
		{
			int iavg = worm->load.tscore / (worm->load.wins + worm->load.draws + worm->load.loses);
			gotoxy(40 - (6 + worm->load.name.length() + int2string(iavg).length()) / 2, i + 5);
			cout << i + 1;
			setTextColor(12, 15);// red
			cout << ' ' << worm->load.name << "    " << iavg;
			setTextColor(0, 15); // white
		}
		else
		{
			int iavg = worm->load.tscore / (worm->load.wins + worm->load.draws + worm->load.loses);
			gotoxy(40 - (6 + worm->load.name.length() + int2string(iavg).length()) / 2, i + 5);
			cout << i + 1 << ' ' << worm->load.name << "    " << iavg;
		}
		worm = worm->next;
	}

	gotoxy(32, 20);
	cout << "Press Any Key...";
	_getch();
	menu();
}
// To do 
// look 2222 line of codes :D WTF
