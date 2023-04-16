#pragma once

#include <string>
#include <thread>

#include <abstract/NumericDataProvider.h>
#include <interfaces/iVisitor.h>

class FileDataProvider : public NumericDataProvider<double>, public iVisitor
{
private:
	std::string m_FilePathName;
	std::thread m_Thread;

public:
	FileDataProvider();
	virtual ~FileDataProvider();
	
	void init();
	void unit();

	void visit() override;

	void setFilepathName(const std::string& vFilePathName);
	void start_read_op(const double& vPeriod);
	void stop_read_op();
};
