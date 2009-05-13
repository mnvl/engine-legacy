
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <util/logger.h>

int main(int argc, char* argv[])
{
	util::logger::instance().set_file_by_executable_name();

	xercesc::XMLPlatformUtils::Initialize();

	CppUnit::TestResult controller;

	CppUnit::TestResultCollector result;
	controller.addListener(&result);        

	CppUnit::BriefTestProgressListener progress;
	controller.addListener(&progress);      

	CppUnit::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
	runner.run(controller);

	CppUnit::CompilerOutputter outputter(&result, CppUnit::stdCOut());
	outputter.write();

	xercesc::XMLPlatformUtils::Terminate();

	return result.wasSuccessful() ? 0 : 1;
}
