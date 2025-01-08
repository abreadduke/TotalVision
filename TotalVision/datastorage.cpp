#include "datastorage.hpp"

using namespace Aspose::Cells;

DataStorage::DataStorage() {}
TextStorage::TextStorage() : DataStorage::DataStorage() {};
void TextStorage::SaveToFile(TimeAnalyzer& timeanalyzer, const std::string& filepath) {
	auto analyzedprocesses = timeanalyzer.GetAnalyzed();
	std::ofstream filestream(filepath);
	if (filestream.is_open()) {
		std::string data;
		for (auto analyzedprocess : analyzedprocesses) {
			data += _STR(analyzedprocess.first) + '\t' + analyzedprocess.second.processName +
				'\t' + _STR(analyzedprocess.second.processMemoryUsage) + '\t' + _STR(analyzedprocess.second.processCPUPersents) + '\n';
		}
		filestream.write(data.c_str(), data.length());
		filestream.close();
	}
}
XLSStorage::XLSStorage() {}
void XLSStorage::SaveToFile(TimeAnalyzer& timeanalyzer, const std::string& filepath) {
	auto analyzedprocesses = timeanalyzer.GetAnalyzed();
	Aspose::Cells::Startup();

	Workbook workbook(FileFormatType::Xlsx);
	Worksheet sheet = workbook.GetWorksheets().Get(0);
	Cells cells = sheet.GetCells();
	cells.Get(0, 0).PutValue(u"ID");
	cells.Get(0, 1).PutValue(u"Процесс");
	cells.Get(0, 2).PutValue(u"Использование памяти");
	cells.Get(0, 3).PutValue(u"Использование процессора");
	cells.SetColumnWidth(1, 150);
	cells.SetColumnWidth(2, 30);
	cells.SetColumnWidth(3, 30);
	int cellid = 1;
	for (auto analyzedprocess : analyzedprocesses) {
		//LPSTR wProcessName = NULL;
		//CharToOemA(analyzedprocess.processName.c_str(), wProcessName);
		cells.Get(cellid, 0).PutValue((int32_t)analyzedprocess.first);
		cells.Get(cellid, 1).PutValue(Aspose::Cells::U16String(analyzedprocess.second.processName.c_str()));
		cells.Get(cellid, 2).PutValue((int32_t)analyzedprocess.second.processMemoryUsage);
		cells.Get(cellid++, 3).PutValue((double)analyzedprocess.second.processCPUPersents);
	}

	int chartIndex = sheet.GetCharts().Add(ChartType::Column, 9, 9, 50, 60);
	Chart chart = sheet.GetCharts().Get(chartIndex);
	int processesLength = analyzedprocesses.size();
	chart.GetNSeries().Add(Aspose::Cells::U16String((std::string("D2:D") + std::to_string(processesLength)).c_str()), true);
	chart.GetNSeries().SetCategoryData(Aspose::Cells::U16String((std::string("A2:A") + std::to_string(processesLength)).c_str()));
	//Series aSeries = chart.GetNSeries().Get(0);
	//aSeries.SetName(u"=B1");
	chart.SetShowLegend(true);
	chart.GetTitle().SetText(u"Использование cpu");
	workbook.Save(Aspose::Cells::U16String(filepath.c_str()));
	Aspose::Cells::Cleanup();
}
void XLSStorage::SaveToFile(std::vector<TimeAnalyzer::AnalyzedProcess> analyzedProcs, const std::string& filepath)
{
	Aspose::Cells::Startup();

	Workbook workbook(FileFormatType::Xlsx);
	Worksheet sheet = workbook.GetWorksheets().Get(0);
	Cells cells = sheet.GetCells();
	cells.Get(0, 0).PutValue(u"Процесс");
	cells.Get(0, 1).PutValue(u"Использование памяти");
	cells.Get(0, 2).PutValue(u"Использование процессора");
	cells.SetColumnWidth(0, 150);
	cells.SetColumnWidth(1, 30);
	cells.SetColumnWidth(2, 30);
	int cellid = 1;
	for (auto analyzedprocess : analyzedProcs) {
		//LPSTR wProcessName = NULL;
		//CharToOemA(analyzedprocess.processName.c_str(), wProcessName);
		cells.Get(cellid, 0).PutValue(Aspose::Cells::U16String(analyzedprocess.processName.c_str()));
		cells.Get(cellid, 1).PutValue((int32_t)analyzedprocess.processMemoryUsage);
		cells.Get(cellid++, 2).PutValue((double)analyzedprocess.processCPUPersents);
	}

	int chartIndex = sheet.GetCharts().Add(ChartType::Column, 9, 9, 50, 60);
	Chart chart = sheet.GetCharts().Get(chartIndex);
	int processesLength = analyzedProcs.size();
	chart.GetNSeries().Add(Aspose::Cells::U16String((std::string("C2:C") + std::to_string(processesLength)).c_str()), true);
	chart.GetNSeries().SetCategoryData(Aspose::Cells::U16String((std::string("A2:A") + std::to_string(processesLength)).c_str()));
	//Series aSeries = chart.GetNSeries().Get(0);
	//aSeries.SetName(u"=B1");
	chart.SetShowLegend(true);
	chart.GetTitle().SetText(u"Использование cpu");
	workbook.Save(Aspose::Cells::U16String(filepath.c_str()));
	Aspose::Cells::Cleanup();
}
StorageReader::StorageReader() {}
BinaryStorage::BinaryStorage() : DataStorage::DataStorage() {}
void BinaryStorage::SaveToFile(TimeAnalyzer& timeanalyzer, const std::string& filepath) {
	if (&timeanalyzer == nullptr) throw std::exception("Attempt to give null analyzer");
	auto rawAnalyzedprocesses = timeanalyzer.GetAnalyzed();
	std::ofstream writeStream(filepath, std::ios::binary);
	if (writeStream.is_open()) {
		std::map<std::string, TimeAnalyzer::AnalyzedProcess> combinedProcs;
		for (auto process : rawAnalyzedprocesses) {
			if (combinedProcs.find(process.second.processName) == combinedProcs.end()) {
				combinedProcs[process.second.processName] = process.second;
			}
			else {
				combinedProcs[process.second.processName].processMemoryUsage += process.second.processMemoryUsage;
				combinedProcs[process.second.processName].processCPUPersents += process.second.processCPUPersents;
			}
		}
		for (auto process : combinedProcs) {
			size_t processNameLength = process.second.processName.length();
			char* pProcessName = (char*)process.first.c_str();
			writeStream.write((const char*)&processNameLength, sizeof(size_t));
			writeStream.write(pProcessName, processNameLength * sizeof(char));
			writeStream.write((const char *)&process.second.processMemoryUsage, sizeof(size_t));
			writeStream.write((const char *)&process.second.processCPUPersents, sizeof(double));
		}
		writeStream.close();
	}
}
BinaryReader::BinaryReader() : StorageReader::StorageReader() {}

std::vector<TimeAnalyzer::AnalyzedProcess> BinaryReader::ReadStorage(const std::string& filepath) {
	std::ifstream readStream(filepath, std::ios::binary);
	std::vector<TimeAnalyzer::AnalyzedProcess> redProcs;
	if (readStream.is_open()) {
		while (!readStream.eof())
		{
			size_t processNameLength = 0;
			size_t processMemoryUsage = 0;
			double processCPUPersents = 0;
			readStream.read((char*)&processNameLength, sizeof(size_t));
			char* pProcessName = new char[processNameLength+1];
			readStream.read(pProcessName, sizeof(char) * processNameLength);
			pProcessName[processNameLength] = '\0';
			readStream.read((char*)&processMemoryUsage, sizeof(size_t));
			readStream.read((char*)&processCPUPersents, sizeof(double));
			redProcs.push_back({std::string(pProcessName), processMemoryUsage, processCPUPersents});
			delete pProcessName;
			pProcessName = nullptr;
		}
		readStream.close();
		return redProcs;
	}
}