import ROOT
import json

ROOT.gSystem.Load("libVsdDict.so")

Vfile = ROOT.TFile("UserVsdTower.root", "RECREATE")
Vtree = ROOT.TTree("VSD", "Custom plain VSD tree")

pcv = ROOT.std.vector('VsdCandidate')()
candBr = Vtree.Branch("PinkCands", pcv)
candCfg = {
   "filter" : "i.pt() > 1",
   "color" : ROOT.kViolet
}
candBr.SetTitle(json.dumps(candCfg))


# calorimeter towers (1)
ctv1 = ROOT.std.vector('VsdCaloTower')()
ct1Br = Vtree.Branch("CaloTowers-1", ctv1)
cfg = {
   "color" : ROOT.kCyan
}
ct1Br.SetTitle(json.dumps(cfg))
# calorimeter towers (1)

ctv2 = ROOT.std.vector('VsdCaloTower')()
ct2Br = Vtree.Branch("CaloTowers-2", ctv2)
cfg = {
   "color" : ROOT.kGray
}
ct2Br.SetTitle(json.dumps(cfg))

for i in range(10):
    pcv.clear()
    ctv1.clear()
    ctv2.clear()

    for j in range(10 + ROOT.gRandom.Integer(11)):
        cnd = ROOT.VsdCandidate(
            ROOT.gRandom.Uniform(0.1, 20),
            ROOT.gRandom.Uniform(-2.5, 2.5),
            ROOT.gRandom.Uniform(-ROOT.TMath.Pi(), ROOT.TMath.Pi()),
            (1 if ROOT.gRandom.Rndm() > 0.5 else -1))
        cnd.name = f"Candidate_{j}"
        cnd.setPos(ROOT.gRandom.Uniform(0.1, 20),ROOT.gRandom.Uniform(0.1, 20), ROOT.gRandom.Uniform(0.1, 20))
        pcv.push_back(cnd);

    for j in range(3 + ROOT.gRandom.Integer(50)):
        tower = ROOT.VsdCaloTower(
            ROOT.gRandom.Uniform(0.1, 10),
            ROOT.gRandom.Uniform(-2.5, 2.5),
            ROOT.gRandom.Uniform(-ROOT.TMath.Pi(), ROOT.TMath.Pi()))
        ctv1.push_back(tower)

    for j in range(3 + ROOT.gRandom.Integer(120)):
        tower = ROOT.VsdCaloTower(
            ROOT.gRandom.Uniform(0.01, 2),
            ROOT.gRandom.Uniform(-2.5, 2.5),
            ROOT.gRandom.Uniform(-ROOT.TMath.Pi(), ROOT.TMath.Pi()))
        ctv2.push_back(tower)




    Vtree.Fill()

# Save the TTree to a file and close it
Vtree.Write()
Vfile.Close()
