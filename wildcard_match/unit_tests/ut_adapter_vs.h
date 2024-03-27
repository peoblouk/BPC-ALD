#ifndef UT_ADAPTER_VS_H
#define UT_ADAPTER_VS_H
/** \file ut_adapter_vs.h
 *  \brief Unit Test adapter for M$ Visual Studio C++ unit test framework
 *  \author Petyovský
 *  \version 2024
 *  $Id: ut_adapter_vs.h 2646 2024-03-18 13:41:07Z petyovsky $
 */

// https://learn.microsoft.com/en-us/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define UT_NAMESPACE(aNamespace) \
	namespace aNamespace \
		{

#define UT_ENDNAMESPACE(aNamespace) \
		}

#define UT_CLASS(aClassName) \
	TEST_CLASS(aClassName) \
		{ \
		public:

#define UT_ENDCLASS(aClassName) \
		};

#define UT_METHOD(aMethodName) \
	TEST_METHOD(aMethodName)

#define UT_ASSERT_FAIL() \
	Assert::Fail()
#define UT_ASSERT2_FAIL(aMsg) \
	Assert::Fail(L##aMsg)

#define UT_ASSERT_IS_TRUE(aTruePredicate) \
	Assert::IsTrue(aTruePredicate)
#define UT_ASSERT2_IS_TRUE(aTruePredicate, aMsg) \
	Assert::IsTrue(aTruePredicate, L##aMsg)

#define UT_ASSERT_IS_FALSE(aFalsePredicate) \
	Assert::IsFalse(aFalsePredicate)
#define UT_ASSERT2_IS_FALSE(aFalsePredicate, aMsg) \
	Assert::IsFalse(aFalsePredicate, L##aMsg)

#define UT_ASSERT_IS_NULL(aValue) \
	Assert::IsNull(aValue)
#define UT_ASSERT2_IS_NULL(aValue, aMsg) \
	Assert::IsNull(aValue, L##aMsg)

#define UT_ASSERT_IS_NOT_NULL(aValue) \
	Assert::IsNotNull(aValue)
#define UT_ASSERT2_IS_NOT_NULL(aValue, aMsg) \
	Assert::IsNotNull(aValue, L##aMsg)

#define UT_ASSERT_ARE_EQUAL(aExpected, aValue) \
	Assert::AreEqual(aExpected, aValue)
#define UT_ASSERT2_ARE_EQUAL(aExpected, aValue, aMsg) \
	Assert::AreEqual(aExpected, aValue, L##aMsg)

#define UT_ASSERT_ARE_NOT_EQUAL(aExpected, aValue) \
	Assert::AreNotEqual(aExpected, aValue)
#define UT_ASSERT2_ARE_NOT_EQUAL(aExpected, aValue, aMsg) \
	Assert::AreNotEqual(aExpected, aValue, L##aMsg)

#define UT_ASSERT_ARE_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance) \
	Assert::AreEqual(aExpected, aValue, aTolerance)
#define UT_ASSERT2_ARE_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance, aMsg) \
	Assert::AreEqual(aExpected, aValue, aTolerance, L##aMsg)

#define UT_ASSERT_ARE_NOT_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance) \
	Assert::AreNotEqual(aExpected, aValue, aTolerance)
#define UT_ASSERT2_ARE_NOT_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance, aMsg) \
	Assert::AreNotEqual(aExpected, aValue, aTolerance, L##aMsg)

#define UT_ASSERT_EXPECT_EXCEPTION(aExpression) \
	Assert::ExpectException(aExpression)
#define UT_ASSERT2_EXPECT_EXCEPTION(aExpression, aMsg) \
	Assert::ExpectException(aExpression, L##aMsg)

#endif /* UT_ADAPTER_VS_H */
