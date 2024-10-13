#include "analyzer.hpp"
#include <fstream>
#include <Aspose.Cells.h>

#define str(fig) std::to_string(fig)

#pragma once
class DataStorage {
public:
	DataStorage();
	virtual void SaveToFile(TimeAnalyzer &timeanalyzer, const std::string &filepath) = 0;
};
class TextStorage : public DataStorage{
public:
	TextStorage();
	virtual void SaveToFile(TimeAnalyzer& timeanalyzer, const std::string& filepath) override;
};
class XLSStorage : public DataStorage {
public:
	XLSStorage();
	virtual void SaveToFile(TimeAnalyzer& timeanalyzer, const std::string& filepath) override;
};