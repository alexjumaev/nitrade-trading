#pragma once
#include "IAssetData.h"
#include "PriceData.h"
#include <memory>

namespace Nitrade {
	class AssetData :
		public IAssetData
	{
	private: 
		//a vector is about 5-10% slower than array in backtesting
		//so keep as array for performance reasons
		std::unique_ptr<std::unique_ptr<IPriceData>[]> _priceData;
		int _size{ 0 };

	public:
		AssetData() = default;
		AssetData(int size);
		virtual ~AssetData() = default;

		int size() { return _size; }
		void set(int index, std::string name, int lookback, int barSize);

		IPriceData* operator [](int pos) { return _priceData[pos].get(); }
	};
}

