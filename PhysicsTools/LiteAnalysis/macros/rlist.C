void rlist()
{
  string filename;
  int i = 0;
  while ( i < gApplication->Argc() )
    {
      string name(gApplication->Argv(i));
      if ( name == "rlist.C" )
        {
          i++;
          filename = string(gApplication->Argv(i));
	  break;
        }
      i++; // Don't forget to increment!
    }

  gSystem->Load("treeutil_cpp.so");
  itreestream s(filename);
  cout << s << endl;
  exit(0);
}
