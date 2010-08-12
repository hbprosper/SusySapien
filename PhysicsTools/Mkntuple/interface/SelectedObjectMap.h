#ifndef SELECTEDOBJECTMAP_H
#define SELECTEDOBJECTMAP_H
//
// Package:    PhysicsTools/Mkntuple
//             SelectedObjectMap.h
//
//             A singleton object to make indices of selected objects 
//             available to whoever needs it
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Jun  8, 2010
//
// $Id: SelectedObjectMap.h,v 1.2 2010/04/21 02:22:43 prosper Exp $

#include <map>
#include <string>
#include <vector>

/// A singleton class to cache indices of objects to be written out.
class SelectedObjectMap
{
public:
  ///
  static SelectedObjectMap& instance()
  {
    static SelectedObjectMap singleton;
    return singleton;
  }

  ///
  void set(std::string name, int index) 
  { 
    if ( selectionmap_.find(name) == selectionmap_.end() )
      selectionmap_[name] = std::vector<int>();
    selectionmap_[name].push_back(index);
  }

  ///
  std::map<std::string, std::vector<int> >& get()
  { 
    return selectionmap_;
  }

  void clear()
  {
    selectionmap_.clear();
  }

private:
  SelectedObjectMap() {}                    // prevent explicit creation
  ~SelectedObjectMap() {}                   // prevent explicit destruction
  SelectedObjectMap(const SelectedObjectMap&);            // prevent copy
  SelectedObjectMap& operator=(const SelectedObjectMap&); // prevent assignment
  
  std::map<std::string, std::vector<int> > selectionmap_;
};

#endif
