/****************************************************************************
  FileName     [ test.cpp ]
  PackageName  [ test ]
  Synopsis     [ Test program for simple database db ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include "dbTable.h"

using namespace std;

extern DBTable dbtbl;

class CmdParser;
CmdParser* cmdMgr = 0; // for linking purpose

int
main(int argc, char** argv)
{
   if (argc != 2) {  // testdb <cvsfile>
      cerr << "Error: using testdb <cvsfile>!!" << endl;
      exit(-1);
   }

   ifstream inf(argv[1]);

   if (!inf) {
      cerr << "Error: cannot open file \"" << argv[1] << "\"!!\n";
      exit(-1);
   }

   if (dbtbl) {
      cout << "Table is resetting..." << endl;
      dbtbl.reset();
   }
   if (!(inf >> dbtbl)) {
      cerr << "Error in reading csv file!!" << endl;
      exit(-1);
   }

   cout << "========================" << endl;
   cout << " Print table " << endl;
   cout << "========================" << endl;
   cout << dbtbl << endl;
   cout << "========================" << endl;

  // TODO
  // Insert what you want to test here by calling DBTable's member functions
   cout << dbtbl[0] << endl;
   cout << dbtbl[1] << endl;
   cout << "MAX 0: " << dbtbl.getMax(0) << endl;
   cout << "MIN 0: " << dbtbl.getMin(0) << endl;
   cout << "SUM 0: " << dbtbl.getSum(0) << endl;
   cout << "COUNT 0: " << dbtbl.getCount(0) << endl;
   cout << "AVE 0: "<< dbtbl.getAve(0) << endl;
   cout << "PRINTCOL 0: ";
   dbtbl.printCol(0);
   cout << "MAX 1: " << dbtbl.getMax(1) << endl;
   cout << "MIN 1: " << dbtbl.getMin(1) << endl;
   cout << "SUM 1: " << dbtbl.getSum(1) << endl;
   cout << "COUNT 1: " << dbtbl.getCount(1) << endl;
   cout << "AVE 1: "<< dbtbl.getAve(1) << endl;
   cout << "PRINTCOL 1: ";
   dbtbl.printCol(1);
   DBSort mysort;
   mysort.pushOrder(3);
   mysort.pushOrder(2);
   dbtbl.sort(mysort);
   cout << dbtbl << endl;
   cout << "========================" << endl;

   dbtbl.delRow(1);
   cout << dbtbl << endl;
   cout << "========================" << endl;
   dbtbl.delCol(1);
   cout << dbtbl << endl;
   cout << "========================" << endl;

   vector<int> d = {1,2,3,4,5};
   dbtbl.addCol(d);
   cout << dbtbl << endl;
   cout << "========================" << endl;

   dbtbl.reset();
   cout << dbtbl << endl;
   cout << "========================" << endl;
   return 0;
}
