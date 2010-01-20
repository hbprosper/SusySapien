#ifndef TREESTREAM_HPP
#define TREESTREAM_HPP
//----------------------------------------------------------------------------
// File: treestream.hpp
//
// Description: The classes itreestream and otreestream provide a convenient 
//              interface to the kind of Root trees typically used in HEP
//              analyses. Our (default) assumption is that there is one tree 
//              per Root file and the objects stored are simple types. The
//              treestream classes hide the boilerplate code for getting at 
//              the tree, its branches and leaves.
//
//              The design is based on the observation that, ultimately, data
//              reduce to a set of name/value pairs in which the value 
//              can be regarded as a homogeneous sequence of doubles, floats 
//              longs, or ints. Moreover, an arbitrarily complex graph of the 
//              name/value pairs is readily achieved with, well, name/value 
//              pairs! Name/value pair is just another name for variable.
// 
//              Thinking in terms of objects, in the context of high energy 
//              physics analysis, has yielded undue complexity. On the other 
//              hand, thinking in terms of name/value pairs reduces the 
//              complexity considerably. The irreducible complexity, which 
//              surely exists, properly resides in the graph generated by 
//              the set of name/value pairs, that is, variables.
//
//              Of course, it is often convenient to package name/value pairs 
//              into objects to avail oneself of their useful behaviour. This 
//              may be useful, for example, to model a particularly 
//              complicated sub-graph of name/value pairs. Or it may be that
//              one wishes to take advantage of an object that defines
//              useful functions on the name/value pairs. A  nice example of 
//              packaging name/value pairs for this reason is the packaging 
//              of the name/value pairs, Px, Py, Pz and E into 
//              HepLorentzVector objects to take advantage of the host of 
//              useful mathematical operations these objects define on them.
//
// Created: 19-Feb-2005 Harrison B. Prosper, based on 12-Mar-2001 version of
//                      the same.
//          14-Jul-2005 Allow for arrays
//          13-Aug-2005 Tidy up
//          11-Aug-2007 Add ls() method as a proxy for print
//$Revision: 1.1.2.2 $
//----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <cctype>

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TChain.h"

/** \example readit.py
 */

/** 
 */
namespace
{
  /// Model a name/value pair.
  struct Field
  {
    Field() : srctype(' '),
              iotype(' '),
              isvector(false),
              iscounter(false),
              maxsize(0),
              branch(0),
              leaf(0),
              address(0),
              branchname(""),
              leafname("") {}

    virtual ~Field() {}

    char   srctype;         /// Source type (type of user name/value pair)
    char   iotype;          /// Input/Output type
    bool   isvector;        /// True if vector type
    bool   iscounter;       /// true if this is a leaf counter
    int    maxsize;         /// Maximum number of elements in source variable

    TBranch* branch;        /// Branch pertaining to source
    TLeaf*   leaf;          /// Leaf pertaining to source
    void*    address;       /// Source address

    std::string branchname; /// Name of branch
    std::string leafname;   /// Name of T-leaf!
  };

  template <class T>
  struct FieldBuffer : public Field
  {
    FieldBuffer() : Field(), value(std::vector<T>()) {}
    virtual ~FieldBuffer() {}

    std::vector<T> value;
  };

  typedef std::map<std::string, Field>  Data;
  typedef std::map<std::string, Field*> SelectedData;
}

/** Model an input stream of Root trees.
              The classes itreestream and otreestream provide a convenient 
              interface to the kind of Root trees typically used in HEP
              analyses.  Our (default) assumption is that there is one tree 
              per Root file and the objects stored are simple types. The
              treestream classes hide the boilerplate code for getting at 
              the tree, its branches and leaves.

              The design is based on the observation that, ultimately, data
              reduce to a set of name/value pairs in which the value 
              can be regarded as a homogeneous sequence of doubles, floats 
              longs, or ints. Moreover, an arbitrarily complex graph of the 
              name/value pairs is readily achieved with, well, name/value 
              pairs! Name/value pair is just another name for variable.
 
              Thinking in terms of objects, in the context of high energy 
              physics analysis, has yielded undue complexity. On the other 
              hand, thinking in terms of name/value pairs reduces the 
              complexity considerably. The irreducible complexity, which 
              surely exists, properly resides in the graph generated by 
              the set of name/value pairs, that is, variables.

              Of course, it is often convenient to package name/value pairs 
              into objects to avail oneself of their useful behaviour. This 
              may be useful, for example, to model a particularly 
              complicated sub-graph of name/value pairs. Or it may be that
              one wishes to take advantage of an object that defines
              useful functions on the name/value pairs. A  nice example of 
              packaging name/value pairs for this reason is the packaging 
              of the name/value pairs, Px, Py, Pz and E into 
              <i>HepLorentzVector</i> objects to take advantage of the host of 
              useful mathematical operations these objects define on them.
*/
class itreestream
{
 public:

  ///
  itreestream();

  /** Create an input stream of trees.
      The default assumption is that the file contains a single kind of tree.
      If the file contains multiple trees, you can supply a tree name.
      <p>
      Note: <i>filename</i> is a string of one of more filenames, each of
      which can have wildcard characters.
  */
  itreestream(std::string filename, int bufsize=1000);

  ///
  itreestream(std::vector<std::string>& filenames, int bufsize=1000);

  ///
  itreestream(std::string filename, std::string treename, int bufsize=1000);

  ///
  itreestream(std::vector<std::string>& filenames, std::string treename,
              int bufsize=1000);
  
  ///
  virtual ~itreestream();

  /// True if all is well.
  bool   good();

  /// Return status code.
  int    status();

  /// Check if given variable is present.
  bool   present(std::string name);

  ///
  void   select(std::string namen);

  ///
  void   select(std::vector<std::string>& namen);


  /** Specify the name of a variable to be read and give the address
      to which its value is to be written.
      <p>
      Note: The type of the buffer need not match that of the variable.
  */
  void   select(std::string namen, double& datum);

  ///
  void   select(std::string namen, float& datum);

  ///
  void   select(std::string namen, long& datum);

  ///
  void   select(std::string namen, int& datum);

  ///
  void   select(std::string namen, short& datum);

  ///
  void   select(std::string namen, unsigned long& datum);

  ///
  void   select(std::string namen, unsigned int& datum);

  ///
  void   select(std::string namen, unsigned short& datum);

  /** Specify the name of a vector-valued variable to be read and give the
      address of a buffer into which its values are to be written. 
      The size of the (vector) buffer determines the maximum number of elements
      of the variable to read. You can check that all is well by
      calling the good() method after a call to select. The most likely
      error is failure to provide a buffer of non-zero length.
      <br>
      <b>Note</b>: The type of the buffer need not match that of the variable.
  */
  void   select(std::string namen, std::vector<double>& data);

  ///
  void   select(std::string namen, std::vector<float>& data);

  ///
  void   select(std::string namen, std::vector<long>& data);

  ///
  void   select(std::string namen, std::vector<int>& data);

  ///
  void   select(std::string namen, std::vector<short>& data);

  ///
  void   select(std::string namen, std::vector<unsigned long>& data);

  ///
  void   select(std::string namen, std::vector<unsigned int>& data);

  ///
  void   select(std::string namen, std::vector<unsigned short>& data);


  /** Read tree with ordinal value entry. 
      Return the ordinal value of the
      entry within the current tree.
  */
  int    read(int entry);

  ///
  void   close();

  /// Return number of entries in stream.
  int    entries();

  /// Proxy for entries.
  int    size();

  /// Return the maxium size of name/value pair.
  int    maximum(std::string name);

  /// Return tree identifier.
  std::string  name();
 
  /// Return tree title.
  std::string  title();

  /// Return tree number. This may be useful when we have a chain of files.
  int          number();

  /// Return name of current file.
  std::string  filename();

  /// Return file names.
  std::vector<std::string> filenames();

  /// Return names of name/value pairs.
  std::vector<std::string> names();

  ///
  std::vector<double> vget();

  ///
  double get(std::string namen);

  /// Print information about name/value pairs.
  void   print(std::ostream& out);

  void   ls(std::ostream& out=std::cout) { print(out); }

  /// Proxy for read.
  int    operator[](int entry);

  ///
  TTree* tree();

 private:

  TTree*  _tree;
  TChain* _chain;
  int     _statuscode;
  int     _current;
  int     _entries;
  int     _entry;
  int     _index;
  std::vector<double> _buffer;

  Data         data;
  SelectedData selecteddata;
  
  int     _bufoffset;
  int     _bufcount;
  
  std::map<std::string, int> _bufmap;
  std::vector<std::string>  branchname;
  std::vector<int>          branchtab;
  std::vector<std::string>  filepath;

  void _open(std::vector<std::string>& filenames, std::string treename="");
  void _getbranches(TBranch* branch, int depth);
  void _getleaf    (TBranch* branch, TLeaf* leaf=0);
  void _select     (std::string name, void* address, int maxsize, 
		    char srctype, bool isvector=false);
  void _update();

 public:

#ifdef __WITH_CINT__
  ClassDef(itreestream, 1)
#endif

};

/// Model an output stream of trees of the same species.
class otreestream
{
 public:
  
  otreestream();
  
  /// Create an output stream of trees.
  otreestream(std::string filename, 
              std::string treename, 
              std::string treetitle,
              int complevel=2,
              int bufsize=1000);
  
  /// Create an output stream of trees.
  otreestream(TFile* file,
              std::string treename, 
              std::string treetitle,
              int complevel=2,
              int bufsize=1000);
  
  virtual ~otreestream();

  ///
  bool   good();

  ///
  int    status();

  /** Specify the name of a variable to be added to the tree and the address 
      from which its value is to be read. 
  */
  void   add(std::string namen, double& datum, char iotype='D');

  ///
  void   add(std::string namen, float& datum);

  ///
  void   add(std::string namen, long& datum);

  ///
  void   add(std::string namen, int& datum);

  ///
  void   add(std::string namen, short& datum);

  ///
  void   add(std::string namen, unsigned long& datum);

  ///
  void   add(std::string namen, unsigned int& datum);

  ///
  void   add(std::string namen, unsigned short& datum);

  /** Specify the name of a variable to be added to the tree and and the 
      address from which its value is to be read. <p>Note: <i>name</i> 
      can have the form
      <name>[<countername>], e.g., JetEt[numberJet] if it is of variable
      length. Of course, the variable <i>numberJet</i> must be stored also! 
      For a variable of fixed length use <name>[<fixedcount>], e.g., 
      JetEt[10].
  */
  void   add(std::string namen, std::vector<double>& data, char iotype='D');

  ///
  void   add(std::string namen, std::vector<float>& data);

  ///
  void   add(std::string namen, std::vector<long>& data);

  ///
  void   add(std::string namen, std::vector<int>& data);

  ///
  void   add(std::string namen, std::vector<short>& data);

  ///
  void   add(std::string namen, std::vector<unsigned long>& data);

  ///
  void   add(std::string namen, std::vector<unsigned int>& data);

  ///
  void   add(std::string namen, std::vector<unsigned short>& data);


  ///
  void   add(std::string namen);

  ///
  void   insert(std::vector<double>& data);

  /// Read values of all name/value pairs and store away.
  void   commit();

  ///
  void   autosave(int nevents=1000);

  ///
  void   close(bool closefile=true);

  ///
  int    entries();

  ///
  int    size();

  /// Return tree identifier.
  std::string  name();
 
  ///
  std::string  title();

  ///
  std::vector<std::string>& names();

  ///
  void   print(std::ostream& out);

  void   ls() { print(std::cout); }

  ///
  TFile* file();

  ///
  TTree* tree();

 private:

  TFile* _file;
  TTree* _tree;
  int    _statuscode;
  int    _entries;
  int    _entry;
  int    _idatabuf;
  std::vector<double> _databuf;
  int    _autosavecount;

  SelectedData selecteddata;

  std::vector<std::string>      branchname;

  void _add(std::string name, void* address, int maxsize,
	    char srctype, char iotype, bool isvector=false);

#ifdef __WITH_CINT__
  ClassDef(otreestream, 1)
#endif

};

std::ostream& operator<<(std::ostream& os, itreestream& tuple);
std::ostream& operator<<(std::ostream& os, otreestream& tuple);

#endif
