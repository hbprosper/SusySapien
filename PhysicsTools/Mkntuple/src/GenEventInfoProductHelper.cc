//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Mkntuple helper class for GenEventInfoProduct
// Created:     Wed Feb 16 01:43:26 2011
// Author:      Harrison B. Prosper      
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include "PhysicsTools/Mkntuple/interface/GenEventInfoProductHelper.h"
#include "LHAPDF/LHAPDF.h"
//-----------------------------------------------------------------------------
using namespace std;
static bool DBGenEventInfo = getenv("DBGenEventInfoProductHelper") > 0; 
//-----------------------------------------------------------------------------
// This constructor is called once per job
GenEventInfoProductHelper::GenEventInfoProductHelper()
  : HelperFor<GenEventInfoProduct>() 
{
  if ( config == 0 )
    throw cms::Exception("NullConfigPointer");

  // Get the PDF set
  pdfsetname_ = string("cteq66.LHgrid");
  npdfset_ = 44;
  try
    {
      pdfsetname_ = config->getUntrackedParameter<string>("PDFSet");
    }
  catch (...)
    {}
  try
    {
      npdfset_ = config->getUntrackedParameter<int>("numberPDFSets");
    }
  catch (...)
    {}
  
  cout << endl << "\t==> using PDF set: " << pdfsetname_ << endl;

  LHAPDF::initPDFSet(1, pdfsetname_);

  pdf1_.clear();
  pdf2_.clear();
  pdfweight_.clear();
  pdfweightsum_.clear();
  for(int i=0; i <= npdfset_; ++i) 
    {
      pdf1_.push_back(0);
      pdf2_.push_back(0);
      pdfweight_.push_back(0);
      pdfweightsum_.push_back(0);
    }
  pdfweight_[0] = 1;
}
    
GenEventInfoProductHelper::~GenEventInfoProductHelper() 
{}


// -- Called once per object
void GenEventInfoProductHelper::analyzeObject()
{
  if ( object == 0 )
    throw cms::Exception("NullObjectPointer");
  if ( !object->hasPDF() ) return;

  const gen::PdfInfo* xpdf = object->pdf();
          
  double x1 = xpdf->x.first;
  double x2 = xpdf->x.second;
  double q  = xpdf->scalePDF;
  int id1   = xpdf->id.first;
  int id2   = xpdf->id.second;

  // Get pdf central values

  LHAPDF::usePDFMember(1, 0);
  pdf1_[0] = LHAPDF::xfx(1, x1, q, id1);
  pdf2_[0] = LHAPDF::xfx(1, x2, q, id2);
  
  if ( DBGenEventInfo )
    {
      char record[256];
      sprintf(record, 
              "    x1 : %10.2e     x2:  %10.2e\n"
              "pdf(x1): %10.2e pdf(x2): %10.2e\tq: %10.2e",
              x1, x2, pdf1_[0], pdf2_[0], q);
      cout << record << endl << endl;
    }

  double w0 = pdf1_[0] * pdf2_[0];
  for(int i=1; i <= npdfset_; ++i)
    {
      LHAPDF::usePDFMember(1, i);
      pdf1_[i] = LHAPDF::xfx(1, x1, q, id1);
      pdf2_[i] = LHAPDF::xfx(1, x2, q, id2);
      pdfweight_[i] = pdf1_[i] * pdf2_[i] / w0;
      pdfweightsum_[i] += pdfweight_[i];
    }
  count = npdfset_+1; // One more than the number of PDF sets
}

// -- Access Methods
double GenEventInfoProductHelper::pdfweight()  const
{
  if ( index <= npdfset_ )
    return  pdfweight_[index];
  else
    return -1;
}

double GenEventInfoProductHelper::pdfweightsum()  const
{
  if ( index <= npdfset_ )
    return  pdfweightsum_[index];
  else
    return -1;
}

double GenEventInfoProductHelper::pdf1()  const
{
  if ( index <= npdfset_ )
    return  pdf1_[index];
  else
    return -1;
}

double GenEventInfoProductHelper::pdf2()  const
{
  if ( index <= npdfset_ )
    return  pdf2_[index];
  else
    return -1;
}
