#include <fstream>
#include <sstream>
#include <iomanip>
#include <orthoPoly3D.H>

int main(int argc, char* argv[])
{
  int len = std::stoi(argv[1]);
  std::vector<double> x(len);
  std::vector<double> y(len);
  std::vector<double> z(len);
  
  VectorXd func(len*len*len);
  
  double tmp = -1;
  for (int i = 0; i < len; ++i)
  {
    x[i] = tmp;
    y[i] = tmp;
    z[i] = tmp;
    tmp += 2./len; 
  }

  std::ifstream inputStream("F.txt");
  std::string line;
  int i = 0;
  while (getline(inputStream, line))
  {
    std::stringstream ss(line);
    ss >> func(i);
    i+=1;
  }
  std::cout << "Starting..." << std::endl;
  #ifdef DEBUG
    Timer T1;
    Timer T2;
    T1.start();
    T2.start();
  #endif
  std::vector<double> node = {0.05, 0.5, 0.5};
  orthoPoly3D test(3,func,x,y,z);
	#ifdef DEBUG
    T2.stop();
    std::cout << "Time for construction: " << T2.elapsed() << std::endl;
  #endif
  std::cout << test({0.7,.5,-.3}) << std::endl;
  std::cout << test(node) << std::endl;
	std::cout << "Finished..." << std::endl;
  #ifdef DEBUG
    T1.stop();
    std::cout << "Time elapsed: " << T1.elapsed() << std::endl;
  #endif
	std::cout << "\nConstructor Tests\n" << std::endl;
	std::cout << "Default Construction" << std::endl;
	orthoPoly3D test1;
	std::cout << "test1 status: (0) " << test1.status() << std::endl;

	std::cout << "\nMove Construction" << std::endl;
	orthoPoly3D test2(std::move(test));
	std::cout << "test2 status: (1) " << test2.status() << std::endl;
	std::cout << "test status: (0) " << test.status() << std::endl;
	
	std::cout << "\nMove Assignment" << std::endl;
	orthoPoly3D test3;
	std::cout << "test3 status: (0) " << test3.status() << std::endl;
	test3 = std::move(test2);
	std::cout << "test3 status: (1) " << test3.status() << std::endl;
	std::cout << "test2 status: (0) " << test2.status() << std::endl;
	std::cout << std::setprecision(15) << test3(node) << std::endl; 
	test2(node);	

  return 0; 
}
 
