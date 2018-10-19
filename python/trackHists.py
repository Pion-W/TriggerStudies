import ROOT
from array import array
from helpers import makeHist

class TrackHists:

    def __init__(self,name, directory):
        
        self.name = name
        self.thisDir = directory
        #self.thisDir = outFile.mkdir(self.name)

        self.nTrk = makeHist(self.thisDir, "nTrk","nTrk;nTracks;Entries",42,-1.5,40.5)
            
        self.ip3d_l = makeHist(self.thisDir, "ip3d_l","ip3d;IP3D [cm]",100,-0.2,0.2)
        self.ip3d   = makeHist(self.thisDir, "ip3d",  "ip3d;IP3D [cm]",100,-0.05,0.05)
            
        self.ip3d_sig_l = makeHist(self.thisDir, "ip3d_sig_l","ip3d sig;IP3D significance",100,-100,100)
        self.ip3d_sig   = makeHist(self.thisDir, "ip3d_sig",  "ip3d sig;IP3D significance",100,-10,10)
            
        self.ip3d_err_l = makeHist(self.thisDir, "ip3d_err_l","ip3d err;IP3D uncertianty [cm]",100,-0.01,0.1)
        self.ip3d_err   = makeHist(self.thisDir, "ip3d_err",  "ip3d err;IP3D uncertianty [cm]",100,-0.001,0.01)
            
        self.ip2d_l = makeHist(self.thisDir, "ip2d_l","ip2d;IP2D [cm]",100,-0.2,0.2)
        self.ip2d   = makeHist(self.thisDir, "ip2d",  "ip2d;IP2D [cm]",100,-0.05,0.05)

            
        nBinsPt = array("d",[0,2,4,6,8,10,15,20,30,40,60])
        self.ip2d_vs_pt   = ROOT.TH2F("ip2d_vs_pt",  "ip2d_vs_pt;P_T [GeV]; IP2D [cm]",len(nBinsPt)-1,nBinsPt,100,-0.03,0.03)
        self.ip2d_vs_pt.SetDirectory(self.thisDir)    


        nBinsEta = array("d",[0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5])
        self.ip2d_vs_eta   = ROOT.TH2F("ip2d_vs_eta",  "ip2d_vs_eta;|#eta|; IP2D [cm]",len(nBinsEta)-1,nBinsEta,100,-0.03,0.03)
        self.ip2d_vs_eta.SetDirectory(self.thisDir)    


        self.ip2d_sig_l = makeHist(self.thisDir, "ip2d_sig_l","ip2d sig;IP2D significance",100,-100,100)
        self.ip2d_sig   = makeHist(self.thisDir, "ip2d_sig",  "ip2d sig;IP2D significance",100,-10,10)
        
        self.ip2d_err_l = makeHist(self.thisDir, "ip2d_err_l","ip2d err;IP2D uncertianty [cm]",100,-0.01,0.1)
        self.ip2d_err   = makeHist(self.thisDir, "ip2d_err",  "ip2d err;IP2D uncertianty [cm]",100,-0.001,0.01)
    
        self.trackDecayLenVal_l         = makeHist(self.thisDir, "trackDecayLenVal_l"  ,    "trackDecayLenVal;trackDecayLenVal [cm];Entries", 100, -0.1,  5)
        self.trackDecayLenVal           = makeHist(self.thisDir, "trackDecayLenVal"    ,    "trackDecayLenVal;trackDecayLenVal [cm];Entries", 100, -0.1,  0.5)
        self.trackJetDistVal            = makeHist(self.thisDir, "trackJetDistVal"     ,    "trackJetDistVal;trackJetDistVal [cm];Entries",  100, -0.1,0.01)      
        self.trackPtRel                 = makeHist(self.thisDir, "trackPtRel"          ,    "trackPtRel;track Pt Rel [GeV];Entries", 100, -0.1, 7)          
        self.trackMomentum              = makeHist(self.thisDir, "trackMomentum"       ,    "trackMomentum;track momentum [GeV];Entries", 100, -0.1, 60)       
        self.trackEta                   = makeHist(self.thisDir, "trackEta"            ,    "trackEta;track #eta;Entries", 100, -2.6, 2.6)            
        self.trackPPar                  = makeHist(self.thisDir, "trackPPar"           ,    "trackPPar;track PPar [GeV];Entries",100, -0.1, 60)           
        self.trackDeltaR                = makeHist(self.thisDir, "trackDeltaR"         ,    "trackDeltaR;track #Delta R;Entries", 100, -0.1, 0.35)         
        self.trackEtaRel                = makeHist(self.thisDir, "trackEtaRel"         ,    "trackEtaRel;track Eta Rel;Entries", 100, 0, 7)         
        self.trackPtRatio               = makeHist(self.thisDir, "trackPtRatio"        ,    "trackPtRatio;track Pt Ratio;Entries", 100, -0.01, 0.3)        
        self.trackPParRatio             = makeHist(self.thisDir, "trackPParRatio"      ,    "trackPParRatio;track P Par Ratio;Entries", 100, 0.95, 1.02)      


    def Fill(self, tracks):
        nTracks = len(tracks)
        self.nTrk.Fill(nTracks)

        for track in tracks:
            this_ip3d = track.Sip3dVal
            self.ip3d  .Fill(this_ip3d)
            self.ip3d_l.Fill(this_ip3d)
            
            this_ip3d_sig = track.Sip3dSig
            self.ip3d_sig  .Fill(this_ip3d_sig)
            self.ip3d_sig_l.Fill(this_ip3d_sig)
    
            this_ip3d_err = this_ip3d/this_ip3d_sig
            self.ip3d_err  .Fill(this_ip3d_err)
            self.ip3d_err_l.Fill(this_ip3d_err)
            
    
            this_ip2d = track.Sip2dVal
            self.ip2d  .Fill(this_ip2d)
            self.ip2d_l.Fill(this_ip2d)
            
            this_ip2d_sig = track.Sip2dSig
            self.ip2d_sig  .Fill(this_ip2d_sig)
            self.ip2d_sig_l.Fill(this_ip2d_sig)
            
            this_ip2d_err = this_ip2d/this_ip2d_sig
            self.ip2d_err  .Fill(this_ip2d_err)
            self.ip2d_err_l.Fill(this_ip2d_err)

            this_trackMomentum = track.Momentum   
            self.ip2d_vs_pt.Fill(this_trackMomentum, this_ip2d)
            
            this_trackEta = track.Eta        
            self.ip2d_vs_eta.Fill(abs(this_trackEta), this_ip2d)
            

            self.trackDecayLenVal_l   .Fill(track.DecayLenVal)
            self.trackDecayLenVal     .Fill(track.DecayLenVal)
            self.trackJetDistVal      .Fill(track.JetDistVal )
            self.trackPtRel           .Fill(track.PtRel      )
            self.trackMomentum        .Fill(this_trackMomentum) 
            self.trackEta             .Fill(this_trackEta)
            self.trackPPar            .Fill(track.PPar       )
            self.trackDeltaR          .Fill(track.DeltaR     )
            #self.trackEtaRel          .Fill(track.EtaRel     )
            self.trackPtRatio         .Fill(track.PtRatio    )
            self.trackPParRatio       .Fill(track.PParRatio  )


    def Write(self):
        self.thisDir.cd()

        self.nTrk.Write()
    
        self.ip3d_l.Write()
        self.ip3d  .Write()
        
        self.ip3d_sig_l.Write()
        self.ip3d_sig  .Write()
    
        self.ip3d_err_l.Write()
        self.ip3d_err  .Write()
    
    
        self.ip2d_l.Write()
        self.ip2d  .Write()
        
        self.ip2d_sig_l.Write()
        self.ip2d_sig  .Write()
        
        self.ip2d_err_l.Write()
        self.ip2d_err  .Write()

        self.ip2d_vs_pt.Write()
        self.ip2d_vs_eta.Write()
    
        self.trackDecayLenVal_l   .Write()
        self.trackDecayLenVal     .Write()
        self.trackJetDistVal      .Write()        
        self.trackPtRel           .Write()
        self.trackMomentum        .Write()
        self.trackEta             .Write()
        self.trackPPar            .Write()
        self.trackDeltaR          .Write()
        self.trackEtaRel          .Write()
        self.trackPtRatio         .Write()
        self.trackPParRatio       .Write()
    