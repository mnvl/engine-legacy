
#include <cppunit/extensions/HelperMacros.h>
#include <util/luna.h>

namespace
{

class A {
public:
	static char const CLASS_NAME[];

	A(lua_State *L)
	{
		name_ = lua_tostring(L, -1);
		lua_pop(L, 1);
	}

private:
	std::string name_;
};

char const A::CLASS_NAME[] = "A";

}

class test_luna: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_luna);
	CPPUNIT_TEST(test_class);
	CPPUNIT_TEST_SUITE_END();

	void test_class()
	{
		util::luna l;
		l.register_class<A>();
		l.execute_string(
			"a = A::new(\"bebe\")"
			"result = a.get_name()"
		);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_luna); 
