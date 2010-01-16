//-----------------------------------------------------------------------------
// File:        generateEnsemble.C
// Description: Generate an ensemble of fake data-sets by sampling
// events from MC files specified in a file-list. Optionally, allow the
// scaling of event weights for any file in the filelist.
//
// Created:     19-Oct-2006 Harrison B. Prosper & Supriya Jain
//-----------------------------------------------------------------------------
#include <vector>
const int NSAMPLES = 10; // Number of samples to generate

void generateEnsemble()
{
  loadlibs();

  //-------------------------------------------------------------------------
  // Get command line arguments
  //--------------------------------------------------ar---------------------
  string varlist;      // File name containing variables to be written out
  string filelist;     // File name containing list of input filenames
  int nsamples=0;      // Number of data-sets to generate
  getoptions(varlist, filelist, nsamples);
  
  //-------------------------------------------------------------------------
  // Read file list
  //-------------------------------------------------------------------------
  ifstream inpf(filelist.c_str());
  if ( ! inpf.good() ) error("unable to open " + filelist);
    
  cout << endl;
  cout << "Files" << endl;
  
  vector<double> scalefactor;
  vector<string> filename;
  string rec;
  while ( getline(inpf, rec) )
    {
      if ( rec == "" ) continue;
      if ( rec[0] == "#" ) continue;
      istringstream line(rec.c_str());
      string name;
      double scale=1.0;
      line >> name >> scale;
      filename.push_back(name);
      scalefactor.push_back(scale);
      cout << "\t" << name << "\t" << scale << endl << endl;
    }
  inpf.close();
  
  //-------------------------------------------------------------------------
  // Read variable list
  //-------------------------------------------------------------------------
  ifstream inpv(varlist.c_str());
  if ( ! inpv.good() ) error("unable to open " + varlist);
  
  cout << endl;
  cout << "Variables" << endl;
  vector<string> varname;
  while ( inpv >> rec )
    {
      if ( rec == "" ) continue;
      if ( rec[0] == "#" ) continue;
      if ( rec[0] == "/" )  continue;
      varname.push_back(rec);
      cout << "\t" << varname.back() << endl;
    }
  inpv.close();
  
  //---------------------------------------------------------------------------
  // Read data
  //---------------------------------------------------------------------------
  cout << "\nReading data..." << endl;
  vector<double> weight;  
  vector<vector<double> > data;  
  vector<int> filenumber;

  int nevents = 0;
  for(unsigned i=0; i < filename.size(); i++)
    {
      Slurper stream(filename[i]);
      if ( ! stream.good() ) error("Can't open file " + filename[i]);
      cout << "\t" << filename[i] << "\t" << stream.size() << endl;

      nevents += stream.size();
      while ( stream.read() )
	{ 
	  filenumber.push_back(i);
	  weight.push_back( stream.get("weight") );
	  data.push_back( stream.vget(varname) );
	}
    }

  //---------------------------------------------------------------------------
  // Get total weight of files
  //---------------------------------------------------------------------------
  cout << "Total number of events: " << nevents << endl;

  double totalweight = 0;
  vector<double> wcdf(nevents);
  vector<double> wt(filename.size(),0);

  for(int entry=0; entry < nevents; entry++)
    {
      if ( entry % 1000 == 0 ) cout << entry << endl;
      
      // Scale event weight in current file by scalefactor
      
      double wgt = weight[entry] * scalefactor[filenumber[entry]];
      wt[filenumber[entry]] += wgt;
      
      if ( entry == 0 )
	wcdf[entry] = wgt;
      if ( entry > 0 )
	wcdf[entry] = wcdf[entry-1] + wgt;
    }

  double totalweight = wcdf[nevents-1];
  for(unsigned j=0; j < filename.size(); j++)
    cout << filename[j] << "\t" << wt[j] << " events" << endl;
  cout << "Summed weight: " << totalweight << " events" << endl;
  cout << "Generate " << nsamples << " samples" << endl;

  //---------------------------------------------------------------------------
  // Generate an ensemble of size nsamples data-sets
  //---------------------------------------------------------------------------
  string name(kit::nameonly(filelist));

  TRandom3 random;

  for (int sample=0; sample < nsamples; sample++)
    {
      //-------------------------------------------------------------------
      // Generate a count from a Poisson distribution with mean equal to 
      // summed event weight and select Ndata events at random from the 
      // chain of events using a modified binary search. Write out each 
      // selected event.
      //-------------------------------------------------------------------
      
      //-------------------------------------------------------------------
      // Open output stream. But first create a directory whose name is
      // the same as the file list, if it does not yet exist.
      //-------------------------------------------------------------------
      char outfilename[256];
      char shellcommand[256];	  
      sprintf(shellcommand, "mkdir -p %s", name.c_str());
      gSystem->Exec(shellcommand);

      sprintf(outfilename, 
	      "%s/data_%5.5d.txt",  
	      name.c_str(), 
	      sample);
      cout << endl 
	   << "Writing sample file: " << outfilename << endl;
 
      ofstream out(outfilename);

      char record[80];
      sprintf(record, "%16s", "weight");
      out << record;
      for(unsigned i=0; i < varname.size(); i++)
	{
	  sprintf(record, "%16s", varname[i].c_str());
	  out << record;
	}
      sprintf(record, "%16s", "filenumber");
      out << record << endl;

      //-------------------------------------------------------------------
      // Loop over input stream and write out Ndata selected events
      //-------------------------------------------------------------------
      double totalweight = wcdf[nevents-1];
      int Ndata = random.Poisson(totalweight);

      cout << "\tNumber of events in current sample " << Ndata << endl;
 
      for(int entry=0; entry < Ndata; entry++)
	{
	  double w = random.Uniform(0, totalweight);
	  int row = binsearch(wcdf, w);
	  if ( row < 0 )
	    error("no event found...shouldn't happen!");

	  if ( entry % 1000 == 0 )
	    cout << "\tevent: " << entry << "\t" << row << endl;

	  // Write out selected event

	  sprintf(record, "%16.4e", 1);
	  out << record;
	  for(unsigned i=0; i < varname.size(); i++)
	    {
	      sprintf(record, "%16.4e", data[row][i]);
	      out << record;
	    }
	  sprintf(record, "%16d", filenumber[row]);
	  out << record << endl;
	}
      out.close();
      }
  exit(0);
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

void 
error(string message)
{
  cout << "** " << message << endl;
  exit(0);
}

void 
loadlibs()
{
  if (gSystem->Load("libFWCoreFWLite.so")!=0)
    error("unable to load libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  if (gSystem->Load("libPhysicsToolsLiteAnalysis.so")!=0)
    error("unable to load libPhysicsToolsLiteAnalysis.so");
}

void 
getoptions(string& varlist, string& filelist, int& nsamples)
{
  varlist  = "";
  filelist  = "";
  int i = 0;
  while ( i < gApplication->Argc() )
    {
      string switch(gApplication->Argv(i));
      {
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
	else if ( switch == "--nsamples" )
	  {
	    i++;
	    nsamples = atoi(gApplication->Argv(i));
	  }
	i++; // Don't forget to increment!
      }
    }
  if ( filelist == "" || varlist == "") 
    error("Usage:\n\t root -l generateEnsemble.C with options\n"
	  "\t\t--filelist  filename of file list\n"
	  "\t\t--varlist   filename of variable list\n"
  	  "\t\t--nsamples  number of samples to generate [10]\n");
  
  if (nsamples <= 0)
    nsamples = NSAMPLES;
  cout << "\n\tWill generate " << nsamples << " data-sets\n" << endl;
}
//----------------------------------------------------------------------------
// Binary search - Slightly modified version of
// http://personal.denison.edu/~havill/102/python/search.py
//----------------------------------------------------------------------------
int  
binsearch(vector<double>& L,  double item)
{
  int first = 0;
  int last = L.size() - 1;
  bool found = false;
  while ((first <= last) && ! found)
    {
      int mid = (first + last) / 2;
      if  ( item < L[mid] )
	last = mid - 1;
      else if (item > L[mid])
	first = mid + 1;
      else
	found = true;
      
      if ( first > last )
	{
	  mid = first;
	  found = true;
	}
      if ( found ) return mid;
    }
  return -1;
}

