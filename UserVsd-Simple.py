import ROOT

ROOT.gSystem.Load("libVsdDict.so")

Vfile = ROOT.TFile("UserVsd-Simple.root", "RECREATE")
Vtree = ROOT.TTree("VSD", "Custom plain VSD tree")

pcv = ROOT.std.vector('VsdCandidate')()
candBr = Vtree.Branch("PinkCands", pcv)

umv = ROOT.std.vector('VsdMuon')()
mb = Vtree.Branch("UMuon", umv)

for i in range(10):

    pcv.clear()
    umv.clear()

    for j in range(10 + ROOT.gRandom.Integer(11)):
        cnd = ROOT.VsdCandidate(
            ROOT.gRandom.Uniform(0.1, 20),
            ROOT.gRandom.Uniform(-2.5, 2.5),
            ROOT.gRandom.Uniform(-ROOT.TMath.Pi(), ROOT.TMath.Pi()),
            (1 if ROOT.gRandom.Rndm() > 0.5 else -1))
        cnd.name = f"Candidate_{j}"
        cnd.setPos(ROOT.gRandom.Uniform(0.1, 20),ROOT.gRandom.Uniform(0.1, 20), ROOT.gRandom.Uniform(0.1, 20))
        pcv.push_back(cnd)


    for j in range(5):
        muon = ROOT.VsdMuon(
            ROOT.gRandom.Uniform(0.1, 20),
            ROOT.gRandom.Uniform(-2.5, 2.5),
            ROOT.gRandom.Uniform(-ROOT.TMath.Pi(), ROOT.TMath.Pi()),
            (1 if ROOT.gRandom.Rndm() > 0.5 else -1))
        muon.name = f"Muon_{j}"
        muon.setPos(ROOT.gRandom.Uniform(0.1, 20),ROOT.gRandom.Uniform(0.1, 20), ROOT.gRandom.Uniform(0.1, 20))
        umv.push_back(muon)

    Vtree.Fill()

# Save the TTree to a file and close it
Vtree.Write()
Vfile.Close()
