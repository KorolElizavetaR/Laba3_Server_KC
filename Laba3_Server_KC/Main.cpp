#include "Details.cpp"
#include <stdlib.h>
#include <winsock2.h>
#include <thread>

//#define NOSERVER
#define SERVERON

#define PORT 1234

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

static int ClientCounter = 0;
static int ErrorCounter = 3;

void ForServer();
void HandleClient(SOCKET newConnection);
Details det;


template<typename VALUE>
void CatchWrongValue(VALUE value)
{
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
	}
}

int main()
{
	#ifdef NOSERVER
	try
	{
		ForServer();
	}
	catch (...)
	{
		cout << "Some sort of program error occured";
	}	
	#endif

#ifdef SERVERON

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		return -1;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	do
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0) {
			cout << "Error. newConnection == 0." << endl;
			return -2;
		}
		else
		{
			cout << "Client " << ++ClientCounter << " connected" << endl;
		}
		thread th(HandleClient, newConnection);
		th.detach();

	} while (ClientCounter > 0);

	system("pause");
	return 1;
#endif
}

void HandleClient(SOCKET newConnection)
{
	const char directives[] = 
		"1. Print all\n2. Add detail\n3. Edit quantity of detail\n4. Delete detail\n5. Find quantity of detail\n6. Exit";
	send(newConnection, directives, sizeof(directives), 0);
	int choice;
	do
	{
		char choicereceived[5] = "    ";
		recv(newConnection, choicereceived, sizeof(choicereceived), 0);
		std::cout << "choice received: " << choicereceived << endl;
		choice = atoi(choicereceived);
		switch (choice)
		{
		case 1: det.SendToServer(newConnection); break;
		case 2: det.AddDetailSERVER(newConnection);  break;
		case 3: det.EditDetailSERVER(newConnection); break;
		case 4: det.DeleteItemSERVER(newConnection);  break;
		case 5: break;
		default:
			cout << "Wrong input" << endl;
			CatchWrongValue(&choice);
			ErrorCounter--;
		}
	} while (ErrorCounter > 0);

	cout << "Error occured." << endl;
	ErrorCounter = 3;
	--ClientCounter;
	cout << "Client disconnected." << endl;
}

void ForServer()
{
	Details det;
	while (true)
	{
		cout << "1. Print all" << endl <<
			"2. Add detail" << endl <<
			"3. Edit quantity of detail" << endl <<
			"4. Delete detail" << endl <<
			"5. Find quantity of detail" << endl <<
			"6. Exit" << endl;
		int choice;
		cout << "\tinput: ";
		cin >> choice;
		switch (choice)
		{
		case 1: det.PrintAllDetails(); break;
		case 2: det.AddDetail(); break;
		case 3: det.EditDetail(); break;
		case 4: det.DeleteDetail();  break;
		case 5: det.FindDetail(); break;
		case 6: return;
		default:
			cout << "Wrong input" << endl;
			CatchWrongValue(&choice);
		}
	}
}