//A C++ code to simulate a bank system using a MySLQ database running on localhost(or any server for that case)
//Support deposits,withdrawals,balance checking etc..
//Uses MYSQL library
/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
int bankAmount;
string amount;
void checkAccountNumber(string accountNumber);
void askPIN(string username,string pin,int balance,string account_number);
void showMenu(string username,string pin,int balance,string account_number);
void showOptions(string account_number);
void showBalance(string accountnumber);
void withdraw(string accountnumber);
void deposit(string accountnumber);
int main(void)
{
    string account;
    cout<<"Enter the account number "<<endl;
    cin>>account;
    checkAccountNumber(account);
    return EXIT_SUCCESS;
}
void checkAccountNumber(string accountNumber){
     try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::ResultSet *res;
  sql::PreparedStatement *prep_stmt;

  driver = get_driver_instance();
  con = driver->connect("127.0.0.1/bank", "root", "");
  con->setSchema("bank");

  prep_stmt = con->prepareStatement("SELECT * FROM users WHERE accountnumber=?");
  prep_stmt->setString(1, accountNumber);
  prep_stmt->execute();
  res = prep_stmt->getResultSet();
  if (res->next()) {
      //The account number exists,ask for pin
        string username=res->getString("username");
        string pin=res->getString("pin");
        int balance=res->getInt("balance");
        string account_number=res->getString("accountnumber");
        bankAmount=balance;
        askPIN(username,pin,balance,account_number);
  }else{
        cout<<"Invalid account number.Try again.\n";
       // cout<<"Enter the account number."<<endl;
        //checkAccountNumber(amount);
  }
  delete prep_stmt;
  delete res;
  delete con;

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
}
}
void askPIN(string username,string pin,int balance,string account_number){
    string enteredPin;
   int index=3;
   do{
        cout<<"Enter PIN("<<index<<" times remaining):";
        cin>>enteredPin;
        if(enteredPin==pin){
            cout<<"Welcome, "<<username<<endl;
            showMenu(username,pin,balance,account_number);
            break;
        }else{
         cout<<"Incorrent pin.\n"<<"Try again later"<<endl;
        }
        --index;
   }while(index != 0);

}
void showMenu(string username,string pin,int balance,string account_number){
        showOptions(account_number);
}
void showOptions(string account_number){
    int option;
    cout<<"1---->Check balance"<<endl;
    cout<<"2---->Withdraw"<<endl;
    cout<<"3---->Deposit"<<endl;
    cout<<"4---->No other transaction"<<endl;
    cin>>option;
   if((option<=4)&&(option>=1)){
    switch(option){
            case 1:
               showBalance(account_number);
            break;
            case 2:
                withdraw(account_number);
            break;
            case 3:
                deposit(account_number);
            break;
            case 4:
            	cout<<"Thank you for using our services!"<<endl<<"Goodbye :)"<<endl;
            	break;
        }
        }
        else{
        cout<<"Invalid option. Try again"<<endl;

       }
   }

void showBalance(string accountnumber){
      cout<<"Your balance:\t"<<bankAmount<<endl;
     showOptions(accountnumber);
}
void withdraw(string accountnumber){
    int amount;
    cout<<"How much do you wish to withdraw?\t"<<endl;
    cin>>amount;
    if(amount>bankAmount){
        cout<<"You have insufficient funds."<<endl;
    }else{
        int newBalance=bankAmount-amount;
        bankAmount=newBalance;
    try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::PreparedStatement *prep_stmt;

  driver = get_driver_instance();
  con = driver->connect("localhost/bank", "root", "");
  con->setSchema("bank");

  prep_stmt = con->prepareStatement("UPDATE users SET balance=? WHERE accountnumber=?");
  prep_stmt->setInt(1, newBalance);
  prep_stmt->setString(2,accountnumber);
  bool status=prep_stmt->execute();
  if (status==false) {
        cout<<"Successfully withdrew "<<amount<<endl;
        cout<<"New balance is Ksh."<<bankAmount<<endl;
  }else{
        cout<<"An error occurred.Try again later"<<endl;
  }
  delete prep_stmt;
  delete con;

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
}
    }
showOptions(accountnumber);
}
void deposit(string accountnumber){
    int amount;
    cout<<"How much do you wish to deposit?\t"<<endl;
    cin>>amount;
    int newBalance=bankAmount+amount;
    bankAmount=newBalance;
    try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::PreparedStatement *prep_stmt;

  driver = get_driver_instance();
  con = driver->connect("localhost/bank", "root", "");
  con->setSchema("bank");

  prep_stmt = con->prepareStatement("UPDATE users SET balance=? WHERE accountnumber=?");
  prep_stmt->setInt(1, newBalance);
  prep_stmt->setString(2,accountnumber);
  bool status=prep_stmt->execute();
  if (status==false){
        cout<<"Successfully deposited "<<amount<<endl;
        cout<<"Your new balance is Ksh."<<bankAmount<<endl;
  }else{
        cout<<"An error occurred.Try again later.\n";
  }
  delete prep_stmt;
  delete con;

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();

}
showOptions(accountnumber);
}
