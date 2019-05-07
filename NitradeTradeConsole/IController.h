#pragma once

#include <iostream>
#include <vector>
#include "BinaryChunkReader.h"
#include "Strategy.h"
#include "TradeManager.h"
#include "DataManager.h"
#include "IPriceData.h"

namespace Nitrade {

	//Controller is used for both back testing and live trading - although different sub classes will be used for TradeManager
	class IController
	{
	protected:
		BinaryChunkReader* _bReader = NULL;
		Strategy* _strategies = NULL;
		TradeManager* _tradeManager = NULL;
		DataManager* _dataManager = NULL;

	public:
		IController() {}
		virtual ~IController() {}

		//binary reader functions
		virtual bool hasBinaryReader() = 0;
		virtual bool openFile() = 0; //returns the size in bytes of the file
		virtual bool eof() = 0; //returns true if end of file
		virtual char* endChunk() = 0; //pointer to last byte of the chunk
		virtual char* getChunk() = 0;
		virtual void closeFile() = 0; //close the file

		//price data functions
		virtual std::vector<Nitrade::IPriceData*>* getAssetData(std::string assetName) = 0;

	};
}

