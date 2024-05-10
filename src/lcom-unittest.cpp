// Test and verify the expected behavior of LCOM metrics.

#include "rose.h"
// ROSE is always included first.
#include <gtest/gtest.h>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "define.hpp"
#include "lcom.hpp"
#include "traverse.hpp"

// using Class = SgAdaPackageSpec*;
using Method = Traverse::MType;
using Attribute = Traverse::AType;

// NOTE: This must be set by the build system using
// -DROOT_DIR=lcom-metric-analyzer.
const boost::filesystem::path ROOT = ROOT_DIR;
// The executable name must be passed in, but it is unused.
const boost::filesystem::path EXEC = ROOT / "build/lcom.out";
// The location of the testcases directory.
// Used to find specific test files used in unit testing.
const boost::filesystem::path TESTS = ROOT / "testcases";

struct LCOMData {
  struct LCOMClass {
    const std::size_t LCOM1;
    const std::size_t LCOM2;
    const std::size_t LCOM3;
    const std::size_t LCOM4;
    const double LCOM5;
    LCOM::LCOM1Data data1;
    LCOM::LCOM5Data data5;
  };
  std::vector<LCOMClass> classes;
};

struct LCOMClassData {
  boost::filesystem::path source;
  DotBehavior dot;
  boost::optional<LCOMData> PackageData = boost::none;
  boost::optional<LCOMData> ProtectedData = boost::none;
  boost::optional<LCOMData> ClassData = boost::none;

  // Generate the test name.
  static std::string PrintTo(
      const testing::TestParamInfo<LCOMClassData>& info) {
    const std::string s(info.param.source.string());
    const size_t start = s.find_last_of('/') + 1;
    const size_t end = s.find_last_of('.') - start;
    const std::string fileName = s.substr(start, end);

    std::stringstream ss;
    ss << fileName << info.param.dot;
    return ss.str();
  }
};

class LCOMTest : public ::testing::TestWithParam<LCOMClassData> {
 protected:
  SgProject* project;
  // Unfortunately, these must be specified for each class type you want to
  // evaluate. This is a limitation in the way GTests support templated code.
  std::vector<LCOM::Class<SgAdaPackageSpec*, Method, Attribute>>
      LCOMInputPackage;
  std::vector<LCOM::Class<SgAdaProtectedSpec*, Method, Attribute>>
      LCOMInputProtected;
  std::vector<LCOM::Class<SgClassDeclaration*, Method, Attribute>>
      LCOMInputClass;

  void SetUpProject(const boost::filesystem::path& source, DotBehavior dot) {
    std::vector<std::string> cmdLineArgs{EXEC.string(), source.string()};
    dotBehavior = dot;
    project = Traverse::GetProject(cmdLineArgs);
  }
};

template <typename C>
void CheckLCOMInput(const std::vector<LCOM::Class<C, Method, Attribute>>& input,
                    const LCOMData& exp) {
  EXPECT_EQ(input.size(), exp.classes.size());
  for (size_t i = 0; i < exp.classes.size(); i++) {
    const LCOMData::LCOMClass& expCls = exp.classes[i];
    LCOM::LCOM1Data data1;
    LCOM::LCOM5Data data5;

    EXPECT_EQ(LCOM::LCOM1(input[i], &data1), expCls.LCOM1);
    EXPECT_EQ(LCOM::LCOM2(input[i]), expCls.LCOM2);
    EXPECT_EQ(LCOM::LCOM3(input[i]), expCls.LCOM3);
    EXPECT_EQ(LCOM::LCOM4(input[i]), expCls.LCOM4);
    if (std::isnan(expCls.LCOM5)) {
      EXPECT_TRUE(std::isnan(LCOM::LCOM5(input[i], &data5)));
    } else {
      EXPECT_EQ(LCOM::LCOM5(input[i], &data5), expCls.LCOM5);
    }
    EXPECT_EQ(data1.sharedPairs, expCls.data1.sharedPairs);
    EXPECT_EQ(data1.unsharedPairs, expCls.data1.unsharedPairs);
    EXPECT_EQ(data1.totalPairs, expCls.data1.totalPairs);
    EXPECT_EQ(data5.a, expCls.data5.a);
    EXPECT_EQ(data5.l, expCls.data5.l);
    EXPECT_EQ(data5.k, expCls.data5.k);
  }
}

TEST_P(LCOMTest, CheckClass) {
  LCOMClassData exp = GetParam();
  SetUpProject(exp.source, exp.dot);
  if (exp.PackageData != boost::none) {
    LCOMInputPackage = Traverse::GetClassData<SgAdaPackageSpec*>(project);
    CheckLCOMInput(LCOMInputPackage, *(exp.PackageData));
  }
  if (exp.ProtectedData != boost::none) {
    LCOMInputProtected = Traverse::GetClassData<SgAdaProtectedSpec*>(project);
    CheckLCOMInput(LCOMInputProtected, *(exp.ProtectedData));
  }
  if (exp.ClassData != boost::none) {
    LCOMInputClass = Traverse::GetClassData<SgClassDeclaration*>(project);
    CheckLCOMInput(LCOMInputClass, *(exp.ClassData));
  }
  return;
}

// Structure of a test:
// LCOMClassData{
//     // Location of the test.
//     .source = TESTS / "other-tests/nested.adb",
//     // Dot behavior, Full or LeftOnly (root).
//     .dot = DotBehavior::Full,
//     // A list of all classes found in this file, in order of appearance.
//     .PackageData{LCOMData{.classes{
//         LCOMData::LCOMClass{
//             // The LCOM values for this class.
//             .LCOM1 = 1,
//             .LCOM2 = 1,
//             .LCOM3 = 2,
//             .LCOM4 = 2,
//             .LCOM5 = (double)1,
//             // The number of shared and unshared pairs, used in LCOM 1-2.
//             .data1{.sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
//             // The number of attribute accesses, attributes, methods.
//             .data5{.a = 2, .l = 2, .k = 2}}}}}},

INSTANTIATE_TEST_SUITE_P(
    SimpleCases, LCOMTest,
    ::testing::Values(
        LCOMClassData{
            .source = TESTS / "simple-cases/c1.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 5,
                .LCOM2 = 4,
                .LCOM3 = 3,
                .LCOM4 = 3,
                .LCOM5 = (double)11 / (double)12,
                .data1{.sharedPairs = 1, .unsharedPairs = 5, .totalPairs = 6},
                .data5{.a = 5, .l = 4, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c2.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 4,
                .LCOM2 = 2,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)10 / (double)12,
                .data1{.sharedPairs = 2, .unsharedPairs = 4, .totalPairs = 6},
                .data5{.a = 6, .l = 4, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c3.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 4,
                .LCOM2 = 2,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)10 / (double)12,
                .data1{.sharedPairs = 2, .unsharedPairs = 4, .totalPairs = 6},
                .data5{.a = 6, .l = 4, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c4.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 5,
                .LCOM2 = 4,
                .LCOM3 = 3,
                .LCOM4 = 1,
                .LCOM5 = (double)11 / (double)12,
                .data1{.sharedPairs = 1, .unsharedPairs = 5, .totalPairs = 6},
                .data5{.a = 5, .l = 4, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c5.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 5,
                .LCOM2 = 4,
                .LCOM3 = 3,
                .LCOM4 = 1,
                .LCOM5 = (double)8 / (double)9,
                .data1{.sharedPairs = 1, .unsharedPairs = 5, .totalPairs = 6},
                .data5{.a = 4, .l = 3, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c6.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 2,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)8 / (double)12,
                .data1{.sharedPairs = 4, .unsharedPairs = 2, .totalPairs = 6},
                .data5{.a = 8, .l = 4, .k = 4}}}}}},
        // NOTE: C7 and C8 LCOM1 from
        // https://site.iugaza.edu.ps/mroos/files/Software-Metrics1.pdf appears
        // to have a mistake. It claims that 4 pairs share attributes, when only
        // 3 pairs do.
        LCOMClassData{
            .source = TESTS / "simple-cases/c7.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 0,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)1 / (double)3,
                .data1{.sharedPairs = 3, .unsharedPairs = 3, .totalPairs = 6},
                .data5{.a = 3, .l = 1, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c8.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 0,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)3 / (double)9,
                .data1{.sharedPairs = 3, .unsharedPairs = 3, .totalPairs = 6},
                .data5{.a = 9, .l = 3, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "simple-cases/c9.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 2,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)8 / (double)12,
                .data1{.sharedPairs = 4, .unsharedPairs = 2, .totalPairs = 6},
                .data5{.a = 8, .l = 4, .k = 4}}}}}}),
    LCOMClassData::PrintTo);
INSTANTIATE_TEST_SUITE_P(
    OtherTests, LCOMTest,
    ::testing::Values(
        LCOMClassData{
            .source = TESTS / "other-tests/record_clash.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                .data5{.a = 7, .l = 7, .k = 1}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/record_clash2.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)1,
                .data1{.sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                .data5{.a = 3, .l = 3, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/debug.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 1,
                .LCOM2 = 1,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)1,
                .data1{.sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                .data5{.a = 1, .l = 1, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/temp1.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 13,
                .LCOM2 = 11,
                .LCOM3 = 4,
                .LCOM4 = 1,
                .LCOM5 = (double)8 / (double)10,
                .data1{.sharedPairs = 2, .unsharedPairs = 13, .totalPairs = 15},
                .data5{.a = 4, .l = 2, .k = 6}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/temp2.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{
                // debug
                LCOMData::LCOMClass{
                    .LCOM1 = 1,
                    .LCOM2 = 1,
                    .LCOM3 = 2,
                    .LCOM4 = 2,
                    .LCOM5 = (double)1,
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                    .data5{.a = 1, .l = 1, .k = 2}},
                // temp2
                LCOMData::LCOMClass{.LCOM1 = 13,
                                    .LCOM2 = 11,
                                    .LCOM3 = 4,
                                    .LCOM4 = 1,
                                    .LCOM5 = (double)8 / (double)10,
                                    .data1{.sharedPairs = 2,
                                           .unsharedPairs = 13,
                                           .totalPairs = 15},
                                    .data5{.a = 4, .l = 2, .k = 6}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/temp3.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{
                // debug
                LCOMData::LCOMClass{
                    .LCOM1 = 1,
                    .LCOM2 = 1,
                    .LCOM3 = 2,
                    .LCOM4 = 2,
                    .LCOM5 = (double)1,
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                    .data5{.a = 1, .l = 1, .k = 2}},
                // temp3
                LCOMData::LCOMClass{.LCOM1 = 13,
                                    .LCOM2 = 11,
                                    .LCOM3 = 4,
                                    .LCOM4 = 1,
                                    .LCOM5 = (double)8 / (double)10,
                                    .data1{.sharedPairs = 2,
                                           .unsharedPairs = 13,
                                           .totalPairs = 15},
                                    .data5{.a = 4, .l = 2, .k = 6}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/temp4.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 13,
                .LCOM2 = 11,
                .LCOM3 = 4,
                .LCOM4 = 1,
                .LCOM5 = (double)8 / (double)10,
                .data1{.sharedPairs = 2, .unsharedPairs = 13, .totalPairs = 15},
                .data5{.a = 4, .l = 2, .k = 6}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/nofunc_renaming.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 3,
                .LCOM3 = 3,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 3, .totalPairs = 3},
                .data5{.a = 0, .l = 0, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/func_renaming.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 1,
                .LCOM2 = 1,
                .LCOM3 = 2,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                .data5{.a = 0, .l = 0, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/func_renaming_use.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 3,
                .LCOM3 = 3,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 3, .totalPairs = 3},
                .data5{.a = 0, .l = 0, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/novar_renaming.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)0 / (double)-1,
                .data1{.sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                .data5{.a = 2, .l = 1, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/var_renaming.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)0 / (double)-1,
                .data1{.sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                .data5{.a = 2, .l = 1, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/array_renaming.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)0 / (double)-1,
                .data1{.sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                .data5{.a = 2, .l = 1, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/complex1.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 1,
                .LCOM2 = 1,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                .data5{.a = 0, .l = 0, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/complex2.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 3,
                .LCOM3 = 3,
                .LCOM4 = 3,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 3, .totalPairs = 3},
                .data5{.a = 0, .l = 0, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/complex3.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 3,
                .LCOM3 = 3,
                .LCOM4 = 3,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 3, .totalPairs = 3},
                .data5{.a = 0, .l = 0, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/internal_packages.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{
                // greeting
                LCOMData::LCOMClass{
                    .LCOM1 = 0,
                    .LCOM2 = 0,
                    .LCOM3 = 1,
                    .LCOM4 = 1,
                    .LCOM5 = (double)0 / (double)-1,
                    .data1{
                        .sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                    .data5{.a = 2, .l = 1, .k = 2}},
                // error
                LCOMData::LCOMClass{
                    .LCOM1 = 0,
                    .LCOM2 = 0,
                    .LCOM3 = 1,
                    .LCOM4 = 1,
                    .LCOM5 = (double)0 / (double)-1,
                    .data1{
                        .sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                    .data5{.a = 2, .l = 1, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/raising1.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 3,
                .LCOM2 = 3,
                .LCOM3 = 3,
                .LCOM4 = 3,
                .LCOM5 = (double)1,
                .data1{.sharedPairs = 0, .unsharedPairs = 3, .totalPairs = 3},
                .data5{.a = 3, .l = 3, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/raising2.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)0 / (double)-2,
                .data1{.sharedPairs = 3, .unsharedPairs = 0, .totalPairs = 3},
                .data5{.a = 3, .l = 1, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/xsoft.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)2 / (double)5,
                .data1{.sharedPairs = 15, .unsharedPairs = 0, .totalPairs = 15},
                .data5{.a = 8, .l = 2, .k = 6}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/ysoft.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)3 / (double)8,
                .data1{.sharedPairs = 10, .unsharedPairs = 0, .totalPairs = 10},
                .data5{.a = 7, .l = 2, .k = 5}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/zsoft.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 8,
                .LCOM2 = 6,
                .LCOM3 = 3,
                .LCOM4 = 3,
                .LCOM5 = (double)9 / (double)10,
                .data1{.sharedPairs = 2, .unsharedPairs = 8, .totalPairs = 10},
                .data5{.a = 7, .l = 5, .k = 5}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/gcomplex.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 5,
                .LCOM2 = 4,
                .LCOM3 = 3,
                .LCOM4 = 3,
                .LCOM5 = (double)2 / (double)3,
                .data1{.sharedPairs = 1, .unsharedPairs = 5, .totalPairs = 6},
                .data5{.a = 2, .l = 1, .k = 4}}}}}},
        // complex_int: Only an .ads file. Will not analyze.
        LCOMClassData{
            .source = TESTS / "other-tests/point_complex.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{
                // GComplex
                LCOMData::LCOMClass{
                    .LCOM1 = 6,
                    .LCOM2 = 6,
                    .LCOM3 = 4,
                    .LCOM4 = 4,
                    .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 6, .totalPairs = 6},
                    .data5{.a = 0, .l = 0, .k = 4}},
                // Complex_Int
                LCOMData::LCOMClass{
                    .LCOM1 = 6,
                    .LCOM2 = 6,
                    .LCOM3 = 4,
                    .LCOM4 = 4,
                    .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 6, .totalPairs = 6},
                    .data5{.a = 0, .l = 0, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/points.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 6,
                .LCOM2 = 6,
                .LCOM3 = 4,
                .LCOM4 = 3,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 6, .totalPairs = 6},
                .data5{.a = 0, .l = 0, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/multivariables.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 4,
                .LCOM2 = 2,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)2 / (double)3,
                .data1{.sharedPairs = 2, .unsharedPairs = 4, .totalPairs = 6},
                .data5{.a = 4, .l = 2, .k = 4}}}}}},
        // TODO: Wait for ROSE to handle seeing the rest of the separated data.
        // Disable this test until go-ahead that ROSE has fixed this.
        // NOTE: Expected test results currently ignore Separated.Xchg because
        // it was not part of the original specification.
        // LCOMClassData{
        //     .source = TESTS / "other-tests/separated.adb",
        //     .dot = DotBehavior::Full,
        //     .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
        //         .LCOM1 = 4,
        //         .LCOM2 = 2,
        //         .LCOM3 = 2,
        //         .LCOM4 = 2,
        //         .LCOM5 = (double)2 / (double)3,
        //         .data1{.sharedPairs = 2, .unsharedPairs = 4, .totalPairs =
        //         6},
        //         .data5{.a = 4, .l = 2, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/concurrent.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 4,
                .LCOM2 = 2,
                .LCOM3 = 2,
                .LCOM4 = 2,
                .LCOM5 = (double)2 / (double)3,
                .data1{.sharedPairs = 2, .unsharedPairs = 4, .totalPairs = 6},
                .data5{.a = 8, .l = 4, .k = 4}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/body_attribute1.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                .data5{.a = 1, .l = 1, .k = 1}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/body_attribute2.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                .data5{.a = 1, .l = 1, .k = 1}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/body_attribute3.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                .data5{.a = 1, .l = 1, .k = 1}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/body_attribute4.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                .data5{.a = 1, .l = 1, .k = 1}}}}}},
        // TODO: Packages currently ignore methods wrapped behind nested
        // packages. Methods in nested packages ignore attributes in the parent
        // package. Are these the behaviors we want?
        LCOMClassData{
            .source = TESTS / "other-tests/nested.adb",
            .dot = DotBehavior::Full,
            .PackageData{LCOMData{.classes{
                LCOMData::LCOMClass{
                    .LCOM1 = 1,
                    .LCOM2 = 1,
                    .LCOM3 = 2,
                    .LCOM4 = 2,
                    .LCOM5 = (double)1,
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                    .data5{.a = 2, .l = 2, .k = 2}},
                LCOMData::LCOMClass{
                    .LCOM1 = 1,
                    .LCOM2 = 1,
                    .LCOM3 = 2,
                    .LCOM4 = 2,
                    .LCOM5 = (double)1,
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 1, .totalPairs = 1},
                    .data5{.a = 2, .l = 2, .k = 2}},
                LCOMData::LCOMClass{
                    .LCOM1 = 2,
                    .LCOM2 = 1,
                    .LCOM3 = 2,
                    .LCOM4 = 2,
                    .LCOM5 = (double)3 / (double)4,
                    .data1{
                        .sharedPairs = 1, .unsharedPairs = 2, .totalPairs = 3},
                    .data5{.a = 3, .l = 2, .k = 3}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/record_clash.adb",
            .dot = DotBehavior::LeftOnly,
            .PackageData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                .data1{.sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                .data5{.a = 3, .l = 3, .k = 1}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/show_protected_objects.adb",
            .dot = DotBehavior::Full,
            .ProtectedData{LCOMData{.classes{LCOMData::LCOMClass{
                .LCOM1 = 0,
                .LCOM2 = 0,
                .LCOM3 = 1,
                .LCOM4 = 1,
                .LCOM5 = (double)0 / (double)-1,
                .data1{.sharedPairs = 1, .unsharedPairs = 0, .totalPairs = 1},
                .data5{.a = 2, .l = 1, .k = 2}}}}}},
        LCOMClassData{
            .source = TESTS / "other-tests/p.adb",
            .dot = DotBehavior::Full,
            .ClassData{LCOMData{.classes{
                LCOMData::LCOMClass{
                    .LCOM1 = 0,
                    .LCOM2 = 0,
                    .LCOM3 = 1,
                    .LCOM4 = 1,
                    .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                    .data5{.a = 1, .l = 1, .k = 1}},
                LCOMData::LCOMClass{
                    .LCOM1 = 0,
                    .LCOM2 = 0,
                    .LCOM3 = 1,
                    .LCOM4 = 1,
                    .LCOM5 = std::numeric_limits<double>::quiet_NaN(),
                    .data1{
                        .sharedPairs = 0, .unsharedPairs = 0, .totalPairs = 0},
                    // TODO: Not seeing A in this class for some reason. I think it's because my code isn't designed to accomodate an attribute existing in multiple classes.
                    .data5{.a = 2, .l = 2, .k = 1}}}}}}),
    LCOMClassData::PrintTo);
