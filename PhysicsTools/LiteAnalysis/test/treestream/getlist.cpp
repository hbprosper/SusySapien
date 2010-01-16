#include <iostream>
#include <string>
#include "TBranch.h"
#include "TLeaf.h"
#include "TObjArray.h"

using namespace std;

const string SPACE("                                                        ");

void getlist(ostream& out, TBranch* branch, int depth=0)
{
  TObjArray* array = branch->GetListOfBranches();
  if ( ! array ) return;
  if ( depth > 10 ) return;

  string name;
  int nitems = array->GetEntries();

  for (int i = 0; i < nitems; i++)
	{
	  TBranch* b = (TBranch*)((*array)[i]);
      if ( ! b ) continue;

      string branchname(b->GetName());
      out << SPACE.substr(0,4*depth) << branchname << endl;
 
      TObjArray* a = b->GetListOfLeaves();
      if ( a )
        {
          int n = a->GetEntries();
          {
              for (int j = 0; j < n; j++)
                {
                  TLeaf* leaf = (TLeaf*)((*a)[j]);
                  int count = 0;
                  int ndata = 0;
                  TLeaf* leafc = leaf->GetLeafCounter(count);
                  if ( ! leafc)
                    ndata = leaf->GetLen();
                  else
                    ndata = leafc->GetMaximum();

                  string leafname(leaf->GetName());
                  out << SPACE.substr(0,4*(depth+1)) 
                      << ndata << " " << leafname << endl;
                }
          }
//           else if ( n == 1 )
//             {
//               TBranch* bc = (TBranch*)((*a)[j]);
//               string leafname(bc->GetName());
//               if ( leafname != branchname )
//               out << SPACE.substr(0,4*(depth+1)) << leafname << endl;
//             }
        }
      getlist(out, b, depth+1);
    }
}
