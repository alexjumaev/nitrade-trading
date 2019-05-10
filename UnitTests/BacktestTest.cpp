#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "NitradeLib.h"
#include "Controller.cc"
#include "PriceData.cc"
#include "MockAsset.cc"

using namespace std;
using namespace Nitrade;
using namespace testing;


class BacktestTest : public ::testing::Test
{
protected:
	BackTest* bt{};
	string _assetName{ "EURUSD" };

	Bar* bars{};
	Bar* bars2{};


	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		if (bars != nullptr)
			delete[] bars;
		if (bars2 != nullptr)
			delete[] bars2;
	}


	Bar* getBars(int size, bool corrupt)
	{
		long long startTime = 1557210984000000000ll;
		if (corrupt)
			startTime = -1000ll;

		Bar* bars = new Bar[size];
		for (int i = 0; i < size; i++)
			//adds 10 minutes onto the timestamp on each iteration
			bars[i] = Bar{ startTime + i * 600000000000, 1.0001f,1.0002f, 1.0003f,1.0001f,1.0005f,1.0006f,1.0007f,1.0005f, 5 };

		return bars;
	}

	
};

TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenPassedNullAndEmptyString) {

	ASSERT_THROW(bt->Run(NULL, ""), std::invalid_argument);
}

TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenPassedNullAndString) {

	ASSERT_THROW(bt->Run(NULL, _assetName), std::invalid_argument);
}


TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenPassedControllerWithoutBinaryReader) {

	MockController controller;

	ASSERT_THROW(bt->Run(&controller, _assetName), std::invalid_argument);
}

TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenBinaryFileNotOpened) {


	MockController controller;
	EXPECT_CALL(controller, hasBinaryReader(_assetName)).WillOnce(Return(true));
	EXPECT_CALL(controller, openFile(_assetName)).WillOnce(Return(false));
	ASSERT_THROW(bt->Run(&controller, _assetName), std::invalid_argument);
}


TEST_F(BacktestTest, RunClosesFileOnComplete) {


	MockController controller;
	MockAsset asset;

	MockPriceData pd;
	IPriceData** pdArray = new IPriceData * [50];
	pdArray[0] = &pd;
	pdArray[1] = nullptr;

	EXPECT_CALL(controller, getAsset(_assetName)).WillOnce(Return(&asset));
	EXPECT_CALL(controller, hasBinaryReader(_assetName)).WillOnce(Return(true));
	EXPECT_CALL(controller, openFile(_assetName)).WillOnce(Return(true));
	
	//succesfully opened the binary file

	//Loop through zero chunks of mocked data
	EXPECT_CALL(controller, eof(_assetName))
		.WillOnce(Return(true));

	EXPECT_CALL(controller, closeFile(_assetName))
		.Times(Exactly(1));

	//mock a PriceData object
	EXPECT_CALL(asset, getAllPriceData()).WillOnce(Return(pdArray));

	bt->Run(&controller, _assetName);

}

TEST_F(BacktestTest, RunProcessesMockBarDataCheckUpdateBarCount) {

	//create some mock price data to return from getAssetData
	MockPriceData pd;

	IPriceData** pdArray = new IPriceData * [50];
	pdArray[0] = &pd;
	pdArray[1] = nullptr;

	MockController controller;
	MockAsset asset;

	EXPECT_CALL(controller, getAsset(_assetName)).WillOnce(Return(&asset));

	//mock that a binaryReader exists
	EXPECT_CALL(controller, hasBinaryReader(_assetName)).WillOnce(Return(true));

	//mock that the binary file opens successfully
	EXPECT_CALL(controller, openFile(_assetName)).WillOnce(Return(true));

	//mock a PriceData object
	EXPECT_CALL(asset, getAllPriceData()).WillOnce(Return(pdArray));

	//create some mock data
	int size = 10;
	bars = getBars(10, false);
	bars2 = getBars(10, false);

	//start chunk and end chunk will be the same so exit internal loop immediately
	//becuase we have no mocked data
	char* start = (char*)&bars[0];
	char* end = start + sizeof(Bar) * size;
	char* start2 = (char*)&bars2[0];
	char* end2 = start2 + sizeof(Bar) * size;
	
	EXPECT_CALL(controller, getChunk(_assetName))
		.WillOnce(Return(start))
		.WillOnce(Return(start2));
	EXPECT_CALL(controller, endChunk(_assetName))
		.WillOnce(Return(end))
		.WillOnce(Return(end2));
	
	//Loop through 2 chunks of mocked data
	EXPECT_CALL(controller, eof(_assetName))
		.WillOnce(Return(false))
		.WillOnce(Return(false))
		.WillOnce(Return(true));

	
	EXPECT_CALL(pd, updateCurrentBarFromBar(_))
		.Times(Exactly(20))
		.WillRepeatedly(Return(false));

	bt->Run(&controller, _assetName);

}

TEST_F(BacktestTest, RunThrowBecauseBarDataInvalid) {

	//create some mock price data to return from getAssetData
	MockPriceData pd;

	IPriceData** pdArray = new IPriceData * [50];
	pdArray[0] = &pd;
	pdArray[1] = nullptr;

	MockController controller;

	MockAsset asset;

	EXPECT_CALL(controller, getAsset(_assetName)).WillOnce(Return(&asset));

	//mock that a binaryReader exists
	EXPECT_CALL(controller, hasBinaryReader(_assetName)).WillOnce(Return(true));

	//mock that the binary file opens successfully
	EXPECT_CALL(controller, openFile(_assetName)).WillOnce(Return(true));

	//mock a PriceData object
	EXPECT_CALL(asset, getAllPriceData()).WillOnce(Return(pdArray));


	//create some mock data
	int size = 10;
	bars = getBars(size, false);
	bars2 = getBars(size, true);


	//start chunk and end chunk will be the same so exit internal loop immediately
	//becuase we have no mocked data
	char* start = (char*)& bars[0];
	char* end = start + sizeof(Bar) * size;
	char* start2 = (char*)& bars2[0];
	char* end2 = start2 + sizeof(Bar) * size;

	EXPECT_CALL(controller, getChunk(_assetName))
		.WillOnce(Return(start))
		.WillOnce(Return(start2));
	EXPECT_CALL(controller, endChunk(_assetName))
		.WillOnce(Return(end))
		.WillOnce(Return(end2));

	//Loop through 2 chunks of mocked data
	EXPECT_CALL(controller, eof(_assetName))
		.WillOnce(Return(false))
		.WillOnce(Return(false));

	EXPECT_THROW(bt->Run(&controller, _assetName), std::invalid_argument);


}