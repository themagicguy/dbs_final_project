/*
Group: 2;
Danny Lei
Shawn Yu
<name>
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <occi.h>
#include <iomanip>
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
int const menu();
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
int getInt(const char* prompt = nullptr);
void displayEmployee(Connection* conn, struct Employee* emp);
void insertEmployee(Connection* conn, struct Employee* emp);
void displayAllEmployees(Connection* conn);

int main(void) {
    // OCCI Variables
    Environment* env = nullptr;
    Connection* conn = nullptr;

    // User Variables
    string user = "dbs211_211k20";
    string pass = "36932186";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
        cout << "Connection is Successful!" << endl;

        

        bool flag = false;
        do {
            struct Employee emp{};
            int menuSelection = menu();
            if (menuSelection == 0) {
                cout << "Ending program... " << endl;
                flag = true;
            }
            else if (menuSelection == 1) {
                // Find Employee
                int employeeNum = getInt("Enter employee number: ");

                int tmp = findEmployee(conn, employeeNum, &emp);
                if (tmp) {
                    //call display function here
                    displayEmployee(conn, &emp);
                }
                else {
                    cout << "Employee " << employeeNum << " does not exist" << endl;
                }
            }
            else if (menuSelection == 2) {
                displayAllEmployees(conn);
            }
            else if (menuSelection == 3) {
                //add new employee
                cout << "--------------New Employee Information--------------" << endl;

                emp.employeeNumber = getInt("Employee Number: ");
                struct Employee check;
                //check if employee is already in system
                int exist = findEmployee(conn, emp.employeeNumber, &check);

                if (!exist) { 
                    cout << "Last Name: "; cin >> emp.lastName; cin.ignore();
                    cout << "First Name: "; cin >> emp.firstName; cin.ignore();
                    cout << "Extension: "; cin >> emp.extension; cin.ignore();
                    cout << "Email: "; cin >> emp.email; cin.ignore();
                    cout << "Office Code: 1" << endl;
                    cout << "Manager ID: 1002" << endl;
                    cout << "Job Title: "; cin >> emp.jobTitle; cin.ignore();
                    insertEmployee(conn, &emp);
                }
                else { cout << "This employee already exist" << endl; }
            }
            else if (menuSelection == 4) {
                cout << "function not yet available" << endl;
            }
            else if (menuSelection == 5) {
                cout << "function not yet available" << endl;
            }
        } while (!flag);

        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return 0;
}

// menu function
int const menu() {
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

int getInt(const char* prompt) {
    int input;
    char next;
    bool flag = false;

    if (prompt) { cout << prompt; }

    do {
        cin >> input;

        if (cin.fail()) {
            cerr << "Invalid option, try again" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << prompt;
        }
        else {
            next = cin.get();
            if (next != '\n') {
                cerr << "Invalid option, try again" << endl;
                cin.ignore(1000, '\n');
            }
            else { flag = true; }
        }
    } while (!flag);

    return input;
}

// findEmployee function
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
    int tmp = 0;

    try {

        Statement* stmt = conn->createStatement("SELECT * FROM dbs211_employees WHERE employeenumber = :1");
        stmt->setNumber(1, employeeNumber);
        ResultSet* rs = stmt->executeQuery();
        
        if (rs->next()) {
            emp->employeeNumber = rs->getInt(1);
            strcpy(emp->lastName, rs->getString(2).c_str());
            strcpy(emp->firstName, rs->getString(3).c_str());
            strcpy(emp->extension, rs->getString(4).c_str());
            string* email = new string(rs->getString(5));
            strcpy(emp->email, email->c_str()); 
            strcpy(emp->officecode, rs->getString(6).c_str());
            emp->reportsTo = rs->getInt(7);
            strcpy(emp->jobTitle, rs->getString(8).c_str());
            
            tmp = 1;
        }
        conn->terminateStatement(stmt);
        return tmp;
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void displayEmployee(Connection* conn, struct Employee* emp)
{
    cout << "-------------- Employee Information -------------" << endl
        << "Employee Number: " << emp->employeeNumber << endl
        << "Last Name: " << emp->lastName << endl
        << "First Name: " << emp->firstName << endl
        << "Extension: " << emp->extension << endl
        << "Email: " << emp->email << endl
        << "Office Code: " << emp->officecode << endl
        << "Manager ID :" << emp->reportsTo << endl
        << "Job Title: " << emp->jobTitle << endl;
        //cout << "Employee found!!" << endl;
        //displayEmployee(conn, emp); 
}

void insertEmployee(Connection* conn, struct Employee* emp) {
    try {
            Statement* stmt = conn->createStatement("INSERT INTO dbs211_employees VALUES (:1, :2, :3, :4, :5, :6, :7, : 8)");
            stmt->setInt(1, emp->employeeNumber);
            stmt->setString(2, emp->lastName);
            stmt->setString(3, emp->firstName);
            stmt->setString(4, emp->extension);
            stmt->setString(5, emp->email);
            stmt->setString(6, "1");
            stmt->setInt(7, 1002);
            stmt->setString(8, emp->jobTitle);
            stmt->executeUpdate();
            conn->commit();
            conn->terminateStatement(stmt);
            cout << "\nThe new employee is added successfully." << endl;
      
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

}

void displayAllEmployees(Connection* conn) {

    try {

        Statement* stmt = conn->createStatement();

        ResultSet* rs = stmt->executeQuery("SELECT e.employeenumber, e.firstname || ' ' || e.lastname AS empname, e.email, o.phone, e.extension, m.firstname || ' ' ||  m.lastname AS manname FROM dbs211_employees e JOIN dbs211_offices o ON e.officecode = o.officecode LEFT JOIN dbs211_employees m ON m.employeenumber = e.reportsto ORDER BY e.employeenumber");

        cout << "------    ---------------    ---------------------------------    ----------------    ---------    -----------------" << endl;
        cout << left << setw(6) << "ID" << "    " << setw(17) << "Employee Name" << "  " << setw(33) << "Email" << "    " << setw(16) << "Phone" << "    " << setw(9) << "Extension" << "    " << setw(17) << "Manager Name" << endl;
        cout << "------    ---------------    ---------------------------------    ----------------    ---------    -----------------" << endl;

        while (rs->next()) {

            int empID = rs->getInt(1);
            string* empname = new string(rs->getString(2));
            string* email = new string(rs->getString(3));
            string* phone = new string(rs->getString(4));
            string* extension = new string(rs->getString(5));
            string* manname = new string(rs->getString(6));

            cout << left << setw(6) << empID << "    " << setw(17) << *empname << "  " << setw(33) << *email << "    " << setw(16) << *phone << "    " << setw(9) << *extension << "    " << setw(17) << *manname << endl;
        }

        conn->terminateStatement(stmt);

    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
        cout << "There is no employees� information to be displayed" << endl;
    }
}
