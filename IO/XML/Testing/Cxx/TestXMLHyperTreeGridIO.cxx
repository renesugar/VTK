/*
This test does a read write cycle of a known good hypertree grid file,
comparing the shape of read and written trees, to exercise the reader
and writer and ensure that they work.
*/

#include "vtkDataArray.h"
#include "vtkHyperTreeGrid.h"
#include "vtkNew.h"
#include "vtkTestUtilities.h"
#include "vtkXMLHyperTreeGridReader.h"
#include "vtkXMLHyperTreeGridWriter.h"

#include <vtksys/SystemTools.hxx>
#include <string>

#define VTK_SUCCESS 0
#define VTK_FAILURE 1
int TestXMLHyperTreeGridIO(int argc, char* argv[])
{
  char* temp_dir = vtkTestUtilities::GetArgOrEnvOrDefault(
    "-T", argc, argv, "VTK_TEMP_DIR", "Testing/Temporary");
  if (!temp_dir)
  {
    cerr << "Could not determine temporary directory." << endl;
    return VTK_FAILURE;
  }

  char* data_dir = vtkTestUtilities::GetDataRoot(argc, argv);
  if (!data_dir)
  {
    delete [] temp_dir;
    cerr << "Could not determine data directory." << endl;
    return VTK_FAILURE;
  }

  std::string fname = "HyperTreeGrid.1.htg";

  std::string ifname = std::string(data_dir) + std::string("/Data/") + fname;

  cout << "- READ --------------------------------" << endl;
  vtkNew<vtkXMLHyperTreeGridReader> reader;
  reader->SetFileName(ifname.c_str());
  reader->Update();

  vtkHyperTreeGrid *read_in = reader->GetOutput();
  unsigned int size[3];
  vtkDataArray *coords;
  int ntp;

  std::string read1;

  read_in->GetGridSize(size);
  read1 += "SIZE "+std::to_string(size[0])+","+std::to_string(size[1])+","+std::to_string(size[2])+"\n";
  read1 +="DIMS "+std::to_string(read_in->GetDimension())+"\n";
  read1 +="#TREES "+std::to_string(read_in->GetNumberOfTrees())+"\n";
  read1 +="ORIENTATION "+std::to_string(read_in->GetOrientation())+"\n";
  read1 +="BRANCHFACTOR "+std::to_string(read_in->GetBranchFactor())+"\n";
  coords = read_in->GetXCoordinates();
  ntp = coords->GetNumberOfTuples();
  read1 +="XCOORDS "+std::to_string(ntp)+":"+std::to_string(coords->GetTuple1(0))+"..."+std::to_string(coords->GetTuple1(ntp-1))+"\n";
  coords = read_in->GetYCoordinates();
  ntp = coords->GetNumberOfTuples();
  read1 +="YCOORDS "+std::to_string(ntp)+":"+std::to_string(coords->GetTuple1(0))+"..."+std::to_string(coords->GetTuple1(ntp-1))+"\n";
  coords = read_in->GetZCoordinates();
  ntp = coords->GetNumberOfTuples();
  read1 +="ZCOORDS "+std::to_string(ntp)+":"+std::to_string(coords->GetTuple1(0))+"..."+std::to_string(coords->GetTuple1(ntp-1))+"\n";
  read1 +="TRANSPOSED "+std::to_string(read_in->GetTransposedRootIndexing())+"\n";
  read1 +="#CHILDREN "+std::to_string(read_in->GetNumberOfChildren())+"\n";
  read1 +="#LEVELS "+std::to_string(read_in->GetNumberOfLevels())+"\n";
  read1 +="#VERTS "+std::to_string(read_in->GetNumberOfVertices())+"\n";
  read1 +="#LEAVES "+std::to_string(read_in->GetNumberOfLeaves())+"\n";
  read1 +="#CELLS "+std::to_string(read_in->GetNumberOfCells())+"\n";
  read1 +="#POINTS "+std::to_string(read_in->GetNumberOfPoints())+"\n";
  cout << read1 << endl;

  std::string output_dir = temp_dir;
  output_dir += "/HTG";

  vtksys::SystemTools::MakeDirectory(output_dir);

  std::string ofname = output_dir + "/" + fname;

  cout << "- WRITE --------------------------------" << endl;

  vtkNew<vtkXMLHyperTreeGridWriter> writer;
  writer->SetFileName(ofname.c_str());
  writer->SetInputData(read_in);
  writer->Write();

  reader->SetFileName(ofname.c_str());
  reader->Update();
  vtkHyperTreeGrid *wrote_out = reader->GetOutput();

  std::string read2;
  wrote_out->GetGridSize(size);
  read2 += "SIZE "+std::to_string(size[0])+","+std::to_string(size[1])+","+std::to_string(size[2])+"\n";
  read2 +="DIMS "+std::to_string(wrote_out->GetDimension())+"\n";
  read2 +="#TREES "+std::to_string(wrote_out->GetNumberOfTrees())+"\n";
  read2 +="ORIENTATION "+std::to_string(wrote_out->GetOrientation())+"\n";
  read2 +="BRANCHFACTOR "+std::to_string(wrote_out->GetBranchFactor())+"\n";
  coords = wrote_out->GetXCoordinates();
  ntp = coords->GetNumberOfTuples();
  read2 +="XCOORDS "+std::to_string(ntp)+":"+std::to_string(coords->GetTuple1(0))+"..."+std::to_string(coords->GetTuple1(ntp-1))+"\n";
  coords = wrote_out->GetYCoordinates();
  ntp = coords->GetNumberOfTuples();
  read2 +="YCOORDS "+std::to_string(ntp)+":"+std::to_string(coords->GetTuple1(0))+"..."+std::to_string(coords->GetTuple1(ntp-1))+"\n";
  coords = wrote_out->GetZCoordinates();
  ntp = coords->GetNumberOfTuples();
  read2 +="ZCOORDS "+std::to_string(ntp)+":"+std::to_string(coords->GetTuple1(0))+"..."+std::to_string(coords->GetTuple1(ntp-1))+"\n";
  read2 +="TRANSPOSED "+std::to_string(wrote_out->GetTransposedRootIndexing())+"\n";
  read2 +="#CHILDREN "+std::to_string(wrote_out->GetNumberOfChildren())+"\n";
  read2 +="#LEVELS "+std::to_string(wrote_out->GetNumberOfLevels())+"\n";
  read2 +="#VERTS "+std::to_string(wrote_out->GetNumberOfVertices())+"\n";
  read2 +="#LEAVES "+std::to_string(wrote_out->GetNumberOfLeaves())+"\n";
  read2 +="#CELLS "+std::to_string(wrote_out->GetNumberOfCells())+"\n";
  read2 +="#POINTS "+std::to_string(wrote_out->GetNumberOfPoints())+"\n";
  cout << read2 << endl;

  bool ret = VTK_FAILURE;
  if (read1 == read2)
  {
    ret = VTK_SUCCESS;
    vtksys::SystemTools::RemoveADirectory(output_dir);
  }
  else
  {
    cerr << "Problem: Written file does not match read in file." << endl;
  }

  delete []temp_dir;
  delete []data_dir;

  return ret;
}
