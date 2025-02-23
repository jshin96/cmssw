import FWCore.ParameterSet.Config as cms

# Configuration file for nanoDQMIO
# Use this file to specify which monitoring elements (MEs) will be stored in the nanoDQMIO format.
# For more information, see https://twiki.cern.ch/twiki/bin/view/CMS/PerLsDQMIO.

# Use the full ME path, as displayed for example in the DQM GUI.

# The current selection of MEs is for the reprocessing of 2022 data.

nanoDQMIO_perLSoutput = cms.PSet(
  MEsToSave = cms.untracked.vstring(*(
    "Hcal/DigiTask/Occupancy/depth/depth1",
    "Hcal/DigiTask/Occupancy/depth/depth2",
    "Hcal/DigiTask/Occupancy/depth/depth3",
    "Hcal/DigiTask/Occupancy/depth/depth4",
    "Hcal/DigiTask/Occupancy/depth/depth5",
    "Hcal/DigiTask/Occupancy/depth/depth6",
    "Hcal/DigiTask/Occupancy/depth/depth7",
    "Hcal/DigiTask/Occupancy/depth/depthHO",
    "Hcal/DigiTask/OccupancyCut/depth/depth1",
    "Hcal/DigiTask/OccupancyCut/depth/depth2",
    "Hcal/DigiTask/OccupancyCut/depth/depth3",
    "Hcal/DigiTask/OccupancyCut/depth/depth4",
    "Hcal/DigiTask/OccupancyCut/depth/depth5",
    "Hcal/DigiTask/OccupancyCut/depth/depth6",
    "Hcal/DigiTask/OccupancyCut/depth/depth7",
    "Hcal/DigiTask/OccupancyCut/depth/depthHO",
    "EcalBarrel/EBOccupancyTask/EBOT digi occupancy",
    "EcalEndcap/EEOccupancyTask/EEOT digi occupancy EE -",
    "EcalEndcap/EEOccupancyTask/EEOT digi occupancy EE +",
    "EcalBarrel/EBOccupancyTask/EBOT DCC entries",
    "EcalEndcap/EEOccupancyTask/EEOT DCC entries",
    "Ecal/EventInfo/processedEvents",
    "PixelPhase1/Tracks/charge_PXBarrel",
    "PixelPhase1/Tracks/charge_PXForward",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_1",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_2",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_3",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_4",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_+1",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_+2",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_+3",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_-1",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_-2",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_-3",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_1",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_2",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_3",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_4",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_+1",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_+2",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_+3",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_-1",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_-2",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_-3",
    "HLT/Vertexing/hltPixelVertices/hltPixelVertices/goodvtxNbr",
    "PixelPhase1/Tracks/num_clusters_ontrack_PXBarrel",
    "PixelPhase1/Tracks/num_clusters_ontrack_PXForward",
    "PixelPhase1/Tracks/clusterposition_zphi_ontrack",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_1",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_2",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_3",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_4",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_+1",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_+2",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_+3",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_-1",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_-2",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_-3",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_1/NormalizedHitResiduals_TEC__wheel__1",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_2/NormalizedHitResiduals_TEC__wheel__2",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_3/NormalizedHitResiduals_TEC__wheel__3",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_4/NormalizedHitResiduals_TEC__wheel__4",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_5/NormalizedHitResiduals_TEC__wheel__5",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_6/NormalizedHitResiduals_TEC__wheel__6",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_7/NormalizedHitResiduals_TEC__wheel__7",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_8/NormalizedHitResiduals_TEC__wheel__8",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_9/NormalizedHitResiduals_TEC__wheel__9",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_1/NormalizedHitResiduals_TEC__wheel__1",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_2/NormalizedHitResiduals_TEC__wheel__2",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_3/NormalizedHitResiduals_TEC__wheel__3",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_4/NormalizedHitResiduals_TEC__wheel__4",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_5/NormalizedHitResiduals_TEC__wheel__5",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_6/NormalizedHitResiduals_TEC__wheel__6",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_7/NormalizedHitResiduals_TEC__wheel__7",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_8/NormalizedHitResiduals_TEC__wheel__8",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_9/NormalizedHitResiduals_TEC__wheel__9",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__1",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__2",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__3",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_4/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__4",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_5/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__5",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_6/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__6",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_7/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__7",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_8/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__8",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_9/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__9",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__1",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__2",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__3",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_4/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__4",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_5/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__5",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_6/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__6",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_7/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__7",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_8/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__8",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_9/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__9",
    "SiStrip/MechanicalView/TIB/layer_1/NormalizedHitResiduals_TIB__Layer__1",
    "SiStrip/MechanicalView/TIB/layer_2/NormalizedHitResiduals_TIB__Layer__2",
    "SiStrip/MechanicalView/TIB/layer_3/NormalizedHitResiduals_TIB__Layer__3",
    "SiStrip/MechanicalView/TIB/layer_4/NormalizedHitResiduals_TIB__Layer__4",
    "SiStrip/MechanicalView/TIB/layer_1/Summary_ClusterStoNCorr__OnTrack__TIB__layer__1",
    "SiStrip/MechanicalView/TIB/layer_2/Summary_ClusterStoNCorr__OnTrack__TIB__layer__2",
    "SiStrip/MechanicalView/TIB/layer_3/Summary_ClusterStoNCorr__OnTrack__TIB__layer__3",
    "SiStrip/MechanicalView/TIB/layer_4/Summary_ClusterStoNCorr__OnTrack__TIB__layer__4",
    "SiStrip/MechanicalView/TID/PLUS/wheel_1/NormalizedHitResiduals_TID__wheel__1",
    "SiStrip/MechanicalView/TID/PLUS/wheel_2/NormalizedHitResiduals_TID__wheel__2",
    "SiStrip/MechanicalView/TID/PLUS/wheel_3/NormalizedHitResiduals_TID__wheel__3",
    "SiStrip/MechanicalView/TID/MINUS/wheel_1/NormalizedHitResiduals_TID__wheel__1",
    "SiStrip/MechanicalView/TID/MINUS/wheel_2/NormalizedHitResiduals_TID__wheel__2",
    "SiStrip/MechanicalView/TID/MINUS/wheel_3/NormalizedHitResiduals_TID__wheel__3",
    "SiStrip/MechanicalView/TID/PLUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TID__PLUS__wheel__1",
    "SiStrip/MechanicalView/TID/PLUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TID__PLUS__wheel__2",
    "SiStrip/MechanicalView/TID/PLUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TID__PLUS__wheel__3",
    "SiStrip/MechanicalView/TID/MINUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TID__MINUS__wheel__1",
    "SiStrip/MechanicalView/TID/MINUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TID__MINUS__wheel__2",
    "SiStrip/MechanicalView/TID/MINUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TID__MINUS__wheel__3",
    "SiStrip/MechanicalView/TOB/layer_1/NormalizedHitResiduals_TOB__Layer__1",
    "SiStrip/MechanicalView/TOB/layer_2/NormalizedHitResiduals_TOB__Layer__2",
    "SiStrip/MechanicalView/TOB/layer_3/NormalizedHitResiduals_TOB__Layer__3",
    "SiStrip/MechanicalView/TOB/layer_4/NormalizedHitResiduals_TOB__Layer__4",
    "SiStrip/MechanicalView/TOB/layer_5/NormalizedHitResiduals_TOB__Layer__5",
    "SiStrip/MechanicalView/TOB/layer_6/NormalizedHitResiduals_TOB__Layer__6",
    "SiStrip/MechanicalView/TOB/layer_1/Summary_ClusterStoNCorr__OnTrack__TOB__layer__1",
    "SiStrip/MechanicalView/TOB/layer_2/Summary_ClusterStoNCorr__OnTrack__TOB__layer__2",
    "SiStrip/MechanicalView/TOB/layer_3/Summary_ClusterStoNCorr__OnTrack__TOB__layer__3",
    "SiStrip/MechanicalView/TOB/layer_4/Summary_ClusterStoNCorr__OnTrack__TOB__layer__4",
    "SiStrip/MechanicalView/TOB/layer_5/Summary_ClusterStoNCorr__OnTrack__TOB__layer__5",
    "SiStrip/MechanicalView/TOB/layer_6/Summary_ClusterStoNCorr__OnTrack__TOB__layer__6",
    "SiStrip/MechanicalView/MainDiagonal Position",
    "SiStrip/MechanicalView/NumberOfClustersInPixel",
    "SiStrip/MechanicalView/NumberOfClustersInStrip",
    "Tracking/TrackParameters/GeneralProperties/Chi2oNDF_lumiFlag_GenTk",
    "Tracking/TrackParameters/GeneralProperties/NumberOfRecHitsPerTrack_lumiFlag_GenTk",
    "Tracking/TrackParameters/GeneralProperties/NumberOfTracks_lumiFlag_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/SIPDxyToPV_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/SIPDzToPV_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/SIP3DToPV_GenTk",
    "Tracking/TrackParameters/generalTracks/HitProperties/NumberOfMissingOuterRecHitsPerTrack_GenTk",
    "Tracking/TrackParameters/generalTracks/HitProperties/NumberMORecHitsPerTrackVsPt_GenTk",
    "OfflinePV/offlinePrimaryVertices/tagVtxProb",
    "OfflinePV/offlinePrimaryVertice/tagType",
    "OfflinePV/Resolution/PV/pull_x",
    "OfflinePV/Resolution/PV/pull_y",
    "OfflinePV/Resolution/PV/pull_z",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_highPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_highPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_mediumPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_mediumPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_lowPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_lowPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_highPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_highPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_mediumPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_mediumPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_lowPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_lowPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Constituents",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Eta",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Eta_uncor",
    "JetMET/Jet/Cleanedak4PFJetsCHS/JetEnergyCorr",
    "JetMET/Jet/Cleanedak4PFJetsCHS/NJets",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Phi",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Phi_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Phi_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Pt",
    "JetMET/MET/pfMETT1/Cleaned/METSig",
    "JetMET/vertices",     
  ) )
)
