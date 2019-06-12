
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <TROOT.h>
#include <boost/bind.hpp>
#include "nTupleAnalysis/baseClasses/interface/helpers.h"

#include "TriggerStudies/NtupleAna/interface/BTagAnalysis.h"

using namespace TriggerStudies;

BTagAnalysis::BTagAnalysis(TChain* _eventsRAW, TChain* _eventsAOD, fwlite::TFileService& fs, bool _isMC, std::string _year, int _histogramming, bool _debug){
  if(_debug) std::cout<<"In BTagAnalysis constructor"<<std::endl;
  debug      = _debug;
  isMC       = _isMC;
  year       = _year;
  eventsRAW     = _eventsRAW;
  eventsRAW->SetBranchStatus("*", 0);

  eventsAOD     = _eventsAOD;
  eventsAOD->SetBranchStatus("*", 0);


  histogramming = _histogramming;

  event      = new eventData(eventsRAW, eventsAOD, isMC, year, debug);
  treeEvents = eventsRAW->GetEntries();

  cutflow    = new nTupleAnalysis::cutflowHists("cutflow", fs);
  cutflow->AddCut("all");
  cutflow->AddCut("foundMatch");
  cutflow->AddCut("passLeptonCut");

  //
  // hists
  //
  dir = fs.mkdir("BTagAnalysis");

  hMuons = new nTupleAnalysis::muonHists("muons", fs, "All Muons");
  hElecs = new nTupleAnalysis::elecHists("elecs", fs, "All Elecs");

  hOffJetsPreOLap      = new nTupleAnalysis::jetHists("offJetsPreOLap",    fs, "Pre Overlap");
  hOffJets             = new nTupleAnalysis::jetHists("offJets",           fs, "");
  hOffJets_matched     = new nTupleAnalysis::jetHists("offJets_matched",   fs, "");
  hOffJets_matchedJet  = new nTupleAnalysis::jetHists("offJets_matchedJet",fs, "");

  hOffJets_L     = new nTupleAnalysis::jetHists("offJets_L",   fs, "");
  hOffJets_matched_L     = new nTupleAnalysis::jetHists("offJets_matched_L",   fs, "");
  hOffJets_matchedJet_L  = new nTupleAnalysis::jetHists("offJets_matchedJet_L",fs, "");
  hOffJets_matched_B     = new nTupleAnalysis::jetHists("offJets_matched_B",   fs, "");
  hOffJets_B     = new nTupleAnalysis::jetHists("offJets_B",   fs, "");
  hOffJets_matchedJet_B  = new nTupleAnalysis::jetHists("offJets_matchedJet_B",fs, "");


  hPfJets          = new nTupleAnalysis::jetHists("pfJets",           fs, "");
  hPfJets_matched  = new nTupleAnalysis::jetHists("pfJets_matched",           fs, "");
  hPfJets_matched_B  = new nTupleAnalysis::jetHists("pfJets_matched_B",           fs, "");
  hPfJets_matched_L  = new nTupleAnalysis::jetHists("pfJets_matched_L",           fs, "");

  hCaloJets          = new nTupleAnalysis::jetHists("caloJets",           fs, "");
  hCaloJets_matched  = new nTupleAnalysis::jetHists("caloJets_matched",           fs, "");

  hOffTracks           = new nTupleAnalysis::trackHists("offTracks",fs, "");
  hOffTracks_unmatched = new nTupleAnalysis::trackHists("offTracks_unmatched",fs, "");
  hOffTracks_matched   = new nTupleAnalysis::trackHists("offTracks_matched",fs, "");
  hPfTracks            = new nTupleAnalysis::trackHists("pfTracks",fs, "");
  hPfTracks_noV0       = new nTupleAnalysis::trackHists("pfTracks_noV0",fs, "");
  hPfTracks_matched    = new nTupleAnalysis::trackHists("pfTracks_matched",fs, "");
  hPfTracks_unmatched  = new nTupleAnalysis::trackHists("pfTracks_unmatched",fs, "");

  hOffTracks_noV0           = new nTupleAnalysis::trackHists("offTracks_noV0",fs, "");
  hOffTracks_matched_noV0   = new nTupleAnalysis::trackHists("offTracks_matched_noV0",fs, "");

  hOffBTags           = new nTupleAnalysis::btaggingHists("offBTags",fs, "");
  hOffBTags_matched   = new nTupleAnalysis::btaggingHists("offBTags_matched",fs, "");
  hOffBTags_noV0           = new nTupleAnalysis::btaggingHists("offBTags_noV0",fs, "");
  hOffBTags_matched_noV0   = new nTupleAnalysis::btaggingHists("offBTags_matched_noV0",fs, "");

  hPfBTags           = new nTupleAnalysis::btaggingHists("pfBTags",fs, "");

  hmttOff           = dir.make<TH1F>("mtt_off",            "BTagAnalysis/mtt_off;             mtt;   Entries", 100,-0.01, 2);
  hmttOff_isFromV0  = dir.make<TH1F>("mtt_off_isFromV0",   "BTagAnalysis/mtt_off_isFromV0;    mtt;   Entries", 100,-0.01, 2);
  hmttPf            = dir.make<TH1F>("mtt_pf",             "BTagAnalysis/mtt_pf;              mtt;   Entries", 100,-0.01, 2);
  hmttPf_isFromV0   = dir.make<TH1F>("mtt_pf_isFromV0",    "BTagAnalysis/mtt_pf_isFromV0;     mtt;   Entries", 100,-0.01, 2);


  //  if(histogramming >= 4) allEvents     = new eventHists("allEvents",     fs);
  //  if(histogramming >= 3) passPreSel    = new   tagHists("passPreSel",    fs, true, isMC, blind);
  //  if(histogramming >= 2) passDijetMass = new   tagHists("passDijetMass", fs, true, isMC, blind);
  //  if(histogramming >= 1) passMDRs      = new   tagHists("passMDRs",      fs, true, isMC, blind);
  //  //if(histogramming > 1        ) passMDCs     = new   tagHists("passMDCs",   fs, true, isMC, blind);
  //  //if(histogramming > 0        ) passDEtaBB   = new   tagHists("passDEtaBB", fs, true, isMC, blind);
} 


void BTagAnalysis::monitor(long int e){
  //Monitor progress
  percent        = (e+1)*100/nEvents;
  duration       = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  eventRate      = (e+1)/duration;
  timeRemaining  = (nEvents-e)/eventRate;
  minutes = static_cast<int>(timeRemaining/60);
  seconds = static_cast<int>(timeRemaining - minutes*60);
  getrusage(who, &usage);
  usageMB = usage.ru_maxrss/1024;
  //print status and flush stdout so that status bar only uses one line
  if(isMC){
    fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s | done in %02i:%02i | memory usage: %li MB)       ", 
	    e+1, nEvents, percent,   eventRate,    minutes, seconds,                usageMB);
  }else{
    fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s | done in %02i:%02i | memory usage: %li MB | LumiBlocks %i  )       ", 
	    e+1, nEvents, percent,   eventRate,    minutes, seconds,                usageMB,            nls );    
  }
  fflush(stdout);
}

int BTagAnalysis::eventLoop(int maxEvents){
  //Set Number of events to process. Take manual maxEvents if maxEvents is > 0 and less than the total number of events in the input files. 
  nEvents = (maxEvents > 0 && maxEvents < treeEvents) ? maxEvents : treeEvents;
  
  std::cout << "\nProcess " << nEvents << " of " << treeEvents << " events.\n";

  start = std::clock();//2546000 //2546043
  for(long int e = 0; e < nEvents; e++){

    event->update(e);    
    processEvent();
    if(debug) event->dump();

    //periodically update status
    if( (e+1)%10000 == 0 || e+1==nEvents || debug) 
      monitor(e);

  }
  
  std::cout << std::endl;
  std::cout << "BTagAnalysis::End of Event Loop" << std::endl;
  if(!isMC) std::cout << "Runs " << firstRun << "-" << lastRun << std::endl;

  minutes = static_cast<int>(duration/60);
  seconds = static_cast<int>(duration - minutes*60);
                                        
  if(isMC){
    fprintf(stdout,"---------------------------\nProcessed in %02i:%02i", minutes, seconds);
  }else{
    fprintf(stdout,"---------------------------\nProcessed in %02i:%02i", minutes, seconds);
  }
  return 0;
}

int BTagAnalysis::processEvent(){
  if(debug) std::cout << "processEvent start" << std::endl;
  float weight = 1.0;

  cutflow->Fill("all", weight);
  if(event->run != event->runAOD)
    return 0;

  if(event->event != event->eventAOD)
    return 0;

  cutflow->Fill("foundMatch", weight);

  hMuons->nMuons->Fill(event->muons.size());
  for(auto muon: event->muons)
    hMuons->Fill(muon,weight);

  hElecs->nElecs->Fill(event->elecs.size());
  for(auto elec: event->elecs)
    hElecs->Fill(elec,weight);


  if((event->muons.size()+event->elecs.size()) < 2)  {
    if(debug) std::cout << "Fail Lepton Cut" << std::endl;
    return 0;
  }
  cutflow->Fill("passLeptonCut", weight);

  //
  // Fill All events
  //
  //eventHists.Fill(eventData);

  //
  //
  //
  unsigned int nOffJetsPreOLap  = 0;
  unsigned int nOffJets         = 0;
  unsigned int nOffJets_matched = 0;

  for(const nTupleAnalysis::jetPtr& offJet : event->offJets){

    if(fabs(offJet->eta) > 2.4) continue;
    if(offJet->pt       < 35)   continue;

    ++nOffJetsPreOLap;
    hOffJetsPreOLap->Fill(offJet, weight);


    if(nTupleAnalysis::failOverlap(offJet->p,event->elecs,0.4)) continue;
    if(nTupleAnalysis::failOverlap(offJet->p,event->muons,0.4)) continue;

    // Match offline to online
    float dR = 1e6;
    nTupleAnalysis::jetPtr matchedJet = nullptr;

    for(const nTupleAnalysis::jetPtr& pfJet : event->pfJets){
      float this_dR = pfJet->p.DeltaR(offJet->p);
      if (this_dR < dR){
	dR = this_dR;
	matchedJet = pfJet;
      }
    }

    if( dR < 0.4){
      matchedJet->matchedJet = offJet;
      matchedJet->match_dR   = dR;
      offJet->matchedJet     = matchedJet;
      offJet->match_dR       = dR;
    }

    // match tracks if we matched jets
    const nTupleAnalysis::jetPtr offJetMatchedJet = offJet->matchedJet.lock();
    if(offJetMatchedJet){

      //
      //  Off tracks
      //
      unsigned int nOffTracks = 0;
      unsigned int nOffTracks_matched = 0;
      unsigned int nOffTracks_noV0 = 0;
      unsigned int nOffTracks_matched_noV0 = 0;

      for(const nTupleAnalysis::trackPtr& offTrack: offJet->tracks){
	
	//
	//  track mass calculation
	//
	for(const nTupleAnalysis::trackPtr& offTrack_pair: offJet->tracks){
	  if(offTrack == offTrack_pair) continue;
	  float thisMass = (offTrack->p + offTrack_pair->p).M();
	  
	  hmttOff->Fill(thisMass , weight);
	  if(offTrack->isfromV0)
	    hmttOff_isFromV0->Fill(thisMass , weight);
	  
	}

	//need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
	//if offTrack.dR > 0.29 - offJet.match_dR: continue
	if(offTrack->dR                              > 0.29) continue; // offTrack is not in cone of offJet
	if(offTrack->p.DeltaR(offJetMatchedJet->p) > 0.29) continue; // offTrack is not in cone of pfJet
	
	
	hOffTracks->Fill(offTrack, weight);
	++nOffTracks;
	if(!offTrack->isfromV0){
	  hOffTracks_noV0->Fill(offTrack, weight);	  
	  ++nOffTracks_noV0;
	}

	float dR = 1e6;
	float dR2 = 1e6;
	float dR_noV0 = 1e6;
	nTupleAnalysis::trackPtr matchedTrack  = nullptr;
	nTupleAnalysis::trackPtr secondClosest = nullptr;

	for(const nTupleAnalysis::trackPtr& pfTrack: offJetMatchedJet->tracks){
	  //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
	  if(pfTrack->dR                  > 0.29) continue; // pfTrack is not in cone of pfJet
	  if(pfTrack->p.DeltaR(offJet->p) > 0.29) continue; // pfTrack is not in cone of offJet

	  float this_dR = offTrack->p.DeltaR(pfTrack->p);
	  if(this_dR > dR && this_dR < dR2){
	    dR2 = this_dR;
	    secondClosest = pfTrack;
	  }

	  if(this_dR < dR){
	    dR2 = dR;
	    secondClosest = matchedTrack;
	    
	    dR  = this_dR;
	    matchedTrack = pfTrack;
	  }
	  
	  if(!pfTrack->isfromV0 && this_dR < dR_noV0){
	    dR_noV0 = this_dR;
	  }
	  
	}// matched pf tracks
	
	if( dR > 0.01){
	  //if dR > 1e5:
	  hOffTracks_unmatched->Fill(offTrack, weight);
	  continue;
	}
	
	if(!offTrack->isfromV0 && (dR_noV0 < 0.01)){
	  hOffTracks_matched_noV0->Fill(offTrack, weight);
	  ++nOffTracks_matched_noV0;
	}

	++nOffTracks_matched;
	matchedTrack->matchedTrack = offTrack;
	offTrack->matchedTrack     = matchedTrack;
	offTrack->secondClosest    = secondClosest;
	offTrack->nMatches              += 1;
	matchedTrack->nMatches += 1;
	hOffTracks_matched    ->Fill(offTrack, weight);
	const nTupleAnalysis::trackPtr offTrackMatchedTrack = offTrack->matchedTrack.lock();
	hPfTracks_matched ->Fill(offTrackMatchedTrack,weight);
      }//off Tracks
      
      hOffTracks             ->nTracks->Fill(nOffTracks, weight);
      hOffTracks_matched     ->nTracks->Fill(nOffTracks_matched, weight);
      hOffTracks_noV0        ->nTracks->Fill(nOffTracks_noV0, weight);
      hOffTracks_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


      //
      //  Off BTags
      //
      unsigned int nTrkTags = 0;
      unsigned int nTrkTags_matched = 0;
      unsigned int nTrkTags_noV0 = 0;
      unsigned int nTrkTags_matched_noV0 = 0;

      for(const nTupleAnalysis::trkTagVarPtr& offTrkTag: offJet->trkTagVars){
	//need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
	//if offTrack.dR > 0.29 - offJet.match_dR: continue
	if(offTrkTag->trackDeltaR                              > 0.29) continue; // offTrack is not in cone of offJet
	if(offTrkTag->p.DeltaR(offJetMatchedJet->p) > 0.29) continue; // offTrack is not in cone of pfJet


	hOffBTags->FillTrkTagVarHists(offTrkTag, weight);
	++nTrkTags;

	if(!offTrkTag->matchIsFromV0){
	  hOffBTags_noV0->FillTrkTagVarHists(offTrkTag, weight);	  
	  ++nTrkTags_noV0;
	}

	float dR = 1e6;
	float dR_noV0 = 1e6;
	nTupleAnalysis::trkTagVarPtr matchedTrkTag  = nullptr;
	for(const nTupleAnalysis::trkTagVarPtr& pfTrkTag: offJetMatchedJet->trkTagVars){
	  if(pfTrkTag->trackDeltaR                  > 0.29) continue; // pfTrack is not in cone of pfJet
	  if(pfTrkTag->p.DeltaR(offJet->p) > 0.29) continue; // pfTrack is not in cone of offJet

	  float this_dR = offTrkTag->p.DeltaR(pfTrkTag->p);
	  if(this_dR < dR){
	    dR  = this_dR;
	    matchedTrkTag = pfTrkTag;
	  }

	  if(!pfTrkTag->matchIsFromV0 && this_dR < dR_noV0){
	    dR_noV0 = this_dR;
	  }

	}
	

	if( dR > 0.01){
	  //if dR > 1e5:
	  //hOffBTags_unmatched->Fill(offTrack, weight);
	  continue;
	}

	if(!offTrkTag->matchIsFromV0 && (dR_noV0 < 0.01)){
	  hOffBTags_matched_noV0->FillTrkTagVarHists(offTrkTag, weight);
	  ++nTrkTags_matched_noV0;
	}

	++nTrkTags_matched;
	matchedTrkTag->matchedTrkTagVar = offTrkTag;
	offTrkTag->matchedTrkTagVar     = matchedTrkTag;
	hOffBTags_matched    ->FillTrkTagVarHists(offTrkTag, weight);
	//const nTupleAnalysis::trackPtr offTrackMatchedTrkTag = offTrkTag->matchedTrkTagVar.lock();
	//hPfTracks_matched ->FillTrkTagVarHists(offTrackMatchedTrkTag,weight);


      }//OffTrkTag

      hOffBTags             ->trkTag_nTracks->Fill(nTrkTags, weight);
      hOffBTags_matched     ->trkTag_nTracks->Fill(nTrkTags_matched, weight);
      hOffBTags_noV0        ->trkTag_nTracks->Fill(nTrkTags_noV0, weight);
      hOffBTags_matched_noV0->trkTag_nTracks->Fill(nTrkTags_matched_noV0, weight);

      //
      // PF Tracks
      //
      unsigned int nPfTracks = 0;
      unsigned int nPfTracks_matched = 0;
      unsigned int nPfTracks_noV0 = 0;
      for(const nTupleAnalysis::trackPtr& pfTrack: offJetMatchedJet->tracks){

	//
	//  track mass calculation
	//
	for(const nTupleAnalysis::trackPtr& pfTrack_pair: offJetMatchedJet->tracks){
	  if(pfTrack == pfTrack_pair) continue;
	  float thisMass = (pfTrack->p + pfTrack_pair->p).M();
	  
	  hmttPf->Fill(thisMass , weight);
	  if(pfTrack->isfromV0)
	    hmttPf_isFromV0->Fill(thisMass , weight);
	  
	}

	//need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
	if(pfTrack->dR                  > 0.29) continue; // pfTrack is not in cone of pfJet
	if(pfTrack->p.DeltaR(offJet->p) > 0.29) continue; // pfTrack is not in cone of offJet
	
	hPfTracks->Fill(pfTrack, weight); //all pftracks in matched jets
	hPfTracks->FillMatchStats(pfTrack, weight); //check how often we match pfTracks to more than one offTrack
	++nPfTracks;

	if(!pfTrack->isfromV0){
	  hPfTracks_noV0->Fill(pfTrack, weight); //all pftracks in matched jets
	  ++nPfTracks_noV0;
	}

	if(!pfTrack->nMatches){
	  hPfTracks_unmatched->Fill(pfTrack, weight); //all unmatched pftracks
	  hPfTracks_unmatched->FillMatchStats(pfTrack, weight);
	}else{
	  hPfTracks_matched->FillMatchStats(pfTrack, weight);
	  ++nPfTracks_matched;
	}
      }// pfTracks

      hPfTracks              ->nTracks->Fill(nPfTracks, weight);
      hPfTracks_matched      ->nTracks->Fill(nPfTracks_matched, weight);
      hPfTracks_noV0         ->nTracks->Fill(nPfTracks_noV0, weight);
      //hOffTracks_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


      //
      //  PF BTags
      //
      unsigned int nPfTrkTags = 0;

      for(const nTupleAnalysis::trkTagVarPtr& pfTrkTag: offJetMatchedJet->trkTagVars){
	//need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
	//if offTrack.dR > 0.29 - offJet.match_dR: continue
	if(pfTrkTag->trackDeltaR                              > 0.29) continue; // offTrack is not in cone of offJet
	if(pfTrkTag->p.DeltaR(offJet->p) > 0.29) continue; // offTrack is not in cone of pfJet


	hPfBTags->FillTrkTagVarHists(pfTrkTag, weight);
	++nPfTrkTags;

      }//OffTrkTag

      hPfBTags             ->trkTag_nTracks->Fill(nPfTrkTags, weight);



    }//offJet has match

    // Fill offJetHists
    // offJets_ROC
    //  wp60 DeepCSV > 0.76 (actual efficiency = 0.604992842829)
    //  wp70 DeepCSV > 0.56 (actual efficiency = 0.717503578586)
    //  wp80 DeepCSV > 0.36 (actual efficiency = 0.808474091039)
    //  wp90 DeepCSV > 0.12 (actual efficiency = 0.912533638706)
    ++nOffJets;
    hOffJets->Fill(offJet,weight);
    if(offJet->hadronFlavour == 5){
      hOffJets_B->Fill(offJet,weight);
    }else if(offJet->hadronFlavour == 0){
      hOffJets_L->Fill(offJet,weight);
    }
      
    if(offJetMatchedJet){
      ++nOffJets_matched;
      hOffJets_matched->Fill(offJet,weight);
      hOffJets_matchedJet->Fill(offJetMatchedJet,weight);

      ///	if( offJet.m_deepcsv > 0.56) offJetHists_offline70_matched.Fill(offJet);

      if(offJet->hadronFlavour == 5){
	hOffJets_matched_B->Fill(offJet, weight);
	hOffJets_matchedJet_B->Fill(offJetMatchedJet, weight);
      }else if(offJet->hadronFlavour == 0){
	hOffJets_matched_L->Fill(offJet, weight);
	hOffJets_matchedJet_L->Fill(offJetMatchedJet, weight);
      }

      ///        // pfJets_matched_ROC
      ///	//  wp60 DeepCSV > 0.64 (actual efficiency = 0.610276798066)
      ///	//  wp70 DeepCSV > 0.48 (actual efficiency = 0.708732661175)
      ///	//  wp80 DeepCSV > 0.28 (actual efficiency = 0.814155211306)
      ///	//  wp90 DeepCSV > 0.08 (actual efficiency = 0.924525480128)
      ///	if(offJet.m_matchedJet->m_deepcsv > 0.64){ //approximate 60% Online WP
      ///	  offJetHists_matched_online60.Fill(offJet);
      ///	  if( offJet.m_deepcsv > 0.56) offJetHists_offline70_matched_online60.Fill(offJet);
      ///	  if(offJet.m_hadronFlavour == 5){
      ///	    offJetHists_B_matched_online60.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_B_matched_online60.Fill(offJet);
      ///	  }else{
      ///	    offJetHists_L_matched_online60.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_L_matched_online60.Fill(offJet);
      ///	  }
      ///	}
      ///
      ///	if(offJet.m_matchedJet->m_deepcsv > 0.48){ //approximate 70% Online WP
      ///	  offJetHists_matched_online70.Fill(offJet);
      ///	  if(offJet.m_deepcsv > 0.56) offJetHists_offline70_matched_online70.Fill(offJet);
      ///	  if(offJet.m_hadronFlavour == 5){
      ///	    offJetHists_B_matched_online70.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_B_matched_online70.Fill(offJet);
      ///	  }else{
      ///	    offJetHists_L_matched_online70.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_L_matched_online70.Fill(offJet);
      ///	  }
      ///	}
      ///	
      ///	if(offJet.m_matchedJet->m_deepcsv > 0.28){ //approximate 80% Online WP
      ///	  offJetHists_matched_online80.Fill(offJet);
      ///	  if(offJet.m_deepcsv > 0.56) offJetHists_offline70_matched_online80.Fill(offJet);
      ///	  if(offJet.m_hadronFlavour == 5){
      ///	    offJetHists_B_matched_online80.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_B_matched_online80.Fill(offJet);
      ///	  }else{
      ///	    offJetHists_L_matched_online80.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_L_matched_online80.Fill(offJet);
      ///	  }
      ///	}
      ///      
      ///	if(offJet.m_matchedJet->m_deepcsv > 0.08){ //approximate 90% Online WP
      ///	  offJetHists_matched_online90.Fill(offJet);
      ///	  if(offJet.m_deepcsv > 0.56) offJetHists_offline70_matched_online90.Fill(offJet);
      ///	  if(offJet.m_hadronFlavour == 5){
      ///	    offJetHists_B_matched_online90.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_B_matched_online90.Fill(offJet);
      ///	  }else{
      ///	    offJetHists_L_matched_online90.Fill(offJet);
      ///	    if(offJet.m_deepcsv > 0.56) offJetHists_offline70_L_matched_online90.Fill(offJet);
      ///	  }
      ///	}
    }//m_matchedJet

    // Match offline to online
    for(const nTupleAnalysis::jetPtr& caloJet : event->caloJets){
      float deltaR = caloJet->p.DeltaR(offJet->p);
      if( deltaR < 0.4){
	caloJet->matchedJet = offJet;
	break;
      }
    }

  }//offJets


  hOffJetsPreOLap    ->nJets->Fill(nOffJetsPreOLap ,weight);
  hOffJets           ->nJets->Fill(nOffJets        ,weight);
  hOffJets_matched   ->nJets->Fill(nOffJets_matched,weight);
  hOffJets_matchedJet->nJets->Fill(nOffJets_matched,weight);



  //
  //  pf Jets
  //
  for(const nTupleAnalysis::jetPtr& pfJet : event->pfJets){
    if(fabs(pfJet->eta) > 2.5) continue;
    if(pfJet->pt       < 35)   continue;
  
    //pfJetHistsPreOLap.Fill(pfJet);
  
    if(nTupleAnalysis::failOverlap(pfJet->p,event->elecs, 0.4)) continue;
    if(nTupleAnalysis::failOverlap(pfJet->p,event->muons, 0.4)) continue;
  
    hPfJets->Fill(pfJet, weight);
  
    const nTupleAnalysis::jetPtr pfJetMatchedJet = pfJet->matchedJet.lock();
    if(pfJetMatchedJet){
      hPfJets_matched->Fill(pfJet, weight);
  
      if( pfJetMatchedJet->hadronFlavour == 5){
        hPfJets_matched_B->Fill(pfJet, weight);
      }else if( pfJetMatchedJet->hadronFlavour == 0){
        hPfJets_matched_L->Fill(pfJet, weight);
      }
    }
  }
  
  //
  //  calo Jets
  //
  for(const nTupleAnalysis::jetPtr& caloJet : event->caloJets){
    if(fabs(caloJet->eta) > 2.5) continue;
    if(caloJet->pt       < 35)   continue;
  
    //caloJetHistsPreOLap.Fill(caloJet);
    if(nTupleAnalysis::failOverlap(caloJet->p,event->elecs, 0.4)) continue;
    if(nTupleAnalysis::failOverlap(caloJet->p,event->muons, 0.4)) continue;
  
    hCaloJets->Fill(caloJet, weight);

    const nTupleAnalysis::jetPtr caloJetMatchedJet = caloJet->matchedJet.lock();  
    if(caloJetMatchedJet){
      hCaloJets_matched->Fill(caloJet, weight);
  
      //if(caloJet->m_matchedJet->m_hadronFlavour == 5)
      //	caloJetHists_matchedB.Fill(caloJet);
      //	else
      //	  caloJetHists_matchedL.Fill(caloJet);
    }
  }
 
    

  
 

  //
  //if we are processing data, first apply lumiMask and trigger
  //
  if(!isMC){
    if(!passLumiMask()){
      if(debug) std::cout << "Fail lumiMask" << std::endl;
      return 0;
    }
    //cutflow->Fill(event, "lumiMask", true);

    //keep track of total lumi

    //if(!event->passHLT){
    //  if(debug) std::cout << "Fail HLT: data" << std::endl;
    //  return 0;
    //}
    //cutflow->Fill(event, "HLT", true);
  }
  //if(allEvents != NULL && event->passHLT) allEvents->Fill(event);





  return 0;
}

bool BTagAnalysis::passLumiMask(){
  // if the lumiMask is empty, then no JSON file was provided so all
  // events should pass
  if(lumiMask.empty()) return true;

  //make lumiID run:lumiBlock
  //edm::LuminosityBlockID lumiID(event->run, event->lumiBlock);

  //define function that checks if a lumiID is contained in a lumiBlockRange
  bool (*funcPtr) (edm::LuminosityBlockRange const &, edm::LuminosityBlockID const &) = &edm::contains;

  //Loop over the lumiMask and use funcPtr to check for a match
  //std::vector< edm::LuminosityBlockRange >::const_iterator iter = std::find_if (lumiMask.begin(), lumiMask.end(), boost::bind(funcPtr, _1, lumiID) );

  //return lumiMask.end() != iter; 
  return true;
}



BTagAnalysis::~BTagAnalysis(){
  std::cout << "BTagAnalysis::destroyed" << std::endl;
} 

