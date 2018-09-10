#include <iostream>
#include "time.h"
#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
using namespace std;
ifstream file0("choose/trp_0.txt");
ifstream file1("choose/trp_1.txt");
ifstream file2("choose/trp_2.txt");
int i = 7;
void gotoxy(int x, int y)
{
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
class Pet
{

public:
	bool Alive = true;
	bool new_pet = false;
	int Heals_Point;
	int Energy_Point;
	int Satiety_Point;
	int Happiness_Point;
	time_t lastTime = time(NULL);
	char number_of_pet;
	string str;
	string name;
	void set_settings()
	{
		ifstream save_file("save_spot.txt");
		if (!save_file.is_open())
		{
			save_file.close();
			ofstream save_file("save_spot.txt");
			Heals_Point = 100;
			Energy_Point = 100;
			Satiety_Point = 100;
			Happiness_Point = 100;
			save_file << Heals_Point << ' ' << Energy_Point << ' ' << Satiety_Point << ' ' << Happiness_Point << ' ' << number_of_pet << ' ' << name << ' '<< new_pet<<' ' << Alive;
			save_file.close();

		}
		if (save_file.is_open() && new_pet == true)
		{
			ofstream save_file("save_spot.txt");
			Heals_Point = 100;
			Energy_Point = 100;
			Satiety_Point = 100;
			Happiness_Point = 100;
			save_file << Heals_Point << ' ' << Energy_Point << ' ' << Satiety_Point << ' ' << Happiness_Point << ' ' << number_of_pet << ' ' << name << ' ' << new_pet << ' ' << Alive;
			save_file.close();

		}
		if (save_file.is_open() && new_pet == false)
		{
			save_file >> Heals_Point >> Energy_Point >> Satiety_Point >> Happiness_Point >> number_of_pet >> name>>new_pet >> Alive;
			save_file.close();
		}

	}
	void write_status()
	{
		gotoxy(15, 0);
		cout << "Heals:     \t" << Heals_Point;
		gotoxy(15, 1);
		cout << "Energy:    \t" << Energy_Point;
		gotoxy(15, 2);
		cout << "Satiety:    \t" << Satiety_Point;
		gotoxy(15, 3);
		cout << "Happiness:  \t" << Happiness_Point;
		gotoxy(0, 4);
	}
	void update_status(int value)
	{
		Satiety_Point -= value;
		if (Satiety_Point <= 0)
			Satiety_Point = 0;

		Energy_Point -= value;
		if (Energy_Point <= 0)
			Energy_Point = 0;

		if (Heals_Point <= 0)
			Alive = false;

		if (Satiety_Point&&Energy_Point <= 80 && Satiety_Point&&Energy_Point >= 0)
		{
			Happiness_Point -= value/2;
			if (Happiness_Point < 0)
				Happiness_Point = 0;
		}
		if (Satiety_Point == 0 && Energy_Point == 0)
		{
			Happiness_Point -= value;
			if (Happiness_Point < 0)
				Happiness_Point = 0;
		}
		if (Satiety_Point&&Energy_Point > 80 && Satiety_Point&&Energy_Point <= 100)
		{
			Happiness_Point += value;
			if (Happiness_Point > 100)
				Happiness_Point = 100;
		}
		if (Happiness_Point < 80)
		{
			Heals_Point -= value;
			if (Heals_Point < 0)
				Heals_Point = 0;
		}
		else
		{
			Heals_Point += 2;
			if (Heals_Point > 100)
				Heals_Point = 100;
		}

	}
	void Draw_All_Pets()
	{
		gotoxy(0, 6);
		cout << "¹1 Dog";
		while (!file0.eof())
		{
			gotoxy(0, i);
			file0 >> str;
			cout << str << endl;
			i++;
		}
		i = 7;
		gotoxy(15, 6);
		cout << "¹2 Rabbit";
		while (!file1.eof())
		{
			gotoxy(15, i);
			file1 >> str;
			cout << str << endl;
			i++;
		}
		i = 7;
		gotoxy(30, 6);
		cout << "¹3 Duck";
		while (!file2.eof())
		{
			gotoxy(30, i);
			file2 >> str;
			cout << str << endl;
			i++;
		}
		gotoxy(0, 11 );
		file0.close();
		file1.close();
		file2.close();
	}
	void Sleep()
	{
		Energy_Point += 20;
		Satiety_Point -= 20;
		if (Satiety_Point < 0)
			Satiety_Point = 0;
		if (Energy_Point > 100)
			Energy_Point = 100;
	}
	void Feed()
	{
		Energy_Point -= 5;
		Satiety_Point += 30;
		if (Energy_Point < 0)
			Energy_Point = 0;
		if (Satiety_Point > 100)
			Satiety_Point = 100;
	}
	void Play()
	{
		Energy_Point -= 10;
		if (Energy_Point < 0)
			Energy_Point = 0;
		Satiety_Point -= 10;
		if (Satiety_Point < 0)
			Satiety_Point = 0;
		Happiness_Point += 30;
		if (Happiness_Point > 100)
			Happiness_Point = 100;
	}
	void Choose_Pet()
	{
		ifstream Dog("choose/trp_0.txt");
		ifstream Rabbit("choose/trp_1.txt");
		ifstream Duck("choose/trp_2.txt");
		i = 10;
		if (number_of_pet == '1')
		{
			while (!Dog.eof())
			{
				Dog >> str;
				cout << str << endl;
			}
		}
		if (number_of_pet == '2')
		{
			while (!Rabbit.eof())
			{
				Rabbit >> str;
				cout << str << endl;
			}
		}
		if (number_of_pet == '3')
		{
			while (!Duck.eof())
			{
				Duck >> str;
				cout << str << endl;
			}
		}
	}
};
int main()
{
	int str = 5;
	setlocale(0, "");
	Pet player;
	cout << "\t\t Hello! You started playing Tamogotchis!\nAt this stage, you must choose your pet:\n";
	player.Draw_All_Pets();
	ifstream save_check("save_spot.txt");
	if (save_check.is_open())
	{
		save_check.close();
		int choose;
		cout << "You already have a pet.\nDo you want to continue playing with him?\n(1)Yew \t(2)No \n";
		cin >> choose;
		if (choose == 2)
		{
			player.new_pet = true;
			cout << "\n\nEnter the pet number you want to take. ";
			cin >> player.number_of_pet;
			cout << "\n\nEnter the name of your pet.\n";
			cin >> player.name;
			cout << "Now your pet's name is " << player.name << endl;
		}
	}
	else
	{
		cout << "\n\nEnter the pet number you want to take. ";
		cin >> player.number_of_pet;
		cout << "\n\nEnter the name of your pet.\n";
		cin >> player.name;
		cout << "Now your pet's name is " << player.name << endl;
	}
	system("pause");
	system("cls");
	player.set_settings();
	while (player.Alive)
	{
		ofstream save_spot1("save_spot.txt");
		if (save_spot1.is_open())
		{
			save_spot1 << player.Heals_Point << endl << player.Energy_Point << endl << player.Satiety_Point << endl << player.Happiness_Point << endl << player.number_of_pet << endl << player.name << endl << player.new_pet << endl << player.Alive;
			save_spot1.close();
		}
		if (str == 0)
		{
			player.update_status(5);
			str = 5;
		}
		player.Choose_Pet();
		player.write_status();
		str--;
		cout << "(1)Feed\n(2)Sleep\n(3)Play\n(Any buttom)Wait\n";
		char a = _getch();
		switch (a) {
		case '1':
			player.Feed();
			break;
		case '2':
			player.Sleep();
			break;
		case '3':
			player.Play();
			break;
		default:
			break;
		}
		system("cls");
	}
	cout << "You can not give animals to custody.\nYour pet passed away.\n";
	system("pause");
	return 0;
}