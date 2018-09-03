#include <iostream>
#include <string>
#include <vector>


using std::vector;
using std::string;

struct ForecastAttributes
{
	double temperature;
	double temp_min;
	double temp_max;
	int humidity;

	string description; //weather...description
	double rain; //rain../if not empty then "3h"
};

class Forecast
{
public:
	Forecast();
	~Forecast();
	void getAndValidateZipcode();

private:
	//inside main
	string date; //dt_txt
	string city; //list...name
	char zipcode[6];

	vector<ForecastAttributes> forecastList;

	bool initializeForecast();
	void populateAllForecasts(bool&);
	void getAndExecuteRequest();


	//function to take user input and determine action

	//function to show the forecast for all five days 
	void fiveDayForecast();

	//function to show the forecast for a specific day
	void oneDayForecast();

	//function to show the temp low/high of the week
	void tempLowAndHigh();

	//function to show the most humid day in the forecast
	void mostHumid();

	//function to show the forecast for a specific zipcode
	void newZipcode();
};



