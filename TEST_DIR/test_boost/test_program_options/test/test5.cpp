#include "stdafx.h"
#include <boost/program_options.hpp>
 
 #include <vector>
 #include <iostream>
 #include <string>
 #include <algorithm>
 #include <iterator>
 using std::copy;
 using std::vector;
 using std::string;
 using std::cout;
 using std::endl;
 using std::exception;
 using std::ostream;
 using std::ostream_iterator;
 
 namespace po=boost::program_options;
 
 // output vector.
 template <typename T>
 ostream& operator<<(ostream& os, const vector<T>& v)
 {
     copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
     return os;
 }
 
 int main5(int argc, char*argv[])
 {
     try
     {
         po::options_description desc("general descriptions.");
         desc.add_options()
             ("help", "generate help information")
             ("input-file", po::value<vector<string> >(), "input files")
             ("link-file,l", po::value<vector<string> >(), "link file");
 
         po::variables_map vm;
         po::store(po::parse_command_line(argc, argv, desc), vm);
         po::notify(vm);
 
         if(vm.count("help"))
         {
             cout<<desc<<endl;
             return 1;
         }
 
         if(vm.count("input-file"))
         {
             cout<<"Input files: "<<vm["input-file"].as<vector<string> >()
                 <<"\n";
         }
 
         if(vm.count("link-file"))
         {
             cout<<"Link file: "<<vm["link-file"].as<vector<string> >()
                 <<"\n";
         }
     }
     catch(exception& e)
     {
         cout<<e.what()<<endl;
         return -1;
     }
 
     return 0;
 }

