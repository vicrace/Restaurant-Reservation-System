/*Contributed by Vicrace Chan Jia Lin, Leong Yue Chien, Ng Sze Jin*/
/*This program a designated restaurant reservation system which helps the restaurant to ease their daily opeation such as search for customer record, record reservation and
allocate customer details by using computerized program*/
/*Search availability and update reservation only allow to carry out within the current month and for first three days of the restaurant will be officialy close.*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cctype>
using namespace std;

#define MAX_RECORD 224 //determine the max size for reservation record stcuture array 
#define MAX_GUEST 8 
#define CURRENT_TIME tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec //display current time
#define LIST "September.txt" // August.txt for August reservation list, September.txt for September list

typedef struct
{
	int date;
	int month;
	int year;

}reservation_date; // structure to store reservation date

typedef struct
{
	char name[30];
	char phone[11]; //customer phone number
	reservation_date res_date;
	int res_session; // customer selected session
	int num_guest; // customer selected guest number

}res_record; // structure to store customer information

char display_menu(); // a main menu that is displayed with main function details to prompt user to proceed. 
void display_rec(res_record rec[], int size, int book[][5], int book_size); // function to display customer record in a particular date or all customer in a list for the particular month
void search_availability(res_record rec[], int size, int book[][5], int book_size); // function for user to search for availability by using either date, session or number of guest.
void update_reservation(res_record rec[], int size, int book[][5], int book_size); // function for user to add on more reservation record within the month.
bool verify_date(char date[], char month[], int mon_size, int date_size); // function to verify the condition for '3 days booking in advance' between reservation date and current date.  

int main()
{
	res_record rec[MAX_RECORD] = { 0 };
	int book[33][5] = { 0 };/*an array to store the count for guest number in a particular date and session*/
	int index = 0;  // number to accumulate total reservation that has entered into the list
	int choice = 0;

	ifstream in_file(LIST);

	if (!in_file)
		cout << "(Error input file open)\n";
	else
	{
		in_file.getline(rec[++index].name, 30);
		while (in_file)
		{
			in_file.getline(rec[index].phone, 11);
			in_file >> rec[index].res_date.date;
			in_file >> rec[index].res_date.month;
			in_file >> rec[index].res_date.year;
			in_file >> rec[index].num_guest;
			in_file >> rec[index].res_session;

			if (in_file.peek() == '\n')
				in_file.ignore(256, '\n');

			book[rec[index].res_date.date][rec[index].res_session] = book[rec[index].res_date.date][rec[index].res_session] + rec[index].num_guest; // update booking count

			in_file.getline(rec[++index].name, 30);
		}

		in_file.close();

		while (choice != '4')
		{

			choice = display_menu();
			switch (choice)
			{
			case '1':
			{
				system("cls");
				display_rec(rec, index, book, 33);
				break;
			}
			case '2':
			{
				system("cls");
				search_availability(rec, index, book, 33);
				break;
			}
			case '3':
			{
				system("cls");
				update_reservation(rec, index, book, 33);
				break;
			}
			case '4':
			{
				break;
			}
			default:
			{
				cout << "\n\t\t\t(INVALID INPUT)\n";
				break;
			}
			}

			while (getchar() != '\n');
		}
	}

	return 0;
}

char display_menu()
{
	char choice;

	cout << "**************************************************************************\n";
	cout << "|\t\tWelcome to Teppanyaki Japanese system                    |";
	cout << "\n|\t                                                                 |";
	cout << "\n|\t[1] Display customer list                                        |";
	cout << "\n|\t[2] Search for availability                                      |";
	cout << "\n|\t[3] Update reservation record                                    |";
	cout << "\n|\t[4] Exit                                                         |";
	cout << "\n|\t                                                                 |";
	cout << "\n|   (enter the number in brackets to perform that particular action)     |";
	cout << "\n**************************************************************************";
	cout << "\n\t\tANS --> ";
	cin >> choice;

	return choice;
}

void display_rec(res_record rec[], int size, int book[][5], int book_size)
{
	char choice = '0';
	ifstream in_file(LIST);
	bool ret = false;
	int index = 0;
	{
		in_file.getline(rec[++index].name, 30);
		while (in_file)
		{
			in_file.getline(rec[size].phone, 11);
			in_file >> rec[index].res_date.date;
			in_file >> rec[index].res_date.month;
			in_file >> rec[index].res_date.year;
			in_file >> rec[index].num_guest;
			in_file >> rec[index].res_session;

			if (in_file.peek() == '\n')
				in_file.ignore(256, '\n');

			in_file.getline(rec[++index].name, 30);
		}

		in_file.close();

		while (!ret) // loop to contorl the access between functions in function 1 and main menu
		{
			cout << "**************************************************************************\n"; // for function 1 main header
			cout << "|\t\t\t\tDISPLAY LIST                             |";
			cout << "\n|                                                                        |";
			cout << "\n|\t[1] Display list                                                 |";
			cout << "\n|\t[2] Search customer record                                       |";
			cout << "\n|\t[3] Return                                                       |";
			cout << "\n|                                                                        |";
			cout << "\n**************************************************************************\n";
			cout << "\t\tANS --> ";
			cin >> choice;
			system("cls");

			if (choice == '1')
			{
				char second_choice;
				int count = 0;
				repeathere:
				cout << "*************************************************\n";  // menu for 'Display List'
				cout << "|\t[1] Display by date                     |";
				cout << "\n|\t[2] Display all                         |";
				cout << "\n*************************************************\n";
				cout << "\t\tANS --> ";
				cin >> second_choice;

				if (second_choice == '1')
				{
					char display_date[10];

					do
					{
						cout << "*******************************\n";  // prompt message for 'search by date'
						cout << "|      Enter display date     |";
						cout << "\n*******************************\n";
						cout << "\tANS --> ";
						cin.ignore();
						cin.getline(display_date, 10);

						if (atoi(display_date) == 0 || atoi(display_date) > 31)
							cout << "\n(INVALID INPUT)\n";

					} while (atoi(display_date) == 0 || atoi(display_date) > 31);

					cout << "**********************************************************************************************************\n";
					cout << "|                                        Session                                                         |";
					cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
					cout << "|                                                                                                        |\n";
					cout << "|                                   CUSTOMER RECORDS                                                     |\n";
					cout << "|--------------------------------------------------------------------------------------------------------|\n";
					cout << "|   Name\t   Phone Number\t\tReservastion date(d/m/y)\tNumber of guest\t  Session        |\n";
					for (int i = 1; i <= size; i++)
					{
						if (atoi(display_date) == rec[i].res_date.date) // find out and display the compatible with search date and reservation date stored in the list
						{
							cout << "|  " << setw(17) << left << rec[i].name << setw(17) << rec[i].phone << "\t\t" << rec[i].res_date.date << "/" << rec[i].res_date.month;
							cout << "/" << rec[i].res_date.year << "\t\t       " << rec[i].num_guest << "\t     " << rec[i].res_session << "           |\n";
							count++;
						}
						else if (i == size && atoi(display_date) != rec[i].res_date.date)
							cout << "|\t\t\t\t\t(NO OTHER RECORD FOUND)\t\t\t\t\t\t |\n";
					}
					cout << "|                                                                                                        |\n";
					cout << "|\tTotal reservation in session "; // display total reservation for each session
					for (int session = 1; session < 5; session++)
					{
						cout << "[" << session << "] = " << book[atoi(display_date)][session] << " guest    ";
					}
					cout << "|\t\t\tThe total reservation = " << count << "                                                                        |\n";
					cout << "**********************************************************************************************************\n";
				}
				else if (second_choice == '2')
				{
					int count = 0;

					cout << "**********************************************************************************************************\n"; // prompt message and header for search by session
					cout << "|                                        Session                                                         |";
					cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
					cout << "|                                                                                                        |\n";
					cout << "|                                   CUSTOMER RECORDS                                                     |\n";
					cout << "|--------------------------------------------------------------------------------------------------------|\n";
					cout << "|Customer ID\t   Name\t\tPhone Number\tReservastion date(d/m/y)\tNumber of guest\t  Session|\n";
					for (int i = 1; i < index; i++)
					{
						if (rec[i].num_guest == NULL)
							break;
						cout << "|   " << setw(3) << i << setw(19) << right << rec[i].name << setw(17) << rec[i].phone << "\t\t" << rec[i].res_date.date << "/" << rec[i].res_date.month;
						cout << "/" << rec[i].res_date.year << "\t\t       " << rec[i].num_guest << "\t    " << rec[i].res_session << "    |\n";
						count++;
					}
					cout << "|--------------------------------------------------------------------------------------------------------|\n";
					cout << "|\t\t\tThe total reservation - " << setw(3) << count << "                                                      |\n";
					cout << "**********************************************************************************************************\n";
					system("pause");
				}
				else
				{
					cout << "\n\t\t\t(INVALID INPUT)\n";
					goto repeathere;
				}
				break;
			}
			else if (choice == '2') // search for customer record by using either name or phone number
			{
				char second_choice;
				char search[15];
				bool result = false;

				cout << "********************************************************\n";
				cout << "|                                                      |";
				cout << "\n|\t[1] Search by name                             |";
				cout << "\n|\t[2] Search by phone number                     |";
				cout << "\n|                                                      |";
				cout << "\n********************************************************\n";
				cout << "\n\t\tANS-- > ";
				cin >> second_choice;
				{
					if (second_choice == '1') // search customer by using customer name
					{
						system("cls");
						cout << "*************************************";
						cout << "\n|\tEnter name to search        |";
						cout << "\n*************************************";
						cout << "\n\tANS --> ";
						cin >> search;

						cout << "**********************************************************************************************************\n";
						cout << "|                                        Session                                                         |";
						cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
						cout << "|                                                                                                        |\n";
						cout << "|                                   CUSTOMER RECORDS                                                     |\n";
						cout << "|--------------------------------------------------------------------------------------------------------|\n";
						cout << "|Customer ID\t   Name\t\tPhone Number\tReservastion date(d/m/y)\tNumber of guest\t  Session|\n";

						for (int record = 1; record <= size; record++)
						{
							result = strstr(rec[record].name, search);
							if (result)
							{
								cout << "|       " << record << setw(17) << right << rec[record].name << setw(17) << rec[record].phone << "\t\t" << rec[record].res_date.date << "/";
								cout << rec[record].res_date.month << "/" << rec[record].res_date.year << "\t\t      " << rec[record].num_guest << "\t\t    " << rec[record].res_session << "    |\n";

							}
							else if (record == size && !result)
								cout << "|\t\t\t\t\t(NO OTHER RECORD FOUND)\t\t\t\t\t\t |\n";
						}
						cout << "**********************************************************************************************************\n";
						system("pause");
					}
					else if (second_choice == '2') // search customer record by using phone number
					{
						system("cls");
						do
						{
							cout << "*************************************";
							cout << "\n|    Enter phone number to search   |";
							cout << "\n*************************************";
							cout << "\n\tANS --> ";
							cin >> search;

							if (atoi(search) == 0)
								cout << "\n\t(INVALID INPUT)\n";

						} while (atoi(search) == 0);

						cout << "**********************************************************************************************************\n";
						cout << "|                                        Session                                                         |";
						cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
						cout << "|                                                                                                        |\n";
						cout << "|                                   CUSTOMER RECORDS                                                     |\n";
						cout << "|--------------------------------------------------------------------------------------------------------|\n";
						cout << "|Customer ID\t   Name\t\tPhone Number\tReservastion date(d/m/y)\tNumber of guest\t  Session|\n";
						for (int record = 1; record <= size; record++)
						{
							result = strstr(rec[record].phone, search);
							if (result)
							{
								cout << "|" << right << setw(8) << record << setw(17) << right << rec[record].name << setw(17) << rec[record].phone << "\t\t" << rec[record].res_date.date << "/";
								cout << rec[record].res_date.month << "/" << rec[record].res_date.year << "\t\t       " << rec[record].num_guest << "\t      " << rec[record].res_session << "  |\n";
							}
							else if (record == size && !result)
								cout << "|\t\t\t\t\t(NO OTHER RECORD FOUND)\t\t\t\t\t\t |\n";
						}
						cout << "**********************************************************************************************************\n";
						system("pause");
					}
					else
						cout << "\n\t\t\t(INVALID INPUT)\n";
				}
			}
			else if (choice == '3')
			{
				ret = true; break;
			}
			else
				cout << "\n\t\t\t(INVALID INPUT)\n";
		}
	}
}

void search_availability(res_record rec[], int size, int book[][5], int book_size)
{
	time_t curr_time;
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);
	char choice = '0';
	bool quit = false;
	char search[99]; // for search mechanism
	char search_session[99] = { 0 }; // for session search mechanism
	int remain_slot = MAX_GUEST;

	while (!quit)
	{
		cout << "**************************************************************************\n"; // function 2 header
		cout << "|\t\t\tSEARCH AVAILABILITY                              |\n";
		cout << "|                                                                        |\n";
		cout << "|\t[1] By date                                                      |\n";
		cout << "|\t[2] By session                                                   |\n";
		cout << "|\t[3] By number of guest                                           |\n";
		cout << "|\t[4] Return                                                       |\n";
		cout << "|                                                                        |\n";
		cout << "**************************************************************************\n";
		cout << "\n\t\tANS-- > ";
		cin >> choice;
		system("cls");

		switch (choice)
		{
		case '1': // search availability by using date
		{
			do
			{
				cout << "***********************************************";
				cout << "\n|    Enter date to search for availability    |";
				cout << "\n***********************************************";
				cout << "\n\t\tANS -->";
				cin >> search;
				if (atoi(search) < tm_local->tm_mday + 3 || atoi(search) > 31 || atoi(search) == 0)
					cout << "\n   (At least 3 days booking in advance)\n";

			} while (atoi(search) < tm_local->tm_mday + 3 || atoi(search) > 31 || atoi(search) == 0);

			cout << "**********************************************************************************************************\n";
			cout << "|                                        Session                                                         |";
			cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
			cout << "|--------------------------------------------------------------------------------------------------------|\n";
			cout << "|   Date\t\tSession\t       Slot Occupied\t\t Slot available                          |\n";
			cout << "|                                                                                                        |";

			for (int date = 1; date < 32; date++)
				for (int session = 1; session <= 4; session++)
				{
					if (atoi(search) == date) // find compatible search keywords with the list and display.
					{
						cout << "\n|   " << date << "/" << tm_local->tm_mon + 1 << "\t\t   " << session << "\t            " << book[date][session] << "\t\t\t      ";
						cout << remain_slot - book[date][session] << "                                  |";
					}
				}
			cout << "\n**********************************************************************************************************";
			cout << endl;
			system("pause");
			break;
		}
		case '2': // search availability by using session
		{
			do
			{
				cout << "**********************************************************************************************************\n";
				cout << "|                                        Session                                                         |";
				cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
				cout << "----------------------------------------------------------------------------------------------------------\n";
				cout << "|                         Enter session to search for availability                                       |";
				cout << "\n**********************************************************************************************************\n";
				cout << "\n\t\tANS --> ";
				cin >> search_session;

				if (atoi(search_session) != 1 && atoi(search_session) != 2 && atoi(search_session) != 3 && atoi(search_session) != 4)
					cout << "\n\t\t(INVALID INPUT)\n";

			} while (atoi(search_session) != 1 && atoi(search_session) != 2 && atoi(search_session) != 3 && atoi(search_session) != 4); //validation for search input

			cout << "\n**********************************************************************************************************\n";
			cout << "|   Session\t\tDate\t       Slot Occupied\t\t Slot available                          |\n";
			cout << "|                                                                                                        |\n";

			for (int date = tm_local->tm_mday; date < 29; date++)
				for (int session = 1; session <= 4; session++)
				{
					if (atoi(search_session) == session) // search for compatible result between search keyword and session store in the list with booking count.
					{
						cout << "|       " << session << "\t\t" << date + 3 << "/" << tm_local->tm_mon + 1 << "\t             " << book[date + 3][session];
						cout << "\t\t                  " << remain_slot - book[date + 3][session] << "                              |\n";
					}
				}
			cout << "**********************************************************************************************************\n";
			system("pause");
			break;
		}
		case '3': // search availability by using guest number
		{
			int count = 0;
			do
			{
				cout << "****************************************************************";
				cout << "\n|    Enter number of guest to search for availability          |";
				cout << "\n****************************************************************";
				cout << "\n\t\tANS --> ";

				cin >> search;

				if (atoi(search) > MAX_GUEST || atoi(search) == 0)
					cout << "\n\t(INVALID INPUT- Max 8 guest)\n";
			} while (atoi(search) > MAX_GUEST || atoi(search) == 0); // validate search keywords

			cout << "**********************************************************************************************************\n";
			cout << "|                                        Session                                                         |";
			cout << "\n|   [1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm      |\n";
			cout << "|________________________________________________________________________________________________________|\n";
			cout << "|  Date\t\t\tSession\t\t\t  Slot available                                         |\n";

			for (int date = tm_local->tm_mday; date < 29; date++)
			{
				for (int session = 1; session <= 4; session++)
				{
					if (atoi(search) <= remain_slot - book[date + 3][session] && atoi(search) > 0) // find out and display any compatible result between search keywords and booking count.
					{
						cout << "|  " << date + 3 << "/" << tm_local->tm_mon + 1 << "\t\t\t   " << session << "\t\t\t       " << remain_slot - book[date + 3][session];
						cout << "                                                 |\n";
						count++;
						if ((date + 4) % (date + 3) != 0)
							cout << "|--------------------------------------------------------------------------------------------------------|\n";
					}

				}
			}
			cout << "**********************************************************************************************************\n";
			system("pause");
			break;
		}
		case '4':
		{
			quit = true;
			break;
		}
		default:
		{
			cout << "\n\t\t\t(INVALID INPUT)\n";
			break;
		}
		}
	}
}

void update_reservation(res_record rec[], int size, int book[][5], int book_size)
{
	time_t curr_time;
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	cout << "**************************************************************************\n";
	cout << "|  Current time - " << CURRENT_TIME;

	ofstream out_file(LIST, ios::app);
	char choice = '0';
	char second_choice[10];

	if (!out_file)
		cout << "\n(Error open ouput file)\n";
	else
	{
		while (choice != '2')
		{
			cout << "\n**************************************************************************\n"; // function 3 main header
			cout << "|\t\t\tUPDATE RESERVATION                               |\n";
			cout << "|                                                                        |";
			cout << "\n|\t[1] Update new reservation                                       |";
			cout << "\n|\t[2] Return                                                       |";
			cout << "\n|                                                                        |";
			cout << "\n**************************************************************************\n";
			cout << "\t\tANS-- > ";
			while (getchar() != '\n');
			cin >> choice;
			system("cls");

			switch (choice)
			{
			case '1':
			{
				cout << "\n****************************************************************************************************\n";
				do
				{
					cout << "\t\t\t\tMake reservation\n\n";
					while (getchar() != '\n');

					cout << "Enter customer name: ";
					cin.getline(rec[++size].name, 30);

				repeatphone:
					cout << "Enter phone number (without '-'): ";
					cin.getline(rec[size].phone, 11);

					while ((atoi(rec[size].phone)) == 0 || strstr(rec[size].phone, "-"))
					{
						cout << "\n\t(INVALID INPUT)\n";
						goto repeatphone;
					}

					char guest[11], session[10], date[10], month[10], year[10];

					do
					{
						cout << "___________________________________________________________________________________________________";

						do
						{
							cout << "\nNumber of guest: ";
							cin >> guest;

							if (atoi(guest) > MAX_GUEST || atoi(guest) == 0 || atoi(guest) <= 0)
								cout << "(INVALID INPUT - Maximum 8 guest per session)\n ";

						} while (atoi(guest) > MAX_GUEST || atoi(guest) == 0 || atoi(guest) <= 0);

						do
						{
							cout << "\nEnter Reservation Date IN NUMBER (press <enter> after entered 'day' to continue to 'month' and 'year'  ";
							cout << "\nDay: ";
							cin.ignore();
							cin >> date;
							cout << "Month: ";
							cin.ignore();
							cin >> month;
							cout << "Year: ";
							cin.ignore();
							cin >> year;

							if (!verify_date(date, month, 10, 10) || atoi(month) != tm_local->tm_mon + 1 || atoi(year) != tm_local->tm_year)
								cout << "\n(At least 3 days booking in advance and within the month)";
							else if (atoi(date) > 31 || atoi(date) <= 0)
								cout << "\n(INVALID DATE)";

						} while (!verify_date(date, month, 10, 10) || atoi(date) == 0 || atoi(year) != tm_local->tm_year + 1900); // validate reservation date with the implicit rules.

						do
						{
							cout << "\n\t\t\t\tSession";
							cout << "\n[1] 11:00am - 12:30pm\t[2] 12:30pm - 2:00pm\t[3] 5:00pm - 6:30pm\t[4] 6:30pm - 8:00pm";
							cout << "\n(Please enter the number in [bracket] to select reservation session)";
							cout << "\n\n\t\tANS --> ";
							cin >> session;

							if (session[0] != '1' && session[0] != '2' && session[0] != '3' && session[0] != '4')
								cout << "(INVALID INPUT)";

						} while (session[0] != '1' && session[0] != '2' && session[0] != '3' && session[0] != '4');

						if (atoi(guest) > MAX_GUEST - book[atoi(date)][atoi(session)]) // prompt message for user that the selected date ,session and guest is full
						{
							cout << "\n\tThe slot for date " << atoi(date) << " , session " << atoi(session) << " remain " << MAX_GUEST - book[atoi(date)][atoi(session)];
							cout << " slot.\n\tPlease choose again\n";
						}
						cout << "_____________________________________________________________________________________________________\n";

					} while (atoi(guest) > MAX_GUEST - book[atoi(date)][atoi(session)]); // validate that the entered details for guest number ,date and session is available

					rec[size].res_date.date = atoi(date); // date conversion from local variable into customer array
					rec[size].res_date.month = atoi(month);
					rec[size].res_date.year = atoi(year);
					rec[size].num_guest = atoi(guest);
					rec[size].res_session = atoi(session);

					out_file << rec[size].name << "\n" << rec[size].phone << "\n" << rec[size].res_date.date << "\n" << rec[size].res_date.month << "\n" << rec[size].res_date.year << "\n"
						<< rec[size].num_guest << "\n" << rec[size].res_session << endl;

					book[rec[size].res_date.date][rec[size].res_session] = book[rec[size].res_date.date][rec[size].res_session] + rec[size].num_guest;
					cout << "\nDo you want to continue?\n[Y] Continue\n[N] Return\nANS --> ";
					cin >> second_choice;
					cout << "\n****************************************************************************************************\n";

					do
					{
						if (second_choice[0] != 'y' && second_choice[0] != 'Y' && second_choice[0] != 'N' && second_choice[0] != 'n')
							cout << "\t\n(INVALID INPUT)\n";

						cout << "\nDo you want to continue?\n[Y] Continue\n[N] Return\nANS --> ";
						cin >> second_choice;
						cout << "\n****************************************************************************************************\n";

					} while (second_choice[0] != 'y' && second_choice[0] != 'Y' && second_choice[0] != 'N' && second_choice[0] != 'n'); // data validation for exit selection

				} while (second_choice[0] == 'y' || second_choice[0] == 'Y');

				out_file.close();
				break;
			}
			case '2':
			{
				out_file.close(); break;
			}
			default: cout << "\n(INVALID INPUT)\n";
			}
		}
	}
	return;
}


bool verify_date(char date[], char month[], int mon_size, int date_size) // for date validation
{
	time_t curr_time;
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	return (atoi(date) > tm_local->tm_mday + 3 && atoi(month) == tm_local->tm_mon + 1);
}