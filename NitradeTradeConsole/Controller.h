#pragma once
#include "IController.h"
#include "PriceData.h"
#include <vector>

namespace Nitrade {
	class Controller :
		public IController
	{
	public:
		Controller();
		virtual ~Controller();

		void setBinaryReader(BinaryChunkReader* bReader);

		//binary reader functions
		bool openFile(); //returns the size in bytes of the file
		bool eof(); //returns true if end of file
		char* endChunk(); //pointer to last byte of the chunk
		char* getChunk();
		void closeFile(); //close the file

		//price data functions
		std::vector<PriceData>* getAssetData(std::string assetName);

	private:
		void tryBinaryReader();
	};
}

