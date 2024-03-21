#pragma once
#include <vector>
#include <iostream>
#include <winsock2.h>

#pragma warning(disable : 4996)

using namespace std;

class Details
{
private:
	class Detail
	{
	public:
		char name[50];
		int quantity;
		char STRDETAIL[70];
		Detail(const char* name, int quantity)
		{
			for (int poz = 0; name[poz] != '\0'; poz++)
			{
				this->name[poz] = name[poz];
			}
			this->quantity = quantity;
			MergeParameters();
		}

		Detail(const char* STRDETAIL)
		{
			int poz = 0;
			for (; STRDETAIL[poz] != '\0'; poz++)
			{
				this->STRDETAIL[poz] = STRDETAIL[poz];
			}
			this->STRDETAIL[poz] = '\0';
			if (!CheckForSpaces())
			{
				throw exception("NO you FUCKING MORON DON'T.....");
			}
			this->STRDETAIL[poz] = '\0';
			try
			{

				SeparateParameters();
			}
			catch (const char* ex)
			{
				throw exception("Something went wrong");
			}
		}

		void MergeParameters()
		{
			int mainpoz = 0;
			char temp_quantity[10];
			itoa(quantity, temp_quantity, 10);
			for (int temppoz = 0; name[temppoz] != '\0'; mainpoz++, temppoz++)
			{
				this->STRDETAIL[mainpoz] = this->name[temppoz];
			}
			this->STRDETAIL[mainpoz++] = ' ';
			for (int temppoz = 0; temp_quantity[temppoz] != '\0'; mainpoz++, temppoz++)
			{
				this->STRDETAIL[mainpoz] = temp_quantity[temppoz];
			}
			this->STRDETAIL[mainpoz] = '\0';
		}

		void SeparateParameters()
		{
			int mainpoz = 0;
			char temp_quantity[10];
			for (; this->STRDETAIL[mainpoz] != ' '; mainpoz++)
			{
				this->name[mainpoz] = this->STRDETAIL[mainpoz];
			}
			this->name[mainpoz++] = '\0';
			for (int temppoz = 0; this->STRDETAIL[mainpoz] != '\0'; mainpoz++, temppoz++)
			{
				temp_quantity[temppoz] = this->STRDETAIL[mainpoz];
			}
			quantity = atoi(temp_quantity);
			if (quantity == 0)
			{
				throw "Nope.";
			}
		}

		bool CheckForSpaces()
		{
			int spacecounter = 0;
			for (int poz = 0; STRDETAIL[poz] != '\0' && spacecounter < 2; poz++)
				if (STRDETAIL[poz] == ' ')
					spacecounter++;
			if (spacecounter != 1)
				return false;
			return true;
		}
	};

public:
	std::vector<Detail> details;
	int size;

	Details()
	{
		this->details.push_back(Detail("Engine 15"));
		this->details.push_back(Detail("Clock_gear 45"));
		this->details.push_back(Detail("Motor 3"));
		this->details.push_back(Detail("Band 32"));
		this->details.push_back(Detail("Gimbal 35"));
		this->details.push_back(Detail("Damper 5"));
		this->details.push_back(Detail("Flywheel 32"));
		this->details.push_back(Detail("valve 14"));
		size = details.size();
	}

	int DetailIndex(char* detail)
	{
		for (int index = 0; index < details.size(); index++)
		{
			for (int poz = 0; details[index].name[poz] != '\0'; poz++)
			{
				if (detail[poz] == details[index].name[poz])
				{
					if (details[index].name[poz + 1] == '\0')
						return index;
					continue;
				}
				break;
			}
		}
		return -1;
	}
	template<typename VALUE>
	void CatchWrongValue(VALUE value)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			throw exception("Wrong input.");
		}
	}
	void AddDetail()
	{
		char BUF_STRDETAIL[70];
		cout << "Input detail in format 'detail(line) [quantity](num)'" << endl;
		cin.getline(BUF_STRDETAIL, 70);
		cin.getline(BUF_STRDETAIL, 70);
		int index = DetailIndex(BUF_STRDETAIL);
		if (index == -1)
		{
			try
			{
				Detail* detail = new Detail(BUF_STRDETAIL);
				this->details.push_back(*detail);
				size++;
			}
			catch(exception ex)
			{
				cout << ex.what() << endl;
				return;
			}
		}
		else
		{
			cout << "This detail already exists" << endl;
		}
	}
	void EditDetail()
	{
		char BUF_NAME[70];
		cout << "Input detail name: ";
		cin.getline(BUF_NAME, 70);
		cin.getline(BUF_NAME, 70);
		int index = DetailIndex(BUF_NAME);
		if (index == -1)
		{
			cout << "There's no such detail in DB" << endl;
			return;
		}
		cout << "Input new quantity: ";
		try
		{
			int buf;
			cin >> buf;
			CatchWrongValue(buf);
			this->details[index].quantity = buf;
		}
		catch(exception ex)
		{
			cout << ex.what() << endl;
		}
	}
	void DeleteDetail()
	{
		char BUF_NAME[70];
		cout << "Input name of detail that should be deleted'" << endl;
		cin.getline(BUF_NAME, 70);
		cin.getline(BUF_NAME, 70);
		int index = DetailIndex(BUF_NAME);
		if (index == -1)
		{
			cout << "There's no such detail in DB" << endl;
			return;
		}
		this->details.erase(std::next(this->details.begin(), index)); size--;
	}
	void PrintAllDetails()
	{
		for (auto detail : details)
		{
			cout << detail.STRDETAIL << endl;
		}
	}
	void FindDetail()
	{
		char buf[50];
		cout << "Input detail name: ";
		cin >> buf;
		int index = DetailIndex(buf);
		if (index == -1)
			cout << "Detail is not found." << endl;
		else
			cout << "Quantity: " << details.at(index).quantity << endl;
	}

	char* GetSTRDETAIL(int index)
	{
		return details[index].STRDETAIL;
	}

	void SendToServer(SOCKET& newConnection)
	{
		char sizeofvec[3];
		itoa(details.size(), sizeofvec, 10);	
		send(newConnection, sizeofvec, 3, 0);
		for (auto const& detail : details)
		{
			send(newConnection, detail.STRDETAIL, sizeof(detail.STRDETAIL), 0);
		}
	}

	void AddDetailSERVER(SOCKET& newConnection)
	{
		char NewDetail[70];
		recv(newConnection, NewDetail, 70, 0);
		int index = DetailIndex(NewDetail);
		if (index == -1)
		{
			try
			{
				Detail* detail = new Detail(NewDetail);
				this->details.push_back(*detail);
				send(newConnection, "Detail added!", 255, 0);
				size++;
			}
			catch (exception ex)
			{
				send(newConnection, ex.what(), 255, 0);
				cout << ex.what() << endl;
				return;
			}
		}
		else
		{
			send(newConnection, "This detail already exists", 255, 0);
		}
	}

	void EditDetailSERVER(SOCKET& newConnection)
	{
		char DetailName[50];
		int quantity;
		char quantity_ch[5];
		recv(newConnection, DetailName, 50, 0);
		int index = DetailIndex(DetailName);
		if (index == -1)
		{
			send(newConnection, "Detail is not found", 20, 0);
			return;
		}
		else
		{
			send(newConnection, "0", 20, 0);
			recv(newConnection, quantity_ch, 5, 0);
			quantity = atoi(quantity_ch);
			if (quantity == 0)
			{
				send(newConnection, "Invalid input", 16, 0);
				return;
			}
			details[index].quantity = quantity;
			details[index].MergeParameters();
			send(newConnection, "Succesful edit!", 16, 0);
		}
	}

	void DeleteItemSERVER(SOCKET& newConnection)
	{
		char DetailName[50];
		recv(newConnection, DetailName, 50, 0);
		int index = DetailIndex(DetailName);
		if (index == -1)
		{
			send(newConnection, "Detail is not found", 20, 0);
			return;
		}
		else
		{
			this->details.erase(std::next(this->details.begin(), index)); size--;
			send(newConnection, "Detail deleted!", 20, 0);
		}
	}
};

