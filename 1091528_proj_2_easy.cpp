#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <bitset>
#include <map>
#include <math.h>
#include <cmath>

using namespace std;


int entry_ = 0;
int entry = 0;
string counterName[8] = { "SN0","WN1","WN2","WN3","WT3","WT2","WT1","ST0" };
struct datas {
	string history = "000"; //3-bits history
	int counter[8] = {};
};
struct data_ {
	string address16;
	string address;
	string operators;
	string code;
	string move;
};
vector<data_> fileContent[10]; //存code
vector<string> tagName;
vector<datas> BTB;

//簡易版 s只會有一行
int compute(string s)
{
	int decimal = (int(s[0]) - 48) * 4 + (int(s[1]) - 48) * 2 + (int(s[2]) - 48) * 1;
	return decimal;
}

//高分版
map<string, int> regValue; //register value
string pattern = ",";
void history(string& bit, char newbit)
{
	string tmp = bit;
	bit[0] = tmp[1];
	bit[1] = tmp[2];
	bit[2] = newbit;

}
int computeAnd(int num, int immediate)
{
	string tmp;
	bitset<8>val(num);
	bitset<8>imm(immediate);
	bitset<8>result = (val & imm);
	tmp += result.template to_string<char, char_traits<char>, allocator<char> >();
	int mul = 1;
	int total = 0;
	for (int i = tmp.size() - 1; i >= 0; i--) {
		total += (int(tmp[i] - 48)) * mul;
		mul *= 2;
	}
	return total;
}
int findBTB(string address)
{
	string::size_type begin, end;
	int total = 0, mul = 1;;

	for (int i = 0, j = address.size() - 3; i < entry_; i++, j--)
	{
		total += ((int(address[j]) - 48) * mul);
		mul *= 2;
	}
	return total;
}
void output(int num, char taken)
{
	int index = compute(BTB[num].history);
	for (int i = 0; i < entry; i++) {
		cout << BTB[i].history << " ";
		for (int j = 0; j < 7; j++) {
			cout << counterName[BTB[i].counter[j]] << " ";
		}
		cout << counterName[BTB[i].counter[7]] << " ";
		if (i == num) {
			cout << setw(5)<<counterName[BTB[num].counter[index]][1] << setw(11) << taken << " ";
			if (counterName[BTB[num].counter[index]][1] != taken)
				cout << setw(9) << "miss" << endl;
			else
				cout << setw(12) << "not miss" << endl;
		}
		else
			cout << endl;
	}
	cout << endl;
	//改變下一次的3-bit history
	if (taken == 'T') {
		history(BTB[num].history, '1');
		if (BTB[num].counter[index] < 7)
			BTB[num].counter[index]++;
	}
	else {
		history(BTB[num].history, '0');
		if (BTB[num].counter[index] > 0)
			BTB[num].counter[index]--;
	}

}
int compute_()
{
	int a = entry;
	int c = 0;
	while (a / 2 != 0) {
		a = a / 2;
		c++;
	}
	return c;
}

void li(string code)
{
	string::size_type begin, end;
	//已開始初始化變數的部分
	begin = code.find(',') + 1;
	end = code.size();
	string tmp = code.substr(begin, end - begin);
	end = code.find(',');
	begin = 0;
	regValue[code.substr(begin, end - begin)] += stoi(tmp);
	
}
int beq(string code) {
	//cout << "beq" << " " << code << endl;
	string::size_type begin, end;
	//第一個reg value
	begin = 0;
	end = code.find(',');
	int reg1 = regValue[code.substr(begin, end - begin)];
	//第二個reg value
	begin = end + 1;
	end = code.find(pattern, begin);
	int reg2 = regValue[code.substr(begin, end - begin)];
	//branch taken.
	if (reg1 == reg2) {
		begin = end + 1;
		end = code.size();
		string name = code.substr(begin, end - begin);
		for (int i = 0; i < tagName.size(); i++)
		{
			if (name == tagName[i])
			{
				return i;
			}
		}

	}
	return -1;
}
int bne(string code) {
	cout << "bne " << code << endl;
	string::size_type begin, end;
	//第一個reg value
	begin = 0;
	end = code.find(',');
	int reg1 = regValue[code.substr(begin, end - begin)];
	//第二個reg value
	begin = end + 1;
	end = code.find(pattern, begin);
	int reg2 = regValue[code.substr(begin, end - begin)];
	//branch taken.
	if (reg1 != reg2) {
		begin = end + 1;
		end = code.size();
		string name = code.substr(begin, end - begin);
		for (int i = 0; i < tagName.size(); i++)
		{
			if (name == tagName[i])
			{
				return i;
			}
		}

	}
	return -1;
}
void andi(string code)
{
	string::size_type begin, end;
	//第一個reg 
	begin = 0;
	end = code.find(',');
	string reg1 = code.substr(begin, end - begin);
	//第二個reg 
	begin = end + 1;
	end = code.find(pattern, begin);
	string reg2 = code.substr(begin, end - begin);
	//immediate
	begin = end + 1;
	end = code.size();
	regValue[reg1] = regValue[reg2] % 4;
}
void add(string code)
{
	string::size_type begin, end;
	//第一個reg 
	begin = 0;
	end = code.find(',');
	string reg1 = code.substr(begin, end - begin);
	//第二個reg 
	begin = end + 1;
	end = code.find(pattern, begin);
	string reg2 = code.substr(begin, end - begin);
	//第三個reg
	begin = end + 1;
	end = code.size();
	string reg3 = code.substr(begin, end - begin);

	//執行add inst
	regValue[reg1] = regValue[reg2] + regValue[reg3];

}
void addi(string code)
{
	string::size_type begin, end;
	//第一個reg 
	begin = 0;
	end = code.find(',');
	string reg1 = code.substr(begin, end - begin);
	//第二個reg 
	begin = end + 1;
	end = code.find(pattern, begin);
	string reg2 = code.substr(begin, end - begin);
	//immediate
	begin = end + 1;
	end = code.size();
	regValue[reg1] = regValue[reg2] + stoi(code.substr(begin, end - begin));
}
int LoopJ()
{
	for (int i = 0; i < fileContent[2].size(); i++)
	{
		if (fileContent[2][i].operators == "beq") {
			int taken = beq(fileContent[2][i].code);
			int num = findBTB(fileContent[2][i].address); //回傳entry的index

			if (taken != -1) {
				output(num, 'T');
				return taken;
			}
			output(num, 'N');
		}
		else if (fileContent[2][i].operators == "li") {
			li(fileContent[2][i].code);
		}
		else if (fileContent[2][i].operators == "andi")
		{
			andi(fileContent[2][i].code);
		}
		else if (fileContent[2][i].operators == "add")
		{
			add(fileContent[2][i].code);
		}
		else if (fileContent[2][i].operators == "bne") {
			int taken = bne(fileContent[2][i].code);
			int num = findBTB(fileContent[2][i].address); //回傳entry的index

			if (taken != -1) {
				output(num, 'T');
				return taken;
			}
			output(num, 'N');
		}
	}
	return 2;
}
int LoopI()
{
	for (int i = 0; i < fileContent[1].size(); i++)
	{
		cout << fileContent[1][i].address16 <<" " << fileContent[1][i].operators << " " << fileContent[1][i].code << endl;
		int num = findBTB(fileContent[1][i].address); //回傳entry的index
		if (fileContent[1][i].operators == "beq") {
			int taken = beq(fileContent[1][i].code);
			if (taken != -1) {
				output(num, 'T');
				return taken;
			}
			output(num, 'N');
		}
		if (fileContent[1][i].operators == "addi") {
			addi(fileContent[1][i].code);
			output(num, 'N');
		}
	}
	return 1; //LoopJ
}
int Endif()
{
	for (int i = 0; i < fileContent[3].size(); i++)
	{
		if (fileContent[3][i].operators == "addi")
			addi(fileContent[3][i].code);
		else if (fileContent[3][i].operators == "beq")
		{
			int taken = beq(fileContent[3][i].code);
			int num = findBTB(fileContent[3][i].address); //回傳entry的index
			cout << fileContent[3][i].code << endl;
			if (taken != -1) {
				output(num, 'T');
				return taken; //必定發生
			}
			output(num, 'N');
		}
	}
	return -1;//default
}
int EndLoopJ()
{
	for (int i = 0; i < fileContent[4].size(); i++)
	{
		if (fileContent[4][i].operators == "addi")
			addi(fileContent[4][i].code);
		else if (fileContent[4][i].operators == "beq")
		{
			int taken = beq(fileContent[3][i].code);
			int num = findBTB(fileContent[3][i].address); //回傳entry的index
			cout << fileContent[3][i].code << endl;
			if (taken != -1) {
				output(num, 'T');
				return taken; //必定發生
			}
			output(num, 'N');
			
		}
	}
	return -1;//default
}
int EndLoopI()
{
	return 5;
}
void predictor(const int entry)
{
	string pattern = ",";
	string::size_type begin, end;

	datas tmp;
	for (int i = 0; i < entry; i++) {
		BTB.push_back(tmp);
	}
	//已開始初始化變數的部分
	for (int i = 0; i < fileContent[0].size(); i++) {
		cout << fileContent[0][i].address16 << " " << fileContent[0][i].operators << " " << fileContent[0][i].code << endl;
		li(fileContent[0][i].code);
		int num = findBTB(fileContent[0][i].address);
		output(num, 'N');
	}
	int part = 1;
	while (part != 5)
	{
		switch (part) {
		case 1:
			part = LoopI();
			break;
		case 2:
			part=EndLoopI();
		
		}
	}

}

string changeBinary(string address)
{
	string addr;
	bitset<4>tmp;
	for (int i = 0; i < address.size(); i++) {
		if (int(address[i]) <= 57) {
			tmp = (int(address[i]) - 48);
		}
		else {
			tmp = (int(address[i]) - 55);//A,B,C,D,E,F
		}
		addr += tmp.template to_string<char, char_traits<char>, allocator<char> >(); //將bit轉成字串

	}
	return addr;

}
int main()
{
	ifstream infile;
	infile.open("input.txt");//這邊要改檔名
	if (!infile.is_open()) {
		cout << "Failed to open file.\n";
	}
	else
	{
		cout << "entry:";
		cin >> entry;
		cout << endl;
		entry_ = compute_();
		int num = 0;
		cout << setw(46) << "prediction" << setw(9) << "outcome" << setw(7) << "miss?" << endl;
		string address, operators, code, move, s;
		string::size_type begin, end;
		move = "start";
		tagName.push_back(move);

		while (infile >> address) {
			data_ tmp;

			if (address[0] == '0')
			{
				//轉成二進制
				tmp.address16 = address;
				address = address.substr(2);
				address = changeBinary(address);
				tmp.address = address;
				//取operator
				infile >> operators;
				tmp.operators = operators;
				//取code
				infile >> code;
				tmp.code = code;
				tmp.move = move;
				fileContent[num].push_back(tmp);

			}
			else if (address == ";") {
				string st;
				getline(infile, st);
				st.clear();
			}
			else {
				num++;
				string::size_type begin, end;
				begin = 0;
				end = address.find(':');
				move = address.substr(begin, end - begin);
				tagName.push_back(move);
			}
		}
		predictor(entry);
	}
	infile.close();

	return 0;
}
