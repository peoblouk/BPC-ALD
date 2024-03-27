#ifndef UT_ADAPTER_XC_H
#define UT_ADAPTER_XC_H
/** \file ut_adapter_xc.h
 *  \brief Unit Test adapter for Apple XCode XCTest Objective C framework
 *  \author Petyovský
 *  \version 2024
 *  $Id: ut_adapter_xc.h 2646 2024-03-18 13:41:07Z petyovsky $
 */

// https://developer.apple.com/documentation/xctest/boolean_assertions

#ifdef __MACH__
	#import <XCTest/XCTest.h>
#endif

#define UT_NAMESPACE(aNamespace)

#define UT_ENDNAMESPACE(aNamespace)

#define UT_CLASS(aClassName) \
	@interface aClassName : XCTestCase \
	@end \
	@implementation aClassName

#define UT_ENDCLASS(aClassName) \
	@end

#define UT_METHOD(aMethodName) \
	- (void)aMethodName

#define UT_ASSERT_FAIL() \
	XCTFail()
#define UT_ASSERT2_FAIL(aMsg) \
	XCTFail(@ aMsg)

#define UT_ASSERT_IS_TRUE(aTruePredicate) \
	XCTAssertTrue(aTruePredicate)
#define UT_ASSERT2_IS_TRUE(aTruePredicate, aMsg) \
	XCTAssertTrue(aTruePredicate, @ aMsg)

#define UT_ASSERT_IS_FALSE(aFalsePredicate) \
	XCTAssertFalse(aFalsePredicate)
#define UT_ASSERT2_IS_FALSE(aFalsePredicate, aMsg) \
	XCTAssertFalse(aFalsePredicate, @ aMsg)

#define UT_ASSERT_IS_NULL(aValue) \
	XCTAssertNil(aValue)
#define UT_ASSERT2_IS_NULL(aValue, aMsg) \
	XCTAssertNil(aValue, @ aMsg)

#define UT_ASSERT_IS_NOT_NULL(aValue) \
	XCTAssertNotNil(aValue)
#define UT_ASSERT2_IS_NOT_NULL(aValue, aMsg) \
	XCTAssertNotNil(aValue, @ aMsg)

#define UT_ASSERT_ARE_EQUAL(aExpected, aValue) \
	XCTAssertEqual(aExpected, aValue)
#define UT_ASSERT2_ARE_EQUAL(aExpected, aValue, aMsg) \
	XCTAssertEqual(aExpected, aValue, @ aMsg)

#define UT_ASSERT_ARE_NOT_EQUAL(aExpected, aValue) \
	XCTAssertNotEqual(aExpected, aValue)
#define UT_ASSERT2_ARE_NOT_EQUAL(aExpected, aValue, aMsg) \
	XCTAssertNotEqual(aExpected, aValue, @ aMsg)

#define UT_ASSERT_ARE_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance) \
	XCTAssertEqualWithAccuracy(aExpected, aValue, aTolerance)
#define UT_ASSERT2_ARE_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance, aMsg) \
	XCTAssertEqualWithAccuracy(aExpected, aValue, aTolerance, @ aMsg)

#define UT_ASSERT_ARE_NOT_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance) \
	XCTAssertNotEqualWithAccuracy(aExpected, aValue, aTolerance)
#define UT_ASSERT2_ARE_NOT_EQUAL_WITH_TOLERANCE(aExpected, aValue, aTolerance, aMsg) \
	XCTAssertNotEqualWithAccuracy(aExpected, aValue, aTolerance, @ aMsg)

#define UT_ASSERT_EXPECT_EXCEPTION(aExpression) \
	XCTAssertThrows(aExpression)
#define UT_ASSERT2_EXPECT_EXCEPTION(aExpression, aMsg) \
	XCTAssertThrows(aExpression, @ aMsg)

#endif /* UT_ADAPTER_XC_H */
