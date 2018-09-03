#include "Forecast.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace std;
using namespace utility; // Common utilities like string conversions
using namespace web; // Common features like URIs.
using namespace web::http; // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace concurrency::streams;

Forecast::Forecast()
{
}

Forecast::~Forecast()
{
}

void Forecast::generateForecast()
{
	char userZip[6];
	bool isZipcodeValid;

	cout << "Enter your zipcode: ";
	cin.getline(userZip, 6);

	strcpy_s(zipcode, userZip);
	isZipcodeValid = initializeForecast();

	//data validation for the user entered zipcode
	while (isZipcodeValid == false)
	{
		cout << "I'm sorry, the data could not be accessed." << endl;
		cout << "Enter your zipcode: ";
		cin >> zipcode;
		cout << endl;

		isZipcodeValid = initializeForecast();
	}
	getAndExecuteRequest();
}

bool Forecast::initializeForecast()
{
	bool isSuccessful;
	populateAllForecasts(isSuccessful);
	return isSuccessful;
}

void Forecast::populateAllForecasts(bool &isSuccessful)
{
	http_client client(U("http://api.openweathermap.org"));
	uri_builder builder(U("/data/2.5/forecast"));
	builder.append_query(U("zip"), conversions::to_utf16string(zipcode));
	builder.append_query(U("appid"), U("3f733167008a688fb1be70bc9950a6d1"));
	builder.append_query(U("units"), U("imperial"));
	builder.append_query(U("mode"), U("json"));
	http_response response = client.request(methods::GET, builder.to_string()).get();
	web::json::value forecastJson = response.extract_json(true).get();

	if (int(response.status_code()) != 200 && int(response.status_code()) != 302)
	{
		isSuccessful = false;
	}
	else 
	{
		web::json::value forecastListJson = forecastJson.at(U("list"));
		isSuccessful = true;
		if (forecastListJson.is_array()) 
		{
			for (int i = 0; i < forecastListJson.size(); i++)
			{
				ForecastAttributes current;
				web::json::value forecastDayJson = forecastListJson[i];
				web::json::value mainJson = forecastDayJson.at(U("main"));

				web::json::value tempJson = mainJson.at(U("temp"));
				current.temperature = tempJson.as_double();

				web::json::value temp_minJson = mainJson.at(U("temp_min"));
				current.temp_min = temp_minJson.as_double();

				web::json::value temp_maxJson = mainJson.at(U("temp_max"));
				current.temp_max = temp_maxJson.as_double();

				web::json::value humidityJson = mainJson.at(U("humidity"));
				current.humidity = humidityJson.as_double();

				web::json::value weatherJson = forecastDayJson.at(U("weather"))[0];
				web::json::value descriptionJson = weatherJson.at(U("description"));
				current.description = conversions::to_utf8string(descriptionJson.as_string());

				forecastList.push_back(current);
			}
		}
	}
}

void Forecast::getAndExecuteRequest()
{
	char input;

	cout << "What would you like to do?" << endl;
	cout << "(A) Show the forecast for the next five days.\n"
		<< "(B) Show the forecast for one specific day.\n"
		<< "(C) Show the high and low temperatures of the next five days.\n"
		<< "(D) Show the highest humidity of the next five days.\n"
		<< "(E) Enter a new zipcode.\n"
		<< "(Q) To Quit. \n";
	cin >> input;

	switch (input)
	{
	case 'A':
		fiveDayForecast();
		getAndExecuteRequest();
		break;
	case 'B':
		oneDayForecast();
		getAndExecuteRequest();
		break;
	case 'C':
		tempLowAndHigh();
		getAndExecuteRequest();
		break;
	case 'D':
		mostHumid();
		getAndExecuteRequest();
		break;
	case 'E':
		newZipcode();
		getAndExecuteRequest();
		break;
	case 'Q':
		break;
	}
}

void Forecast::displayForecastAttributes(int index)
{
	cout << "Description: " << forecastList[index].description << endl;
	cout << "Current Temperature: " << forecastList[index].temperature << " F" << endl;
	cout << "Maximum Temperature: " << forecastList[index].temp_max << " F" << endl;
	cout << "Minimum Temperature: " << forecastList[index].temp_min << " F" << endl;
	cout << "Humidity: " << forecastList[index].humidity << endl << endl;
}

void Forecast::fiveDayForecast()
{
	for (int i = 0; i < forecastList.size(); i++) 
	{ 
		displayForecastAttributes(i); 
	}
}

void Forecast::oneDayForecast()
{
	int input;
	cout << "Which day's forecast would you like to see? (1 - 5): ";
	cin >> input;
	cout << endl;

	while (input < 0 || input > 5)
	{
		cout << "I'm sorry, please enter a number from 1 through 5: ";
		cin >> input;
		cout << endl;
	}

	int forecastsPerDay = forecastList.size() / 5;

	for (int i = forecastsPerDay * (input - 1); i < forecastsPerDay * input; i++)
	{ 
		displayForecastAttributes(i);
	}
}

void Forecast::tempLowAndHigh()
{
	double low = forecastList[0].temp_min;
	double high = forecastList[0].temp_max;

	for (int i = 1; i < forecastList.size(); i++)
	{
		if (forecastList[i].temp_min < low)
		{
			low = forecastList[i].temp_min;
		}

		if (forecastList[i].temp_max > high)
		{
			high = forecastList[i].temp_max;
		}
	}
	cout << "The lowest temperature of the week will be " << low
		<< " and the highest will be " << high << ".\n";
}

void Forecast::mostHumid()
{
	double high = forecastList[0].humidity;
	for (int i = 1; i < forecastList.size(); i++)
	{
		if (forecastList[i].humidity > high)
		{
			high = forecastList[i].humidity;
		}
	}
	cout << "The highest humidity over the next five days is " << high << ".\n";
}

void Forecast::newZipcode()
{
	generateForecast();
}