#include "nano/analysis/interface/nanoBase.h"
#include <string>
#include <fstream>

using std::string;

inline bool exists_test (string& name) {
  std::ifstream f(name.c_str());
  return f.good();
}

nanoBase::nanoBase(TTree *tree, TTree *had, TTree *hadTruth, Bool_t isMC) :
  Events(tree, had, hadTruth),
  m_output(0),
  m_tree(0),
  m_isMC(isMC)
{
  m_pileUp = new pileUpTool();
  string env = getenv("CMSSW_BASE");
  string lumi = env+"/src/nano/analysis/data/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt";

  if (!exists_test(lumi)) {
     std::cout << "Missing data file, run getFiles and try again" << std::endl;
     exit(50);
  }
  m_lumi = new lumiTool(lumi);
  string csvFileName = "CSVv2_Moriond17_B_H.csv";
  std::string csvFile = env+"/src/nano/analysis/data/btagSF/"+csvFileName;
  BTagCalibration calib("csvv2", csvFile);
  m_btagSF = BTagCalibrationReader(BTagEntry::OP_RESHAPING,"central",{"up_jes","down_jes",
                                                                   "up_lf", "down_lf",
                                                                   "up_hf", "down_hf",
                                                                   "up_hfstats1", "down_hfstats1",
                                                                   "up_hfstats2", "down_hfstats2",
                                                                   "up_lfstats1", "down_lfstats1",
                                                                   "up_lfstats2", "down_lfstats2",
                                                                   "up_cferr1", "down_cferr1",
                                                                   "up_cferr2", "down_cferr2",});
  m_btagSF.load(calib, BTagEntry::FLAV_B, "iterativefit");
  m_btagSF.load(calib, BTagEntry::FLAV_C, "iterativefit");
  m_btagSF.load(calib, BTagEntry::FLAV_UDSG, "iterativefit");
}

nanoBase::~nanoBase()
{
  if (m_output) {
    m_output->Write();
    m_output->Close();
  }
}

void nanoBase::Loop()
{
}
