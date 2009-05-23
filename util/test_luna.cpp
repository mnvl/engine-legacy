
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

	static int get_name(lua_State *L)
	{
		A *a = (A *) lua_touserdata(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, a->name_.c_str());
		return 1;
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
			"a = A:new(\"bebe\")\n"
			"result = a:get_name()\n"
		);
		CPPUNIT_ASSERT(l.get_global_string("result") == "bebe");
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_luna); 
