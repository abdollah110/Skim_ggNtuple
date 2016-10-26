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
        
        
        bool isMuTau=0;
        bool isEleTau=0;
	bool isEleMu=0;
        bool isMu=0;
        bool isEle=0;
        bool isTau=0;

                               TLorentzVector Mu4Momentum, Tau4Momentum,Ele4Momentum, Z4Momentum, Jet4Momentum,ExtraMu4Momentum, Extraele4Momentum;

        
        for (int iele = 0; iele < nEle; ++iele){
         

                           bool eleMVAIdExtra= false;
                if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVANonTrg->at(iele) >  0.913286 ) eleMVAIdExtra= true;
                else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.444 && eleIDMVANonTrg->at(iele) >  0.805013 ) eleMVAIdExtra= true;
                else if ( fabs (eleSCEta->at(iele)) >=  1.444 && eleIDMVANonTrg->at(iele) >  0.358969  ) eleMVAIdExtra= true;
                else eleMVAIdExtra= false; 
            
            
            if (elePt->at(iele) > 50 && fabs(eleEta->at(iele))< 2.1 && eleMVAIdExtra )
            {isEle=1;
                hcount->Fill(3);
 Ele4Momentum.SetPtEtaPhiM(elePt->at(iele),eleEta->at(iele),elePhi->at(iele),eleMass);
}
        }
        
        for (int imu = 0; imu < nMu; ++imu){
            
            float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
            if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
                IsoMu= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
            
            if (muPt->at(imu) > 50 &&  fabs(muEta->at(imu)) < 2.1 &&   muIsTightID->at(imu) > 0  && IsoMu < 0.15 )
            {isMu=1;
                hcount->Fill(4);
Mu4Momentum.SetPtEtaPhiM(muPt->at(imu),muEta->at(imu),muPhi->at(imu),MuMass);
            }
            
            
        }
        
        
        
        
        for (int itau = 0; itau < nTau; ++itau){
            if (tauPt->at(itau) > 48 && fabs(tauEta->at(itau)) < 2.3 && tauCombinedIsolationDeltaBetaCorrRaw3Hits->at(itau) < tauPt->at(itau) * 0.3 && tauByLooseMuonRejection3->at(itau) &&  tauByMVA6LooseElectronRejection->at(itau) ){
                isTau=1;hcount->Fill(5);
// Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauMass->at(itau));
}
        }
        
int numJet=0;
 //                       for (int ijet= 0 ; ijet < nJet ; ijet++){
   //                         Jet4Momentum.SetPtEtaPhiE(jetPt->at(ijet),jetEta->at(ijet),jetPhi->at(ijet),jetEn->at(ijet));
     //                      
       //                     if ( jetPt->at(ijet) > 40 && fabs(jetEta->at(ijet)) < 2.5 ){
	//			numJet++;
          //                  }
            //            }


        if ( isMu && isTau ) {isMuTau=1; hcount->Fill(6);}
        if ( isEle && isTau ) {isEleTau=1;hcount->Fill(7);}
        if ( isEle && isMu ) {isEleMu=1;hcount->Fill(8);}
        
       // if(!(isMuTau) && !(isEleTau) && !(isEleMu)) continue;
        if(!(isEleMu)) continue;
 
       hcount->Fill(9);
        
        MyNewTree->Fill();
        
    }
    MyNewTree->AutoSave();
    //    MyNewTree->Write();
   // hPU->Write();
   // hPUTrue->Write();
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









