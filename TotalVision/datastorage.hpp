#include "analyzer.hpp"
#include <fstream>
#include <Aspose.Cells.h>
#include <set>

#define _STR(fig) std::to_string(fig)

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
class BinaryStorage : public DataStorage {
public:
	BinaryStorage();
	virtual void SaveToFile(TimeAnalyzer& timeanalyzer, const std::string& filepath) override;
};
class StorageReader {
public:
	StorageReader();
	virtual std::vector<TimeAnalyzer::AnalyzedProcess> ReadStorage(const std::string& filepath) = 0;
};
class BinaryReader : public StorageReader {
public:
	BinaryReader();
	virtual std::vector<TimeAnalyzer::AnalyzedProcess> ReadStorage(const std::string& filepath) override;
};