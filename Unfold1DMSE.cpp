#include <iostream>
#include <vector>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TMatrixD.h"
#include "TError.h"
#include "TSpline.h"
#include "TGraph.h"
#include "TRandom3.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/ProbFuncMathCore.h"

#include "RooUnfold.h"
#include "RooUnfoldInvert.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldSvd.h"
#include "TUnfold.h"
#include "TUnfoldDensity.h"

#include "RootUtilities.h"
#include "CommandLine.h"
#include "CustomAssert.h"

class Spectrum;
int main(int argc, char *argv[]);
vector<double> DetectBins(TH1D *HMin, TH1D *HMax);
void RemoveOutOfRange(TH1D *H);
void RemoveOutOfRange(TH2D *HResponse);
void ReweightResponse(TH2D *HResponse, TH1D *HPrior, bool NormalizePrior = true);
TH1D *ConstructPriorCopy(TH1D *HMC);
TH1D *ConstructPriorCopyExternal(string FileName, string HistName);
TH1D *ConstructPriorFlat(vector<double> GenBinsPrimary, vector<double> GenBinsSecondary);
void DoProjection(TH2D *HResponse, TH1D **HGen, TH1D **HReco);
TH1D *ForwardFold(TH1 *HGen, TH2D *HResponse, TH1 *HErrors = nullptr);
TH1D *Collapse(TH1 *HFlat, vector<double> &BinsPrimary, vector<double> &BinsSecondary, int Axis);
TH1D *VaryWithinError(TH1D *H);
TH1D* GetVariance(vector<vector<TH1 *>> &Asimov, vector<int> &Regularization, vector<TH1 *> &Dists, int Axis = -1);
TH1D* GetBias(vector<vector<TH1 *>> &Asimov, TH1* HTruth, vector<int> &Regularization, vector<TH1 *> &Dists, int Axis = -1);
TH1D* GetMSE(TH1* Variance, TH1* Bias, int Axis = -1);
TH1D* GetCoverage(vector<TH1 *> &VarianceDists, vector<TH1 *> &BiasDists, vector<TH1 *> &CoverageDists, vector<int> &Regularization, int Axis = -1);

class Spectrum
{
public:
   vector<double> Prior;
   vector<double> Reco;
   vector<double> EReco;
   vector<vector<double>> Matrix;
public:
   Spectrum()
   {
      Prior.clear();
      Reco.clear();
      EReco.clear();
      Matrix.clear();
   }
   ~Spectrum() {}
   bool SetPrior(TH1D *H)
   {
      if(H == nullptr)
         return false;
      int N = H->GetNbinsX();
      Prior.resize(N);
      for(int i = 0; i < N; i++)
         Prior[i] = H->GetBinContent(i + 1);
      return true;
   }
   bool SetReco(TH1D *H)
   {
      if(H == nullptr)
         return false;
      int N = H->GetNbinsX();
      Reco.resize(N);
      EReco.resize(N);
      for(int i = 0; i < N; i++)
      {
         Reco[i] = H->GetBinContent(i + 1);
         EReco[i] = H->GetBinError(i + 1);
      }
      return true;
   }
   bool SetMatrix(TH2D *H)
   {
      if(H == nullptr)
         return false;
      int NX = H->GetNbinsX();
      int NY = H->GetNbinsY();
      Matrix.resize(NX);
      for(int iX = 0; iX < NX; iX++)
      {
         Matrix[iX].resize(NY);
         for(int iY = 0; iY < NY; iY++)
            Matrix[iX][iY] = H->GetBinContent(iX + 1, iY + 1);
      }
      return true;
   }
   bool Initialize()
   {
      int NG = Prior.size();
      int NR = Reco.size();

      Assert(NG > 0, "Fit: Please initialize gen");
      Assert(NR > 0, "Fit: Please initialize reco");
      Assert(NR == (int)Matrix.size(), "Fit: matrix size mismatch reco");
      for(int i = 0; i < NR; i++) {
         cout << NG << " " << (int)Matrix[i].size() << endl;
         Assert(NG == (int)Matrix[i].size(), "Fit: matrix size mismatch gen");
      }

      double SumR = 0;
      for(int i = 0; i < NR; i++)
         SumR = SumR + Reco[i];
      double SumG = 0;
      for(int i = 0; i < NG; i++)
         SumG = SumG + Prior[i];
      for(int i = 0; i < NG; i++)
         Prior[i] = Prior[i] / SumG * SumR;

      for(int i = 0; i < NG; i++)
      {
         double Sum = 0;
         for(int j = 0; j < NR; j++)
            Sum = Sum + Matrix[j][i];
         for(int j = 0; j < NR; j++)
            Matrix[j][i] = Matrix[j][i] / Sum;
      }

      return true;
   }
   double LogLikelihood(const double *Parameters)
   {
      int NReco = Reco.size();
      int NGen = Prior.size();
      vector<double> Folded(NReco, 0);
      for(int iR = 0; iR < NReco; iR++)
         for(int iG = 0; iG < NGen; iG++)
            Folded[iR] = Folded[iR] + Prior[iG] * Parameters[iG] * Matrix[iR][iG];

      double LL = 0;
      for(int iR = 0; iR < NReco; iR++)
      {
         double D = Folded[iR] - Reco[iR];
         double E = EReco[iR];
         if(E == 0)
            E = 1;
         LL = LL + D * D / E * E;
      }

      // Regularization
      for(int iG = 0; iG < NGen; iG++)
      {
         double R = log(Parameters[iG]);
         LL = LL + 100 * R * R;
      }
      for(int iG = 0; iG < NGen - 1; iG++)
      {
         double R = log(Parameters[iG]) - log(Parameters[iG+1]);
         LL = LL + 100 * R * R;
      }

      return 0.5 * LL;
   }
   const double *DoFit()
   {
      ROOT::Math::Minimizer *Minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");
      Minimizer->SetMaxFunctionCalls(1000000);
      Minimizer->SetMaxIterations(100000);
      Minimizer->SetTolerance(0.00001);
      Minimizer->SetPrintLevel(-1);

      int N = Prior.size();

      ROOT::Math::Functor F(this, &Spectrum::LogLikelihood, N);
      Minimizer->SetFunction(F);

      for(int i = 0; i < N; i++)
         Minimizer->SetLimitedVariable(i, Form("S%d", i), 1.00, 0.001, 0.001, 1000);
      for(int i = 0; i < 5; i++)
         Minimizer->Minimize();

      return Minimizer->X();
   }
};

int main(int argc, char *argv[])
{
   SilenceRoot();

   CommandLine CL(argc, argv);

   string InputFileName              = CL.Get("Input");
   string DataGenName                = CL.Get("InputGenName",              "HDataGen");
   string DataRecoName               = CL.Get("InputRecoName",             "HDataReco");
   string DataErrorName              = CL.Get("DataErrorName",             "HDataErrors");
   string ResponseName               = CL.Get("ResponseName",              "HResponse");
   string ResponseTruth              = CL.Get("ResponseTruth",             "HMCGen");
   string ResponseMeasured           = CL.Get("ResponseMeasured",          "HMCReco");
   string ResponseTruthEfficiency    = CL.Get("ResponseTruthEfficiency",   "HMCGenEfficiency");
   string ResponseMeasuredEfficiency = CL.Get("ResponseMeasuredEfficiency","HMCRecoEfficiency");
   string Output                     = CL.Get("Output");
   string PriorChoice                = CL.Get("Prior",                     "Original");
   string Error                      = CL.Get("Error",                     "kErrors");
   bool DoBayes                      = CL.GetBool("DoBayes",               true);
   bool DoRepeatedBayes              = CL.GetBool("DoRepeatedBayes",       true);
   bool DoSVD                        = CL.GetBool("DoSVD",                 true);
   bool DoInvert                     = CL.GetBool("DoInvert",              true);
   bool DoTUnfold                    = CL.GetBool("DoTUnfold",             true);
   bool DoFit                        = CL.GetBool("DoFit",                 true);
   bool DoFoldNormalize              = CL.GetBool("FoldNormalize",         false);
   
   TFile InputFile(InputFileName.c_str());

   TH1D *HMeasured      = (TH1D *)InputFile.Get(ResponseMeasured.c_str());
   TH1D *HTruth         = (TH1D *)InputFile.Get(ResponseTruth.c_str());
   TH1D *HMeasuredEfficiency      = (TH1D *)InputFile.Get(ResponseMeasuredEfficiency.c_str());
   TH1D *HTruthEfficiency         = (TH1D *)InputFile.Get(ResponseTruthEfficiency.c_str());
   TH2D *HResponse      = (TH2D *)InputFile.Get(ResponseName.c_str());
   TH2D *HRawResponse   = (TH2D *)HResponse->Clone("HRawResponse");
   TH1D *HInputGen      = (TH1D *)InputFile.Get(DataGenName.c_str())->Clone();
   TH1D *HInputReco     = (TH1D *)InputFile.Get(DataRecoName.c_str())->Clone();
   TH1D *HInputErrors   = (TH1D *)InputFile.Get(DataErrorName.c_str())->Clone();

   int NGen = HResponse->GetNbinsY();
   int NReco = HResponse->GetNbinsX();
   int NA = 50;

   RemoveOutOfRange(HMeasured);
   RemoveOutOfRange(HTruth);
   RemoveOutOfRange(HResponse);
   RemoveOutOfRange(HRawResponse);
   RemoveOutOfRange(HInputGen);

   RooUnfold::ErrorTreatment ErrorChoice;

   vector<double> GenBinsPrimary    = DetectBins((TH1D *)InputFile.Get("HGenPrimaryBinMin"),
                                                 (TH1D *)InputFile.Get("HGenPrimaryBinMax"));
   vector<double> GenBinsSecondary  = DetectBins((TH1D *)InputFile.Get("HGenBinningBinMin"),
                                                 (TH1D *)InputFile.Get("HGenBinningBinMax"));
   vector<double> RecoBinsPrimary   = DetectBins((TH1D *)InputFile.Get("HRecoPrimaryBinMin"),
                                                 (TH1D *)InputFile.Get("HRecoPrimaryBinMax"));
   vector<double> RecoBinsSecondary = DetectBins((TH1D *)InputFile.Get("HRecoBinningBinMin"),
                                                 (TH1D *)InputFile.Get("HRecoBinningBinMax"));

   if(Error == "kNoError") {
      cout << "kNoError" << endl;
      ErrorChoice = RooUnfold::kNoError;
   }
   else if(Error == "kErrors") {
      cout << "kErrors" << endl;
      ErrorChoice = RooUnfold::kErrors;
   }
   else if(Error == "kCovariance") {
      cout << "kCovariance" << endl;
      ErrorChoice = RooUnfold::kCovariance;
   }
   else if(Error == "kCovToys") {
      cout << "kCovToys" << endl;
      ErrorChoice = RooUnfold::kCovToys;
   }
   else if(Error == "kErrorsToys") {
      cout << "kErrorsToys" << endl;
      ErrorChoice = RooUnfold::kErrorsToys;
   }
   else
   {
      cout << "Unsupported errors option \"" << ErrorChoice << "\"!" << endl;
      return -1;
   }

   TH1D *HPrior = nullptr;
   if(PriorChoice == "MC")
      HPrior = ConstructPriorCopy(HTruth);
   else if(PriorChoice == "ExternalMC")
   {
      string ExternalMCPriorFileName = CL.Get("ExternalPriorFile");
      HPrior = ConstructPriorCopyExternal(ExternalMCPriorFileName, ResponseTruth);
   }
   else if(PriorChoice == "External")
   {
      string ExternalPriorFileName  = CL.Get("ExternalPriorFile");
      string ExternalPriorHistogram = CL.Get("ExternalPriorHistogram");
      HPrior = ConstructPriorCopyExternal(ExternalPriorFileName, ExternalPriorHistogram);
   }
   else if(PriorChoice == "Flat")
   {
      vector<double> GenBinsPrimary = DetectBins((TH1D *)InputFile.Get("HGenPrimaryBinMin"),
                                                 (TH1D *)InputFile.Get("HGenPrimaryBinMax"));
      vector<double> GenBinsSecondary = DetectBins((TH1D *)InputFile.Get("HGenBinningBinMin"),
                                                   (TH1D *)InputFile.Get("HGenBinningBinMax"));

      HPrior = ConstructPriorFlat(GenBinsPrimary, GenBinsSecondary);

      int NX = HPrior->GetNbinsX();

      for(int iX = 1; iX <= NX; iX++)
      {
         HPrior->SetBinError(iX, HInputErrors->GetBinError(iX));
      }
   }
   else if(PriorChoice == "Original")
   {
      TH1D *HProjected = nullptr;
      DoProjection(HResponse, &HPrior, &HProjected);
   }
   else
   {
      cout << "Unsupported prior option \"" << PriorChoice << "\"!" << endl;
      return -1;
   }

   ReweightResponse(HResponse, HPrior);

   TH1D *HGen = nullptr;
   TH1D *HReco = nullptr;
   DoProjection(HResponse, &HGen, &HReco);

   TH1D *HInputReco = SetErrors(HInputErrors);

   TH1D *HInputRecoFold0 = Collapse(HInputReco, RecoBinsPrimary, RecoBinsSecondary, 0);
   TH1D *HInputRecoFold1 = Collapse(HInputReco, RecoBinsPrimary, RecoBinsSecondary, 1);
   TH1D *HInputGenFold0 = Collapse(HInputGen, GenBinsPrimary, GenBinsSecondary, 0);
   TH1D *HInputGenFold1 = Collapse(HInputGen, GenBinsPrimary, GenBinsSecondary, 1);

   vector<TH1 *> HAsimov;

   vector<vector<TH1 *>> HUnfolded(NA, vector<TH1 *>(0));
   vector<vector<TH1 *>> HUnfoldedFold0(NA, vector<TH1 *>(0));
   vector<vector<TH1 *>> HUnfoldedFold1(NA, vector<TH1 *>(0));

   // vector<vector<TH1 *>> HRefolded(NA, vector<TH1 *>(0));
   // vector<map<string, TMatrixD>> Covariance(NA, map<string, TMatrixD>());

   TH1D *Variance;
   TH1D *VarianceFold0;
   TH1D *VarianceFold1;

   TH1D *Bias;
   TH1D *BiasFold0;
   TH1D *BiasFold1;

   TH1D *MSE;
   TH1D *MSEFold0;
   TH1D *MSEFold1;

   TH1D *Coverage;
   TH1D *CoverageFold0;
   TH1D *CoverageFold1;

   vector<TH1 *> VarianceDists(0);
   vector<TH1 *> VarianceDistsFold0(0);
   vector<TH1 *> VarianceDistsFold1(0);

   vector<TH1 *> BiasDists(0);
   vector<TH1 *> BiasDistsFold0(0);
   vector<TH1 *> BiasDistsFold1(0);

   vector<TH1 *> CoverageDists(0);
   vector<TH1 *> CoverageDistsFold0(0);
   vector<TH1 *> CoverageDistsFold1(0);

   RooUnfoldResponse *Response = new RooUnfoldResponse(HReco, HGen, HResponse);

   for(int A = 0; A < NA; A++) {
      HAsimov.push_back((TH1D *) VaryWithinError(HInputReco)->Clone(Form("HTest%d", A)));
      HAsimov[A]->Multiply(HMeasuredEfficiency);
   }

   if(DoBayes == true)
   {
      vector<int> Iterations{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150};

      for(int A = 0; A < NA; A++) 
      {
         cout << A << endl;

         for(int I : Iterations)
         {
            RooUnfoldBayes BayesUnfold(Response, HAsimov[A], I); 
            BayesUnfold.SetNToys(1000);
            BayesUnfold.SetVerbose(-1);
            HUnfolded[A].push_back((TH1 *)(BayesUnfold.Hunfold(ErrorChoice)->Clone(Form("Test%dHUnfoldedBayes%d", A, I))));
            HUnfolded[A].back()->Divide(HTruthEfficiency);
            HUnfoldedFold0[A].push_back((TH1 *) Collapse(HUnfolded[A].back(), GenBinsPrimary, GenBinsSecondary, 0)->Clone(Form("Test%dHUnfoldedBayes%dFold0", A, I)));
            HUnfoldedFold1[A].push_back((TH1 *) Collapse(HUnfolded[A].back(), GenBinsPrimary, GenBinsSecondary, 1)->Clone(Form("Test%dHUnfoldedBayes%dFold1", A, I)));

            // Covariance[A].insert(pair<string, TMatrixD>(Form("Test%dMUnfoldedBayes%d", A, I), BayesUnfold.Eunfold()));
            // TH1D *HFold = ForwardFold(HUnfolded[A][HUnfolded[A].size()-1], HResponse);
            // HFold->SetName(Form("Test%dHRefoldedBayes%d", A, I));
            // HRefolded[A].push_back(HFold);
         }
      }

      Variance = GetVariance(HUnfolded, Iterations, VarianceDists);
      VarianceFold0 = GetVariance(HUnfoldedFold0, Iterations, VarianceDistsFold0, 0);
      VarianceFold1 = GetVariance(HUnfoldedFold1, Iterations, VarianceDistsFold1, 1);

      Bias = GetBias(HUnfolded, HInputGen, Iterations, BiasDists);
      BiasFold0 = GetBias(HUnfoldedFold0, HInputGenFold0, Iterations, BiasDistsFold0, 0);
      BiasFold1 = GetBias(HUnfoldedFold1, HInputGenFold1, Iterations, BiasDistsFold1, 1);
      
      MSE = GetMSE(Variance, Bias);
      MSEFold0 = GetMSE(VarianceFold0, BiasFold0);
      MSEFold1 = GetMSE(VarianceFold1, BiasFold1);

      Coverage = GetCoverage(VarianceDists, BiasDists, CoverageDists, Iterations);
      CoverageFold0 = GetCoverage(VarianceDistsFold0, BiasDistsFold0, CoverageDistsFold0, Iterations, 0);
      CoverageFold1 = GetCoverage(VarianceDistsFold1, BiasDistsFold1, CoverageDistsFold1, Iterations, 1);
   }

   if(DoSVD == true)
   {
      vector<int> SVDRegularization{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65};

      for(int A = 0; A < NA; A++) 
      {
         cout << A << endl;

         for(int D : SVDRegularization)
         {
            if(D >= HGen->GetNbinsX())
               continue;

            RooUnfoldSvd SVDUnfold(Response, HAsimov[A], D); 
            SVDUnfold.SetNToys(1000);
            SVDUnfold.SetVerbose(-1);
            HUnfolded[A].push_back((TH1 *)(SVDUnfold.Hunfold(ErrorChoice)->Clone(Form("Test%dHUnfoldedSVD%d", A, D))));
            HUnfoldedFold0[A].push_back((TH1 *) Collapse(HUnfolded[A].back(), GenBinsPrimary, GenBinsSecondary, 0)->Clone(Form("Test%dHUnfoldedSVD%dFold0", A, D)));
            HUnfoldedFold1[A].push_back((TH1 *) Collapse(HUnfolded[A].back(), GenBinsPrimary, GenBinsSecondary, 1)->Clone(Form("Test%dHUnfoldedSVD%dFold1", A, D)));

            // Covariance[A].insert(pair<string, TMatrixD>(Form("Test%dMUnfoldedSVD%d", A, D), SVDUnfold.Eunfold()));
            // TH1D *HFold = ForwardFold(HUnfolded[A][HUnfolded[A].size()-1], HResponse);
            // HFold->SetName(Form("Test%dHRefoldedSVD%d", A, D));
            // HRefolded[A].push_back(HFold);
         }
      }

      Variance = GetVariance(HUnfolded, SVDRegularization, VarianceDists);
      VarianceFold0 = GetVariance(HUnfoldedFold0, SVDRegularization, VarianceDistsFold0, 0);
      VarianceFold1 = GetVariance(HUnfoldedFold1, SVDRegularization, VarianceDistsFold1, 1);

      Bias = GetBias(HUnfolded, HInputGen, SVDRegularization, BiasDists);
      BiasFold0 = GetBias(HUnfoldedFold0, HInputGenFold0, SVDRegularization, BiasDistsFold0, 0);
      BiasFold1 = GetBias(HUnfoldedFold1, HInputGenFold1, SVDRegularization, BiasDistsFold1, 1);
      
      MSE = GetMSE(Variance, Bias);
      MSEFold0 = GetMSE(VarianceFold0, BiasFold0);
      MSEFold1 = GetMSE(VarianceFold1, BiasFold1);

      Coverage = GetCoverage(VarianceDists, BiasDists, CoverageDists, SVDRegularization);
      CoverageFold0 = GetCoverage(VarianceDistsFold0, BiasDistsFold0, CoverageDistsFold0, SVDRegularization, 0);
      CoverageFold1 = GetCoverage(VarianceDistsFold1, BiasDistsFold1, CoverageDistsFold1, SVDRegularization, 1);
   }

   TFile OutputFile(Output.c_str(), "RECREATE");
   HMeasured->Clone("HMCMeasured")->Write();
   HTruth->Clone("HMCTruth")->Write();
   HMeasuredEfficiency->Clone("HMCMeasuredEfficiency")->Write();
   HTruthEfficiency->Clone("HMCTruthEfficiency")->Write();
   HGen->Clone("HMCGen")->Write();
   HReco->Clone("HMCReco")->Write();
   HResponse->Clone("HMCResponse")->Write();
   Response->Mresponse().Clone("HMCFilledResponse")->Write();

   HInputReco->Clone("HInputReco")->Write();
   HInputRecoFold0->Clone("HInputRecoFold0")->Write();
   HInputRecoFold1->Clone("HInputRecoFold1")->Write();
   HInputGen->Clone("HInputGen")->Write();
   HInputGenFold0->Clone("HInputGenFold0")->Write();
   HInputGenFold1->Clone("HInputGenFold1")->Write();
   Variance->Clone("HVariance")->Write();
   VarianceFold0->Clone("HVarianceFold0")->Write();
   VarianceFold1->Clone("HVarianceFold1")->Write();
   Bias->Clone("HBias")->Write();
   BiasFold0->Clone("HBiasFold0")->Write();
   BiasFold1->Clone("HBiasFold1")->Write();
   MSE->Clone("HMSE")->Write();
   MSEFold0->Clone("HMSEFold0")->Write();
   MSEFold1->Clone("HMSEFold1")->Write();
   Coverage->Clone("HCoverage")->Write();
   CoverageFold0->Clone("HCoverageFold0")->Write();
   CoverageFold1->Clone("HCoverageFold1")->Write();

   // for(TH1 *H : HAsimov)              if(H != nullptr)   H->Write();
   for(TH1 *H : VarianceDists)        if(H != nullptr)   H->Write();
   for(TH1 *H : VarianceDistsFold0)   if(H != nullptr)   H->Write();
   for(TH1 *H : VarianceDistsFold1)   if(H != nullptr)   H->Write();
   for(TH1 *H : BiasDists)            if(H != nullptr)   H->Write();
   for(TH1 *H : BiasDistsFold0)       if(H != nullptr)   H->Write();
   for(TH1 *H : BiasDistsFold1)       if(H != nullptr)   H->Write();
   for(TH1 *H : CoverageDists)        if(H != nullptr)   H->Write();
   for(TH1 *H : CoverageDistsFold0)   if(H != nullptr)   H->Write();
   for(TH1 *H : CoverageDistsFold1)   if(H != nullptr)   H->Write();

   for(int A = 0; A < NA; A++) {
      if (A == 0) {
         for(TH1 *H : HUnfolded[A])       if(H != nullptr)   H->Write();
         for(TH1 *H : HUnfoldedFold0[A])  if(H != nullptr)   H->Write();
         for(TH1 *H : HUnfoldedFold1[A])  if(H != nullptr)   H->Write();
         // for(TH1 *H : HRefolded[A])       if(H != nullptr)   H->Write();
         // for(auto I : Covariance[A])      I.second.Write(I.first.c_str());
      }
   }

   vector<string> ToCopy
   {
      "MCEventCount", "MCAllEventCount", "MCBaselineEventCount",
      "DataEventCount", "DataAllEventCount", "DataBaselineEventCount"
   };
   for(string S : ToCopy)
      if(InputFile.Get(S.c_str()) != nullptr)
         InputFile.Get(S.c_str())->Clone(S.c_str())->Write();

   HPrior->Clone("HPrior")->Write();

   OutputFile.Close();
   InputFile.Close();

   return 0;
}

vector<double> DetectBins(TH1D *HMin, TH1D *HMax)
{
   if(HMin == nullptr || HMax == nullptr)
      return vector<double>{};

   vector<pair<double, double>> Bins;

   for(int i = 1; i <= HMin->GetNbinsX(); i++)
      Bins.push_back(pair<double, double>(HMin->GetBinContent(i), HMax->GetBinContent(i)));

   sort(Bins.begin(), Bins.end());
   auto iterator = unique(Bins.begin(), Bins.end());
   Bins.erase(iterator, Bins.end());

   vector<double> Result;
   for(auto iterator : Bins)
   {
      if(iterator.second == 999)
         iterator.second = 9999;

      Result.push_back(iterator.first);
      Result.push_back(iterator.second);
   }

   sort(Result.begin(), Result.end());
   auto iterator2 = unique(Result.begin(), Result.end());
   Result.erase(iterator2, Result.end());

   return Result;
}

void RemoveOutOfRange(TH1D *H)
{
   if(H == nullptr)
      return;

   H->SetBinContent(0, 0);
   H->SetBinContent(H->GetNbinsX() + 1, 0);
}

void RemoveOutOfRange(TH2D *HResponse)
{
   if(HResponse == nullptr)
      return;

   int NX = HResponse->GetNbinsX();
   int NY = HResponse->GetNbinsY();

   for(int iX = 0; iX <= NX + 1; iX++)
   {
      HResponse->SetBinContent(iX, 0, 0);
      HResponse->SetBinContent(iX, NY + 1, 0);
   }
   for(int iY = 0; iY <= NY + 1; iY++)
   {
      HResponse->SetBinContent(0, iY, 0);
      HResponse->SetBinContent(NX + 1, iY, 0);
   }
}

void ReweightResponse(TH2D *HResponse, TH1D *HPrior, bool NormalizePrior)
{
   if(HResponse == nullptr)
      return;
   if(HPrior == nullptr)
      return;

   if(NormalizePrior == true)
      HPrior->Scale(1 / HPrior->Integral());

   int NX = HResponse->GetNbinsX();
   int NY = HResponse->GetNbinsY();

   vector<double> Total(NY, 0);
   for(int X = 1; X <= NX; X++)
      for(int Y = 1; Y <= NY; Y++)
         Total[Y-1] = Total[Y-1] + HResponse->GetBinContent(X, Y);
   
   for(int X = 1; X <= NX; X++)
   {
      for(int Y = 1; Y <= NY; Y++)
      {
         if(Total[Y-1] > 0)
         {
            HResponse->SetBinContent(X, Y, HResponse->GetBinContent(X, Y) / Total[Y-1] * HPrior->GetBinContent(Y));
            HResponse->SetBinError(X, Y, HResponse->GetBinError(X, Y) / Total[Y-1] * HPrior->GetBinContent(Y));
         }
      }
   }
}

TH1D *ConstructPriorCopy(TH1D *HMC)
{
   if(HMC == nullptr)
      return nullptr;

   TH1D *HPrior = (TH1D *)HMC->Clone("HP");
   HPrior->Reset();

   int N = HPrior->GetNbinsX();
   for(int i = 1; i <= N; i++)
   {
      HPrior->SetBinContent(i, HMC->GetBinContent(i));
      HPrior->SetBinError(i, HMC->GetBinError(i));
   }
   HPrior->Scale(1 / HPrior->Integral());

   return HPrior;
}

TH1D *ConstructPriorCopyExternal(string FileName, string HistName)
{
   vector<double> Bins;

   TFile File(FileName.c_str());
   TH1D *H = (TH1D *)File.Get(HistName.c_str());

   if(H != nullptr)
   {
      for(int i = 1; i <= H->GetNbinsX(); i++)
         Bins.push_back(H->GetBinContent(i));
   }
   File.Close();

   if(Bins.size() == 0)
      return nullptr;

   TH1D *HPrior = new TH1D("HP", "", Bins.size(), 0, Bins.size());
   for(int i = 1; i <= HPrior->GetNbinsX(); i++)
      HPrior->SetBinContent(i, Bins[i-1]);
   return HPrior;
}

TH1D *ConstructPriorFlat(vector<double> GenBinsPrimary, vector<double> GenBinsSecondary)
{
   int N = GenBinsPrimary.size() - 1; 
   int M = GenBinsSecondary.size() - 1; 
   TH1D *HPrior = new TH1D("HP", "", N*M, 0, N*M);

   for(int i = 1; i <= N; i++) {
      for(int j = 1; j <= M; j++) {
         HPrior->SetBinContent(i+(j-1)*N, (GenBinsPrimary[i] - GenBinsPrimary[i-1])*(GenBinsSecondary[j] - GenBinsSecondary[j-1]));
      }
   }
   
   HPrior->Scale(1 / HPrior->Integral());

   return HPrior;
}

void DoProjection(TH2D *HResponse, TH1D **HGen, TH1D **HReco)
{
   if(HResponse == nullptr)
      return;
   if((*HGen) != nullptr || (*HReco) != nullptr)
      return;

   static int Count = 0;
   Count = Count + 1;

   int NX = HResponse->GetNbinsX();
   int NY = HResponse->GetNbinsY();

   *HGen = new TH1D(Form("HGen%d", Count), "", NY, 0, NY);
   *HReco = new TH1D(Form("HReco%d", Count), "", NX, 0, NX);

   for(int iX = 1; iX <= NX; iX++)
   {
      for(int iY = 1; iY <= NY; iY++)
      {
         double V = HResponse->GetBinContent(iX, iY);
         (*HGen)->AddBinContent(iY, V);
         (*HReco)->AddBinContent(iX, V);
      }
   }
}

TH1D *ForwardFold(TH1 *HGen, TH2D *HResponse, TH1 *HErrors)
{
   if(HGen == nullptr || HResponse == nullptr)
      return nullptr;

   static int Count = 0;
   Count = Count + 1;

   int NGen = HResponse->GetNbinsY();
   int NReco = HResponse->GetNbinsX();

   TH1D *HResult = new TH1D(Form("HFold%d", Count), "", NReco, 0, NReco);

   HResult->Sumw2();

   for(int iG = 1; iG <= NGen; iG++)
   {
      double N = 0;
      for(int iR = 1; iR <= NReco; iR++)
         N = N + HResponse->GetBinContent(iR, iG);

      if(N == 0)
         continue;

      for(int iR = 1; iR <= NReco; iR++)
      {
         double T = HResponse->GetBinContent(iR, iG) / N;
         double G = HGen->GetBinContent(iG);
         double ET = HResponse->GetBinError(iR, iG) / HResponse->GetBinContent(iR, iG);
         double EG = HGen->GetBinError(iG) / HGen->GetBinContent(iG);

         if(HResponse->GetBinContent(iR, iG) == 0)
            ET = 0;
         if(HGen->GetBinContent(iG) == 0)
            EG = 0;

         double V = T * G;
         double E = sqrt(ET * ET + EG * EG) * V;

         double Current = HResult->GetBinContent(iR);
         HResult->SetBinContent(iR, Current + V);

         double Error = HResult->GetBinError(iR);
         Error = sqrt(Error * Error + E * E);
         HResult->SetBinError(iR, Error);
      }
   }

   if(HErrors != nullptr) {
      for(int iR = 1; iR <= NReco; iR++)
      {
         HResult->SetBinError(iR, HErrors->GetBinError(iR));
      }
   }

   return HResult;
}

TH1D *VaryWithinError(TH1D *H)
{
   TRandom3* Random = new TRandom3(0);

   if(H == nullptr)
      return nullptr;

   TH1D *HVary = (TH1D *)H->Clone("HVary");
   HVary->Reset();

   int N = HVary->GetNbinsX();
   for(int i = 1; i <= N; i++)
   {
      double Value = Random->Gaus(H->GetBinContent(i), H->GetBinError(i));
      HVary->SetBinContent(i, Value);
      HVary->SetBinError(i, H->GetBinError(i));
   }

   return HVary;
}

TH1D* GetVariance(vector<vector<TH1 *>> &Asimov, vector<int> &Regularization, vector<TH1 *> &Dists, int Axis)
{
   int NX = Asimov[0][0]->GetNbinsX();
   int NA = Asimov.size();
   int NI = Regularization.size(); cout << NI << endl; cout << Asimov[0].size() << endl;

   TH1D *Variance = new TH1D("HVariance", "", Regularization.back(), 0, Regularization.back());

   for(int I = 0; I < NI; I++)
   {
      vector<double> BinVariance(NX, 0);
      if (Axis == -1) Dists.push_back((TH1D *)Asimov[0][0]->Clone(Form("HVarianceDist%d", (int) Regularization[I])));
      if (Axis == 0)  Dists.push_back((TH1D *)Asimov[0][0]->Clone(Form("HVarianceDist%dFold0", (int) Regularization[I])));
      if (Axis == 1)  Dists.push_back((TH1D *)Asimov[0][0]->Clone(Form("HVarianceDist%dFold1", (int) Regularization[I])));
      Dists[I]->Reset();

      for(int X = 0; X < NX; X++)
      {
         // Calculate mean for bin X
         double Mean = 0;
         double Variance = 0;

         for(int A = 0; A < NA; A++) Mean += Asimov[A][I]->GetBinContent(X + 1);

         Mean /= NA;

         for(int A = 0; A < NA; A++) Variance += TMath::Power(Asimov[A][I]->GetBinContent(X + 1) - Mean, 2);

         Variance /= NA > 1 ? (NA - 1) : NA;

         BinVariance[X] = Variance;
         Dists[I]->SetBinContent(X + 1, Variance);
      }

      double BinMeanVariance = 0;
      for (auto V : BinVariance) BinMeanVariance += V/NX;

      Variance->SetBinContent(Regularization[I], BinMeanVariance);
   }

   return Variance;
}

TH1D* GetBias(vector<vector<TH1 *>> &Asimov, TH1* HTruth, vector<int> &Regularization, vector<TH1 *> &Dists, int Axis)
{
   int NX = Asimov[0][0]->GetNbinsX();
   int NA = Asimov.size();
   int NI = Regularization.size();

   TH1D *Bias = new TH1D("HBias", "", Regularization.back(), 0, Regularization.back());

   for(int I = 0; I < NI; I++)
   {
      vector<double> BinBias(NX, 0);
      if (Axis == -1) Dists.push_back((TH1D *)Asimov[0][0]->Clone(Form("HBiasDist%d", (int) Regularization[I])));
      if (Axis == 0)  Dists.push_back((TH1D *)Asimov[0][0]->Clone(Form("HBiasDist%dFold0", (int) Regularization[I])));
      if (Axis == 1)  Dists.push_back((TH1D *)Asimov[0][0]->Clone(Form("HBiasDist%dFold1", (int) Regularization[I])));
      Dists[I]->Reset();

      for(int X = 0; X < NX; X++)
      {
         // Calculate mean for bin X
         double Expectation = HTruth->GetBinContent(X + 1);
         double Bias = 0;

         for(int A = 0; A < NA; A++) Bias += Asimov[A][I]->GetBinContent(X + 1) - Expectation;

         Bias /= NA;

         BinBias[X] = Bias;
         Dists[I]->SetBinContent(X + 1, Bias*Bias);
      }

      double BinMeanBias = 0;
      for (auto B : BinBias) BinMeanBias += B*B/NX;

      Bias->SetBinContent(Regularization[I], BinMeanBias);
   }

   return Bias;
}

TH1D* GetMSE(TH1* Variance, TH1* Bias, int Axis)
{
   TH1D *MSE = (TH1D *)Variance->Clone("HMSE");
   MSE->Add(Bias);
   return MSE;
}

TH1D* GetCoverage(vector<TH1 *> &VarianceDists, vector<TH1 *> &BiasDists, vector<TH1 *> &CoverageDists, vector<int> &Regularization, int Axis)
{
   int NX = VarianceDists[0]->GetNbinsX();
   int NI = Regularization.size();

   TH1D *Coverage = new TH1D("HCoverage", "", Regularization.back(), 0, Regularization.back());

   for(int I = 0; I < NI; I++)
   {
      vector<double> BinCoverage(NX, 0);
      if (Axis == -1) CoverageDists.push_back((TH1D *)BiasDists[0]->Clone(Form("HCoverageDist%d", (int) Regularization[I])));
      if (Axis == 0)  CoverageDists.push_back((TH1D *)BiasDists[0]->Clone(Form("HCoverageDist%dFold0", (int) Regularization[I])));
      if (Axis == 1)  CoverageDists.push_back((TH1D *)BiasDists[0]->Clone(Form("HCoverageDist%dFold1", (int) Regularization[I])));
      CoverageDists[I]->Reset();

      for(int X = 0; X < NX; X++)
      {
         // Calculate coverage for bin X
         double Bias = sqrt(BiasDists[I]->GetBinContent(X + 1));
         double Error = sqrt(VarianceDists[I]->GetBinContent(X + 1));
         double Coverage = ROOT::Math::normal_cdf(Bias/Error + 1) - ROOT::Math::normal_cdf(Bias/Error - 1);

         BinCoverage[X] = Coverage;
         CoverageDists[I]->SetBinContent(X + 1, Coverage);
      }

      double BinMeanCoverage = 0;
      for (auto C : BinCoverage) BinMeanCoverage += C/NX;

      Coverage->SetBinContent(Regularization[I], BinMeanCoverage);
   }

   return Coverage;
}

TH1D *Collapse(TH1 *HFlat, vector<double> &BinsPrimary, vector<double> &BinsSecondary, int Axis) 
{
   int N = BinsPrimary.size() - 1; 
   int M = BinsSecondary.size() - 1; 

   TH1D *HCollapse;

   if(Axis == 0) HCollapse = new TH1D(Form("HCollapseAxis%d", Axis), "", N, &BinsPrimary[0]);
   else          HCollapse = new TH1D(Form("HCollapseAxis%d", Axis), "", M, &BinsSecondary[0]);

   for(int iX = 1; iX <= N; iX++)
   {
      for(int iY = 1; iY <= M; iY++)
      {
         int Index = iX + (iY-1) * N;

         if(Axis == 0) 
         {
            double Content = HFlat->GetBinContent(Index) + HCollapse->GetBinContent(iX);
            double E = HFlat->GetBinError(Index);
            double Error = HCollapse->GetBinError(iX);
            Error = sqrt(Error * Error + E * E);
            
            HCollapse->SetBinContent(iX, Content);
            HCollapse->SetBinError(iX, Error);
         }
         else 
         {
            double Content = HFlat->GetBinContent(Index) + HCollapse->GetBinContent(iY);
            double E = HFlat->GetBinError(Index);
            double Error = HCollapse->GetBinError(iY);
            Error = sqrt(Error * Error + E * E);
            
            HCollapse->SetBinContent(iY, Content);
            HCollapse->SetBinError(iY, Error);
         }
      }
   }

   HCollapse->Scale(1., "width");

   return HCollapse;
}