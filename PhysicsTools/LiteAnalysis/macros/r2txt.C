//-----------------------------------------------------------------------------
// File:        r2txt.C
//
// Description:
//
// Read one or more root files, whose names are specified in a file list,
// and write out the variables given a variables file to a text file with a 
// name constructed from that of the file list. Finally, randomly shuffle the 
// rows in the text file.
//
// Usage:
//       root -l r2txt.C    --filelist <filelist> 
//                          --varlist  <variables-list>
//                          --treename <tree-name>
//
// Uses treeutil_cpp.so
//
// Created:     04-Dec-2005 Harrison B. Prosper
// Updated:     21-Oct-2006 HBP & Daekwang Kau
//              31-Mar-2008 HBP minor changes
//-----------------------------------------------------------------------------
#include <vector>

void r2txt()
{
  cout << "\n\tr2txt\n" << endl;

  //---------------------------------------------------------------------------
  // Get command line arguments
  //---------------------------------------------------------------------------
  string filelist;  // File name of file containing list of root file names
  string varlist;   // File name containing variables to be written out
  string treename;  // Name of tree to be read
  getoptions(filelist, varlist, treename);

  //---------------------------------------------------------------------------
  // Read file names
  //---------------------------------------------------------------------------
  vector<string> filename;
  readfilelist(filelist, filename);

  //---------------------------------------------------------------------------
  // Read variable list
  //---------------------------------------------------------------------------
  vector<string> varname;
  readvarlist(varlist, varname);

  //---------------------------------------------------------------------------
  // Load itreestream class
  //---------------------------------------------------------------------------
  cout << "Load treeutil_cpp.so" << endl;
  if ( gSystem->Load("treeutil_cpp.so") != 0 )
    error("\nUnable to load treeutil_cpp.so; check LD_LIBRARY_PATH\n");

  //---------------------------------------------------------------------------
  // Open stream of root files and select variables to be read
  //---------------------------------------------------------------------------
  itreestream stream(filename, treename);
  if ( ! stream.good() ) error("Can't open file " + filename);

  // Define buffer to receive data for variables

  vector<float> buffer(varname.size());
  for(int i=0; i < (int)varname.size(); i++)
    {
      if ( ! stream.present(varname[i]) ) 
	error("Can't find variable " + varname[i]);
      stream.select(varname[i], buffer[i]);
    }

  //---------------------------------------------------------------------------
  // Open output text file
  //---------------------------------------------------------------------------
  string outfilename(nameonly(filelist)+".dat");

  cout << endl 
       << "Output file: " << outfilename << endl;

  int entries = stream.entries();
  cout << "Number of entries " << entries << endl;

  ofstream out(outfilename.c_str());
  char record[80];

  sprintf(record, "%16s%16s", "entry","eventcode");
  out << record;
  for(int i=0; i < (int)varname.size(); i++)
    { 
      sprintf(record, "%16s", varname[i].c_str());
      out << record;
    }
  out << endl;

  //---------------------------------------------------------------------------
  // Loop over input stream and write out selected data
  //---------------------------------------------------------------------------
  int modrep = entries / 20;
  modrep = modrep / 500;
  modrep = modrep * 500;
  for(int entry=0; entry < entries; entry++)
    {
      stream.read(entry);

      int number = stream.number();
      
      if ( entry % modrep == 0 )
	cout << entry
	     << "\t" << number
	     << endl;

      sprintf(record,"%16d%16d", entry, number);
      out << record;

      for(int i=0; i < (int)buffer.size(); i++)
	{
	  sprintf(record,"%16.4e", buffer[i]);
	  out << record;
	}
      out << endl;
    }
  stream.close();
  out.close();

  cout << "Shuffle rows in text file " << outfilename << endl;
  shuffle_table(outfilename);
  exit(0);
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------
void error(string message)
{
  cout << "** " << message << endl;
  exit(0);
}

void getoptions(string& filelist,
		string& varlist, 
		string& treename)
{
  filelist = "";
  varlist  = "";
  treename = "";

  int i = 0;
  while ( i < gApplication->Argc() )
    {
      string switch(gApplication->Argv(i));
      if ( switch == "--filelist" )
        {
          i++;
          filelist = string(gApplication->Argv(i));
        }
      else if ( switch == "--varlist" )
	{
	  i++;
	  varlist = string(gApplication->Argv(i));
	}
      else if ( switch == "--treename" )
	{
	  i++;
	  treename = string(gApplication->Argv(i));
	}
      i++; // Don't forget to increment!
    }
  if ( filelist == "" || varlist == "")
    {
      cout << 
	"Usage:\n"
	" r2txt --filelist <filename>\n"
	"       --varlist  <filename>\n"
	"      [--treename <name>]\n"
	" If treename is not given, the first tree in the file is used."
	   << endl;
      exit(0);
    }
}

void readfilelist(string filelist, vector<string>& filename)
{
  string record;
  ifstream inpf(filelist.c_str());
  if ( ! inpf.good() ) error("unable to open " + filelist);

  cout << endl;
  cout << "Files" << endl;
  while ( inpf >> record )
    {
      if ( record == "" ) continue;
      if ( record[0] == "#" ) continue;
      filename.push_back(record);
      cout << "\t" << record << endl;
    }
  inpf.close();
}

void readvarlist(string varlist, vector<string>& varname)
{
  string record;
  ifstream inpv(varlist.c_str());
  if ( ! inpv.good() ) error("unable to open " + varlist);

  cout << endl;
  cout << "Variables" << endl;

  while ( inpv >> record )
    {
      if ( record == "" ) continue;
      if ( record[0] == "#" ) continue;
      varname.push_back(record);
      cout << "\t" << record << endl;
    }
  inpv.close();
}
