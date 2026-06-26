#include <ROOT/REveGeoTopNode.hxx>
#include <ROOT/REveGeoPolyShape.hxx>
#include <ROOT/REveManager.hxx>
#include "TFile.h"
#include <vector>
#include <string>

using namespace ROOT::Experimental;

//------------------------------------------------------------------------------
// Reusable shape builder (safe + clean)
//------------------------------------------------------------------------------
REveGeoShape *makeShape(const std::string &targetPath, const std::string &name)
{
   if (!gGeoManager->cd(targetPath.c_str())) {
      Error("makeShape", "Path not found: %s", targetPath.c_str());
      return nullptr;
   }

   TGeoNode *node = gGeoManager->GetCurrentNode();
   TGeoHMatrix *mat = gGeoManager->GetCurrentMatrix();

   Double_t m[16];
   mat->GetHomogenousMatrix(m);

   TGeoShape *shape =
      (TGeoShape*) node->GetVolume()->GetShape()->Clone();
   shape->SetName(name.c_str());

   auto rgs = new REveGeoShape(name.c_str());
   rgs->SetShape(shape);

   rgs->InitMainTrans();
   rgs->RefMainTrans().SetFrom(m);

   rgs->SetMainTransparency(70);

   return rgs;
}
std::string extractNameFromPath(const std::string &path)
{
   // get last component after '/'
   size_t slashPos = path.find_last_of('/');
   std::string last = (slashPos == std::string::npos) ?
                      path : path.substr(slashPos + 1);

   // remove prefix before ':'
   size_t colonPos = last.find(':');
   if (colonPos != std::string::npos)
      last = last.substr(colonPos + 1);

   return last;
}
//------------------------------------------------------------------------------
// Build a subsystem (tracker/muon/ecal/hcal)
//------------------------------------------------------------------------------
REveGeoShape* buildGroup(const std::string &groupName,
                         const std::vector<std::string> &paths,
                         Color_t color)
{
   auto top = new REveGeoShape(groupName.c_str());
   top->SetMainColor(color);
   top->SetMainTransparency(70);

   int i = 0;
std::set<std::string> usedNames;

for (const auto &p : paths) {
   std::string baseName = extractNameFromPath(p);
   std::string name = baseName;

   int counter = 1;
   while (usedNames.count(name)) {
      name = baseName + "_" + std::to_string(counter++);
   }
   usedNames.insert(name);

   auto shape = makeShape(p, name);
   if (shape) {
      shape->SetMainColor(top->GetMainColor());
      top->AddElement(shape);
   }
}
/*
   for (const auto &p : paths) {
      std::string childName = groupName + "_" + std::to_string(i++);
      auto shape = makeShape(p, childName);
      if (shape)
         top->AddElement(shape);
   }
*/
   return top;
}

//------------------------------------------------------------------------------
void write_geo_extract_full()
{
   TFile::SetCacheFileDir(".");

   auto eveMng = REveManager::Create();
   eveMng->AllowMultipleRemoteConnections(false, false);

   TGeoManager::Import("/home/viz/root-ws/cmsSimGeo2021.root");

   // ---------------- TRACKER ----------------
   std::vector<std::string> trackerPaths = {
      "cms:OCMS/tracker:Tracker_1/trackerother:TrackerOuterCylinder_1",
      "cms:OCMS/tracker:Tracker_1/trackerbulkhead:TrackerBulkhead_2",
      "cms:OCMS/tracker:Tracker_1/trackerbulkhead:TrackerBulkhead_1",
      "cms:OCMS/tracker:Tracker_1/tib:TIB_1"
   };

   // ---------------- MUON ----------------
   std::vector<std::string> muonPaths = {
      "cms:OCMS/muonBase:MUON_1/muonBase:MB_1/muonBase:MBWheel_2P_5",
      "cms:OCMS/muonBase:MUON_1/muonBase:MB_1/muonBase:MBWheel_1P_4",
      "cms:OCMS/muonBase:MUON_1/muonBase:MB_1/muonBase:MBWheel_0_3",
      "cms:OCMS/muonBase:MUON_1/muonBase:MB_1/muonBase:MBWheel_1N_2",
      "cms:OCMS/muonBase:MUON_1/muonBase:MB_1/muonBase:MBWheel_2N_1",
      "cms:OCMS/muonBase:MUON_1/mf:MEP_1",
      "cms:OCMS/muonBase:MUON_1/mf:MEN_2",
      "cms:OCMS/muonBase:MUON_1/mgnt:MGNT_1"
   };

   // ---------------- ECAL ----------------
   std::vector<std::string> ecalPaths = {
      "cms:OCMS/caloBase:CALO_1/eregalgo:ECAL_1/eregalgo:EBAR_1",
      "cms:OCMS/caloBase:CALO_1/eregalgo:ECAL_1/eregalgo:EREG_1",
      "cms:OCMS/caloBase:CALO_1/eregalgo:ECAL_1/eregalgo:EREG_2",
      "cms:OCMS/caloBase:CALO_1/eregalgo:ECAL_1/ectkcable:ETCA_1",
      "cms:OCMS/caloBase:CALO_1/eregalgo:ECAL_1/ectkcable:ETCA_2"
   };

   // ---------------- HCAL ----------------
   std::vector<std::string> hcalPaths = {
      "cms:OCMS/caloBase:CALO_1/hcalalgo:HCal_1/hcalcablealgo:HRCF_1",
      "cms:OCMS/caloBase:CALO_1/hcalalgo:HCal_1/hcalcablealgo:HRCF_2",
      "cms:OCMS/caloBase:CALO_1/hcalalgo:HCal_1/hcalbarrelalgo:HB_1",
      "cms:OCMS/caloBase:CALO_1/hcalalgo:HCal_1/hcalendcapalgo:HE_1",
      "cms:OCMS/caloBase:CALO_1/hcalalgo:HCal_1/hcalendcapalgo:HE_2"
   };

   // Build subsystems
   auto tracker = buildGroup("tracker", trackerPaths, kRed);
   auto muon    = buildGroup("muon",    muonPaths,    kBlue);
   auto ecal    = buildGroup("ecal",    ecalPaths,    kGreen+2);
   auto hcal    = buildGroup("hcal",    hcalPaths,    kOrange+7);

   // Add to scene
   auto scene = eveMng->GetEventScene();
 //  scene->AddElement(tracker);
  // scene->AddElement(muon);
   //scene->AddElement(ecal);
   //scene->AddElement(hcal);

   // Save everything under one top node (optional)
   auto all = new REveGeoShape("CMS");
   //auto all = makeShape("cms:OCMS", "CMS");
   all->SetRnrSelf(false);
   all->AddElement(tracker);
   all->AddElement(muon);
   all->AddElement(ecal);
   all->AddElement(hcal);
   scene->AddElement(all);

    TFile f("cms_extract.root", "RECREATE");
   all->WriteExtract("VSDGeo");

// RPhi
   tracker->FindChild("TrackerBulkhead_1")->SetRnrSelf(false);
   tracker->FindChild("TrackerBulkhead_2")->SetRnrSelf(false);
   tracker->FindChild("TIB_1")->SetMainTransparency(90);

   muon->FindChild("MBWheel_2P_5")->SetRnrSelf(false);
   muon->FindChild("MBWheel_1P_4")->SetRnrSelf(false);
   muon->FindChild("MBWheel_1N_2")->SetRnrSelf(false);
   muon->FindChild("MBWheel_2N_1")->SetRnrSelf(false);
   muon->FindChild("MEP_1")->SetRnrSelf(false);
   muon->FindChild("MEN_2")->SetRnrSelf(false);
   muon->FindChild("MGNT_1")->SetRnrSelf(false);

   ecal->FindChild("EREG_1")->SetRnrSelf(false);
   ecal->FindChild("EREG_2")->SetRnrSelf(false);
   ecal->FindChild("ETCA_1")->SetRnrSelf(false);
   ecal->FindChild("ETCA_2")->SetRnrSelf(false);

   hcal->FindChild("HRCF_1")->SetRnrSelf(false);
   hcal->FindChild("HRCF_2")->SetRnrSelf(false);
   hcal->FindChild("HE_1")->SetRnrSelf(false);
   hcal->FindChild("HE_2")->SetRnrSelf(false);

   all->WriteExtract("VSDGeoProj");

// 3D
   tracker->FindChild("TIB_1")->SetRnrSelf(false);
   tracker->FindChild("TrackerOuterCylinder_1")->SetMainTransparency(95);
   muon->SetRnrChildren(false);
ecal->SetRnrChildren(false);
hcal->SetRnrChildren(false);
   all->WriteExtract("VSDGeo3D");

   f.Close();
   eveMng->Show();
}