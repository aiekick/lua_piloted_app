#pragma once

#include <string>
#include <unordered_map>

#include <abstract/NumericDataConsumer.h>
#include <abstract/NumericDataProvider.h>

class DataConnector
{
private:
	NumericDataConsumer<double> m_Consumer;
	std::vector<NumericDataProvider<double>> m_Providers;

public:
	void init();
	void unit();

	//void connect();
};

