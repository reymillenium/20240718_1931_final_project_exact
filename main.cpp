/*
* Your Name
* Final Project
*/
#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <numeric>
#include <regex>

using namespace std;

struct Employee {
	string firstname = "";
	string lastname = "";
	int empNum = 0;
	double regRate = 0.0;
	double otRate = 0.0;

	Employee() = default;
};

struct Payroll {
	int empNumber;
	double hoursWorked = 0.0;
	double regHours = 0.0;
	double otHours = 0.0;
	double regRate = 0.0;
	double otRate = 0.0;
	double regPay = 0.0;
	double otPay = 0.0;
	double totalPay = 0.0;
	double fica = 0.0;
	double socSec = 0.0;
	double totDeductions = 0.0;
	double netPay = 0.0;

	Payroll() = default;

};

struct PayrollTotals {
	double regHours = 0.0;
	double otHours = 0.0;
	double regRate = 0.0;
	double otRate = 0.0;
	double regPay = 0.0;;
	double otPay = 0.0;;
	double totalPay = 0.0;;
	double fica = 0.0;;
	double socSec = 0.0;;
	double totDeductions = 0.0;;
	double netPay = 0.0;

	PayrollTotals() = default;

};

const int MAX_EMPLOYEES = 2;
const int MAX_REG_HOURS = 40;
const int MAX_HOURS_WORKED = 50;
const double MIN_HOURLY_RATE = 10.00;
const double MAX_HOURLY_RATE = 30.00;
const double FICA_PCT = .20; //20 8 * 0.01
const double SS_MED_PCT = .0765;
const double OT_MULT = 1.5;
const int MAX_TOTALS = 2;
const int SENTINEL = -999;

int getInteger(string, int, int);
int getInteger(string);
string trim(string);
bool isNumber(string);
bool isIntReg(string);
double getFloat(string);
bool isFloatReg(string);

void displayMenu();
char getSelection();
struct Employee getEmployeeInfo();
vector <Payroll> inputHours(vector <Employee>);
vector<PayrollTotals> initializeTotals();
vector<Payroll>calcPayroll(vector<Employee>vEmployees, const vector<Payroll>&);
vector<PayrollTotals>calcPayrollTotals(vector < Payroll>);
struct PayrollTotals calcTotals(const vector<Payroll>&);
PayrollTotals calcAverages(struct PayrollTotals&);
int getEmpNumber();

enum PayEnum { TOTALS, AVERAGES }; // TOTALS = 0, AVERAGES = 1

int main() {

	vector<Employee> vEmployees;
	struct Employee sOneEmployee;
	vector<Payroll> vPayroll;
	vector<PayrollTotals>vPayrollTotals;
	bool processMenu = true;
	char selection = ' ';
	int dayOfWeek;
	int empNum = 0;

	//To access employees use employees[index].field name
	// Example employees[1].lastname = "Jones";
	//To access pay use employees[index].field name
	// Example pay[1].regHours = 40;

	//display menu in a while loop using following
	//use switch to error check and call functions

	vPayrollTotals = initializeTotals();

	do {
		displayMenu();
		selection = getSelection();
		cout << "You chose " << selection << endl;
		processMenu = true;

		switch (selection) {
		case '1':
			for (int i = 0; i < MAX_EMPLOYEES; i++) {
				sOneEmployee = getEmployeeInfo();
				vEmployees.push_back(sOneEmployee);
			}

			// input employees
			break;
		case '2':
			vPayroll = inputHours(vEmployees);
			//input hours
			break;
		case '3':
			// calc payroll
			vPayroll = calcPayroll(vEmployees, vPayroll);
			break;
		case '4':
			// calc totals
			break;
		case '5':
			// print payroll
			break;
		case 'X':
			processMenu = false;
			break;
		} // switch

		//display menu - function
		//get menu selection - function
		//process menu selection using a switch
		//exit when x is pressed
		//switch(choice) {
		//case 1: etc
		//remember that you will need to set processMenu to false in order to exit the program
	} while (processMenu);

	cout << "Press enter to exit ";
	cin.get();

	return 0;
}

string trim(string str)
{
	/*
	* ^: Anchors the pattern to the start of the string.
	\s+: Matches one or more whitespace characters.
	|: Represents the logical OR operator, which allows either the pattern before or after it to match.
	\s+$: Matches one or more whitespace characters followed by the end of the string.
	When put together, this regex pattern matches:

	One or more whitespace characters at the beginning of the string (^\s+).
	OR
	One or more whitespace characters at the end of the string (\s+$).
	*/
	regex pattern(R"(^\s+|\s+$)"); //pattern to match spaces, tabs, and new line characters

	//replace white spaces with ""
	return regex_replace(str, pattern, "");
}

int getInteger(string prompt)
{
	/*
	* This function will get a string input for a number and will call the function isNumber to make sure the user typed in only digits.
	* If the function isNumber returns false the user will be notified and asked to reinput.
	*/
	int number = 0;
	string strNumber = "";
	bool isANumber = false;


	do {
		cout << prompt;
		getline(cin, strNumber);
		strNumber = trim(strNumber);
		isANumber = isIntReg(strNumber);

		if (!isANumber) {

			cout << "\aYou must type in an integer  - Reenter " << endl;
		}

	} while (!isANumber); // isANumber = false

	//we have a string number to convert to an integer
	number = stoi(strNumber);

	return number;
} //getInteger

int getInteger(string prompt, int lowest, int highest)
{
	/*
	* This function will get a string input for a number and will call the function isNumber to make sure the user typed in only digits.
	* If the function isNumber returns false the user will be notified and asked to reinput.
	* In addition to the overloaded plain getInteger, this function will also check for low and high constraints
	*/
	int number = 0;
	bool needNumber = true;

	prompt += to_string(lowest) + " and " + to_string(highest) + " ";
	do {
		number = getInteger(prompt);

		if (number == SENTINEL) {
			return (number);
		}

		needNumber = ((number < lowest) || (number > highest)); //check if in bounds
		if (needNumber) {
			cout << "\a Invalid number - please enter" << endl;
		}
	} while (needNumber);


	return number;
} //getInteger

bool isNumber(string num) {

	int length = num.length();

	for (int i = 0; i < length; i++) {

		/*
		* This function will check all characters of the string to make sure they are all digits.
		* The function will assume leading spaces have been removed but will flag if a second space is input.
		* If the first character is a '-' or '+' which are not a digit it will ignore to allow minus and plus signs.
		* They are put in an switch so a better comment can be made.
		*/

		if (i == 0) { // only check if first character

			switch (num[i]) {
			case '-': //negative number
			case '+': //positive number
				continue;

			} //switch

		}
		//check each character to make sure it is a digit
		if (!isdigit(num[i])) {

			return false; //if a non number is in string it is not a number

		} //if
		else {
			// this else is not needed but put in for testing purposes as should never
			// get here
			//put in message here
			//cout << num[i] << endl;

		} //else

	} //for

	return true; //if we get here then every character is a digit

}  //isnumber

bool isIntReg(string num)
{
	/*
	*
^: Anchors the regular expression at the beginning of the string.

[+|-]?: Specifies an optional sign character. The square brackets denote a character class, and + and - are included as possible characters. The ? indicates that the preceding character class is optional, meaning there may be zero or one occurrence of either + or -.

[0-9]+: Matches one or more digits. The character class [0-9] represents any digit from 0 to 9, and the + quantifier specifies that there must be at least one digit.

$: Anchors the regular expression at the end of the string.
	*/

	regex intExpression("^[+|-]?[0-9]+$");


	/*if (regex_match(num, intExpression)) {

		cout << num + " Is an integer" << endl;
	}

	else {
		cout << num + " Is not integer " << endl;
	}*/

	return (regex_match(num, intExpression));
}

double getFloat(string prompt)
{
	double dNumber = 0.0;
	string strNumber = "";
	bool isANumber = false;

	do {
		cout << prompt;
		getline(cin, strNumber);
		strNumber = trim(strNumber);
		isANumber = isFloatReg(strNumber);

		if (!isANumber) {

			cout << "\aYou must type in an valid float  - Reenter " << endl;
		}

	} while (!isANumber); // isANumber = false

	//we have a string number to convert to an double
	dNumber = stod(strNumber);

	return dNumber;
}


bool isFloatReg(string num)
{
	/*
	^[+-]?: Anchors the regular expression at the beginning of the string. It allows an optional sign character (+ or -).

	([0-9]+([.][0-9]*)?|[.][0-9]+): This part contains two alternatives separated by |.

	[0-9]+([.][0-9]*)?: Matches an integer part followed by an optional decimal part. Breaking it down:

	[0-9]+: Matches one or more digits for the integer part.
	([.][0-9]*)?: Optionally matches a decimal part consisting of a dot (.) followed by zero or more digits.
	|[.][0-9]+: Alternatively, matches a decimal part that starts with a dot (.) followed by one or more digits.

	$: Anchors the regular expression at the end of the string.
	*/

	regex floatExpression("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");

	return (regex_match(num, floatExpression));
}

void displayMenu() {


	cout << "Press 1 to input employees";
	cout << "\nPress 2 to input hours ";
	cout << "\nPress 3 to calculate payroll ";
	cout << "\nPress 4 to calculate totals ";
	cout << "\nPress 5 to print payroll ";
	cout << "\nPress X to exit ";
	cout << "\nEnter Selection ";

} // dispaly Menu

char getSelection() {

	char choice = ' ';
	bool needInput = true;

	do {
		cin >> choice;
		cin.ignore(INT_MAX, '\n');
		choice = toupper(choice);
		switch (choice) {
			//finish
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case 'X':
			needInput = false;
			break;

		default:
			cout << "You have entered invalid input ";
			displayMenu();

		} // switch
	} while (needInput);

	return choice;

}

struct Employee getEmployeeInfo()
{
	/*
	* �	User input
	o	string firstname;
	o	string lastname;
	o	int empNum;
	o	double regular rate
	o	double ot rate

	*/
	struct Employee sOneEmployee;

	sOneEmployee.empNum = getEmpNumber();
	cout << "What is employee " << sOneEmployee.empNum << "'s firstname ? ";
	getline(cin, sOneEmployee.firstname);
	cout << "What is employee " << sOneEmployee.empNum << "'s firstname ? ";
	getline(cin, sOneEmployee.lastname);
	sOneEmployee.regRate = getFloat("What is employee's regular rate? ");
	sOneEmployee.otRate = (sOneEmployee.regRate * OT_MULT);

	return sOneEmployee;
}

vector<Payroll> inputHours(vector<Employee>vEmployees)
{
	vector<Payroll>vPayroll(MAX_EMPLOYEES);

	for (int i = 0; i < vEmployees.size(); i++) {
		cout << "How many hours did " << vEmployees[i].firstname << " " << vEmployees[i].lastname << " work? ";
		vPayroll[i].hoursWorked = getInteger(" ", 0, MAX_HOURS_WORKED);
		//calc OT hours

	}
	return vPayroll;
}

vector<PayrollTotals> initializeTotals() {

	vector<PayrollTotals> vPayrollTotals;
	struct PayrollTotals sOneTotal;

	for (int i = 0; i < MAX_TOTALS; i++) {

		sOneTotal.regHours = 0;
		sOneTotal.otHours = 0;
		// do for rest

		vPayrollTotals.push_back(sOneTotal);
	} // for

	return vPayrollTotals;


} // initialize totals

vector<Payroll> calcPayroll(vector<Employee>vEmployees, const vector<Payroll>& vCurrPayroll)
{
	/*
	* struct Payroll {
		*int empNumber;
		*double hoursWorked;
		*double regHours;
		*double otHours;
		double regRate;
		double otRate;
		double regPay;
		double otPay;
		double totalPay;
		double fica;
		double socSec;
		double totDeductions;
		double netPay;

	};
	*/
	vector<Payroll>vPayroll = vCurrPayroll;
	int numEmployees = vEmployees.size();

	for (int i = 0; i < numEmployees; i++) {
		vPayroll[i].regPay = (vPayroll[i].regRate * vPayroll[i].regHours);
		//same ot etc
	}


	return vPayroll;
}

vector<PayrollTotals> calcPayrollTotals(vector<Payroll>vPayroll)
{
	vector<PayrollTotals>vPayTotals;
	struct PayrollTotals sTotPayroll;


	//calc totals and push_back
	sTotPayroll = calcTotals(vPayroll);
	vPayTotals[TOTALS].regPay = sTotPayroll.regPay;
	//calc averages
	sTotPayroll = calcAverages(sTotPayroll);
	vPayTotals[AVERAGES].regPay = sTotPayroll.regPay;

	return vPayTotals;
}

struct PayrollTotals calcTotals(const vector<Payroll>& vPayroll)
{
	struct PayrollTotals sTotPayroll;

	for (int i = 0; i < vPayroll.size(); i++) {
		sTotPayroll.regPay += vPayroll[i].regPay;
		//do rest
	}

	return sTotPayroll;
}

PayrollTotals calcAverages(struct PayrollTotals& sPayTot)
{
	struct PayrollTotals sTotPayroll;

	sTotPayroll.regPay = (sPayTot.regPay / MAX_EMPLOYEES);


	return sTotPayroll;

}


int getEmpNumber()
{
	static int empNum = 1000;
	empNum++;

	return empNum;
}


