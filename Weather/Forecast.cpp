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

bool Forecast::initializeForecast()
{
	bool isSuccessful = true;
	populateAllForecasts(isSuccessful);
	return isSuccessful;
}

//void Forecast::setZipcode(char userZip[])
//{
//	strcpy_s(zipcode, userZip);
//}

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

	if (int(response.status_code()) == 404)
	{
		isSuccessful = false;
	}
	else 
	{
		web::json::value forecastListJson = forecastJson.at(U("list"));		web::json::value cityInfoJson = forecastJson.at(U("city"));

		if (forecastListJson.is_array())
		{
			for (int i = 0; i < forecastListJson.size(); i++)
			{
				web::json::value forecastSegment = forecastListJson[i];
				web::json::value forecastSegmentMain = forecastSegment.at(U("main"));
				web::json::value forecastListJson = forecastSegmentMain.at(U("list"));

				date = conversions::to_utf8string(forecastSegmentMain.at(U("temp")).as_string());
				city = conversions::to_utf8string(cityInfoJson.at(U("name")).as_string());

				ForecastAttributes current;

				//populate the following attriubutes of current
				//double temperature;
				//double temp_min;
				//double temp_max;
				//int humidity;
				//string description; //weather...description
				//double rain; //rain../if not empty then "3h"

				forecastList.push_back(current);

				//SAMPLE CODE BELOW
				//cout << tempJson.as_double() << endl;
				//web::json::value weatherJson = forecastSegment.at(U("weather"))[0];
				//web::json::value mainWeatherJson = weatherJson.at(U("main"));
				//cout << conversions::to_utf8string(mainWeatherJson.as_string()) << endl;
			}
		}
	}
}


void Forecast::getAndValidateZipcode()
{
	char userZip[6];
	bool isZipcodeValid;

	cout << "Enter your zipcode: ";
	cin >> userZip;

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
}

void Forecast::fiveDayForecast()
{

}

void Forecast::oneDayForecast()
{

}

void Forecast::tempLowAndHigh()
{

}

void Forecast::mostHumid()
{

}

void Forecast::newZipcode()
{
	getAndValidateZipcode();
	getAndExecuteRequest();
}

void Forecast::getAndExecuteRequest()
{
	char input;

	cout << "What would you like to do?" << endl;
	cout << "(A) Show the forecast for the next five days.\n"
		<< "(B) Show the forecast for one specific day.\n"
		<< "(C) Show the high and low temperatures of the next five days.\n"
		<< "(D) Show the most humid of the next five days.\n"
		<< "(E) Enter a new zipcode.\n";
	cin >> input;

	switch (input) 
	{
	case 'A':
		fiveDayForecast();
		break;
	case 'B':
		oneDayForecast();
		break;
	case 'C':
		tempLowAndHigh();
		break;
	case 'D':
		mostHumid();
		break;
	case 'E':
		newZipcode();
		break;
	}

}


