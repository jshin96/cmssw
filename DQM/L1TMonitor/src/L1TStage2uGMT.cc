#include "DQM/L1TMonitor/interface/L1TStage2uGMT.h"

L1TStage2uGMT::L1TStage2uGMT(const edm::ParameterSet& ps)
    : ugmtMuonToken_(consumes<l1t::MuonBxCollection>(ps.getParameter<edm::InputTag>("muonProducer"))),
      ugmtMuonShowerToken_(consumes<l1t::MuonShowerBxCollection>(ps.getParameter<edm::InputTag>("muonShowerProducer"))),
      monitorDir_(ps.getUntrackedParameter<std::string>("monitorDir")),
      emul_(ps.getUntrackedParameter<bool>("emulator")),
      verbose_(ps.getUntrackedParameter<bool>("verbose")),
      displacedQuantities_(ps.getUntrackedParameter<bool>("displacedQuantities")),
      hadronicShowers_(ps.getUntrackedParameter<bool>("hadronicShowers")),
      etaScale_(0.010875),  // eta scale (CMS DN-2015/017)
      phiScale_(0.010908)   // phi scale (2*pi/576 HW values)
{
  if (!emul_) {
    ugmtBMTFToken_ = consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("bmtfProducer"));
    ugmtOMTFToken_ = consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("omtfProducer"));
    ugmtEMTFToken_ = consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("emtfProducer"));
    ugmtEMTFShowerToken_ =
        consumes<l1t::RegionalMuonShowerBxCollection>(ps.getParameter<edm::InputTag>("emtfShowerProducer"));
  }
}

L1TStage2uGMT::~L1TStage2uGMT() {}

void L1TStage2uGMT::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("muonProducer")->setComment("uGMT output muons.");

  desc.add<edm::InputTag>("bmtfProducer")->setComment("RegionalMuonCands from BMTF.");
  desc.add<edm::InputTag>("omtfProducer")->setComment("RegionalMuonCands from OMTF.");
  desc.add<edm::InputTag>("emtfProducer")->setComment("RegionalMuonCands from EMTF.");
  desc.add<edm::InputTag>("muonShowerProducer")->setComment("uGMT output showers.");
  desc.add<edm::InputTag>("emtfShowerProducer")->setComment("RegionalMuonShowers from EMTF.");
  desc.addUntracked<std::string>("monitorDir", "")
      ->setComment("Target directory in the DQM file. Will be created if not existing.");
  desc.addUntracked<bool>("emulator", false)
      ->setComment("Create histograms for muonProducer input only. xmtfProducer inputs are ignored.");
  desc.addUntracked<bool>("verbose", false);
  desc.addUntracked<bool>("displacedQuantities", false);
  desc.addUntracked<bool>("hadronicShowers", false);
  descriptions.add("l1tStage2uGMT", desc);
}

void L1TStage2uGMT::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {
  if (!emul_) {
    // BMTF Input
    ibooker.setCurrentFolder(monitorDir_ + "/BMTFInput");

    ugmtBMTFnMuons = ibooker.book1D("ugmtBMTFnMuons", "uGMT BMTF Input Muon Multiplicity", 37, -0.5, 36.5);
    ugmtBMTFnMuons->setAxisTitle("Muon Multiplicity (BX == 0)", 1);

    ugmtBMTFhwPt = ibooker.book1D("ugmtBMTFhwPt", "uGMT BMTF Input HW p_{T}", 512, -0.5, 511.5);
    ugmtBMTFhwPt->setAxisTitle("Hardware p_{T}", 1);

    if (displacedQuantities_) {
      ugmtBMTFhwPtUnconstrained =
          ibooker.book1D("ugmtBMTFhwPtUnconstrained", "uGMT BMTF Input HW p_{T} unconstrained", 256, -0.5, 255.5);
      ugmtBMTFhwPtUnconstrained->setAxisTitle("Hardware p_{T} unconstrained", 1);

      ugmtBMTFhwDXY = ibooker.book1D("ugmtBMTFhwDXY", "uGMT BMTF Input HW impact parameter", 4, -0.5, 3.5);
      ugmtBMTFhwDXY->setAxisTitle("Hardware dXY", 1);
    }

    ugmtBMTFhwEta = ibooker.book1D("ugmtBMTFhwEta", "uGMT BMTF Input HW #eta", 201, -100.5, 100.5);
    ugmtBMTFhwEta->setAxisTitle("Hardware #eta", 1);

    ugmtBMTFhwPhi = ibooker.book1D("ugmtBMTFhwPhi", "uGMT BMTF Input HW #phi", 71, -10.5, 60.5);
    ugmtBMTFhwPhi->setAxisTitle("Hardware #phi", 1);

    ugmtBMTFglbPhi = ibooker.book1D("ugmtBMTFglbhwPhi", "uGMT BMTF Input HW #phi", 577, -1.5, 575.5);
    ugmtBMTFglbPhi->setAxisTitle("Global Hardware #phi", 1);

    ugmtBMTFProcvshwPhi =
        ibooker.book2D("ugmtBMTFProcvshwPhi", "uGMT BMTF Processor vs HW #phi", 71, -10.5, 60.5, 12, 0, 12);
    ugmtBMTFProcvshwPhi->setAxisTitle("Hardware #phi", 1);
    ugmtBMTFProcvshwPhi->setAxisTitle("Wedge", 2);
    for (int bin = 1; bin <= 12; ++bin) {
      ugmtBMTFProcvshwPhi->setBinLabel(bin, std::to_string(bin), 2);
    }

    ugmtBMTFhwSign = ibooker.book1D("ugmtBMTFhwSign", "uGMT BMTF Input HW Sign", 4, -1.5, 2.5);
    ugmtBMTFhwSign->setAxisTitle("Hardware Sign", 1);

    ugmtBMTFhwSignValid = ibooker.book1D("ugmtBMTFhwSignValid", "uGMT BMTF Input SignValid", 2, -0.5, 1.5);
    ugmtBMTFhwSignValid->setAxisTitle("SignValid", 1);

    ugmtBMTFhwQual = ibooker.book1D("ugmtBMTFhwQual", "uGMT BMTF Input Quality", 16, -0.5, 15.5);
    ugmtBMTFhwQual->setAxisTitle("Quality", 1);

    ugmtBMTFlink = ibooker.book1D("ugmtBMTFlink", "uGMT BMTF Input Link", 12, 47.5, 59.5);
    ugmtBMTFlink->setAxisTitle("Link", 1);

    ugmtBMTFMuMuDEta =
        ibooker.book1D("ugmtBMTFMuMuDEta", "uGMT BMTF input muons #Delta#eta between wedges", 100, -0.5, 0.5);
    ugmtBMTFMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtBMTFMuMuDPhi =
        ibooker.book1D("ugmtBMTFMuMuDPhi", "uGMT BMTF input muons #Delta#phi between wedges", 100, -0.5, 0.5);
    ugmtBMTFMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtBMTFMuMuDR = ibooker.book1D("ugmtBMTFMuMuDR", "uGMT BMTF input muons #DeltaR between wedges", 50, 0., 0.5);
    ugmtBMTFMuMuDR->setAxisTitle("#DeltaR", 1);

    // OMTF Input
    ibooker.setCurrentFolder(monitorDir_ + "/OMTFInput");

    ugmtOMTFnMuons = ibooker.book1D("ugmtOMTFnMuons", "uGMT OMTF Input Muon Multiplicity", 37, -0.5, 36.5);
    ugmtOMTFnMuons->setAxisTitle("Muon Multiplicity (BX == 0)", 1);

    ugmtOMTFhwPt = ibooker.book1D("ugmtOMTFhwPt", "uGMT OMTF Input HW p_{T}", 512, -0.5, 511.5);
    ugmtOMTFhwPt->setAxisTitle("Hardware p_{T}", 1);

    ugmtOMTFhwEta = ibooker.book1D("ugmtOMTFhwEta", "uGMT OMTF Input HW #eta", 231, -115.5, 115.5);
    ugmtOMTFhwEta->setAxisTitle("Hardware #eta", 1);

    ugmtOMTFhwPhiPos = ibooker.book1D("ugmtOMTFhwPhiPos", "uGMT OMTF Input HW #phi, Positive Side", 122, -16.5, 105.5);
    ugmtOMTFhwPhiPos->setAxisTitle("Hardware #phi", 1);

    ugmtOMTFhwPhiNeg = ibooker.book1D("ugmtOMTFhwPhiNeg", "uGMT OMTF Input HW #phi, Negative Side", 122, -16.5, 105.5);
    ugmtOMTFhwPhiNeg->setAxisTitle("Hardware #phi", 1);

    ugmtOMTFglbPhiPos =
        ibooker.book1D("ugmtOMTFglbhwPhiPos", "uGMT OMTF Input HW #phi, Positive Side", 577, -1.5, 575.5);
    ugmtOMTFglbPhiPos->setAxisTitle("Global Hardware #phi", 1);

    ugmtOMTFglbPhiNeg =
        ibooker.book1D("ugmtOMTFglbhwPhiNeg", "uGMT OMTF Input HW #phi, Negative Side", 577, -1.5, 575.5);
    ugmtOMTFglbPhiNeg->setAxisTitle("Global Hardware #phi", 1);

    ugmtOMTFProcvshwPhiPos =
        ibooker.book2D("ugmtOMTFProcvshwPhiPos", "uGMT OMTF Processor vs HW #phi", 122, -16.5, 105.5, 6, 0, 6);
    ugmtOMTFProcvshwPhiPos->setAxisTitle("Hardware #phi", 1);
    ugmtOMTFProcvshwPhiPos->setAxisTitle("Sector (Positive Side)", 2);

    ugmtOMTFProcvshwPhiNeg =
        ibooker.book2D("ugmtOMTFProcvshwPhiNeg", "uGMT OMTF Processor vs HW #phi", 122, -16.5, 105.5, 6, 0, 6);
    ugmtOMTFProcvshwPhiNeg->setAxisTitle("Hardware #phi", 1);
    ugmtOMTFProcvshwPhiNeg->setAxisTitle("Sector (Negative Side)", 2);

    for (int bin = 1; bin <= 6; ++bin) {
      ugmtOMTFProcvshwPhiPos->setBinLabel(bin, std::to_string(bin), 2);
      ugmtOMTFProcvshwPhiNeg->setBinLabel(bin, std::to_string(bin), 2);
    }

    ugmtOMTFhwSign = ibooker.book1D("ugmtOMTFhwSign", "uGMT OMTF Input HW Sign", 4, -1.5, 2.5);
    ugmtOMTFhwSign->setAxisTitle("Hardware Sign", 1);

    ugmtOMTFhwSignValid = ibooker.book1D("ugmtOMTFhwSignValid", "uGMT OMTF Input SignValid", 2, -0.5, 1.5);
    ugmtOMTFhwSignValid->setAxisTitle("SignValid", 1);

    ugmtOMTFhwQual = ibooker.book1D("ugmtOMTFhwQual", "uGMT OMTF Input Quality", 16, -0.5, 15.5);
    ugmtOMTFhwQual->setAxisTitle("Quality", 1);

    ugmtOMTFlink = ibooker.book1D("ugmtOMTFlink", "uGMT OMTF Input Link", 24, 41.5, 65.5);
    ugmtOMTFlink->setAxisTitle("Link", 1);

    ugmtOMTFMuMuDEta =
        ibooker.book1D("ugmtOMTFMuMuDEta", "uGMT OMTF input muons #Delta#eta between sectors", 100, -0.5, 0.5);
    ugmtOMTFMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtOMTFMuMuDPhi =
        ibooker.book1D("ugmtOMTFMuMuDPhi", "uGMT OMTF input muons #Delta#phi between sectors", 100, -0.5, 0.5);
    ugmtOMTFMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtOMTFMuMuDR = ibooker.book1D("ugmtOMTFMuMuDR", "uGMT OMTF input muons #DeltaR between sectors", 50, 0., 0.5);
    ugmtOMTFMuMuDR->setAxisTitle("#DeltaR", 1);

    // EMTF Input
    ibooker.setCurrentFolder(monitorDir_ + "/EMTFInput");

    ugmtEMTFnMuons = ibooker.book1D("ugmtEMTFnMuons", "uGMT EMTF Input Muon Multiplicity", 37, -0.5, 36.5);
    ugmtEMTFnMuons->setAxisTitle("Muon Multiplicity (BX == 0)", 1);

    ugmtEMTFhwPt = ibooker.book1D("ugmtEMTFhwPt", "uGMT EMTF HW p_{T}", 512, -0.5, 511.5);
    ugmtEMTFhwPt->setAxisTitle("Hardware p_{T}", 1);

    if (displacedQuantities_) {
      ugmtEMTFhwPtUnconstrained =
          ibooker.book1D("ugmtEMTFhwPtUnconstrained", "uGMT EMTF Input HW p_{T} unconstrained", 256, -0.5, 255.5);
      ugmtEMTFhwPtUnconstrained->setAxisTitle("Hardware p_{T} unconstrained", 1);

      ugmtEMTFhwDXY = ibooker.book1D("ugmtEMTFhwDXY", "uGMT EMTF Input HW impact parameter", 4, -0.5, 3.5);
      ugmtEMTFhwDXY->setAxisTitle("Hardware dXY", 1);
    }

    ugmtEMTFhwEta = ibooker.book1D("ugmtEMTFhwEta", "uGMT EMTF HW #eta", 461, -230.5, 230.5);
    ugmtEMTFhwEta->setAxisTitle("Hardware #eta", 1);

    ugmtEMTFhwPhiPos = ibooker.book1D("ugmtEMTFhwPhiPos", "uGMT EMTF HW #phi, Positive Side", 146, -40.5, 105.5);
    ugmtEMTFhwPhiPos->setAxisTitle("Hardware #phi", 1);

    ugmtEMTFhwPhiNeg = ibooker.book1D("ugmtEMTFhwPhiNeg", "uGMT EMTF HW #phi, Negative Side", 146, -40.5, 105.5);
    ugmtEMTFhwPhiNeg->setAxisTitle("Hardware #phi", 1);

    ugmtEMTFglbPhiPos =
        ibooker.book1D("ugmtEMTFglbhwPhiPos", "uGMT EMTF Input Global HW #phi, Positive Side", 577, -1.5, 575.5);
    ugmtEMTFglbPhiPos->setAxisTitle("Global Hardware #phi", 1);

    ugmtEMTFglbPhiNeg =
        ibooker.book1D("ugmtEMTFglbhwPhiNeg", "uGMT EMTF Input Global HW #phi, Negative Side", 577, -1.5, 575.5);
    ugmtEMTFglbPhiNeg->setAxisTitle("Global Hardware #phi", 1);

    ugmtEMTFProcvshwPhiPos =
        ibooker.book2D("ugmtEMTFProcvshwPhiPos", "uGMT EMTF Processor vs HW #phi", 146, -40.5, 105.5, 6, 0, 6);
    ugmtEMTFProcvshwPhiPos->setAxisTitle("Hardware #phi", 1);
    ugmtEMTFProcvshwPhiPos->setAxisTitle("Sector (Positive Side)", 2);

    ugmtEMTFProcvshwPhiNeg =
        ibooker.book2D("ugmtEMTFProcvshwPhiNeg", "uGMT EMTF Processor vs HW #phi", 146, -40.5, 105.5, 6, 0, 6);
    ugmtEMTFProcvshwPhiNeg->setAxisTitle("Hardware #phi", 1);
    ugmtEMTFProcvshwPhiNeg->setAxisTitle("Sector (Negative Side)", 2);

    for (int bin = 1; bin <= 6; ++bin) {
      ugmtEMTFProcvshwPhiPos->setBinLabel(bin, std::to_string(bin), 2);
      ugmtEMTFProcvshwPhiNeg->setBinLabel(bin, std::to_string(bin), 2);
    }

    ugmtEMTFhwSign = ibooker.book1D("ugmtEMTFhwSign", "uGMT EMTF HW Sign", 4, -1.5, 2.5);
    ugmtEMTFhwSign->setAxisTitle("Hardware Sign", 1);

    ugmtEMTFhwSignValid = ibooker.book1D("ugmtEMTFhwSignValid", "uGMT EMTF SignValid", 2, -0.5, 1.5);
    ugmtEMTFhwSignValid->setAxisTitle("SignValid", 1);

    ugmtEMTFhwQual = ibooker.book1D("ugmtEMTFhwQual", "uGMT EMTF Quality", 16, -0.5, 15.5);
    ugmtEMTFhwQual->setAxisTitle("Quality", 1);

    ugmtEMTFlink = ibooker.book1D("ugmtEMTFlink", "uGMT EMTF Link", 36, 35.5, 71.5);
    ugmtEMTFlink->setAxisTitle("Link", 1);

    ugmtEMTFMuMuDEta =
        ibooker.book1D("ugmtEMTFMuMuDEta", "uGMT EMTF input muons #Delta#eta between sectors", 100, -0.5, 0.5);
    ugmtEMTFMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtEMTFMuMuDPhi =
        ibooker.book1D("ugmtEMTFMuMuDPhi", "uGMT EMTF input muons #Delta#phi between sectors", 100, -0.5, 0.5);
    ugmtEMTFMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtEMTFMuMuDR = ibooker.book1D("ugmtEMTFMuMuDR", "uGMT EMTF input muons #DeltaR between sectors", 50, 0., 0.5);
    ugmtEMTFMuMuDR->setAxisTitle("#DeltaR", 1);

    // EMTF muon showers
    if (hadronicShowers_) {
      ibooker.setCurrentFolder(monitorDir_ + "/EMTFInput/Muon showers");

      ugmtEMTFShowerTypeOccupancyPerSector = ibooker.book2D(
          "ugmtEMTFShowerTypeOccupancyPerSector", "Shower type occupancy per sector", 12, 1, 13, 2, 1, 3);
      ugmtEMTFShowerTypeOccupancyPerSector->setAxisTitle("Processor", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(12, "+6", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(11, "+5", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(10, "+4", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(9, "+3", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(8, "+2", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(7, "+1", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(6, "-6", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(5, "-5", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(4, "-4", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(3, "-3", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(2, "-2", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(1, "-1", 1);
      ugmtEMTFShowerTypeOccupancyPerSector->setAxisTitle("Shower type", 2);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(IDX_TIGHT_SHOWER, "Tight", 2);
      ugmtEMTFShowerTypeOccupancyPerSector->setBinLabel(IDX_NOMINAL_SHOWER, "Nominal", 2);
    }

    // inter-TF muon correlations
    ibooker.setCurrentFolder(monitorDir_ + "/muon_correlations");

    ugmtBOMTFposMuMuDEta =
        ibooker.book1D("ugmtBOMTFposMuMuDEta", "uGMT input muons #Delta#eta between BMTF and OMTF+", 100, -0.5, 0.5);
    ugmtBOMTFposMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtBOMTFposMuMuDPhi =
        ibooker.book1D("ugmtBOMTFposMuMuDPhi", "uGMT input muons #Delta#phi between BMTF and OMTF+", 100, -0.5, 0.5);
    ugmtBOMTFposMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtBOMTFposMuMuDR =
        ibooker.book1D("ugmtBOMTFposMuMuDR", "uGMT input muons #DeltaR between BMTF and OMTF+", 50, 0., 0.5);
    ugmtBOMTFposMuMuDR->setAxisTitle("#DeltaR", 1);

    ugmtBOMTFnegMuMuDEta =
        ibooker.book1D("ugmtBOMTFnegMuMuDEta", "uGMT input muons #Delta#eta between BMTF and OMTF-", 100, -0.5, 0.5);
    ugmtBOMTFnegMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtBOMTFnegMuMuDPhi =
        ibooker.book1D("ugmtBOMTFnegMuMuDPhi", "uGMT input muons #Delta#phi between BMTF and OMTF-", 100, -0.5, 0.5);
    ugmtBOMTFnegMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtBOMTFnegMuMuDR =
        ibooker.book1D("ugmtBOMTFnegMuMuDR", "uGMT input muons #DeltaR between BMTF and OMTF-", 50, 0., 0.5);
    ugmtBOMTFnegMuMuDR->setAxisTitle("#DeltaR", 1);

    ugmtEOMTFposMuMuDEta =
        ibooker.book1D("ugmtEOMTFposMuMuDEta", "uGMT input muons #Delta#eta between EMTF+ and OMTF+", 100, -0.5, 0.5);
    ugmtEOMTFposMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtEOMTFposMuMuDPhi =
        ibooker.book1D("ugmtEOMTFposMuMuDPhi", "uGMT input muons #Delta#phi between EMTF+ and OMTF+", 100, -0.5, 0.5);
    ugmtEOMTFposMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtEOMTFposMuMuDR =
        ibooker.book1D("ugmtEOMTFposMuMuDR", "uGMT input muons #DeltaR between EMTF+ and OMTF+", 50, 0., 0.5);
    ugmtEOMTFposMuMuDR->setAxisTitle("#DeltaR", 1);

    ugmtEOMTFnegMuMuDEta =
        ibooker.book1D("ugmtEOMTFnegMuMuDEta", "uGMT input muons #Delta#eta between EMTF- and OMTF-", 100, -0.5, 0.5);
    ugmtEOMTFnegMuMuDEta->setAxisTitle("#Delta#eta", 1);

    ugmtEOMTFnegMuMuDPhi =
        ibooker.book1D("ugmtEOMTFnegMuMuDPhi", "uGMT input muons #Delta#phi between EMTF- and OMTF-", 100, -0.5, 0.5);
    ugmtEOMTFnegMuMuDPhi->setAxisTitle("#Delta#phi", 1);

    ugmtEOMTFnegMuMuDR =
        ibooker.book1D("ugmtEOMTFnegMuMuDR", "uGMT input muons #DeltaR between EMTF- and OMTF-", 50, 0., 0.5);
    ugmtEOMTFnegMuMuDR->setAxisTitle("#DeltaR", 1);
  }

  // Subsystem Monitoring and Muon Output
  ibooker.setCurrentFolder(monitorDir_);

  ugmtMuonBX = ibooker.book1D("ugmtMuonBX", "uGMT output muon BX", 7, -3.5, 3.5);
  ugmtMuonBX->setAxisTitle("BX", 1);

  ugmtnMuons = ibooker.book1D("ugmtnMuons", "uGMT output muon Multiplicity", 9, -0.5, 8.5);
  ugmtnMuons->setAxisTitle("Muon Multiplicity (BX == 0)", 1);

  ugmtMuonIndex = ibooker.book1D("ugmtMuonIndex", "uGMT Input Muon Index", 108, -0.5, 107.5);
  ugmtMuonIndex->setAxisTitle("Index", 1);

  ugmtMuonhwPt = ibooker.book1D("ugmtMuonhwPt", "uGMT output muon HW p_{T}", 512, -0.5, 511.5);
  ugmtMuonhwPt->setAxisTitle("Hardware p_{T}", 1);

  if (displacedQuantities_) {
    ugmtMuonhwPtUnconstrained =
        ibooker.book1D("ugmtMuonhwPtUnconstrained", "uGMT output muon HW p_{T} unconstrained", 256, -0.5, 255.5);
    ugmtMuonhwPtUnconstrained->setAxisTitle("Hardware p_{T} unconstrained", 1);

    ugmtMuonhwDXY = ibooker.book1D("ugmtMuonhwDXY", "uGMT output muon HW impact parameter", 4, -0.5, 3.5);
    ugmtMuonhwDXY->setAxisTitle("Hardware dXY", 1);
  }

  ugmtMuonhwEta = ibooker.book1D("ugmtMuonhwEta", "uGMT output muon HW #eta", 461, -230.5, 230.5);
  ugmtMuonhwEta->setAxisTitle("Hardware Eta", 1);

  ugmtMuonhwPhi = ibooker.book1D("ugmtMuonhwPhi", "uGMT output muon HW #phi", 577, -1.5, 575.5);
  ugmtMuonhwPhi->setAxisTitle("Hardware Phi", 1);

  ugmtMuonhwEtaAtVtx = ibooker.book1D("ugmtMuonhwEtaAtVtx", "uGMT output muon HW #eta at vertex", 461, -230.5, 230.5);
  ugmtMuonhwEtaAtVtx->setAxisTitle("Hardware Eta at Vertex", 1);

  ugmtMuonhwPhiAtVtx = ibooker.book1D("ugmtMuonhwPhiAtVtx", "uGMT output muon HW #phi at vertex", 577, -1.5, 575.5);
  ugmtMuonhwPhiAtVtx->setAxisTitle("Hardware Phi at Vertex", 1);

  ugmtMuonhwCharge = ibooker.book1D("ugmtMuonhwCharge", "uGMT output muon HW Charge", 4, -1.5, 2.5);
  ugmtMuonhwCharge->setAxisTitle("Hardware Charge", 1);

  ugmtMuonhwChargeValid = ibooker.book1D("ugmtMuonhwChargeValid", "uGMT output muon ChargeValid", 2, -0.5, 1.5);
  ugmtMuonhwChargeValid->setAxisTitle("ChargeValid", 1);

  ugmtMuonhwQual = ibooker.book1D("ugmtMuonhwQual", "uGMT output muon Quality", 16, -0.5, 15.5);
  ugmtMuonhwQual->setAxisTitle("Quality", 1);

  ugmtMuonhwIso = ibooker.book1D("ugmtMuonhwIso", "uGMT output muon Isolation", 4, -0.5, 3.5);
  ugmtMuonhwIso->setAxisTitle("Isolation", 1);

  ugmtMuonPt = ibooker.book1D("ugmtMuonPt", "uGMT output muon p_{T}", 128, -0.5, 255.5);
  ugmtMuonPt->setAxisTitle("p_{T} [GeV]", 1);

  if (displacedQuantities_) {
    ugmtMuonPtUnconstrained =
        ibooker.book1D("ugmtMuonPtUnconstrained", "uGMT output muon p_{T} unconstrained", 128, -0.5, 255.5);
    ugmtMuonPtUnconstrained->setAxisTitle("p_{T} unconstrained [GeV]", 1);
  }

  ugmtMuonEta = ibooker.book1D("ugmtMuonEta", "uGMT output muon #eta", 52, -2.6, 2.6);
  ugmtMuonEta->setAxisTitle("#eta", 1);

  ugmtMuonPhi = ibooker.book1D("ugmtMuonPhi", "uGMT output muon #phi", 66, -3.3, 3.3);
  ugmtMuonPhi->setAxisTitle("#phi", 1);

  ugmtMuonEtaAtVtx = ibooker.book1D("ugmtMuonEtaAtVtx", "uGMT output muon #eta at vertex", 52, -2.6, 2.6);
  ugmtMuonEtaAtVtx->setAxisTitle("#eta at vertex", 1);

  ugmtMuonPhiAtVtx = ibooker.book1D("ugmtMuonPhiAtVtx", "uGMT output muon #phi at vertex", 66, -3.3, 3.3);
  ugmtMuonPhiAtVtx->setAxisTitle("#phi at vertex", 1);

  ugmtMuonCharge = ibooker.book1D("ugmtMuonCharge", "uGMT output muon Charge", 3, -1.5, 1.5);
  ugmtMuonCharge->setAxisTitle("Charge", 1);

  ugmtMuonPhiBmtf = ibooker.book1D("ugmtMuonPhiBmtf", "uGMT output muon #phi for BMTF Inputs", 66, -3.3, 3.3);
  ugmtMuonPhiBmtf->setAxisTitle("#phi", 1);

  ugmtMuonPhiOmtf = ibooker.book1D("ugmtMuonPhiOmtf", "uGMT output muon #phi for OMTF Inputs", 66, -3.3, 3.3);
  ugmtMuonPhiOmtf->setAxisTitle("#phi", 1);

  ugmtMuonPhiEmtf = ibooker.book1D("ugmtMuonPhiEmtf", "uGMT output muon #phi for EMTF Inputs", 66, -3.3, 3.3);
  ugmtMuonPhiEmtf->setAxisTitle("#phi", 1);

  const float dPhiScale = 4 * phiScale_;
  const float dEtaScale = etaScale_;
  ugmtMuonDEtavsPtBmtf = ibooker.book2D("ugmtMuonDEtavsPtBmtf",
                                        "uGMT output muon from BMTF #eta_{at vertex} - #eta_{at muon system} vs p_{T}",
                                        32,
                                        0,
                                        64,
                                        31,
                                        -15.5 * dEtaScale,
                                        15.5 * dEtaScale);
  ugmtMuonDEtavsPtBmtf->setAxisTitle("p_{T} [GeV]", 1);
  ugmtMuonDEtavsPtBmtf->setAxisTitle("#eta_{at vertex} - #eta", 2);

  ugmtMuonDPhivsPtBmtf = ibooker.book2D("ugmtMuonDPhivsPtBmtf",
                                        "uGMT output muon from BMTF #phi_{at vertex} - #phi_{at muon system} vs p_{T}",
                                        32,
                                        0,
                                        64,
                                        31,
                                        -15.5 * dPhiScale,
                                        15.5 * dPhiScale);
  ugmtMuonDPhivsPtBmtf->setAxisTitle("p_{T} [GeV]", 1);
  ugmtMuonDPhivsPtBmtf->setAxisTitle("#phi_{at vertex} - #phi", 2);

  ugmtMuonDEtavsPtOmtf = ibooker.book2D("ugmtMuonDEtavsPtOmtf",
                                        "uGMT output muon from OMTF #eta_{at vertex} - #eta_{at muon system} vs p_{T}",
                                        32,
                                        0,
                                        64,
                                        31,
                                        -15.5 * dEtaScale,
                                        15.5 * dEtaScale);
  ugmtMuonDEtavsPtOmtf->setAxisTitle("p_{T} [GeV]", 1);
  ugmtMuonDEtavsPtOmtf->setAxisTitle("#eta_{at vertex} - #eta", 2);

  ugmtMuonDPhivsPtOmtf = ibooker.book2D("ugmtMuonDPhivsPtOmtf",
                                        "uGMT output muon from OMTF #phi_{at vertex} - #phi_{at muon system} vs p_{T}",
                                        32,
                                        0,
                                        64,
                                        31,
                                        -15.5 * dPhiScale,
                                        15.5 * dPhiScale);
  ugmtMuonDPhivsPtOmtf->setAxisTitle("p_{T} [GeV]", 1);
  ugmtMuonDPhivsPtOmtf->setAxisTitle("#phi_{at vertex} - #phi", 2);

  ugmtMuonDEtavsPtEmtf = ibooker.book2D("ugmtMuonDEtavsPtEmtf",
                                        "uGMT output muon from EMTF #eta_{at vertex} - #eta_{at muon system} vs p_{T}",
                                        32,
                                        0,
                                        64,
                                        31,
                                        -15.5 * dEtaScale,
                                        15.5 * dEtaScale);
  ugmtMuonDEtavsPtEmtf->setAxisTitle("p_{T} [GeV]", 1);
  ugmtMuonDEtavsPtEmtf->setAxisTitle("#eta_{at vertex} - #eta", 2);

  ugmtMuonDPhivsPtEmtf = ibooker.book2D("ugmtMuonDPhivsPtEmtf",
                                        "uGMT output muon from EMTF #phi_{at vertex} - #phi_{at muon system} vs p_{T}",
                                        32,
                                        0,
                                        64,
                                        31,
                                        -15.5 * dPhiScale,
                                        15.5 * dPhiScale);
  ugmtMuonDPhivsPtEmtf->setAxisTitle("p_{T} [GeV]", 1);
  ugmtMuonDPhivsPtEmtf->setAxisTitle("#phi_{at vertex} - #phi", 2);

  ugmtMuonPtvsEta =
      ibooker.book2D("ugmtMuonPtvsEta", "uGMT output muon p_{T} vs #eta", 100, -2.5, 2.5, 128, -0.5, 255.5);
  ugmtMuonPtvsEta->setAxisTitle("#eta", 1);
  ugmtMuonPtvsEta->setAxisTitle("p_{T} [GeV]", 2);

  ugmtMuonPtvsPhi =
      ibooker.book2D("ugmtMuonPtvsPhi", "uGMT output muon p_{T} vs #phi", 64, -3.2, 3.2, 128, -0.5, 255.5);
  ugmtMuonPtvsPhi->setAxisTitle("#phi", 1);
  ugmtMuonPtvsPhi->setAxisTitle("p_{T} [GeV]", 2);

  ugmtMuonPhivsEta = ibooker.book2D("ugmtMuonPhivsEta", "uGMT output muon #phi vs #eta", 100, -2.5, 2.5, 64, -3.2, 3.2);
  ugmtMuonPhivsEta->setAxisTitle("#eta", 1);
  ugmtMuonPhivsEta->setAxisTitle("#phi", 2);

  ugmtMuonPhiAtVtxvsEtaAtVtx = ibooker.book2D(
      "ugmtMuonPhiAtVtxvsEtaAtVtx", "uGMT output muon #phi at vertex vs #eta at vertex", 100, -2.5, 2.5, 64, -3.2, 3.2);
  ugmtMuonPhiAtVtxvsEtaAtVtx->setAxisTitle("#eta at vertex", 1);
  ugmtMuonPhiAtVtxvsEtaAtVtx->setAxisTitle("#phi at vertex", 2);

  ugmtMuonBXvsLink =
      ibooker.book2D("ugmtMuonBXvsLink", "uGMT output muon BX vs Input Links", 36, 35.5, 71.5, 5, -2.5, 2.5);
  ugmtMuonBXvsLink->setAxisTitle("Muon Input Links", 1);
  for (int bin = 1; bin <= 6; ++bin) {
    ugmtMuonBXvsLink->setBinLabel(bin, Form("E+%d", bin), 1);
    ugmtMuonBXvsLink->setBinLabel(bin + 6, Form("O+%d", bin), 1);
    ugmtMuonBXvsLink->setBinLabel(bin + 12, Form("B%d", bin), 1);
    ugmtMuonBXvsLink->setBinLabel(bin + 18, Form("B%d", bin + 6), 1);
    ugmtMuonBXvsLink->setBinLabel(bin + 24, Form("O-%d", bin), 1);
    ugmtMuonBXvsLink->setBinLabel(bin + 30, Form("E-%d", bin), 1);
  }
  ugmtMuonBXvsLink->setAxisTitle("BX", 2);

  ugmtMuonChargevsLink =
      ibooker.book2D("ugmtMuonChargevsLink", "uGMT output muon Charge vs Input Links", 36, 35.5, 71.5, 3, -1.5, 1.5);
  ugmtMuonChargevsLink->setAxisTitle("Muon Input Links", 1);
  for (int bin = 1; bin <= 6; ++bin) {
    ugmtMuonChargevsLink->setBinLabel(bin, Form("E+%d", bin), 1);
    ugmtMuonChargevsLink->setBinLabel(bin + 6, Form("O+%d", bin), 1);
    ugmtMuonChargevsLink->setBinLabel(bin + 12, Form("B%d", bin), 1);
    ugmtMuonChargevsLink->setBinLabel(bin + 18, Form("B%d", bin + 6), 1);
    ugmtMuonChargevsLink->setBinLabel(bin + 24, Form("O-%d", bin), 1);
    ugmtMuonChargevsLink->setBinLabel(bin + 30, Form("E-%d", bin), 1);
  }
  ugmtMuonChargevsLink->setAxisTitle("Charge", 2);

  if (hadronicShowers_) {
    ugmtMuonShowerTypeOccupancyPerBx =
        ibooker.book2D("ugmtMuonShowerTypeOccupancyPerBx", "Shower type occupancy per BX", 7, -3.5, 3.5, 2, 1, 3);
    ugmtMuonShowerTypeOccupancyPerBx->setAxisTitle("BX", 1);
    ugmtMuonShowerTypeOccupancyPerBx->setAxisTitle("Shower type", 2);
    ugmtMuonShowerTypeOccupancyPerBx->setBinLabel(IDX_TIGHT_SHOWER, "Tight", 2);
    ugmtMuonShowerTypeOccupancyPerBx->setBinLabel(IDX_NOMINAL_SHOWER, "Nominal", 2);
  }

  ugmtMuonBXvshwPt =
      ibooker.book2D("ugmtMuonBXvshwPt", "uGMT output muon BX vs HW p_{T}", 128, -0.5, 511.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwPt->setAxisTitle("Hardware p_{T}", 1);
  ugmtMuonBXvshwPt->setAxisTitle("BX", 2);

  ugmtMuonBXvshwEta =
      ibooker.book2D("ugmtMuonBXvshwEta", "uGMT output muon BX vs HW #eta", 93, -232.5, 232.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwEta->setAxisTitle("Hardware #eta", 1);
  ugmtMuonBXvshwEta->setAxisTitle("BX", 2);

  ugmtMuonBXvshwPhi =
      ibooker.book2D("ugmtMuonBXvshwPhi", "uGMT output muon BX vs HW #phi", 116, -2.5, 577.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwPhi->setAxisTitle("Hardware #phi", 1);
  ugmtMuonBXvshwPhi->setAxisTitle("BX", 2);

  ugmtMuonBXvshwCharge =
      ibooker.book2D("ugmtMuonBXvshwCharge", "uGMT output muon BX vs HW Charge", 2, -0.5, 1.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwCharge->setAxisTitle("Hardware Charge", 1);
  ugmtMuonBXvshwCharge->setAxisTitle("BX", 2);

  ugmtMuonBXvshwChargeValid =
      ibooker.book2D("ugmtMuonBXvshwChargeValid", "uGMT output muon BX vs ChargeValid", 2, -0.5, 1.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwChargeValid->setAxisTitle("ChargeValid", 1);
  ugmtMuonBXvshwChargeValid->setAxisTitle("BX", 2);

  ugmtMuonBXvshwQual =
      ibooker.book2D("ugmtMuonBXvshwQual", "uGMT output muon BX vs Quality", 16, -0.5, 15.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwQual->setAxisTitle("Quality", 1);
  ugmtMuonBXvshwQual->setAxisTitle("BX", 2);

  ugmtMuonBXvshwIso =
      ibooker.book2D("ugmtMuonBXvshwIso", "uGMT output muon BX vs Isolation", 4, -0.5, 3.5, 5, -2.5, 2.5);
  ugmtMuonBXvshwIso->setAxisTitle("Isolation", 1);
  ugmtMuonBXvshwIso->setAxisTitle("BX", 2);

  // muon correlations
  ibooker.setCurrentFolder(monitorDir_ + "/muon_correlations");

  ugmtMuMuInvMass = ibooker.book1D("ugmtMuMuInvMass", "uGMT dimuon invariant mass", 200, 0., 200.);
  ugmtMuMuInvMass->setAxisTitle("m(#mu#mu) [GeV]", 1);

  ugmtMuMuInvMassAtVtx =
      ibooker.book1D("ugmtMuMuInvMassAtVtx", "uGMT dimuon invariant mass with coordinates at vertex", 200, 0., 200.);
  ugmtMuMuInvMassAtVtx->setAxisTitle("m(#mu#mu) [GeV]", 1);

  ugmtMuMuDEta = ibooker.book1D("ugmtMuMuDEta", "uGMT output muons #Delta#eta", 100, -1., 1.);
  ugmtMuMuDEta->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhi = ibooker.book1D("ugmtMuMuDPhi", "uGMT output muons #Delta#phi", 100, -1., 1.);
  ugmtMuMuDPhi->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDR = ibooker.book1D("ugmtMuMuDR", "uGMT output muons #DeltaR", 50, 0., 1.);
  ugmtMuMuDR->setAxisTitle("#DeltaR", 1);

  // barrel - overlap
  ugmtMuMuDEtaBOpos =
      ibooker.book1D("ugmtMuMuDEtaBOpos", "uGMT output muons #Delta#eta barrel-overlap positive side", 100, -1., 1.);
  ugmtMuMuDEtaBOpos->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiBOpos =
      ibooker.book1D("ugmtMuMuDPhiBOpos", "uGMT output muons #Delta#phi barrel-overlap positive side", 100, -1., 1.);
  ugmtMuMuDPhiBOpos->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDRBOpos =
      ibooker.book1D("ugmtMuMuDRBOpos", "uGMT output muons #DeltaR barrel-overlap positive side", 50, 0., 1.);
  ugmtMuMuDRBOpos->setAxisTitle("#DeltaR", 1);

  ugmtMuMuDEtaBOneg =
      ibooker.book1D("ugmtMuMuDEtaBOneg", "uGMT output muons #Delta#eta barrel-overlap negative side", 100, -1., 1.);
  ugmtMuMuDEtaBOneg->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiBOneg =
      ibooker.book1D("ugmtMuMuDPhiBOneg", "uGMT output muons #Delta#phi barrel-overlap negative side", 100, -1., 1.);
  ugmtMuMuDPhiBOneg->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDRBOneg =
      ibooker.book1D("ugmtMuMuDRBOneg", "uGMT output muons #DeltaR barrel-overlap negative side", 50, 0., 1.);
  ugmtMuMuDRBOneg->setAxisTitle("#DeltaR", 1);

  // endcap - overlap
  ugmtMuMuDEtaEOpos =
      ibooker.book1D("ugmtMuMuDEtaEOpos", "uGMT output muons #Delta#eta endcap-overlap positive side", 100, -1., 1.);
  ugmtMuMuDEtaEOpos->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiEOpos =
      ibooker.book1D("ugmtMuMuDPhiEOpos", "uGMT output muons #Delta#phi endcap-overlap positive side", 100, -1., 1.);
  ugmtMuMuDPhiEOpos->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDREOpos =
      ibooker.book1D("ugmtMuMuDREOpos", "uGMT output muons #DeltaR endcap-overlap positive side", 50, 0., 1.);
  ugmtMuMuDREOpos->setAxisTitle("#DeltaR", 1);

  ugmtMuMuDEtaEOneg =
      ibooker.book1D("ugmtMuMuDEtaEOneg", "uGMT output muons #Delta#eta endcap-overlap negative side", 100, -1., 1.);
  ugmtMuMuDEtaEOneg->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiEOneg =
      ibooker.book1D("ugmtMuMuDPhiEOneg", "uGMT output muons #Delta#phi endcap-overlap negative side", 100, -1., 1.);
  ugmtMuMuDPhiEOneg->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDREOneg =
      ibooker.book1D("ugmtMuMuDREOneg", "uGMT output muons #DeltaR endcap-overlap negative side", 50, 0., 1.);
  ugmtMuMuDREOneg->setAxisTitle("#DeltaR", 1);

  // barrel wedges
  ugmtMuMuDEtaB = ibooker.book1D("ugmtMuMuDEtaB", "uGMT output muons #Delta#eta between barrel wedges", 100, -1., 1.);
  ugmtMuMuDEtaB->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiB = ibooker.book1D("ugmtMuMuDPhiB", "uGMT output muons #Delta#phi between barrel wedges", 100, -1., 1.);
  ugmtMuMuDPhiB->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDRB = ibooker.book1D("ugmtMuMuDRB", "uGMT output muons #DeltaR between barrel wedges", 50, 0., 1.);
  ugmtMuMuDRB->setAxisTitle("#DeltaR", 1);

  // overlap sectors
  ugmtMuMuDEtaOpos = ibooker.book1D(
      "ugmtMuMuDEtaOpos", "uGMT output muons #Delta#eta between overlap positive side sectors", 100, -1., 1.);
  ugmtMuMuDEtaOpos->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiOpos = ibooker.book1D(
      "ugmtMuMuDPhiOpos", "uGMT output muons #Delta#phi between overlap positive side sectors", 100, -1., 1.);
  ugmtMuMuDPhiOpos->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDROpos =
      ibooker.book1D("ugmtMuMuDROpos", "uGMT output muons #DeltaR between overlap positive side sectors", 50, 0., 1.);
  ugmtMuMuDROpos->setAxisTitle("#DeltaR", 1);

  ugmtMuMuDEtaOneg = ibooker.book1D(
      "ugmtMuMuDEtaOneg", "uGMT output muons #Delta#eta between overlap negative side sectors", 100, -1., 1.);
  ugmtMuMuDEtaOneg->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiOneg = ibooker.book1D(
      "ugmtMuMuDPhiOneg", "uGMT output muons #Delta#phi between overlap negative side sectors", 100, -1., 1.);
  ugmtMuMuDPhiOneg->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDROneg =
      ibooker.book1D("ugmtMuMuDROneg", "uGMT output muons #DeltaR between overlap negative side sectors", 50, 0., 1.);
  ugmtMuMuDROneg->setAxisTitle("#DeltaR", 1);

  // endcap sectors
  ugmtMuMuDEtaEpos = ibooker.book1D(
      "ugmtMuMuDEtaEpos", "uGMT output muons #Delta#eta between endcap positive side sectors", 100, -1., 1.);
  ugmtMuMuDEtaEpos->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiEpos = ibooker.book1D(
      "ugmtMuMuDPhiEpos", "uGMT output muons #Delta#phi between endcap positive side sectors", 100, -1., 1.);
  ugmtMuMuDPhiEpos->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDREpos =
      ibooker.book1D("ugmtMuMuDREpos", "uGMT output muons #DeltaR between endcap positive side sectors", 50, 0., 1.);
  ugmtMuMuDREpos->setAxisTitle("#DeltaR", 1);

  ugmtMuMuDEtaEneg = ibooker.book1D(
      "ugmtMuMuDEtaEneg", "uGMT output muons #Delta#eta between endcap negative side sectors", 100, -1., 1.);
  ugmtMuMuDEtaEneg->setAxisTitle("#Delta#eta", 1);

  ugmtMuMuDPhiEneg = ibooker.book1D(
      "ugmtMuMuDPhiEneg", "uGMT output muons #Delta#phi between endcap negative side sectors", 100, -1., 1.);
  ugmtMuMuDPhiEneg->setAxisTitle("#Delta#phi", 1);

  ugmtMuMuDREneg =
      ibooker.book1D("ugmtMuMuDREneg", "uGMT output muons #DeltaR between endcap negative side sectors", 50, 0., 1.);
  ugmtMuMuDREneg->setAxisTitle("#DeltaR", 1);
}

void L1TStage2uGMT::analyze(const edm::Event& e, const edm::EventSetup& c) {
  if (verbose_)
    edm::LogInfo("L1TStage2uGMT") << "L1TStage2uGMT: analyze..." << std::endl;

  if (!emul_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> BMTFBxCollection;
    e.getByToken(ugmtBMTFToken_, BMTFBxCollection);

    ugmtBMTFnMuons->Fill(BMTFBxCollection->size(0));

    for (int itBX = BMTFBxCollection->getFirstBX(); itBX <= BMTFBxCollection->getLastBX(); ++itBX) {
      for (l1t::RegionalMuonCandBxCollection::const_iterator BMTF = BMTFBxCollection->begin(itBX);
           BMTF != BMTFBxCollection->end(itBX);
           ++BMTF) {
        ugmtBMTFhwPt->Fill(BMTF->hwPt());
        if (displacedQuantities_) {
          ugmtBMTFhwPtUnconstrained->Fill(BMTF->hwPtUnconstrained());
          ugmtBMTFhwDXY->Fill(BMTF->hwDXY());
        }
        ugmtBMTFhwEta->Fill(BMTF->hwEta());
        ugmtBMTFhwPhi->Fill(BMTF->hwPhi());
        ugmtBMTFhwSign->Fill(BMTF->hwSign());
        ugmtBMTFhwSignValid->Fill(BMTF->hwSignValid());
        ugmtBMTFhwQual->Fill(BMTF->hwQual());
        ugmtBMTFlink->Fill(BMTF->link());

        int global_hw_phi =
            l1t::MicroGMTConfiguration::calcGlobalPhi(BMTF->hwPhi(), BMTF->trackFinderType(), BMTF->processor());
        ugmtBMTFglbPhi->Fill(global_hw_phi);

        ugmtBMTFProcvshwPhi->Fill(BMTF->hwPhi(), BMTF->processor());

        // Analyse muon correlations
        for (l1t::RegionalMuonCandBxCollection::const_iterator BMTF2 = BMTF + 1; BMTF2 != BMTFBxCollection->end(itBX);
             ++BMTF2) {
          int global_hw_phi2 =
              l1t::MicroGMTConfiguration::calcGlobalPhi(BMTF2->hwPhi(), BMTF2->trackFinderType(), BMTF2->processor());
          float dEta = (BMTF->hwEta() - BMTF2->hwEta()) * etaScale_;
          float dPhi = (global_hw_phi - global_hw_phi2) * phiScale_;
          float dR = sqrt(dEta * dEta + dPhi * dPhi);

          int dLink = std::abs(BMTF->link() - BMTF2->link());
          if (dLink == 1 || dLink == 11) {  // two adjacent wedges and wrap around
            ugmtBMTFMuMuDEta->Fill(dEta);
            ugmtBMTFMuMuDPhi->Fill(dPhi);
            ugmtBMTFMuMuDR->Fill(dR);
          }
        }
      }
    }

    edm::Handle<l1t::RegionalMuonCandBxCollection> OMTFBxCollection;
    e.getByToken(ugmtOMTFToken_, OMTFBxCollection);

    ugmtOMTFnMuons->Fill(OMTFBxCollection->size(0));

    for (int itBX = OMTFBxCollection->getFirstBX(); itBX <= OMTFBxCollection->getLastBX(); ++itBX) {
      for (l1t::RegionalMuonCandBxCollection::const_iterator OMTF = OMTFBxCollection->begin(itBX);
           OMTF != OMTFBxCollection->end(itBX);
           ++OMTF) {
        ugmtOMTFhwPt->Fill(OMTF->hwPt());
        ugmtOMTFhwEta->Fill(OMTF->hwEta());
        ugmtOMTFhwSign->Fill(OMTF->hwSign());
        ugmtOMTFhwSignValid->Fill(OMTF->hwSignValid());
        ugmtOMTFhwQual->Fill(OMTF->hwQual());
        ugmtOMTFlink->Fill(OMTF->link());

        int global_hw_phi =
            l1t::MicroGMTConfiguration::calcGlobalPhi(OMTF->hwPhi(), OMTF->trackFinderType(), OMTF->processor());

        l1t::tftype trackFinderType = OMTF->trackFinderType();

        if (trackFinderType == l1t::omtf_neg) {
          ugmtOMTFhwPhiNeg->Fill(OMTF->hwPhi());
          ugmtOMTFglbPhiNeg->Fill(global_hw_phi);
          ugmtOMTFProcvshwPhiNeg->Fill(OMTF->hwPhi(), OMTF->processor());
        } else {
          ugmtOMTFhwPhiPos->Fill(OMTF->hwPhi());
          ugmtOMTFglbPhiPos->Fill(global_hw_phi);
          ugmtOMTFProcvshwPhiPos->Fill(OMTF->hwPhi(), OMTF->processor());
        }

        // Analyse muon correlations
        for (l1t::RegionalMuonCandBxCollection::const_iterator OMTF2 = OMTF + 1; OMTF2 != OMTFBxCollection->end(itBX);
             ++OMTF2) {
          int global_hw_phi2 =
              l1t::MicroGMTConfiguration::calcGlobalPhi(OMTF2->hwPhi(), OMTF2->trackFinderType(), OMTF2->processor());
          float dEta = (OMTF->hwEta() - OMTF2->hwEta()) * etaScale_;
          float dPhi = (global_hw_phi - global_hw_phi2) * phiScale_;
          float dR = sqrt(dEta * dEta + dPhi * dPhi);

          int dLink = std::abs(OMTF->link() - OMTF2->link());
          if (dLink == 1 || dLink == 5) {  // two adjacent sectors and wrap around
            ugmtOMTFMuMuDEta->Fill(dEta);
            ugmtOMTFMuMuDPhi->Fill(dPhi);
            ugmtOMTFMuMuDR->Fill(dR);
          }
        }
      }
    }

    edm::Handle<l1t::RegionalMuonCandBxCollection> EMTFBxCollection;
    e.getByToken(ugmtEMTFToken_, EMTFBxCollection);

    ugmtEMTFnMuons->Fill(EMTFBxCollection->size(0));

    for (int itBX = EMTFBxCollection->getFirstBX(); itBX <= EMTFBxCollection->getLastBX(); ++itBX) {
      for (l1t::RegionalMuonCandBxCollection::const_iterator EMTF = EMTFBxCollection->begin(itBX);
           EMTF != EMTFBxCollection->end(itBX);
           ++EMTF) {
        ugmtEMTFhwPt->Fill(EMTF->hwPt());
        if (displacedQuantities_) {
          ugmtEMTFhwPtUnconstrained->Fill(EMTF->hwPtUnconstrained());
          ugmtEMTFhwDXY->Fill(EMTF->hwDXY());
        }
        ugmtEMTFhwEta->Fill(EMTF->hwEta());
        ugmtEMTFhwSign->Fill(EMTF->hwSign());
        ugmtEMTFhwSignValid->Fill(EMTF->hwSignValid());
        ugmtEMTFhwQual->Fill(EMTF->hwQual());
        ugmtEMTFlink->Fill(EMTF->link());

        int global_hw_phi =
            l1t::MicroGMTConfiguration::calcGlobalPhi(EMTF->hwPhi(), EMTF->trackFinderType(), EMTF->processor());

        l1t::tftype trackFinderType = EMTF->trackFinderType();

        if (trackFinderType == l1t::emtf_neg) {
          ugmtEMTFhwPhiNeg->Fill(EMTF->hwPhi());
          ugmtEMTFglbPhiNeg->Fill(global_hw_phi);
          ugmtEMTFProcvshwPhiNeg->Fill(EMTF->hwPhi(), EMTF->processor());
        } else {
          ugmtEMTFhwPhiPos->Fill(EMTF->hwPhi());
          ugmtEMTFglbPhiPos->Fill(global_hw_phi);
          ugmtEMTFProcvshwPhiPos->Fill(EMTF->hwPhi(), EMTF->processor());
        }

        // Analyse muon correlations
        for (l1t::RegionalMuonCandBxCollection::const_iterator EMTF2 = EMTF + 1; EMTF2 != EMTFBxCollection->end(itBX);
             ++EMTF2) {
          int global_hw_phi2 =
              l1t::MicroGMTConfiguration::calcGlobalPhi(EMTF2->hwPhi(), EMTF2->trackFinderType(), EMTF2->processor());
          float dEta = (EMTF->hwEta() - EMTF2->hwEta()) * etaScale_;
          float dPhi = (global_hw_phi - global_hw_phi2) * phiScale_;
          float dR = sqrt(dEta * dEta + dPhi * dPhi);

          int dLink = std::abs(EMTF->link() - EMTF2->link());
          if (dLink == 1 || dLink == 5) {  // two adjacent sectors and wrap around
            ugmtEMTFMuMuDEta->Fill(dEta);
            ugmtEMTFMuMuDPhi->Fill(dPhi);
            ugmtEMTFMuMuDR->Fill(dR);
          }
        }
      }
    }

    // Fill shower plots
    if (hadronicShowers_) {
      edm::Handle<l1t::RegionalMuonShowerBxCollection> EMTFShowersBxCollection;
      e.getByToken(ugmtEMTFShowerToken_, EMTFShowersBxCollection);

      for (int itBX = EMTFShowersBxCollection->getFirstBX(); itBX <= EMTFShowersBxCollection->getLastBX(); ++itBX) {
        for (l1t::RegionalMuonShowerBxCollection::const_iterator shower = EMTFShowersBxCollection->begin(itBX);
             shower != EMTFShowersBxCollection->end(itBX);
             ++shower) {
          if (not shower->isValid()) {
            continue;
          }
          if (shower->isOneNominalInTime()) {
            ugmtEMTFShowerTypeOccupancyPerSector->Fill(
                shower->processor() + 1 + (shower->trackFinderType() == l1t::tftype::emtf_pos ? 6 : 0),
                IDX_NOMINAL_SHOWER);
          }
          if (shower->isOneTightInTime()) {
            ugmtEMTFShowerTypeOccupancyPerSector->Fill(
                shower->processor() + 1 + (shower->trackFinderType() == l1t::tftype::emtf_pos ? 6 : 0),
                IDX_TIGHT_SHOWER);
          }
        }
      }
    }

    // barrel-overlap muon correlations
    int firstBxBO = (BMTFBxCollection->getFirstBX() < OMTFBxCollection->getFirstBX()) ? OMTFBxCollection->getFirstBX()
                                                                                      : BMTFBxCollection->getFirstBX();
    int lastBxBO = (BMTFBxCollection->getLastBX() > OMTFBxCollection->getLastBX()) ? OMTFBxCollection->getLastBX()
                                                                                   : BMTFBxCollection->getLastBX();
    for (int itBX = firstBxBO; itBX <= lastBxBO; ++itBX) {
      if (BMTFBxCollection->size(itBX) < 1 || OMTFBxCollection->size(itBX) < 1) {
        continue;
      }
      for (l1t::RegionalMuonCandBxCollection::const_iterator BMTF = BMTFBxCollection->begin(itBX);
           BMTF != BMTFBxCollection->end(itBX);
           ++BMTF) {
        int global_hw_phi_bmtf =
            l1t::MicroGMTConfiguration::calcGlobalPhi(BMTF->hwPhi(), BMTF->trackFinderType(), BMTF->processor());

        for (l1t::RegionalMuonCandBxCollection::const_iterator OMTF = OMTFBxCollection->begin(itBX);
             OMTF != OMTFBxCollection->end(itBX);
             ++OMTF) {
          int global_hw_phi_omtf =
              l1t::MicroGMTConfiguration::calcGlobalPhi(OMTF->hwPhi(), OMTF->trackFinderType(), OMTF->processor());
          float dEta = (BMTF->hwEta() - OMTF->hwEta()) * etaScale_;
          float dPhi = (global_hw_phi_bmtf - global_hw_phi_omtf) * phiScale_;
          float dR = sqrt(dEta * dEta + dPhi * dPhi);
          if (OMTF->trackFinderType() == l1t::omtf_neg) {
            ugmtBOMTFnegMuMuDEta->Fill(dEta);
            ugmtBOMTFnegMuMuDPhi->Fill(dPhi);
            ugmtBOMTFnegMuMuDR->Fill(dR);
          } else {
            ugmtBOMTFposMuMuDEta->Fill(dEta);
            ugmtBOMTFposMuMuDPhi->Fill(dPhi);
            ugmtBOMTFposMuMuDR->Fill(dR);
          }
        }
      }
    }

    // endcap-overlap muon correlations
    int firstBxEO = (EMTFBxCollection->getFirstBX() < OMTFBxCollection->getFirstBX()) ? OMTFBxCollection->getFirstBX()
                                                                                      : EMTFBxCollection->getFirstBX();
    int lastBxEO = (EMTFBxCollection->getLastBX() > OMTFBxCollection->getLastBX()) ? OMTFBxCollection->getLastBX()
                                                                                   : EMTFBxCollection->getLastBX();
    for (int itBX = firstBxEO; itBX <= lastBxEO; ++itBX) {
      if (EMTFBxCollection->size(itBX) < 1 || OMTFBxCollection->size(itBX) < 1) {
        continue;
      }
      for (l1t::RegionalMuonCandBxCollection::const_iterator EMTF = EMTFBxCollection->begin(itBX);
           EMTF != EMTFBxCollection->end(itBX);
           ++EMTF) {
        int global_hw_phi_emtf =
            l1t::MicroGMTConfiguration::calcGlobalPhi(EMTF->hwPhi(), EMTF->trackFinderType(), EMTF->processor());

        for (l1t::RegionalMuonCandBxCollection::const_iterator OMTF = OMTFBxCollection->begin(itBX);
             OMTF != OMTFBxCollection->end(itBX);
             ++OMTF) {
          int global_hw_phi_omtf =
              l1t::MicroGMTConfiguration::calcGlobalPhi(OMTF->hwPhi(), OMTF->trackFinderType(), OMTF->processor());
          float dEta = (EMTF->hwEta() - OMTF->hwEta()) * etaScale_;
          float dPhi = (global_hw_phi_emtf - global_hw_phi_omtf) * phiScale_;
          float dR = sqrt(dEta * dEta + dPhi * dPhi);
          if (EMTF->trackFinderType() == l1t::emtf_neg && OMTF->trackFinderType() == l1t::omtf_neg) {
            ugmtEOMTFnegMuMuDEta->Fill(dEta);
            ugmtEOMTFnegMuMuDPhi->Fill(dPhi);
            ugmtEOMTFnegMuMuDR->Fill(dR);
          } else if (EMTF->trackFinderType() == l1t::emtf_pos && OMTF->trackFinderType() == l1t::omtf_pos) {
            ugmtEOMTFposMuMuDEta->Fill(dEta);
            ugmtEOMTFposMuMuDPhi->Fill(dPhi);
            ugmtEOMTFposMuMuDR->Fill(dR);
          }
        }
      }
    }
  }

  edm::Handle<l1t::MuonBxCollection> MuonBxCollection;
  e.getByToken(ugmtMuonToken_, MuonBxCollection);

  ugmtnMuons->Fill(MuonBxCollection->size(0));

  for (int itBX = MuonBxCollection->getFirstBX(); itBX <= MuonBxCollection->getLastBX(); ++itBX) {
    for (l1t::MuonBxCollection::const_iterator Muon = MuonBxCollection->begin(itBX);
         Muon != MuonBxCollection->end(itBX);
         ++Muon) {
      int tfMuonIndex = Muon->tfMuonIndex();

      ugmtMuonBX->Fill(itBX);
      ugmtMuonIndex->Fill(tfMuonIndex);
      ugmtMuonhwPt->Fill(Muon->hwPt());
      if (displacedQuantities_) {
        ugmtMuonhwPtUnconstrained->Fill(Muon->hwPtUnconstrained());
        ugmtMuonhwDXY->Fill(Muon->hwDXY());
      }
      ugmtMuonhwEta->Fill(Muon->hwEta());
      ugmtMuonhwPhi->Fill(Muon->hwPhi());
      ugmtMuonhwEtaAtVtx->Fill(Muon->hwEtaAtVtx());
      ugmtMuonhwPhiAtVtx->Fill(Muon->hwPhiAtVtx());
      ugmtMuonhwCharge->Fill(Muon->hwCharge());
      ugmtMuonhwChargeValid->Fill(Muon->hwChargeValid());
      ugmtMuonhwQual->Fill(Muon->hwQual());
      ugmtMuonhwIso->Fill(Muon->hwIso());

      ugmtMuonPt->Fill(Muon->pt());
      if (displacedQuantities_) {
        ugmtMuonPtUnconstrained->Fill(Muon->ptUnconstrained());
      }
      ugmtMuonEta->Fill(Muon->eta());
      ugmtMuonPhi->Fill(Muon->phi());
      ugmtMuonEtaAtVtx->Fill(Muon->etaAtVtx());
      ugmtMuonPhiAtVtx->Fill(Muon->phiAtVtx());
      ugmtMuonCharge->Fill(Muon->charge());

      l1t::tftype tfType{getTfOrigin(tfMuonIndex)};
      if (tfType == l1t::emtf_pos || tfType == l1t::emtf_neg) {
        ugmtMuonPhiEmtf->Fill(Muon->phi());
        ugmtMuonDEtavsPtEmtf->Fill(Muon->pt(), Muon->hwDEtaExtra() * etaScale_);
        ugmtMuonDPhivsPtEmtf->Fill(Muon->pt(), Muon->hwDPhiExtra() * phiScale_);
      } else if (tfType == l1t::omtf_pos || tfType == l1t::omtf_neg) {
        ugmtMuonPhiOmtf->Fill(Muon->phi());
        ugmtMuonDEtavsPtOmtf->Fill(Muon->pt(), Muon->hwDEtaExtra() * etaScale_);
        ugmtMuonDPhivsPtOmtf->Fill(Muon->pt(), Muon->hwDPhiExtra() * phiScale_);
      } else if (tfType == l1t::bmtf) {
        ugmtMuonPhiBmtf->Fill(Muon->phi());
        ugmtMuonDEtavsPtBmtf->Fill(Muon->pt(), Muon->hwDEtaExtra() * etaScale_);
        ugmtMuonDPhivsPtBmtf->Fill(Muon->pt(), Muon->hwDPhiExtra() * phiScale_);
      }

      ugmtMuonPtvsEta->Fill(Muon->eta(), Muon->pt());
      ugmtMuonPtvsPhi->Fill(Muon->phi(), Muon->pt());
      ugmtMuonPhivsEta->Fill(Muon->eta(), Muon->phi());

      ugmtMuonPhiAtVtxvsEtaAtVtx->Fill(Muon->etaAtVtx(), Muon->phiAtVtx());

      ugmtMuonBXvsLink->Fill(int(Muon->tfMuonIndex() / 3.) + 36, itBX);
      ugmtMuonBXvshwPt->Fill(Muon->hwPt(), itBX);
      ugmtMuonBXvshwEta->Fill(Muon->hwEta(), itBX);
      ugmtMuonBXvshwPhi->Fill(Muon->hwPhi(), itBX);
      ugmtMuonBXvshwCharge->Fill(Muon->hwCharge(), itBX);
      ugmtMuonBXvshwChargeValid->Fill(Muon->hwChargeValid(), itBX);
      ugmtMuonBXvshwQual->Fill(Muon->hwQual(), itBX);
      ugmtMuonBXvshwIso->Fill(Muon->hwIso(), itBX);
      ugmtMuonChargevsLink->Fill(int(Muon->tfMuonIndex() / 3.) + 36, Muon->charge());

      int link = (int)std::floor(tfMuonIndex / 3.);
      reco::Candidate::PolarLorentzVector mu1{Muon->pt(), Muon->eta(), Muon->phi(), 0.106};
      reco::Candidate::PolarLorentzVector muAtVtx1{Muon->pt(), Muon->etaAtVtx(), Muon->phiAtVtx(), 0.106};

      // Analyse multi muon events
      for (l1t::MuonBxCollection::const_iterator Muon2 = Muon + 1; Muon2 != MuonBxCollection->end(itBX); ++Muon2) {
        reco::Candidate::PolarLorentzVector mu2{Muon2->pt(), Muon2->eta(), Muon2->phi(), 0.106};
        reco::Candidate::PolarLorentzVector muAtVtx2{Muon2->pt(), Muon2->etaAtVtx(), Muon2->phiAtVtx(), 0.106};
        ugmtMuMuInvMass->Fill((mu1 + mu2).M());
        ugmtMuMuInvMassAtVtx->Fill((muAtVtx1 + muAtVtx2).M());

        float dEta = Muon->eta() - Muon2->eta();
        float dPhi = Muon->phi() - Muon2->phi();
        float dR = sqrt(dEta * dEta + dPhi * dPhi);
        ugmtMuMuDEta->Fill(dEta);
        ugmtMuMuDPhi->Fill(dPhi);
        ugmtMuMuDR->Fill(dR);

        // muon distances between muons from different TFs and from different wedges/sectors of one TF
        int link2 = (int)std::floor(Muon2->tfMuonIndex() / 3.);
        l1t::tftype tfType2{getTfOrigin(Muon2->tfMuonIndex())};
        if ((tfType == l1t::bmtf && tfType2 == l1t::omtf_pos) || (tfType == l1t::omtf_pos && tfType2 == l1t::bmtf)) {
          ugmtMuMuDEtaBOpos->Fill(dEta);
          ugmtMuMuDPhiBOpos->Fill(dPhi);
          ugmtMuMuDRBOpos->Fill(dR);
        } else if ((tfType == l1t::bmtf && tfType2 == l1t::omtf_neg) ||
                   (tfType == l1t::omtf_neg && tfType2 == l1t::bmtf)) {
          ugmtMuMuDEtaBOneg->Fill(dEta);
          ugmtMuMuDPhiBOneg->Fill(dPhi);
          ugmtMuMuDRBOneg->Fill(dR);
        } else if ((tfType == l1t::emtf_pos && tfType2 == l1t::omtf_pos) ||
                   (tfType == l1t::omtf_pos && tfType2 == l1t::emtf_pos)) {
          ugmtMuMuDEtaEOpos->Fill(dEta);
          ugmtMuMuDPhiEOpos->Fill(dPhi);
          ugmtMuMuDREOpos->Fill(dR);
        } else if ((tfType == l1t::emtf_neg && tfType2 == l1t::omtf_neg) ||
                   (tfType == l1t::omtf_neg && tfType2 == l1t::emtf_neg)) {
          ugmtMuMuDEtaEOneg->Fill(dEta);
          ugmtMuMuDPhiEOneg->Fill(dPhi);
          ugmtMuMuDREOneg->Fill(dR);
        } else if (tfType == l1t::bmtf && tfType2 == l1t::bmtf) {
          if (std::abs(link - link2) == 1 || (std::abs(link - link2) == 11)) {  // two adjacent wedges and wrap around
            ugmtMuMuDEtaB->Fill(dEta);
            ugmtMuMuDPhiB->Fill(dPhi);
            ugmtMuMuDRB->Fill(dR);
          }
        } else if (tfType == l1t::omtf_pos && tfType2 == l1t::omtf_pos) {
          if (std::abs(link - link2) == 1 || (std::abs(link - link2) == 5)) {  // two adjacent sectors and wrap around
            ugmtMuMuDEtaOpos->Fill(dEta);
            ugmtMuMuDPhiOpos->Fill(dPhi);
            ugmtMuMuDROpos->Fill(dR);
          }
        } else if (tfType == l1t::omtf_neg && tfType2 == l1t::omtf_neg) {
          if (std::abs(link - link2) == 1 || (std::abs(link - link2) == 5)) {  // two adjacent sectors and wrap around
            ugmtMuMuDEtaOneg->Fill(dEta);
            ugmtMuMuDPhiOneg->Fill(dPhi);
            ugmtMuMuDROneg->Fill(dR);
          }
        } else if (tfType == l1t::emtf_pos && tfType2 == l1t::emtf_pos) {
          if (std::abs(link - link2) == 1 || (std::abs(link - link2) == 5)) {  // two adjacent sectors and wrap around
            ugmtMuMuDEtaEpos->Fill(dEta);
            ugmtMuMuDPhiEpos->Fill(dPhi);
            ugmtMuMuDREpos->Fill(dR);
          }
        } else if (tfType == l1t::emtf_neg && tfType2 == l1t::emtf_neg) {
          if (std::abs(link - link2) == 1 || (std::abs(link - link2) == 5)) {  // two adjacent sectors and wrap around
            ugmtMuMuDEtaEneg->Fill(dEta);
            ugmtMuMuDPhiEneg->Fill(dPhi);
            ugmtMuMuDREneg->Fill(dR);
          }
        }
      }
    }
  }
  // Fill shower plots
  if (hadronicShowers_) {
    edm::Handle<l1t::MuonShowerBxCollection> showersBxCollection;
    e.getByToken(ugmtMuonShowerToken_, showersBxCollection);

    for (int itBX = showersBxCollection->getFirstBX(); itBX <= showersBxCollection->getLastBX(); ++itBX) {
      for (l1t::MuonShowerBxCollection::const_iterator shower = showersBxCollection->begin(itBX);
           shower != showersBxCollection->end(itBX);
           ++shower) {
        if (not shower->isValid()) {
          continue;
        }
        if (shower->isOneNominalInTime()) {
          ugmtMuonShowerTypeOccupancyPerBx->Fill(itBX, IDX_NOMINAL_SHOWER);
        }
        if (shower->isOneTightInTime()) {
          ugmtMuonShowerTypeOccupancyPerBx->Fill(itBX, IDX_TIGHT_SHOWER);
        }
      }
    }
  }
}

l1t::tftype L1TStage2uGMT::getTfOrigin(const int tfMuonIndex) {
  if (tfMuonIndex >= 0 && tfMuonIndex <= 17) {
    return l1t::emtf_pos;
  } else if (tfMuonIndex >= 90 && tfMuonIndex <= 107) {
    return l1t::emtf_neg;
  } else if (tfMuonIndex >= 18 && tfMuonIndex <= 35) {
    return l1t::omtf_pos;
  } else if (tfMuonIndex >= 72 && tfMuonIndex <= 89) {
    return l1t::omtf_neg;
  } else {
    return l1t::bmtf;
  }
}
