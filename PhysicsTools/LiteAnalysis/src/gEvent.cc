//----------------------------------------------------------------------------
// File:    gEvent.cc
// Purpose: Simple interface to GenParticles
// Created: 16-Feb-2010 Harrison B. Prosper
//          Based on code written in 2005
//$Revision: 1.2 $
//----------------------------------------------------------------------------
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include "TLorentzVector.h"
#include "PhysicsTools/LiteAnalysis/interface/gEvent.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
//----------------------------------------------------------------------------

using namespace std;

// Hide in an anonymous namespace

namespace {

  const int MAXDEPTH=99;
  const int DEBUG=0;

  struct PID
  {
    std::string name;
    int index;
    bool operator<(const PID& o) const { return this->name < o.name; }
  };
}

gEvent::gEvent()
  : nhep(0),

    mother(std::vector<int>(MAXSIZE, 0)),

    firstDaughter(std::vector<int>(MAXSIZE, 0)),
    lastDaughter (std::vector<int>(MAXSIZE, 0)),

    pdgId  (std::vector<int>(MAXSIZE, 0)),
    status (std::vector<int>(MAXSIZE, 0)),
    pt     (std::vector<float>(MAXSIZE, 0)),
    eta    (std::vector<float>(MAXSIZE, 0)),
    phi    (std::vector<float>(MAXSIZE, 0)),
    mass   (std::vector<float>(MAXSIZE, 0)),
    charge (std::vector<float>(MAXSIZE,  0)),
  
    motherpdgId (std::vector<int>(MAXSIZE, 0)),
    motherstatus(std::vector<int>(MAXSIZE, 0)),
    motherpt    (std::vector<float>(MAXSIZE, 0)),
    mothereta   (std::vector<float>(MAXSIZE, 0)),
    motherphi   (std::vector<float>(MAXSIZE, 0)),
    mothermass  (std::vector<float>(MAXSIZE, 0)),
    mothercharge(std::vector<float>(MAXSIZE, 0))
{}

gEvent::~gEvent() {}

//-----------------------------------------------------------------------------
// Update internal buffers
//-----------------------------------------------------------------------------
void
gEvent::update()
{
  map<string, vector<int> > dMap;
  map<string, int> pMap;
  map<string, int> mMap;

  nhep = pdgId.size();

  // Create a key for each particle

  vector<string> keys(nhep);
  
  for (int i=0; i < nhep; i++)
    {
      char key[512];
      sprintf(key, "%d %d %10.2e %10.2e %10.2e",
              pdgId[i], 
              status[i], 
              pt[i],  
              eta[i], 
              phi[i]);
      keys[i] = key;
      dMap[key] = vector<int>(); // mother to daughter map
      mMap[key] =-1;             // daughter to mother map
      pMap[key] = i;             // key to position map

      firstDaughter[i] =-1;
      lastDaughter[i]  =-1;
      mother[i] =-1;
    }

  // Find the mother/daughter relationships

  for (int i=0; i < nhep; i++)
    {
      if ( motherstatus[i] < 1 ) continue;

      char mkey[512];
      sprintf(mkey, "%d %d %10.2e %10.2e %10.2e",
              motherpdgId[i], 
              motherstatus[i], 
              motherpt[i],  
              mothereta[i],
              motherphi[i]);

      if ( dMap.find(mkey) == dMap.end() )
        {
          cout << " ** error ** can't find key: " << mkey << endl;
          continue;
        }
      dMap[mkey].push_back(i);
      mMap[keys[i]] = pMap[mkey]; // location of mother
    }

  // Set first and last daughters

  for (int i=0; i < nhep; i++)
    { 
      string key = keys[i];
      
      if ( dMap.find(key) != dMap.end() )
        {
          vector<int>& d = dMap[key];
          if ( d.size() > (unsigned)0 )
            {
              firstDaughter[i] = d.front();
              lastDaughter[i]  = d.back();
            }
        }
      else
        cout << " ** error ** can't find key: " << key 
             << " in daughter map" << endl;
 
      if ( mMap.find(key) != mMap.end() )
        mother[i] = mMap[key];
      else
        cout << " ** error ** can't find key: " << key 
             << " in mother map" << endl;
    }
}


//---------------------------------------------------------------------------
// Purpose: Print Gen Event in a tree format
// Created: 06-Sep-2004 Harrison B. Prosper
//---------------------------------------------------------------------------
void 
gEvent::printTable(std::ostream& stream, int maxrows)
{
  char record[512];
  sprintf(record, "%-4s %-16s %4s %4s %4s %10s %10s %10s %10s %6s", 
          " ", "name", "m", "d1", "d2", 
          "pt", "eta", "phi", "mass", "status");
  stream << record << endl;
  int nrows = maxrows < nhep ? maxrows : nhep;
  for(int i=0; i < nrows; i++)
    {
      sprintf(record,"%4d %-16s %4d %4d %4d %10.2e %10.2e %10.2e %10.2e %6d", 
              i, kit::particleName(pdgId[i]).c_str(), 
              mother[i], 
              firstDaughter[i], 
              lastDaughter[i],
              pt[i],
              eta[i],
              phi[i],
              mass[i],
              status[i]);
      stream << record << endl;
    }
}

std::string
gEvent::table(int maxcount)
{
  std::ostringstream os;
  printTable(os, maxcount);
  return os.str();
}

//---------------------------------------------------------------------------
// Purpose: Print Gen Event in a tree format
// Created: 06-Sep-2004 Harrison B. Prosper
//---------------------------------------------------------------------------
void 
gEvent::printTree(std::ostream& stream,
                  int           printLevel,
                  int           index,
                  int           maxdepth,
                  int           depth)
{
  if ( index < 0 ) return;
  if ( index >= nhep )  return;
  if ( depth >  (maxdepth <= 0 ? MAXDEPTH : maxdepth) ) return;    
  
  int ppid = pdgId[index];

  if ( status[index] < 4 )
    {
      depth++;  
      stream << "  ";
      if ( depth > 1 ) for (int i = 1; i < depth; i++) stream << ". ";
      
      stream.setf(std::ios::left);  // left justify
      
      if ( printLevel <= 0 )
        {
          if ( abs(ppid) < 4 )
            if ( ppid < 0 )
              stream << std::setw(16) 
                     << std::string("q~                  ").substr(0,16);
            else
              stream << std::setw(16) 
                     << std::string("q                   ").substr(0,16);
          else
            stream << std::setw(16) << kit::particleName(ppid);
        }
      else
        stream << std::setw(16) << kit::particleName(ppid);
      
      if ( abs(printLevel) > 1 )
        {
           stream.setf(std::ios::right);  // right justify  
          stream.precision(4);
          stream << "  "	    
                 << "(" 
                 << pt[index]  << ", "
                 << eta[index] << ", "  
                 << phi[index] << ", "
                 << mass[index]
                 << ")";
        }
      if ( abs(printLevel) > 2 )
        {
          stream << "\t" 
                 << pdgId[index] <<  " "
                 << index << " "
                 << status[index];
        }
      stream << std::endl;
    }
  
  if ( firstDaughter[index] < 0 ) return;

  if ( printLevel <= 0 )
    // Ignore daughters of non-top quarks, gluons and taus
    if ( abs(ppid) <   6 ||
         abs(ppid) == 15 ||
         abs(ppid) == 21 ) return;
  
  // If daughter is just a clone of parent then use index of clone instead
  // of that of the cloned particle. We do this to get around an artifact of
  // StdHep.

  int dindex = firstDaughter[index];
  if ( pdgId[dindex] == ppid ) index = dindex;
  
  // Find daughters. Sort them in order of increasing PDG ID.
  std::vector<PID> d;
  for (int dindex = firstDaughter[index]; dindex <= lastDaughter[index]; 
       dindex++)
    {
      PID p; 
      p.index = dindex;
      p.name  = kit::particleName(pdgId[dindex]);
      d.push_back(p);
    }
  std::sort(d.begin(), d.end());
  
  // Loop over daughters
  for (int i=0; i < (int)d.size(); i++)
    printTree(stream,
              printLevel,
              d[i].index,
              maxdepth,
              depth);
}

std::string
gEvent::tree(int      printLevel,
             int      index,
             int      maxdepth,
             int      depth)
{
  std::ostringstream os;
  if ( index < 0 )
    {
      printTree(os, printLevel, 0, maxdepth, depth);
      printTree(os, printLevel, 1, maxdepth, depth);
    }
  else
    printTree(os, printLevel, index, maxdepth, depth);
  
  return os.str();
}
