#ifndef GEVENT_HPP
#define GEVENT_HPP
//-----------------------------------------------------------------------------
// $Revision: 1.2 $
//-----------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <vector>

///
class gEvent
{
 public:
  enum SIZE {MAXSIZE=4000};

  ///
  gEvent();
  
  ///
  virtual ~gEvent();

  ///
  void        update();

  ///
  void        printTree(std::ostream& stream,
                        int  printLevel,
                        int  index,
                        int  maxdepth=0,     // no restriction on depth
                        int  depth=0);

  ///
  std::string tree(int  printLevel=-2,
                   int  index=-1,
                   int  maxdepth=0,   // no restriction on depth
                   int  depth=0);

  ///
  void        printTable(std::ostream& stream, int maxrows=50);

  ///
  std::string table(int maxcount=50);

  int                 nhep;
            
  std::vector<int>    mother;

  std::vector<int>    firstDaughter;   
  std::vector<int>    lastDaughter;  

  std::vector<int>    pdgId;           
  std::vector<int>    status;          
  std::vector<float>  pt;
  std::vector<float>  eta;
  std::vector<float>  phi;
  std::vector<float>  mass;
  std::vector<float>  charge;           

  std::vector<int>    motherpdgId;           
  std::vector<int>    motherstatus;          
  std::vector<float>  motherpt;
  std::vector<float>  mothereta;
  std::vector<float>  motherphi;
  std::vector<float>  mothermass;
  std::vector<float>  mothercharge;           
};
#endif
