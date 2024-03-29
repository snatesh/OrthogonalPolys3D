#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <gtest.h>
#include <orthoPoly3D.H>

const char* file_name;

// The fixture for testing class orthoPoly. From google test primer.
class orthoPolyTest : public ::testing::Test {
protected:

  orthoPolyTest() 
  {
    int len = 40;
    x.resize(len);
    y.resize(len);
    z.resize(len);
    func.resize(len*len*len);
    double tmp = -1;
    for (int i = 0; i < len; ++i)
    {
      x[i] = tmp;
      y[i] = tmp;
      z[i] = tmp;
      tmp += 2./len; 
    }

    std::ifstream inputStream(file_name);
    if (!inputStream.good())
    {
      std::cout << "Could not read " << file_name << std::endl;
      exit(1);
    }
    std::string line;
    int i = 0;
    while (getline(inputStream, line))
    {
      std::stringstream ss(line);
      ss >> func(i);
      i+=1;
    }
    aEx.resize(20);
    aEx << 1559.11157584,
          -5973.95348363,
          2454.025,
          6.02540239925e-12,
          7664.33651638,
          -1091.760655,
          21839,
          1823,
          -1.36424205266e-12,
          -5.58242341242e-12,
          87.746461375,
          1581.695545,
          1.01465502667e-11,
          26.995545,
          0.178199999999,
          5.2073900747e-12,
          456.573,
          -18234,
          -1.37072575512e-11,
          -2.69761990523e-12;
  }

  virtual ~orthoPolyTest() 
  {
    x.resize(0);
    y.resize(0);
    z.resize(0);
    func.resize(0);
    aEx.resize(0);
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() 
  {
    
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for orthoPoly.
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  VectorXd func;
  VectorXd aEx;
  const std::vector<double> node = {0.05, 0.5, 0.5};
  const double val = 4295.95998500002;
};

TEST_F(orthoPolyTest, Constructors)
{
  // test construction with data
  orthoPoly3D testD(3,func,x,y,z);
  VectorXd aRet = testD.getCoeffs();
  ASSERT_EQ(aRet.size(),aEx.size());
  for (int i = 0; i < aEx.size(); ++i)
  {
    ASSERT_LT(std::abs(aRet(i)-aEx(i)),1e-6) 
    //ASSERT_DOUBLE_EQ(aRet(i),aEx(i)) 
     << "Coeffs aRet and aEx differ at index " << i;
  }
  ASSERT_EQ(testD.status(),1);

  // test default construction and move-assignment
  orthoPoly3D test;
  ASSERT_EQ(test.status(),0) ;
  ASSERT_EQ(test.getCoeffs().size(),0) << "Unsuccessfull default construction";
  test = std::move(testD);
  ASSERT_EQ(test.status(),1) << "Unsuccessfull move-assignment";
  ASSERT_EQ(testD.status(),0) << "Unsuccessfull move-assignent";
  aRet = test.getCoeffs();
  for (int i = 0; i < aEx.size(); ++i)
  {
    ASSERT_LT(std::abs(aRet(i)-aEx(i)),1e-6) 
      << "Coeffs aRet and aEx differ at index " << i;
  }

  // test move-copy construction
  orthoPoly3D test1(std::move(test));
  ASSERT_EQ(test1.status(),1) << "Unsuccessfull move-copy construction";
  ASSERT_EQ(test.status(),0) << "Unsuccessfull move-copy construction";
  aRet = test1.getCoeffs();
  for (int i = 0; i < aEx.size(); ++i)
  {
    ASSERT_LT(std::abs(aRet(i)-aEx(i)),1e-6) 
      << "Coeffs aRet and aEx differ at index " << i;
  }
  
  // test evaluation
  ASSERT_LT(std::abs(test1(node)-val),1e-6);      
}

TEST(InvalidOpDeathTest, evalWithNoInit)
{
  orthoPoly3D test;
  EXPECT_EXIT(  test({0,0,0}), ::testing::ExitedWithCode(1),
                "Coefficients to basis have not been generated");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  assert(argc == 2);
  file_name = argv[1];
  return RUN_ALL_TESTS();
}
