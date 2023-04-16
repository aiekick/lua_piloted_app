#include "FileDataProvider.h"

FileDataProvider::FileDataProvider()
	: NumericDataProvider<double>(), iVisitor()
{

}

FileDataProvider::~FileDataProvider()
{

}

void FileDataProvider::init()
{

}

void FileDataProvider::unit()
{

}

void FileDataProvider::visit()
{

}

void FileDataProvider::setFilepathName(const std::string& vFilePathName)
{
	m_FilePathName = vFilePathName;
}

void FileDataProvider::start_read_op(const double& vPeriod)
{

}

void FileDataProvider::stop_read_op()
{

}
