import ROOT
import json
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

if vsd_tree.GetBranch("GenParticles"):
    print("GenParticles branch already present in UserVsd-NanoAOD.root — nothing to do.")
    vsd_file.Close()
    sys.exit(0)

print(f"Adding GenParticles branch to UserVsd-NanoAOD.root ({n_vsd} entries)")

# --- Open NanoAOD input ---
nano_file = ROOT.TFile.Open("data/BdToJpsiKShort.root")
nano_tree = nano_file.Get("Events")

nano_tree.SetBranchStatus("*", 0)
for branch in [
    # same selection branches used by UserVsd-NanoAOD.py, so VSD entry i
    # still lines up with the correct NanoAOD entry
    "nbjpsiks", "bjpsiks_kin_valid",
    # GenPart
    "nGenPart", "GenPart_pt", "GenPart_eta", "GenPart_phi",
    "GenPart_pdgId", "GenPart_vx", "GenPart_vy", "GenPart_vz",
]:
    nano_tree.SetBranchStatus(branch, 1)

# --- Reproduce the same event selection used by UserVsd-NanoAOD.py ---
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

# electric charge (in units of e) for a *positive* pdgId, per PDG convention;
# antiparticles (negative pdgId) flip the sign. Particles not listed default to neutral (0), which covers photons/gluons/neutrinos/pi0/K0/neutron/etc.
CHARGE_FOR_POSITIVE_ID = {
    11: -1,   # e-
    13: -1,   # mu-
    15: -1,   # tau-
    211: 1,   # pi+
    321: 1,   # K+
    2212: 1,  # proton
    24: 1,    # W+
}

def gen_charge(pdg_id):
    sign = 1 if pdg_id > 0 else -1
    return sign * CHARGE_FOR_POSITIVE_ID.get(abs(pdg_id), 0)

# --- Add new branch to existing VSD tree ---
gen_vec = ROOT.std.vector("VsdCandidate")()
gen_br = vsd_tree.Branch("GenParticles", gen_vec)
gen_br.SetTitle(json.dumps({
    "color": ROOT.kMagenta + 1,
    "filter": "i.pt() > 0.5",
}))

# --- Fill new branch in sync with NanoAOD events ---

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

    gen_vec.clear()

    for j in range(nano_tree.nGenPart):
        pdg_id = nano_tree.GenPart_pdgId[j]
        gp = ROOT.VsdCandidate(
            nano_tree.GenPart_pt[j],
            nano_tree.GenPart_eta[j],
            nano_tree.GenPart_phi[j],
            gen_charge(pdg_id),
        )
        gp.setPos(
            nano_tree.GenPart_vx[j],
            nano_tree.GenPart_vy[j],
            nano_tree.GenPart_vz[j],
        )
        gp.setPdgId(int(pdg_id))
        gen_vec.push_back(gp)

    gen_br.Fill()

progress_bar(n_vsd - 1, n_vsd, t0)
print(f"\nDone in {time.time()-t0:.1f}s.")

vsd_file.cd()
vsd_tree.Write("", ROOT.TObject.kOverwrite)
vsd_file.Close()
nano_file.Close()
print("GenParticles branch written to UserVsd-NanoAOD.root")
