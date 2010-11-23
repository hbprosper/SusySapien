//----------------------------------------------------------------------------
// File: jetnet.cc
// Purpose: Wrapper for JETNET v3.4
// Created: 21-Jan-1999 Harrison B. Prosper
// Updated:  6-Oct-2000 HBP (Add features needed for 
//                           ACAT 2000 workshop talk)
//          27-Jun-2005 HBP Adapt to CMS project structure
//          04-Jul-2005 HBP Add mean and sigmas
//          03-Oct-2005 HBP Add option to return error rate
//          14-Feb-2006 HBP Make it possible to continue training an
//                      existing network
// $Revision:$
//----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <stdio.h>

#include "PhysicsTools/Mkntuple/interface/network.h"
#include "PhysicsTools/Mkntuple/interface/jetnet.h"
#include "PhysicsTools/Mkntuple/interface/kit.h"

using namespace jetnet;
using namespace std;

#ifdef __WITH_CINT__
ClassImp(NN)
#endif

// Definitions in global scope

// External functions

extern "C" 
{
  void jninit_(void);
  void jntral_(void);
  void jntest_(void);

  void jnreadweights_   (const char* filename, int* status, int);
  void jndumpparams_    (void);
  void jndumpweights_   (const char* filename, int);
  void jndumpweightsmlp_(const char* filename, int);
  void jnwritename_     (const char* filename, float& mean, float& sigma, int);
  void jncloseweights_  (int& outtype);
}

//  Declare JETNET common blocks

const int MAXI = 50000;
const int MAXO = 1000;

extern struct jndat1 
{
  int   mstjn[40];
  float parjn[40];
  int   mstjm[20];
  float parjm[20];
  float oin[MAXI];
  float out[MAXO];
  int   mxndjm;
} jndat1_; // Note: all FORTRAN names are postfixed with an "_"

extern struct jndat2
{
  float  tinv[10];
  int    igfn[10];
  float  etal[10];
  float  widl[10];
  float  satm[10];
} jndat2_; // Note: all FORTRAN names are postfixed with an "_"

double sigmoid(double x)
{
  return tanh(x);
}

double sigmoidout(double x)
{
  return 1.0/(1.0+exp(-2*x));
}

// Constructor

NN::NN(string var, int hidden, Output outType)
  : _status(kSUCCESS),
    _sample(kTRAINING),
    _outputType(0),
    _initialized(false)
{ 
  _init(var, hidden, outType); 
}

NN::NN(vector<string>& variables, int hidden, Output outType)
  : _status(kSUCCESS),
    _sample(kTRAINING),
    _outputType(0),
    _initialized(false)
{
  string var("");
  for(int i=0; i < (int)variables.size(); i++) var += variables[i] + '\t';
  _init(var, hidden, outType);
}
    
// Constructor
// Already trained network. Load weights from file

NN::NN(string filename)
  : _status(kSUCCESS),
    _sample(kTESTING),
    _outputType(0),
    _initialized(false)
{
  _nodes.clear();
  _wgt.clear();
  _mean.clear();
  _sigma.clear();

  if ( ! _load(filename, 1) )
    {
      cout << "NN::NN Failed to load network " << endl;
      exit(0);
    }
  _init();
  _initialized = true;
}

  
// Destructor

NN::~NN(){}


// Setters
//////////

void NN::setParameter(string name, float val)
{
  if ( _id.find(name) == _id.end() ) return;
  _id[name].value = val;
  _id[name].set   = true;
  _setParameter(name);
}

void NN::setMethod(int val)
{
  setParameter("method",val);
}

void NN::setEta(float val)
{
  setParameter("eta",val);
}

void NN::setAlpha(float val)
{
  setParameter("alpha",val);
}

void NN::setEpsilon(float val)
{
  setParameter("alpha",val);
}

void NN::setWidth(float val)
{
  setParameter("width",val);
}

void NN::setDalpha(float val)
{
  setParameter("dalpha",val);
}

void NN::setDeta(float val)
{
  setParameter("deta",val);
}

void NN::setSample(Sample sample)
{
  _sample = sample;
}

void NN::setPattern(vfloat& inp, 
                    float   out, 
                    float   weight)
{
  // Check size of inputs and outputs
  if ( (int)inp.size() < _ninput )
    {
      _status = kBADINPSIZE;
      cout << "NN::setPattern - mis-match in length of input data" << endl;
      cout << "               - inp.size(): " << inp.size() << endl;
      cout << "               - ninput:     " << _ninput << endl;
      exit(0);
    }

  _input[_sample].push_back(inp); // Makes a copy of object
  _output[_sample].push_back(out);
}

void NN::setPattern(vdouble& inp, 
                    double   out, 
                    double   weight)
{
  vector<float> input(inp.size());
  for(int i=0; i < (int)inp.size(); i++) input[i] = (float)inp[i];
  float output = (float)out;
  setPattern(input, output);
}

void NN::loadPatterns(vvfloat& inp, 
		      vfloat&  out, 
		      vfloat&  weight)
{
  for (int i = 0; i < (int)inp.size(); i++)
    setPattern(inp[i], out[i], weight[i]);
}

// Getters
//////////

float NN::parameter(string name)
{
  float val = 0;
  if ( _id.find(name) == _id.end() ) 
    return val;

  ID a = _id[name];
  if      ( a.type == 0 )
    val = (float)jndat1_.mstjn[a.index];
  else if ( a.type == 1 )
    val = jndat1_.parjn[a.index];
  else if ( a.type == 4 )
    val = (float)jndat2_.igfn[_nlayer-2];
  return val;
}

vstring& NN::names()
{
  return _var;
}


// Run neural network and return results
///////////////////////////////////////////////////////////
float NN::evaluate(vfloat& inp)
{
  // load pattern into array oin(*) 

  for (int j=0; j < _ninput; j++)
    {
      jndat1_.oin[j] = (inp[j] - _mean[j]) / _sigma[j];
    }
  // Run network

  jntest_(); 

  // Get network output

  return jndat1_.out[0];
}

float NN::evaluate(vdouble& inp)
{
  // load pattern into array oin(*) 

  for (int j=0; j < _ninput; j++)
    {
      jndat1_.oin[j] = (inp[j] - _mean[j]) / _sigma[j];
    }
  // Run network

  jntest_(); 

  // Get network output

  return jndat1_.out[0];
}

void NN::save(string file, bool savecpp)
{
  // JETNET format

  file = kit::truncate(file,".");
  string file1 = file + ".jetnet";
  jndumpweights_(file1.c_str(), file1.length());

  // MLPfit format

  string file2 = file + ".net";
  jndumpweightsmlp_(file2.c_str(), file2.length());

  for (int i = 0; i < (int)_var.size(); i++)
    jnwritename_(_var[i].c_str(), _mean[i], _sigma[i], _var[i].size());
  
  jncloseweights_(_outputType);
  
  if ( savecpp ) _saveCPP(file);
}


void NN::printParameters(int flag)
{
  cout << "Network Parameters\n";
  if     ( flag == 0 )
    {
      for (mid::iterator it = _id.begin(); it != _id.end(); it++)
	{
	  string key = (*it).first + ".......................................";
	  ID id = (*it).second;

	  cout << key.substr(0,20); 
	  if      (id.type == 0)
	    cout << " MSTJN[" << setw(2) << id.index+1 << "] = " 
		 << setw(10)  << jndat1_.mstjn[id.index]; 
	  else if (id.type == 1)
	    cout << " PARJN[" << setw(2) << id.index+1 << "] = " 
		 << setw(10)  << jndat1_.parjn[id.index];
	  else if (id.type == 4)
	    cout << "  IGFN[" << setw(2) << _nlayer-1 << "] = " 
		 << setw(10)  << jndat2_.igfn[_nlayer-2];
	  cout << endl;
	}
    }
  else if ( flag == 1 ) 
    {      
      for ( int i = 0; i < 40; i++ )
	{
	  printf
	    ("mstjn[%2d] = %d\t mstjn[%2d] "
	     "= %d\t parjn[%2d] = %f\t parjn[%2d] = %f\n",
	     i+1,jndat1_.mstjn[i], i+2,jndat1_.mstjn[i+1],
	     i+1,jndat1_.parjn[i], i+2,jndat1_.parjn[i+1]); i++;
	}
    }
  else
    {
      jndumpparams_();
    }
}

bool NN::begin(string filename)
{
  _sample = kTRAINING; // IMPORTANT, set to training sample
  _power  = 0;

  if ( filename == "" )
    {
      // Method and width must be set before jninit

      _setParameter("method");
      _setParameter("width");
      
      // Initialize JETNET
      jninit_();
      
      _setParameter("alpha");
      _setParameter("eta");
      _setParameter("deta");
      _setParameter("dalpha");
    }
  else
    {
      // load existing weights
      if ( ! _load(filename, 1) ) return false;
    }

  // Define number of updates per cycle

  int  patterns_per_update = (int)parameter("patternsPerUpdate");
  int  updates_per_cycle   = _input[_sample].size()/patterns_per_update;
  if ( updates_per_cycle < 1 ) updates_per_cycle = 1;
  
  setParameter("updatesPerCycle", (float)updates_per_cycle);

  // Randomly shuffle patterns
  _setpattern(kTRAINING);
  _setpattern(kTESTING);
  // scale data
  _findscale();

   // Improve error handling later!
  return true;
}

void NN::end()
{
  save(".jetnet",false);  // do not save cpp file 
  _load(".jetnet",0);     // load weight file in MLP format
}

float NN::train(int epoch)
{
  // Training loop 
 
  for (int p=0; p < (int)_input[_sample].size(); p++ )
    {
      // load pattern into array oin(*) 

      for (int j=0; j < _ninput; j++)
	jndat1_.oin[j] = (_input[_sample][p][j] - _mean[j]) / _sigma[j];

      // load target into array out(*) 

      jndat1_.out[0] = _output[_sample][p];
	   
      // apply training algorithm 

      jntral_();

    } // End of training loop
 
  return parameter("error");
}


float NN::test(Sample sample, float cutpoint, int nbin)
{
  _status = kSUCCESS;
  if ( _input.find(sample) == _input.end() )
    {
      _status = kBADSAMPLE;
      return -99.0;
    }

  // Clear histograms

  _es.clear();
  _eb.clear();
  _s.clear();
  _b.clear();
  for (int k = 0; k < nbin; k++ )
    {
      _es.push_back(0);
      _eb.push_back(0);
      _s.push_back(0);
      _b.push_back(0);
    }

  // Testing loop

  int total = 0;
  _rms = 0.0;      // rms error 
  _error = 0.0;    // mis-classification rate
  _divergencebyMC = 0;

  for (int p=0; p < (int)_input[sample].size(); p++ ) // Begin testing loop
    {
      // load pattern into array oin(*) 

      for (int j=0; j < _ninput; j++)
	jndat1_.oin[j] = (_input[sample][p][j] - _mean[j]) / _sigma[j];

      // Run network

      jntest_(); 

      // Network output

      float out = jndat1_.out[0];
      float target = _output[sample][p];
      int   bin = (int)(out * nbin);

      // Apply cut

      if ( target > 0.5 )
	{
	  if ( out < cutpoint ) _error += 1;
	}
      else
	{
	  if ( out > cutpoint ) _error += 1;
	}

      double x = out - target;
      _rms += x*x;

      // Fill histograms

      if ( target > 0.5 )
	{	  
	  _s[bin]++;
	  if ( out != 1.0 )
	    {
	      _divergencebyMC += log(out/(1-out));
	      total++;
	    }
	}
      else
	{
	  _b[bin]++;
	}
    }

  // Compute some useful statistics

  nnefficiencies(_s, _es);
  nnefficiencies(_b, _eb);
  _area  = nnarea(_eb, _es);
  _power = nnpower(_s, _b);
  _divergence = nndivergence(_s, _b);

  if ( total > 0 ) _divergencebyMC /= total;

  _error = _error/_input[sample].size();
  _rms   = sqrt(_rms/_input[sample].size());

  return _rms;
}

vint& NN::histogram(int target)
{
  if ( target > 0.5 )
    return _s;
  else
    return _b;
}

vfloat& NN::efficiencies(int target)
{
  if ( target > 0.5 )
    return _es;
  else
    return _eb;
}

float NN::power()
{
  return _power;
}

float NN::area()
{
  return _area;
}

float NN::divergence(Divergence how)
{
  if ( how == kBYBINNING )
    return _divergence;
  else
    return _divergencebyMC;
}

float NN::error()
{
  return _error;
}

bool NN::good()
{
  return (_status == 0);
}


// Internal methods
///////////////////

// Read contents of weight file

bool NN::_load(string filename, int which)
{
  if ( which == 1 )
    {
      filename = kit::truncate(filename,".")+".jetnet";
      int status;
      jnreadweights_(filename.c_str(), &status, filename.length());
    }
  
  // Read weight file in MLP format; extension must be "net"

  filename = kit::truncate(filename,".")+".net";
  if ( nnload(filename.c_str(), 
	      _nodes, _wgt, _var, _mean, _sigma,
	      _outputType) == 0 )
    {
      _status = kSUCCESS;
      return true;
    }
  else
    {
      _status = kFILEOPENERROR;
      return false;
    }
}

void NN::_findscale()
{
  Sample sample=kTRAINING;
  int npat = _input[sample].size();
  _mean.clear();
  _sigma.clear();
  for (int j = 0; j < _ninput; j++)
    {
      _mean.push_back(0);
      _sigma.push_back(0);
    }

  for (int p = 0; p < npat; p++)
    {
      for (int j = 0; j < _ninput; j++)
	{
	  float x = _input[sample][p][j];
	  _mean[j]  += x;
	  _sigma[j] += x*x;
	}
    }

  for (int j = 0; j < _ninput; j++)
    {
      _mean[j]  /= npat;
      _sigma[j] /= npat;
      _sigma[j] /= sqrt(_sigma[j] - _mean[j] * _mean[j]);
    }
}

void NN::_setpattern(Sample sample)
{
  _status = kSUCCESS;
  if ( _input.find(sample) == _input.end() )
    {
      _status = kBADSAMPLE;
      return;
    }

  int npat = _input[sample].size();

  // Randomize order of patterns

  vector<int> rows(npat);
  for (int i = 0; i < npat; i++) rows[i] = i;
  random_shuffle(rows.begin(), rows.end());

  // Copy to temporary buffers

  vvfloat ibuff(npat);
  vfloat  obuff(npat);

  for (int i = 0; i < npat; i++)
    {
      int p = rows[i];
      ibuff[i] = vfloat(_ninput);

      copy(_input[sample][p].begin(),
	   _input[sample][p].end(), ibuff[i].begin());  

      obuff[i] = _output[sample][p];
    } 

  // Copy back randomized patterns from temporary buffers

  copy(ibuff.begin(), ibuff.end(), _input[sample].begin());
  copy(obuff.begin(), obuff.end(), _output[sample].begin());
}

void NN::_init(string vars, int hidden, Output outType)
{
  // Array code
  // mstjn: 0
  // parjn: 1
  // mstjm; 2
  // parjm: 3
  // igfn:  4

  ID a;

  // MSTJN
  a.type  = 0;
  a.set   = false;
  a.value = 0;
  a.index = 0; _id["numberLayers"]      = a;
  a.index = 1; _id["patternsPerUpdate"] = a;
  //a.index = 2; _id["sigmoidFunction"]   = a;
  a.index = 3; _id["errorMeasure"]      = a;
  a.index = 4; _id["method"]            = a;
  a.index = 5; _id["statFile"]          = a;
  a.index = 8; _id["updatesPerCycle"]   = a;
  a.index = 9; _id["inputNodes"]        = a;
  a.index =30; _id["warningProcedure"]  = a;
  a.index =31; _id["maximumWarnings"]   = a;

  a.type  = 4;
  a.index = 0; _id["outputType"] = a;

  // PARJN
  a.type  = 1; 
  a.index = 0; _id["eta"]   = a;
  a.index = 1; _id["alpha"] = a;
  a.index = 2; _id["beta"]  = a;
  a.index = 3; _id["width"] = a;
  a.index = 8; _id["error"] = a; 
  a.index =10; _id["deta"]  = a;
  a.index =11; _id["dalpha"]= a;
  a.index =12; _id["dbeta"] = a;
  a.index =13; _id["lambda"]= a;
  a.index =14; _id["dlambda"] = a;
  a.index =15; _id["gamma"] = a;
  a.index =16; _id["gammaCutoff"] = a;
  a.index =17; _id["scaleParameter"] = a;

  if ( vars != "" )
    {
      // This is a new network

      kit::split(vars.c_str(), _var);

      _input[kTRAINING] = vector<vfloat>();
      _input[kTESTING]  = vector<vfloat>();
      
      _output[kTRAINING]= vector<float>();
      _output[kTESTING] = vector<float>();

      // Set up vector of node counts
      _nodes.clear();
      _nodes.push_back(_var.size());
      _nodes.push_back(hidden);
      _nodes.push_back(1);
    }

  _ninput  = _nodes[0];    // Number of inputs
  _nhidden = _nodes[1];
  _nlayer  = _nodes.size();
  _noutput = 1;

  if ( vars != "" )
    {
      int loc;
      loc = _id["numberLayers"].index; jndat1_.mstjn[loc] = _nlayer;  
      loc = _id["inputNodes"].index;
      copy(_nodes.begin(), _nodes.end(), &jndat1_.mstjn[loc]);
      
      jndat1_.mstjn[34] = 50; // Max. iterations allowed in line search
      jndat1_.mstjn[35] = 50; // Max. allowed restarts in line search.

      // Determine sigmoid functions to use
  
      // MSTJN(3) (D=1)      overall transfer function used in net
      // 1 -> g(x)=1/(1+exp(-2x))
      // 2 -> g(x)=tanh(x)
      // 4 -> g(x)=x
      
      // Hard-code internal transfer function
  
      jndat1_.mstjn[2] = 2; // Sigmoid output
      
      // Determine output node type
      
      if ( outType == kSIGMOID )
	{
	  _outputType = 0;
	  jndat2_.igfn[_nlayer-2] = 1; // Sigmoid output
	}
      else
	{
	  _outputType = 1;
	  jndat2_.igfn[_nlayer-2] = 4; // Linear output
	}
    }
}

void NN::_setParameter(string name)
{
  if ( _id.find(name) == _id.end() ) return;
  if ( !_id[name].set ) return;

  if      ( _id[name].type == 0 )
    jndat1_.mstjn[_id[name].index] = (int)_id[name].value;
  else if ( _id[name].type == 1 )
    jndat1_.parjn[_id[name].index] = _id[name].value;
}


// Write out C++ function

void NN::_saveCPP(string filename)
{
  filename = filename + ".cpp";

  if ( nnsaveCPP("Python module jetnet",
		 "JETNET Version 3.4",
		 "tanh(x)",
		 "1.0/(1+exp(-2*x))",
		 filename.c_str(),
		 _nodes,
		 _wgt,
		 _var,
		 _mean,
		 _sigma,
		 _outputType) == 0 )
    _status = kSUCCESS;
  else
    _status = kFAILURE;
}













