#pragma once

#include <iostream>
#include "PriceData.h"
#include "Structs.h"
#include "SeriesBuffer.h"

namespace Nitrade {

	//interface for mocking
	class IPriceData
	{

	public:
		IPriceData() {}
		virtual ~IPriceData() {}

		virtual bool updateCurrentBarFromBar(Nitrade::Bar* newInfo) = 0;
		virtual bool updateCurrentBarFromTick(float quote, bool isBid) = 0;

		virtual Bar* operator [](int barOffset) = 0;
	};


	class PriceData : 
		public IPriceData
	{
	private:				
		int _barSize{0}; //the size of the bar in minutes

		//the array (as a series) of recent price data
		Utils::ISeriesBuffer<Bar*>* _pLookbackBars{nullptr};

	public:
		PriceData(int lookBack, int barSize);
		PriceData(int lookBack, int barSize, Utils::ISeriesBuffer<Bar*>* mockSeriesBuffer);
		virtual ~PriceData();

		bool updateCurrentBarFromBar(Nitrade::Bar* newInfo); //used update current bar from minute bar
		bool updateCurrentBarFromTick(float quote, bool isBid); //used to update current bar on a new tick 

		Bar* operator [](int barOffset) { return _pLookbackBars->get(barOffset); }

	};
}
