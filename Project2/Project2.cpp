#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

struct stClient
{
    string AcountNumber;
    string PINcode;
    string Name;
    string PhoneNumber;
    double Balance;
    bool MarkClient = false;
};

stClient CurrentClient;

enum enAtmMainMenueOptions {eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5};
enum enQuickWithdrawOptions {e20 = 1, e50 = 2, e100 = 3, e200 = 4, e400 = 5, e600 = 6,e800 = 7,e1000 = 8,eExit = 9};

void ShowAtmMainMenueScreen();
void Login();

vector <string> SplitEachWord(string S1,string delim)
{
    short pos = 0;

    string sWord;
    vector <string> vWord;
    // use find() function to find position of the delimiters

    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sWord = S1.substr(0,pos); // store thr word

        if (S1 != " ")
        {
            vWord.push_back(sWord);
        }

        (S1.erase(0,pos + delim.length())); //erase() until position and move to next word
    }
    if (S1 != "") // it print last word of the string
    {
        vWord.push_back(S1);
    }
    return vWord;
}

stClient ConvertLineToRecord(string Line, string Seporator = "#//#")
{
    stClient Client;
    vector <string> vClient = SplitEachWord(Line,Seporator);

    Client.AcountNumber = vClient[0];     
    Client.PINcode = vClient[1];    
    Client.Name = vClient[2];    
    Client.PhoneNumber = vClient[3];    
    Client.Balance = stod(vClient[4]);

    return Client;
}

string ConvertRecordToLine(stClient Client, string Seporator = "#//#")
{
    string sClient;
    
    sClient += Client.AcountNumber + Seporator;
    sClient += Client.PINcode + Seporator;
    sClient += Client.Name + Seporator;
    sClient += Client.PhoneNumber + Seporator;
    sClient += to_string(Client.Balance);

    return sClient;
}

vector <stClient> LoadClientsDataFromFile(string FileName)
{
    vector <stClient> vClient;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile,Line))
        {
            Client = ConvertLineToRecord(Line);

            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return vClient;
}

vector <stClient> SaveClientDataToFile(string FileName, vector <stClient> vClient)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClient &c : vClient)
        {
            if (c.MarkClient == false)
            {
                DataLine = ConvertRecordToLine(c);

                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}

bool FindClientByAcountNumberAndPinCode(string AcountNumber,string Pin,stClient &CurrentClient)
{
    vector <stClient> vClient = LoadClientsDataFromFile("Clients.txt");

    for (stClient &c : vClient)
    {
        if (c.AcountNumber == AcountNumber && c.PINcode == Pin)
        {
            CurrentClient = c;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AcountNumber, string Pin)
{
    if (FindClientByAcountNumberAndPinCode(AcountNumber,Pin,CurrentClient))
       return true;
    else
       return false;   
}

bool YouWantToPerfromTransaction()
{
    char Answer = ' ';

    cout << "\nAre you sure you want to Perfrom this transaction ? Y/N ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return true;
    }
    return false;
}

short ReadQuickWithdrawChoice()
{
    short Choice;

    cout << "Choce what to withdraw from [1] to [8] ?\n";
    cin >> Choice;

    return Choice;
}

short PerformQuickWithdraw(enQuickWithdrawOptions QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case enQuickWithdrawOptions::e20:
    {
        return 20;
        break;
    }
    case enQuickWithdrawOptions::e50:
    {
        return 50;
        break;
    }
    case enQuickWithdrawOptions::e100:
    {
        return 100;
        break;
    }
    case enQuickWithdrawOptions::e200:
    {
        return 200;
        break;
    }
    case enQuickWithdrawOptions::e400:
    {
        return 400;
        break;
    }
    case enQuickWithdrawOptions::e600:
    {
        return 600;
        break;
    }
    case enQuickWithdrawOptions::e800:
    {
        return 800;
        break;
    }
    case enQuickWithdrawOptions::e1000:
    {
        return 1000;
        break;
    }
    case enQuickWithdrawOptions::eExit:
    {
        ShowAtmMainMenueScreen();
        break;
    }
    }
    return 0;
}

bool QuickWithdraw(vector <stClient> &vClient)
{

    short WithdrawAmount = PerformQuickWithdraw((enQuickWithdrawOptions)ReadQuickWithdrawChoice());

    while (WithdrawAmount > CurrentClient.Balance)
    {
        
        cout << "Amount Exceeds the balance, you can withraw up to : " << CurrentClient.Balance << endl;
        WithdrawAmount = PerformQuickWithdraw((enQuickWithdrawOptions)ReadQuickWithdrawChoice());
    } 

    if (YouWantToPerfromTransaction())
    {
        for (stClient &c : vClient)
        {
            if (c.AcountNumber == CurrentClient.AcountNumber)
            {
                c.Balance -= WithdrawAmount;
                break;
            }
        }

        SaveClientDataToFile("Clients.txt",vClient);

        cout << "\n\nTransaction done Successfully New Balance is : " << CurrentClient.Balance - WithdrawAmount << endl;
        return true;
    }
    return false;
}

string ReadAcountNumber()
{
    string AcountNmber;

    cout << "Pleas enter the acount number\n";
    cin >> AcountNmber;

    return AcountNmber;
}

void ShowQuickWithdrawScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "            Quick WithdrawAmount \n";
    cout << "-----------------------------------\n" << endl;
    cout << "   [1] 20 \t[2] 50\n";
    cout << "   [3] 100 \t[4] 200\n";
    cout << "   [5] 400 \t[6] 600\n";
    cout << "   [7] 800 \t[8] 1000\n";
    cout << "   [9] Exit\n";
    cout << "-----------------------------------\n" << endl;

    vector <stClient> vClient = LoadClientsDataFromFile("Clients.txt");

    QuickWithdraw(vClient);
}

int ReadNormalWithdrawAmount()
{
    int Amount;

    cout << "Enter Withdraw amount must be multiple of 5's ?";
    cin >> Amount;

    while (Amount % 5 != 0)
    {
        cout << "Enter an amount must be multiple of 5's ?";
        cin >> Amount;
    }
    return Amount;
}

bool NormalWithdraw(vector <stClient> &vClient)
{
    int WithdrawAmont;

    WithdrawAmont = ReadNormalWithdrawAmount();
        
    while (WithdrawAmont > CurrentClient.Balance)
    {
        cout << "Amount Exceeds the balance, you can withraw up to : " << CurrentClient.Balance << endl;
        cout << "Pleas enter withdraw amount ? "; 
        cin >> WithdrawAmont;
    } 
        
    if (YouWantToPerfromTransaction())
    {
        for (stClient &c : vClient)
        {
            if (c.AcountNumber == CurrentClient.AcountNumber)
            {
                c.Balance -= WithdrawAmont;
                break;
            }
        }

        SaveClientDataToFile("Clients.txt",vClient);

        cout << "\n\nTransaction done Successfully New Balance is : " << CurrentClient.Balance - WithdrawAmont << endl;

        return true;
    }
    return false;
}

void ShowNormalWithdrawScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "          Withdraw Screen\n";
    cout << "-----------------------------------\n" << endl;

    vector <stClient> vClient = LoadClientsDataFromFile("Clients.txt");

    NormalWithdraw(vClient);
}

bool Deposit(vector <stClient> &vClient)
{
    double DepositAmount;

    cout << "Enter deposit amount ?";
    cin >> DepositAmount;

    if (YouWantToPerfromTransaction())
    {
        for (stClient &c : vClient)
        {
            if (c.AcountNumber == CurrentClient.AcountNumber)
            {
                c.Balance += DepositAmount;
                break;
            }
        }

        SaveClientDataToFile("Clients.txt",vClient);

        cout << "\n\nTransaction done Successfully New Balance is : " << CurrentClient.Balance + DepositAmount << endl;

        return true;
    }

    return false;
}

void ShowDepositScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "          Deposit Screen\n";
    cout << "-----------------------------------\n" << endl;

    vector <stClient> vClient = LoadClientsDataFromFile("Clients.txt");

    Deposit(vClient);
}

void ShowCheckBalanceScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "          Check Balance\n";
    cout << "-----------------------------------\n" << endl;

    cout << "Your balance is : " << CurrentClient.Balance << endl;
}

void PerfromAtmMainMenue(enAtmMainMenueOptions AtmMainMenueOption)
{
    switch (AtmMainMenueOption)
    {
    case enAtmMainMenueOptions::eQuickWithdraw:
    {
        ShowQuickWithdrawScreen();
        break;
    }    
    case enAtmMainMenueOptions::eNormalWithdraw:
    {
        ShowNormalWithdrawScreen();
        break;
    }
    case enAtmMainMenueOptions::eDeposit:
    {
        ShowDepositScreen();
        break;
    }
    case enAtmMainMenueOptions::eCheckBalance:
    {
        ShowCheckBalanceScreen();
        break;
    }
    case enAtmMainMenueOptions::eLogout:
    {
        Login();
        break;
    }
    }
}

short ReadAtmMainMenueOption()
{
    short Choice;

    cout << "Choose what do you want to do ? [1 to 5]? ";
    cin >> Choice;

    return Choice;
}

void ShowAtmMainMenueScreen()
{
    cout << "\n\n==================================\n";
    cout << "       Transactions Menue Screen\n";
    cout << "==================================\n";
    cout << "[1] Quick Withdraw." << endl;
    cout << "[2] Normal Withdraw." << endl;
    cout << "[3] Deposit." << endl;
    cout << "[4] Check Balances." << endl;
    cout << "[5] Logout." << endl;

    PerfromAtmMainMenue((enAtmMainMenueOptions)ReadAtmMainMenueOption());

}

void Login()
{
    bool LoginFailed = true;
    string AcountNumber,Pin;

    cout << "\n\n==================================\n";
    cout << "         Login Screen\n";
    cout << "==================================\n";

    do 
    {
        if (LoginFailed)
        {
            cout << "Invalide AcountNumber/PIN\n";
        }    

        cout << "Enter AcountNumber : ";
        cin >> AcountNumber;

        cout << "Enter PIN : ";
        cin >> Pin;

        LoginFailed = !LoadClientInfo(AcountNumber,Pin);

    } while (LoginFailed);

    ShowAtmMainMenueScreen();
}

int main()
{
    Login();

    return 0;
}