#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdlib.h>

using namespace std;

string counterName[8] = { "SN0","WN1","WN2","WN3","WT3","WT2","WT1","ST0" };
struct datas {
	string history="000"; //3-bits history
	int counter[8]={};
};
string fileContent; //存code

datas BTB;


//簡易版 s只會有一行
int compute(string s)
{
	int decimal = (int(s[0])-48) * 4 + (int(s[1])-48) * 2 + (int(s[2])-48) * 1;
	return decimal;
}
void predictor(string s)
{
	int miss = 0;
	for (int i = 0; i < s.size(); i++)
	{
		//改history的值
		int a = i - 3;
		if (a < 0) a = 0;
		for (int j = i - 1, x = 2; j >= a; j--, x--) {
			if (s[j] == 'T') BTB.history[x] = '1';
			else BTB.history[x]='0';
		}

		cout << BTB.history << " ";
		for (int i = 0; i < 7; i++)
		{
			cout << counterName[BTB.counter[i]] << " ";
		}
		cout << counterName[BTB.counter[7]] << " ";
		int n = compute(BTB.history);
		string pred = counterName[BTB.counter[n]];
		
		cout <<setw(5) << pred[1] << setw(11) << s[i];
		if (pred[1] != s[i])
			cout << setw(9)<<"miss" << endl;
		else
			cout << setw(12)<<"not miss" << endl;
		//改變下一次的值
		if (s[i] == 'T')
		{
			
			if (BTB.counter[n] < 7)
				BTB.counter[n]++;

		}
	}
}

//高分版
//void addressToBinary()
//{
//
//}
//void predictor()
//{
//	for (int i = 0; i < fileContent.size(); i++)
//	{
//		//去除多餘空白
//		for (int x = 0; x < fileContent[i].size(); x++)
//		{
//			if (fileContent[i][x] == ' ' && fileContent[i][x + 1] == ' ')
//			{
//				string::iterator it = fileContent[i].begin()+x;
//				fileContent[i].erase(it);
//
//			}
//		}
//		string::size_type begin, end;
//		end = fileContent[i].find(' ');
//
//
//	}
//}


int main()
{
	

	int choose; //輸入0:簡易版 輸入1:高分版
	cout << "choose:";
	cin >> choose;
	cout << endl;
	//read file
	ifstream infile;
	infile.open("outcome.txt"); //使用同一個檔案 所以要記得改檔案內容!
	if (!infile.is_open()) {
		cout << "Failed to open file.\n";
	}
	else if(choose==0) //簡易版讀檔
	{
		string s;

		getline(infile, s);
		//infile.close();

		cout << setw(46) << "prediction" << setw(9) << "outcome" << setw(7) << "miss?" << endl;
		predictor(s);
	}
	
	infile.close();

	
	
	


	return 0;
}
