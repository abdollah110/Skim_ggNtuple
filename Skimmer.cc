#define Skimmer_cxx
#include "Skimmer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <TMath.h>
#include <string>
#include <sstream>
using namespace std;


void Skimmer::Loop(TString outputName, int skm)
{
    
    
    TH1F* hEvents = (TH1F*)gDirectory->Get("ggNtuplizer/hEvents");
    //    TH1F* hPU     = (TH1F*)gDirectory->Get("ggNtuplizer/hPU");
    //    TH1F* hPUTrue = (TH1F*)gDirectory->Get("ggNtuplizer/hPUTrue");
    //TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
    //TH1F* hPU     = (TH1F*)gDirectory->Get("hPU");
    //TH1F* hPUTrue = (TH1F*)gDirectory->Get("hPUTrue");
    
    TFile* file = TFile::Open(outputName, "RECREATE");
    TTree* MyNewTree = fChain->CloneTree(0);
    
    fChain->SetBranchStatus("*",0);
    fChain->SetBranchStatus("hasGoodVtx",1);
    fChain->SetBranchStatus("vt*",1);
    fChain->SetBranchStatus("EventTag",1);
    fChain->SetBranchStatus("run",1);
    fChain->SetBranchStatus("event",1);
    fChain->SetBranchStatus("lumis",1);
    fChain->SetBranchStatus("isData",1);
    fChain->SetBranchStatus("HLT*",1);
    fChain->SetBranchStatus("gen*",1);
    fChain->SetBranchStatus("pdf",1);
    fChain->SetBranchStatus("pthat",1);
    fChain->SetBranchStatus("processID",1);
    fChain->SetBranchStatus("rho*",1);
    fChain->SetBranchStatus("pu*",1);
    fChain->SetBranchStatus("mc*",1);
    fChain->SetBranchStatus("pfMET*",1);
    fChain->SetBranchStatus("n*",1);
    fChain->SetBranchStatus("c*",1);
    fChain->SetBranchStatus("jet*",1);
    fChain->SetBranchStatus("AK8*",0);
    fChain->SetBranchStatus("ele*",1);
    fChain->SetBranchStatus("mu*",1);
    fChain->SetBranchStatus("pho",0);
    fChain->SetBranchStatus("tau*",1);
   fChain->SetBranchStatus("m*",1); 
    
    TH1F* hcount = new TH1F("hcount", "", 10, 1, 10);
    
    if (fChain == 0) return;
    
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    float MuMass= 0.10565837;
    float eleMass= 0.000511;
    
    
    for (int jentry=0; jentry<nentries;jentry++) {
        
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        
        
        if(jentry % 10000 == 0) cout << "Processed " << jentry << " events out of " <<nentries<<endl;
        hcount->Fill(1);
        hcount->Fill(2,genWeight);
        
        
        bool isMuJet=0;
        bool isEleTau=0;
        
        TLorentzVector Mu4Momentum, Jet4Momentum;
        
  


        //MuJet
        for (int imu = 0; imu < nMu; ++imu){
            float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
            if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
                IsoMu= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
            if (muPt->at(imu) > 60 &&  fabs(muEta->at(imu)) < 2.4 &&   muIsLooseID->at(imu) > 0  && IsoMu < 0.50 ){
                Mu4Momentum.SetPtEtaPhiM(muPt->at(imu),muEta->at(imu),muPhi->at(imu),MuMass);
                
                for (int ijet = 0; ijet < nJet; ++ijet){
Jet4Momentum.SetPtEtaPhiM(jetPt->at(ijet),jetEta->at(ijet),jetPhi->at(ijet),jetEn->at(ijet));
	if (jetPt->at(ijet) > 50 &&  fabs(jetEta->at(ijet)) < 3.0   && Jet4Momentum.DeltaR(Mu4Momentum) > 0.5 ){
                            isMuJet=1;
                            break;
                            
                        }
                    }
                }
            }





        
        if(!(isMuJet) ||  pfMET < 50) continue;
        
        hcount->Fill(9);
        MyNewTree->Fill();
    }
    
    MyNewTree->AutoSave();
    hEvents->Write();
    hcount->Write();
    file->Close();
}

int main(int argc, char* argv[]){
    
    string FinaName=argv[1];
    stringstream ss(FinaName);
    
    string token;
    string M;
    int count=0;
    string realName;
    while (getline(ss,token, '/'))
    {
        count++;
        cout<< token <<endl;
        if (count == 5) {
            cout<<"   ----->    5   "<<token<<"  _____   \n";
            realName=token;
        }
        M=token;
    }
    
    TString outputName = "skimed_"+realName+M;
    cout<<" outputName is ---> "<<outputName<<"\n";
    
    Skimmer t("root://cmsxrootd.fnal.gov//store/user/abdollah/Crab3/ggNtuple/80X/"+FinaName);
    //FinaName.erase(FinaName.begin(),FinaName.end()-10);
    //  Skimmer t("root://cmsxrootd.fnal.gov//"+FinaName);
    t.Loop(outputName, 0);
    return 0;
}









