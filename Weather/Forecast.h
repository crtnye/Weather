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
	double humidity;
	string description;
};

class Forecast
{
public:
	Forecast();
	~Forecast();
	void generateForecast();

private:
	char zipcode[6];
	vector<ForecastAttributes> forecastList;

	bool initializeForecast();
	void populateAllForecasts(bool&);
	void getAndExecuteRequest();
	void displayForecastAttributes(int);
	void fiveDayForecast();
	void oneDayForecast();
	void tempLowAndHigh();
	void mostHumid();
	void newZipcode();
};



