/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include "dbTable.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for (size_t j = 0, m = r.size(); j < m; ++j) {
     if (r[j] == INT_MAX) {
       cout << '.';
     } else {
       cout << r[j];
     }
     if (j < m-1)
       cout << ' ';
   }
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells are printed as '.'
   for (size_t i = 0, n = t.nRows(); i < n; ++i) {
     for (size_t j = 0, m = t[i].size(); j < m; ++j) {
       if (t[i][j] == INT_MAX) {
         cout << setw(6) << '.';
       } else {
         cout << setw(6) << t[i][j];
       }
     }
     if (i < n-1)
       cout << endl;
   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume the input file is with correct csv file format
   // - NO NEED to handle error file format
   string buffer;
   while (!ifs.eof()) {
     DBRow tempRow;
     bool checkfinal = false;
     getline(ifs,buffer,'\r');
     buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
     string delimiter = ",";
     size_t pos = 0;
     string token;

     while ((pos = buffer.find(delimiter)) != std::string::npos) {
       checkfinal = true;
       token = buffer.substr(0, pos);
       if (token == "") {
         tempRow.addData(INT_MAX);
       } else {
         tempRow.addData(atof(token.c_str()));
       }
       buffer.erase(0, pos + delimiter.length());
     }

     if (buffer == "") {
       tempRow.addData(INT_MAX);;
     } else {
       tempRow.addData(atof(buffer.c_str()));
     }

     if (checkfinal)
       t.addRow(tempRow);
   }
   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
   _data.erase(_data.begin() + c);
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
   for (size_t i = 0, n = _sortOrder.size(); i < n; ++i) {
     if (r1[_sortOrder[i]] > r2[_sortOrder[i]]) {
       return false;
     } else if (r1[_sortOrder[i]] < r2[_sortOrder[i]]) {
       return true;
     }
   }
   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO
   _table.clear();
}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
   for (size_t i = 0, n = d.size(); i < n; ++i)
     _table[i].addData(d[i]);
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
   _table.erase(_table.begin() + c);
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
   int temp = INT_MIN;
   bool check = false;
   for (size_t i = 0, n = _table.size(); i < n; ++i) {
     if (_table[i][c] != INT_MAX && temp <= _table[i][c]) {
       temp = _table[i][c];
       check = true;
     }
   }
   if (check) {
     return temp;
   } else {
     return NAN;
   }
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
   int temp = INT_MAX;
   bool check = false;
   for (size_t i = 0, n = _table.size(); i < n; ++i) {
     if (temp > _table[i][c]) {
       temp = _table[i][c];
       check = true;
     }
   }
   if (check) {
     return temp;
   } else {
     return NAN;
   }
}

float
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
   int sum = 0;
   bool check = false;
   for (size_t i = 0, n = _table.size(); i < n; ++i) {
     if (_table[i][c] != INT_MAX) {
       sum += _table[i][c];
       check = true;
     }
   }
   if (check) {
     return sum;
   } else {
     return NAN;
   }
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
   vector<int> count;
   bool check = false;
   for (size_t i = 0, n = _table.size(); i < n; ++i) {
     if (_table[i][c] != INT_MAX) {
       count.push_back(_table[i][c]);
       check = true;
     }
   }
   std::sort(count.begin(), count.end());
   auto last = unique(count.begin(), count.end());
   count.erase(last, count.end());
   if (check) {
     return count.size();
   } else {
     return 0;
   }
}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
   float sum = 0;
   float count = 0;
   bool check = false;
   for (size_t i = 0, n = _table.size(); i < n; ++i) {
     if (_table[i][c] != INT_MAX) {
       sum += (double)_table[i][c];
       count++;
       check = true;
     }
   }
   if (check) {
     return sum/count;
   } else {
     return NAN;
   }
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
   std::sort(_table.begin(), _table.end(), s);
}

void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for (size_t i = 0, n = _table.size(); i < n; ++i) {
     if (_table[i][c] != INT_MAX) {
       cout << _table[i][c];
     } else {
       cout << '.';
     }
     if (i < n-1) {
       cout << ' ';
     } else {
       cout << endl;
     }
   }
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}
