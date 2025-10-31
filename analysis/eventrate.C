#include <vector>
#include <string>
#include <algorithm>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TStyle.h"
#include "TText.h"

void eventrate()
{
  gStyle->SetOptStat(0);

	int eventID[4];
	
	TFile*out15degreeFile = new TFile("/Users/seonhonam/geant4/FAZIA_test/outputfiles/output_1_15degree.root","read");
	TFile*out30degreeFile = new TFile("/Users/seonhonam/geant4/FAZIA_test/outputfiles/output_1_30degree.root","read");
	TFile*out60degreeFile = new TFile("/Users/seonhonam/geant4/FAZIA_test/outputfiles/output_1_60degree.root","read");
	TFile*out90degreeFile = new TFile("/Users/seonhonam/geant4/FAZIA_test/outputfiles/output_1_90degree.root","read");
	TTree *out15degreeTree  = (TTree*)out15degreeFile -> Get("event_edep");
	TTree *out30degreeTree  = (TTree*)out30degreeFile -> Get("event_edep");
	TTree *out60degreeTree  = (TTree*)out60degreeFile -> Get("event_edep");
	TTree *out90degreeTree  = (TTree*)out90degreeFile -> Get("event_edep");
	
	out15degreeTree -> SetBranchAddress("eventID",&eventID[0]);
	out30degreeTree -> SetBranchAddress("eventID",&eventID[1]);
	out60degreeTree -> SetBranchAddress("eventID",&eventID[2]);
	out90degreeTree -> SetBranchAddress("eventID",&eventID[3]);


	int eventID_Pure_15 = 0;
	int eventID_Pure_30 = 0;
	int eventID_Pure_60 = 0;
	int eventID_Pure_90 = 0;

	TH1D* hitNumber[4];
	int entries;
	for(int i = 0;i < 4;i++){
		
		hitNumber[i] = new TH1D(Form("hitNumber%d",i),"",10000000,0,10000000);
		entries = 0;
		if(i == 0)entries = out15degreeTree -> GetEntries();
		if(i == 1)entries = out30degreeTree -> GetEntries();
		if(i == 2)entries = out60degreeTree -> GetEntries();
		if(i == 3)entries = out90degreeTree -> GetEntries();
		cout <<i<<" " <<entries <<endl;
		for(int ient = 0;ient < entries;ient++){
		
			if(i == 0)out15degreeTree -> GetEntry(ient);
			if(i == 1)out30degreeTree -> GetEntry(ient);
			if(i == 2)out60degreeTree -> GetEntry(ient);
			if(i == 3)out90degreeTree -> GetEntry(ient);
			
			if(i == 0 && eventID[i] >=1)eventID_Pure_15++;
			if(i == 1 && eventID[i] >=1)eventID_Pure_30++;
			if(i == 2 && eventID[i] >=1)eventID_Pure_60++;
			if(i == 3 && eventID[i] >=1)eventID_Pure_90++;
			
			hitNumber[i] ->Fill(eventID[i]);
		}	
	

	}
	TLatex info;
	info.SetTextSize(0.08);
	info.SetTextAlign(22);
	info.SetTextAngle(0);

	TCanvas* eventIDCan = new TCanvas("eventIDCan","",1000,800);
	eventIDCan ->Divide(2,2);
	
	eventIDCan ->cd(1);
	hitNumber[0] ->Draw();
	hitNumber[0] ->GetYaxis()->SetRangeUser(0,2);
	info.DrawLatex(5000000,1.5,Form("15 Degree rate : %.0lf",eventID_Pure_15*10000.));

	eventIDCan ->cd(2);
	hitNumber[1] ->Draw();
	hitNumber[1] ->GetYaxis()->SetRangeUser(0,2);
	info.DrawLatex(5000000,1.50,Form("30 Degree rate : %.0lf",eventID_Pure_30*10000.));
	
	eventIDCan ->cd(3);
	hitNumber[2] ->Draw();
	hitNumber[2] ->GetYaxis()->SetRangeUser(0,2);
	info.DrawLatex(5000000,1.50,Form("60 Degree rate : %.0lf",eventID_Pure_60*10000.));
	
	eventIDCan ->cd(4);
	hitNumber[3] ->Draw();
	hitNumber[3] ->GetYaxis()->SetRangeUser(0,2);
	info.DrawLatex(5000000,1.50,Form("90 Degree rate : %.0lf",eventID_Pure_90*10000.));
	
	cout <<"15 degree hit number : "<< eventID_Pure_15<<endl;
	cout <<"30 degree hit number : "<< eventID_Pure_30<<endl;

	cout <<"60 degree hit number : "<< eventID_Pure_60<<endl;
	cout <<"90 degree hit number : "<< eventID_Pure_90<<endl;
	
}
