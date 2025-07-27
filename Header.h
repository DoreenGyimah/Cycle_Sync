#pragma once
//valerie
struct Login {
	char mem;       // Y/N for membership check
	char user[50];  // username or email
	char pass[20];  // password
	char confirmPass[20];  // confirmation password
};

//doreen
struct Calendar
{
	char month[25];
	int monthNum;
	int day;
	int year;
	int displayDays;
	int missingDates[31]; //added to store missing dates for each month
};

struct CycleData {

	int S_day, S_year, S_month;
	int E_day, E_year, E_month;
};

typedef struct {
	int day, month, year;
	int cycle_length;
} PeriodEntry;

