import ROOT
import json
import math
import sys
import time

ROOT.gSystem.Load("libVsdDict.so")

nano_file = ROOT.TFile.Open("data/BdToJpsiKShort.root")
nano_tree = nano_file.Get("Events")
n_entries = nano_tree.GetEntries()
print(f"NanoAOD entries: {n_entries}")

# Disable all branches, then enable only the ones we need.

nano_tree.SetBranchStatus("*", 0)
for branch in [
    "nMuon", "Muon_pt", "Muon_eta", "Muon_phi",
    "Muon_charge", "Muon_dxy", "Muon_dz", "Muon_isGlobal",
    "nJet", "Jet_pt", "Jet_eta", "Jet_phi", "Jet_chHEF", "Jet_neHEF",
    "run", "luminosityBlock", "event",
]:
    nano_tree.SetBranchStatus(branch, 1)

vsd_file = ROOT.TFile("UserVsd-NanoAOD.root", "RECREATE")
vsd_tree = ROOT.TTree("VSD", "NanoAOD -> VSD")

# --- Muons ---
muon_vec = ROOT.std.vector('VsdMuon')()
muon_br = vsd_tree.Branch("Muons", muon_vec)
muon_br.SetTitle(json.dumps({
    "color": ROOT.kRed,
    "filter": "i.pt() > 1",
}))

# --- Jets ---
jet_vec = ROOT.std.vector('VsdJet')()
jet_br = vsd_tree.Branch("Jets", jet_vec)
jet_br.SetTitle(json.dumps({
    "color": ROOT.kYellow,
}))

# --- EventInfo ---
ei_vec = ROOT.std.vector('VsdEventInfo')()
vsd_tree.Branch("EventInfo", ei_vec)

def progress_bar(i, total, t0, width=40):
    frac = (i + 1) / total
    filled = int(width * frac)
    bar = "#" * filled + "-" * (width - filled)
    elapsed = time.time() - t0
    eta = (elapsed / (i + 1)) * (total - i - 1) if i > 0 else 0
    sys.stdout.write(f"\r[{bar}] {i+1}/{total}  elapsed {elapsed:.0f}s  ETA {eta:.0f}s")
    sys.stdout.flush()

t0 = time.time()
# for idx in range(n_entries):
for idx in range(20):
    nano_tree.GetEntry(idx)

    if idx % 500 == 0:
        progress_bar(idx, n_entries, t0)

    muon_vec.clear()
    jet_vec.clear()
    ei_vec.clear()

    for i in range(nano_tree.nMuon):
        pt        = nano_tree.Muon_pt[i]
        eta       = nano_tree.Muon_eta[i]
        phi       = nano_tree.Muon_phi[i]
        charge    = nano_tree.Muon_charge[i]
        dxy       = nano_tree.Muon_dxy[i]
        dz        = nano_tree.Muon_dz[i]
        is_global = bool(nano_tree.Muon_isGlobal[i])

        muon = ROOT.VsdMuon(pt, eta, phi, charge, is_global)
        # PCA to beam axis, assuming PV at (0,0,0)
        muon.setPos(-dxy * math.sin(phi), dxy * math.cos(phi), dz)
        muon_vec.push_back(muon)

    for i in range(nano_tree.nJet):
        jet = ROOT.VsdJet(
            nano_tree.Jet_pt[i], nano_tree.Jet_eta[i], nano_tree.Jet_phi[i],
            0,
            nano_tree.Jet_chHEF[i] + nano_tree.Jet_neHEF[i],
            0.4,
        )
        jet_vec.push_back(jet)

    ei_vec.push_back(ROOT.VsdEventInfo(
        nano_tree.run, nano_tree.luminosityBlock, nano_tree.event
    ))

    vsd_tree.Fill()

progress_bar(n_entries - 1, n_entries, t0)
print(f"\nDone in {time.time()-t0:.1f}s. Written to UserVsd-NanoAOD.root")

vsd_tree.Write()
vsd_file.Close()
nano_file.Close()
