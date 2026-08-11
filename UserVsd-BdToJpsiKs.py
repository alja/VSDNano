import ROOT
import json
import math
import sys
import time

ROOT.gSystem.Load("libVsdDict.so")

# --- Open VSD file (written by UserVsd-NanoAOD.py) in update mode ---
vsd_file = ROOT.TFile("UserVsd-NanoAOD.root", "UPDATE")
if vsd_file.IsZombie():
    print("ERROR: UserVsd-NanoAOD.root not found. Run UserVsd-NanoAOD.py first.")
    sys.exit(1)
vsd_tree = vsd_file.Get("VSD")
n_vsd = vsd_tree.GetEntries()

if vsd_tree.GetBranch("BVertices"):
    print("Custom branches already present in UserVsd-NanoAOD.root — nothing to do.")
    vsd_file.Close()
    sys.exit(0)

print(f"Adding custom B0->J/psi Ks branches to UserVsd-NanoAOD.root ({n_vsd} entries)")

# --- Open NanoAOD input ---
nano_file = ROOT.TFile.Open("data/BdToJpsiKShort.root")
nano_tree = nano_file.Get("Events")

nano_tree.SetBranchStatus("*", 0)
for branch in [
    # B candidate
    "nbjpsiks",
    "bjpsiks_kin_valid",
    "bjpsiks_kin_vtx_x", "bjpsiks_kin_vtx_y", "bjpsiks_kin_vtx_z",
    "bjpsiks_mm_index",
    "bjpsiks_ks_decay_length",
    "bjpsiks_pion1_pt", "bjpsiks_pion1_eta", "bjpsiks_pion1_phi",
    "bjpsiks_pion2_pt", "bjpsiks_pion2_eta", "bjpsiks_pion2_phi",
    # J/psi (dimuon)
    "nmm",
    "mm_kin_vtx_x", "mm_kin_vtx_y", "mm_kin_vtx_z",
    "mm_kin_mu1_pt", "mm_kin_mu1_eta", "mm_kin_mu1_phi",
    "mm_kin_mu2_pt", "mm_kin_mu2_eta", "mm_kin_mu2_phi",
    "mm_mu1_index", "mm_mu2_index",
    # muon charge from standard Muon collection
    "nMuon", "Muon_charge",
]:
    nano_tree.SetBranchStatus(branch, 1)

# --- Reproduce the same event selection used by UserVsd-NanoAOD.py, so that
# VSD entry i still lines up with the correct NanoAOD entry. ---
n_nano_entries = nano_tree.GetEntries()
selected_entries = []
for idx in range(n_nano_entries):
    nano_tree.GetEntry(idx)
    if any(nano_tree.bjpsiks_kin_valid[i] for i in range(nano_tree.nbjpsiks)):
        selected_entries.append(idx)
        if len(selected_entries) == n_vsd:
            break

if len(selected_entries) != n_vsd:
    print(f"ERROR: found {len(selected_entries)} matching NanoAOD events but "
          f"VSD tree has {n_vsd} entries — re-run UserVsd-NanoAOD.py.")
    vsd_file.Close()
    nano_file.Close()
    sys.exit(1)

# --- Add new branches to existing VSD tree ---

b_vtx_vec = ROOT.std.vector("VsdVertex")()
b_vtx_br = vsd_tree.Branch("BVertices", b_vtx_vec)
b_vtx_br.SetTitle(json.dumps({"color": ROOT.kRed + 1}))

jpsi_vtx_vec = ROOT.std.vector("VsdVertex")()
jpsi_vtx_br = vsd_tree.Branch("JpsiVertices", jpsi_vtx_vec)
jpsi_vtx_br.SetTitle(json.dumps({"color": ROOT.kAzure + 2}))

ks_vtx_vec = ROOT.std.vector("VsdVertex")()
ks_vtx_br = vsd_tree.Branch("KsVertices", ks_vtx_vec)
ks_vtx_br.SetTitle(json.dumps({"color": ROOT.kGreen + 1}))

muon_vec = ROOT.std.vector("VsdMuon")()
muon_br = vsd_tree.Branch("JpsiMuons", muon_vec)
muon_br.SetTitle(json.dumps({"color": ROOT.kAzure + 2}))

pion_vec = ROOT.std.vector("VsdCandidate")()
pion_br = vsd_tree.Branch("KsPions", pion_vec)
pion_br.SetTitle(json.dumps({"color": ROOT.kGreen + 1}))

# --- Fill new branches in sync with NanoAOD events ---

def progress_bar(i, total, t0, width=40):
    frac = (i + 1) / total
    filled = int(width * frac)
    bar = "#" * filled + "-" * (width - filled)
    elapsed = time.time() - t0
    eta = (elapsed / (i + 1)) * (total - i - 1) if i > 0 else 0
    sys.stdout.write(f"\r[{bar}] {i+1}/{total}  elapsed {elapsed:.0f}s  ETA {eta:.0f}s")
    sys.stdout.flush()

t0 = time.time()
for i, idx in enumerate(selected_entries):
    nano_tree.GetEntry(idx)

    if i % 500 == 0:
        progress_bar(i, n_vsd, t0)

    b_vtx_vec.clear()
    jpsi_vtx_vec.clear()
    ks_vtx_vec.clear()
    muon_vec.clear()
    pion_vec.clear()

    for i in range(nano_tree.nbjpsiks):
        if not nano_tree.bjpsiks_kin_valid[i]:
            continue

        # B decay vertex
        bvx = nano_tree.bjpsiks_kin_vtx_x[i]
        bvy = nano_tree.bjpsiks_kin_vtx_y[i]
        bvz = nano_tree.bjpsiks_kin_vtx_z[i]
        b_vtx_vec.push_back(ROOT.VsdVertex(bvx, bvy, bvz))

        # J/psi vertex (from fitted dimuon)
        mm_idx = nano_tree.bjpsiks_mm_index[i]
        jvx = nano_tree.mm_kin_vtx_x[mm_idx]
        jvy = nano_tree.mm_kin_vtx_y[mm_idx]
        jvz = nano_tree.mm_kin_vtx_z[mm_idx]
        jpsi_vtx_vec.push_back(ROOT.VsdVertex(jvx, jvy, jvz))

        # Muon tracks originating at J/psi vertex
        mu1_idx = nano_tree.mm_mu1_index[mm_idx]
        mu1 = ROOT.VsdMuon(
            nano_tree.mm_kin_mu1_pt[mm_idx],
            nano_tree.mm_kin_mu1_eta[mm_idx],
            nano_tree.mm_kin_mu1_phi[mm_idx],
            int(nano_tree.Muon_charge[mu1_idx]),
        )
        mu1.setPos(jvx, jvy, jvz)
        muon_vec.push_back(mu1)

        mu2_idx = nano_tree.mm_mu2_index[mm_idx]
        mu2 = ROOT.VsdMuon(
            nano_tree.mm_kin_mu2_pt[mm_idx],
            nano_tree.mm_kin_mu2_eta[mm_idx],
            nano_tree.mm_kin_mu2_phi[mm_idx],
            int(nano_tree.Muon_charge[mu2_idx]),
        )
        mu2.setPos(jvx, jvy, jvz)
        muon_vec.push_back(mu2)

        # K_S vertex: B vertex + decay_length * unit direction vector.
        # Direction is taken from the vector sum of the two pion momenta
        # (as fit directly for this B candidate), not from the separate,
        # much less complete standalone K_S (V0) collection.
        pi1_pt, pi1_eta, pi1_phi = (
            nano_tree.bjpsiks_pion1_pt[i],
            nano_tree.bjpsiks_pion1_eta[i],
            nano_tree.bjpsiks_pion1_phi[i],
        )
        pi2_pt, pi2_eta, pi2_phi = (
            nano_tree.bjpsiks_pion2_pt[i],
            nano_tree.bjpsiks_pion2_eta[i],
            nano_tree.bjpsiks_pion2_phi[i],
        )

        px = pi1_pt * math.cos(pi1_phi) + pi2_pt * math.cos(pi2_phi)
        py = pi1_pt * math.sin(pi1_phi) + pi2_pt * math.sin(pi2_phi)
        pz = pi1_pt * math.sinh(pi1_eta) + pi2_pt * math.sinh(pi2_eta)
        p = math.sqrt(px * px + py * py + pz * pz)

        ks_decay_length = nano_tree.bjpsiks_ks_decay_length[i]
        ksvx = bvx + ks_decay_length * px / p
        ksvy = bvy + ks_decay_length * py / p
        ksvz = bvz + ks_decay_length * pz / p
        ks_vtx_vec.push_back(ROOT.VsdVertex(ksvx, ksvy, ksvz))

        # Pion tracks originating at K_S vertex (K_S -> pi+ pi-)
        pi1 = ROOT.VsdCandidate(pi1_pt, pi1_eta, pi1_phi, +1)
        pi1.setPos(ksvx, ksvy, ksvz)
        pion_vec.push_back(pi1)

        pi2 = ROOT.VsdCandidate(pi2_pt, pi2_eta, pi2_phi, -1)
        pi2.setPos(ksvx, ksvy, ksvz)
        pion_vec.push_back(pi2)

    # Fill only the new branches for this entry
    b_vtx_br.Fill()
    jpsi_vtx_br.Fill()
    ks_vtx_br.Fill()
    muon_br.Fill()
    pion_br.Fill()

progress_bar(n_vsd - 1, n_vsd, t0)
print(f"\nDone in {time.time()-t0:.1f}s.")

vsd_file.cd()
vsd_tree.Write("", ROOT.TObject.kOverwrite)
vsd_file.Close()
nano_file.Close()
print("Custom branches written to UserVsd-NanoAOD.root")
