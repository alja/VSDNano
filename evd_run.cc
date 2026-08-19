#include "TApplication.h"
#include "TKey.h"
#include "TEnv.h"

#include "ROOT/REveManager.hxx"
#include "ROOT/RWebWindowsManager.hxx"
#include "ROOT/REveScalableStraightLineSet.hxx"
//#include "ROOT/REveViewContext.hxx"
#include <ROOT/REveGeoShape.hxx>
// #include <ROOT/REveJetCone.hxx>
#include <ROOT/REveProjectionBases.hxx>
#include <ROOT/REveProjectionManager.hxx>
#include <ROOT/REveScene.hxx>
#include <ROOT/REveTableProxyBuilder.hxx>
#include <ROOT/REveTableInfo.hxx>
#include <ROOT/REveTrack.hxx>
#include <ROOT/REveTrackPropagator.hxx>
#include <ROOT/REveViewer.hxx>
#include <ROOT/REveViewContext.hxx>
#include <ROOT/REveDataCollection.hxx>
#include <ROOT/REveSelection.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REveGeoShapeExtract.hxx>

#include "FWCollectionManager.h"
#include "FWEventManager.h"
#include "FWInvMassDialog.cc"

// globals
//ROOT::Experimental::REveManager* eveMng;
ROOT::Experimental::REveProjectionManager* mngRhoZ;
ROOT::Experimental::REveProjectionManager* mngRPhi;
ROOT::Experimental::REveViewContext* viewContext;
ROOT::Experimental::REveCaloDataHist* caloData;

////////////////////////////////////////////////////

using namespace ROOT::Experimental;

void printElements(REveElement* p, bool recurse) {
   printf("Printing children of %s\n", p->GetCName());
   for (auto c : p->RefChildren())
   {
      printf( "child %s parent %s \n", c->GetCName(), p->GetCName());
      if (recurse)
       printElements(c, recurse);
   }

}


class FWSelectionDeviator : public REveSelection::Deviator {
public:
   FWSelectionDeviator() {}

   using REveSelection::Deviator::DeviateSelection;
   bool DeviateSelection(REveSelection *selection, REveElement *el, bool multi, bool secondary,
                         const std::set<int> &secondary_idcs)
   {
      if (el) {
         auto *colItems = dynamic_cast<REveDataItemList *>(el);
         if (colItems) {
            // std::cout << "Deviate RefSelected=" << colItems->RefSelectedSet().size() << " passed set " << secondary_idcs.size() << "\n";
            ExecuteNewElementPicked(selection, colItems, multi, true, colItems->RefSelectedSet());
            return true;
         }
      }
      return false;
   }
};


void doFishEyeDistortion(REveProjectionManager* projMgr)
{
    float caloDistortion = 0.05;
    float muonDistortion = 0.025;
    if (projMgr->GetProjection()->GetType() == REveProjection::kPT_RPhi)
    {
        projMgr->GetProjection()->ChangePreScaleEntry(0, 1, caloDistortion);
        projMgr->GetProjection()->ChangePreScaleEntry(0, 2, muonDistortion);
    }
    else
    {
        projMgr->GetProjection()->ChangePreScaleEntry(0, 1, caloDistortion);
        projMgr->GetProjection()->ChangePreScaleEntry(0, 2, muonDistortion);
        projMgr->GetProjection()->ChangePreScaleEntry(1, 1, caloDistortion);
        projMgr->GetProjection()->ChangePreScaleEntry(1, 2, muonDistortion);
    }
    projMgr->UpdateName();

    // static const float s_distortF = 0.001;
      REveProjection* p = projMgr->GetProjection();
    p->SetDistortion(0.005);
    p->SetFixR(310);

    // force an update
    projMgr->ProjectChildren();
}

REveGeoShape* getExtract(const char* extract_name)
{
   const char *file = "data/cms_extract.root";
   auto f = TFile::Open(file);

   TIter next(f->GetListOfKeys());
   TKey *key = nullptr;

   while ((key = (TKey *)next())) {

      std::cout << "class name = " << key->GetClassName() << "\n";

      TClass *cl = TClass::GetClass(key->GetClassName());

      if (!cl)
         continue;

      if (cl->InheritsFrom("ROOT::Experimental::REveGeoShapeExtract")) {

         std::cout << "Found extract: " << key->GetName() << "\n";
         if (std::strcmp(key->GetName(), extract_name) == 0)
         {
             auto gse =
                 dynamic_cast<REveGeoShapeExtract *>(key->ReadObj());

             auto gs =
                 REveGeoShape::ImportShapeExtract(gse, 0);
             return gs;
         }
      }
   }

   return nullptr;
}

//==============================================================================
//== init scenes and views  =============================================================
//==============================================================================
void createScenesAndViews()
{
   //view context
   float r = 139.5;
   float z = 290;
   auto prop = new REveTrackPropagator();
   prop->SetMagFieldObj(new REveMagFieldDuo(350, -3.5, 2.0));
   prop->SetMaxR(r);
   prop->SetMaxZ(z);
   prop->SetMaxOrbs(6);
   prop->IncRefCount();



   viewContext = new REveViewContext();
   viewContext->SetBarrel(r, z);
   viewContext->SetTrackPropagator(prop);

   // table specs
   auto tableInfo = new REveTableViewInfo();
   tableInfo->table("VsdVertex").
      column("x",  1, "i.x()").
      column("y",  1, "i.y()").
      column("z",  1, "i.z()");

   tableInfo->table("VsdCandidate").
      column("pt",  1, "i.pt()").
      column("eta", 3, "i.eta()").
      column("phi", 3, "i.phi()").
      column("charge", 3, "i.charge()");

   tableInfo->table("VsdElectron").
      column("pt",  1, "i.pt()").
      column("eta", 3, "i.eta()").
      column("phi", 3, "i.phi()").
      column("HoE", 3, "i.hadronicOverEm()");

   tableInfo->table("VsdMET").
      column("pt",  1, "i.pt()").
      column("sumEt", 3, "i.sumEt()").
      column("phi", 3, "i.phi()");

   tableInfo->table("VsdJet").
      column("pt",  1, "i.pt()").
      column("eta", 3, "i.eta()").
      column("phi", 3, "i.phi()").
      column("hadFraction", 3, "i.hadFraction()");

   viewContext->SetTableViewInfo(tableInfo);

     

    auto baseHist = new TH2F("dummy", "dummy", fw3dlego::xbins_n - 1, fw3dlego::xbins, 72, -TMath::Pi(), TMath::Pi());
    caloData = new REveCaloDataHist();
    caloData->AddHistogram(baseHist);
    auto selector = new REveCaloDataSelector();
    caloData->SetSelector(selector);
    gEve->GetWorld()->AddElement(caloData);

   auto calo3d = new REveCalo3D(caloData);
   calo3d->SetBarrelRadius(r);
   calo3d->SetEndCapPos(z);
   calo3d->SetMaxTowerH(300);
   gEve->GetEventScene()->AddElement(calo3d);

   // Geom  ry
   auto b1 = new REveGeoShape("Barrel 1");
   b1->SetShape(new TGeoTube(r -2 , r+2, z));
   b1->SetMainColor(kCyan);

   REveGeoShape* gse = getExtract("VSDGeo3D");
   std::cout << "Exreact " << gse << "\n"; 
   gEve->GetGlobalScene()->AddElement(gse);
   // Projected RPhi
   if (1)
   {
       auto rPhiEventScene = gEve->SpawnNewScene("RPhi Scene", "RPhiProjected");
       mngRPhi = new REveProjectionManager(REveProjection::kPT_RPhi);

       // distortion
       mngRPhi->GetProjection()->AddPreScaleEntry(0, r - 2, 1.0);
       mngRPhi->GetProjection()->AddPreScaleEntry(0, 300, 0.6);

       mngRPhi->SetImportEmpty(true);
       auto rPhiView = gEve->SpawnNewViewer("RPhi View");
       rPhiView->SetCameraType(REveViewer::kCameraOrthoXOY);
       rPhiView->AddScene(rPhiEventScene);

       auto pgeoScene = gEve->SpawnNewScene("Projection Geometry RPhi");
       mngRPhi->SetCurrentDepth(-4);
       mngRPhi->ImportElements(b1, pgeoScene);

       REveGeoShape *gseProj = getExtract("VSDGeoProj");
       mngRPhi->ImportElements(gseProj, pgeoScene);
       rPhiView->AddScene(pgeoScene);
       mngRPhi->SetCurrentDepth(-1);
       mngRPhi->ImportElements(calo3d, rPhiEventScene);
       mngRPhi->SetCurrentDepth(0);
       doFishEyeDistortion(mngRPhi);
   }
   // Projected RhoZ
   if (1)
   {
       auto rhoZEventScene = gEve->SpawnNewScene("RhoZ Scene", "RhoZProjected");
       mngRhoZ = new REveProjectionManager(REveProjection::kPT_RhoZ);

       // distortion
       mngRhoZ->GetProjection()->AddPreScaleEntry(0, r - 2, 1.0);
       mngRhoZ->GetProjection()->AddPreScaleEntry(1, 310, 1.0);
       mngRhoZ->GetProjection()->AddPreScaleEntry(0, 370, 0.6);
       mngRhoZ->GetProjection()->AddPreScaleEntry(1, 580, 0.4);

       mngRhoZ->SetImportEmpty(true);
       auto rhoZView = gEve->SpawnNewViewer("RhoZ View");
       rhoZView->SetCameraType(REveViewer::kCameraOrthoXOY);
       // it is working but in the wrong direction
       std::vector<double> v = {1,0,0,0,0,1,0,0,0,0,1,0,1803.8861589595506,0,0,1,3.098029906503475};
       rhoZView->GetCamera()->SetCamTransMtx(v);
       rhoZView->GetCamera()->SetOrthoZoom(3.098);
       rhoZView->AddScene(rhoZEventScene);

       auto pgeoScene = gEve->SpawnNewScene("Projection Geometry RhoZ");
       mngRhoZ->SetCurrentDepth(-4);
       mngRhoZ->ImportElements(b1, pgeoScene);

       REveGeoShape *gseRhoZ = getExtract("VSDGeo");
       mngRhoZ->ImportElements(gseRhoZ, pgeoScene);
       rhoZView->AddScene(pgeoScene);
       mngRhoZ->SetCurrentDepth(-1);
       mngRhoZ->ImportElements(calo3d, rhoZEventScene);
       mngRhoZ->SetCurrentDepth(0);
       //doFishEyeDistortion(mngRhoZ);
   }
      // collections
   gEve->SpawnNewScene("Collections", "Collections");

   // Table
   if (1) {
      auto tableScene = gEve->SpawnNewScene ("Tables", "Tables");
      auto tableView  = gEve->SpawnNewViewer("Table", "Table View");
      tableView->AddScene(tableScene);
      tableScene->AddElement(viewContext->GetTableViewInfo());
   }

    //((REveViewer*)(gEve->GetViewers()->FirstChild()))->SetMandatory(false);
}

void evd_run(VsdProvider *prov)
{
   auto eveMng = REveManager::Create();
   gEve->AllowMultipleRemoteConnections(false, false);

   ROOT::RWebWindowsManager::SetLoopbackMode(false);
   ROOT::Experimental::gEve->GetWebWindow()->SetRequireAuthKey(false);
   ROOT::Experimental::gEve->GetWebWindow()->SetClientVersion("10.3");

   std::string locPath = "ui5";
   gEve->AddLocation("unidir/", locPath);
   gEve->SetDefaultHtmlPage("file:unidir/eventDisplay.html");

   //printElements(gEve->GetWorld(), true);
   createScenesAndViews();
   //printElements(gEve->GetWorld(), true);

   auto collectionMng = new CollectionManager(prov);
   collectionMng->m_caloData = caloData;
   collectionMng->m_viewContext = viewContext;
   collectionMng->m_mngRPhi = mngRPhi;
   collectionMng->m_mngRhoZ = mngRhoZ;

   auto eventMng = new EventManager(collectionMng, prov);
TClass::GetClass("EventManager", true);
  // gSystem->Load("./libFWDict.so");
   /*
std::cout << "REveElement class = "
          << TClass::GetClass("ROOT::Experimental::REveElement", false)
          << std::endl;

std::cout << "EventManager class = "
          << TClass::GetClass("EventManager", false)
          << std::endl;

auto c1 = TClass::GetClass("EventManager", false);
std::cout << "before true: " << c1 << "\n";

auto c2 = TClass::GetClass("EventManager", true);
std::cout << "after true:  " << c2 << "\n";

auto c3 = TClass::GetClass("EventManager", false);
std::cout << "after true/false: " << c3 << "\n";*/

/*
   auto c = TClass::GetClass("EventManager", false);
   if (c)
   {

      std::cout << c->GetName() << std::endl;
      std::cout << c->GetBaseClass("ROOT::Experimental::REveElement") << std::endl;
      c->IsA();
   }
   else
   {
      printf("MISSING DICT \n");
   }
*/
   gEve->GetWorld()->AddElement(eventMng);

   eventMng->UpdateTitle();
   eventMng->SetName(prov->m_title.c_str());

   auto massDialog = new InvMassDialog();
   eventMng->AddElement(massDialog);

   auto deviator = std::make_shared<FWSelectionDeviator>();
   gEve->GetSelection()->SetDeviator(deviator);
   gEve->GetHighlight()->SetDeviator(deviator);

   for (auto &vsdc : prov->m_collections)
   {
      if (vsdc->m_purpose == "EventInfo")
         continue;

      collectionMng->addCollection(vsdc);
   }

   eventMng->GotoEvent(0);

   ROOT::Experimental::gEve->GetViewers()->FirstChild()->SetName("3D View");

   gEnv->SetValue("WebEve.DisableShow", 1);
   gEve->Show();
}

////////////////////////////////////////////////////
int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s <VSD.root>\n", argv[0]);
      return 1;
   }

   const char *dummyArgvArray[] = {argv[0]};
   char **dummyArgv = const_cast<char **>(dummyArgvArray);

   int dummyArgc = 1;
   auto *app = new TApplication("evd-test", &dummyArgc, dummyArgv);

   auto *prov = new VsdProvider(argv[1]);

   evd_run(prov);

   app->Run();
   // REveManager::Create()/Show() owns the event loop.
   return 0;
}
