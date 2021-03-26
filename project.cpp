#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <occi.h>
#include <cstring>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee
{
    int  employeeNumber;
    char lastName[50];
    char firstName[50];
    char extension[10];
    char email[100];
    char officecode[10];
    int  reportsTo;
    char jobTitle[50];

};

//prototypes
int const menuu();
int findEmployee(Connection *conn, int employeeNumber, struct Employee* emp);
int getInt(string prompt = nullptr);
void displayEmployee(Connection* conn, struct Employee emp);


int main(void) {
    // OCCI Variables
    Environment* env = nullptr;
    Connection* conn = nullptr;

    // User Variables
    string user = "dbs211_211k39";
    string pass = "xxxxxxx";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    try{
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
        cout << "Connection is Successful!" << endl;

        bool flag = false;
        do {
            int menu = menuu();
            if (menu == 0) {
                cout << "Ending program... " << endl;
                flag = true;
            }
            else if (menu == 1) {
                // Find Employee
                struct Employee emp;
                int employeeNum = getInt("Enter employee number: ");

                int tmp = findEmployee(conn, employeeNum, &emp);
                if (tmp) {
                    //call display function here
                    displayEmployee(conn, emp);
                }
                else {
                    cout << "Employee " << employeeNum << " does not exist" << endl;
                }
            }
            else if (menu == 2) {
            cout << "function not yet available" << endl;   
            }
            else if (menu == 3) {
            cout << "function not yet available" << endl;   
            }
            else if (menu == 4) {
            cout << "function not yet available" << endl;
            }
            else if (menu == 5) {
            cout << "function not yet available" << endl;
            }
        } while (!flag); 

        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException & sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return 0;
}

// menu function
int const menuu()  {
    int tmp; 
    bool flag = false;

    cout << "********************* HR Menu *********************" << endl;
    cout << "1) Find Employee" << endl;
    cout << "2) Employees Report" << endl;
    cout << "3) Add Employee" << endl;
    cout << "4) Update Employee" << endl;
    cout << "5) Remove Employee" << endl;
    cout << "0) Exit" << endl;
    
    do {
        tmp = getInt("Enter an option (0-5): ");
        if (tmp > 5 || tmp < 0) 
        {
            cout << "Invalid option, try again" << endl; 
        }
        else { flag = true; }
    } while (!flag);

    return tmp;
}

int getInt(string prompt) {
    int input;
    char next;
    bool flag = false;

    if (prompt != "")
    {
        cout << prompt;
    }

     do {
        cin >> input;

        if (cin.fail()) {
            cerr << "Invalid option, try again" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << prompt;
        }
       /* else if (input < -1 || input > 6)
        {
            cerr << "Invalid option, try again" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << prompt;
        }*/
        else {
            next = cin.get();
            if (next != '\n') {
                cerr << "Invalid option, try again" << endl;
                cin.ignore(1000, '\n');
            }
            else { flag = true; }
        }
    } while(!flag);

    return input;
}

// findEmployee function
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
    int tmp = 0;

    try {
       
        Statement* stmt = conn->createStatement("SELECT e.employeenumber, e.lastname, e.firstname, e.extension, e.email, e.officecode, e.reportsTo, e.jobtitle FROM dbs211_employees e JOIN dbs211_offices o ON e.officecode = o.officecode WHERE e.employeenumber = :1");
        stmt->setNumber(1, employeeNumber);
        ResultSet* rs = stmt->executeQuery();

        if (rs->next()) {  
            emp->employeeNumber = rs->getInt(1);
            strcpy(emp->lastName, rs->getString(2).c_str());
            strcpy(emp->firstName, rs->getString(3).c_str());
            strcpy(emp->extension, rs->getString(4).c_str());
            strcpy(emp->email, rs->getString(5).c_str());
            strcpy(emp->officecode, rs->getString(6).c_str());
            emp->reportsTo = rs->getInt(7);
            strcpy(emp->jobTitle, rs->getString(8).c_str());
            
            tmp = 1;    
        }
        conn->terminateStatement(stmt);
        return tmp;
    }
    catch (SQLException & sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void displayEmployee(Connection* conn, struct Employee emp)
{
    cout << "please enter the employee number: "; 
    int empNumber = getInt();

    if (findEmployee(conn, empNumber, &emp) == 0)
    {
        cout << "Employee " << empNumber << "does not exist" << endl; 
    }
    else
    {
        cout << "-------------- Employee Information -------------" << endl
            << "Employee Number: " << emp.employeeNumber << endl
            << "Last Name: " << emp.lastName << endl
            << "First Name: " << emp.firstName << endl
            << "Extension: " << emp.extension << endl
            << "Email: " << emp.email << endl
            << "Office Code: " << emp.officecode << endl
            << "Manager ID :" << emp.reportsTo << endl
            << "Job Title: " << emp.jobTitle << endl;
        //cout << "Employee found!!" << endl;
        //displayEmployee(conn, emp); 
    }

}
