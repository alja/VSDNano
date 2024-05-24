#ifndef FWContext_h
#define FWContext_h

#include <ROOT/REveSelection.hxx>
//using namespace ROOT::Experimental;

namespace ROOT
{
    namespace Experimental
    {
        class REveProjectionManager;
        class REveViewContext;
        class REveCaloDataHist;
    }
}
//==============================================================================
//== Selection =================================================================
//==============================================================================

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

class FWContext
{
    public:
ROOT::Experimental::REveProjectionManager* mngRhoZ{nullptr};
ROOT::Experimental::REveProjectionManager* mngRPhi{nullptr};
ROOT::Experimental::REveViewContext* viewContext{nullptr};
ROOT::Experimental::REveCaloDataHist* caloData{nullptr};


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

   auto eveMng = ROOT::Experimental::gEve;

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
    eveMng->GetWorld()->AddElement(caloData);

   auto calo3d = new REveCalo3D(caloData);
   calo3d->SetBarrelRadius(r);
   calo3d->SetEndCapPos(z);
   calo3d->SetMaxTowerH(300);
   eveMng->GetEventScene()->AddElement(calo3d);

   // Geom  ry
   auto b1 = new REveGeoShape("Barrel 1");
   b1->SetShape(new TGeoTube(r -2 , r+2, z));
   b1->SetMainColor(kCyan);
   // eveMng->GetGlobalScene()->AddElement(b1);

   // Projected RPhi
   if (1)
   {
      auto rPhiEventScene = eveMng->SpawnNewScene("RPhi Scene", "RPhiProjected");
      mngRPhi = new REveProjectionManager(REveProjection::kPT_RPhi);

      mngRPhi->GetProjection()->AddPreScaleEntry(0, r - 2, 1.0);
      mngRPhi->GetProjection()->AddPreScaleEntry(0, 300, 0.6);

      mngRPhi->SetImportEmpty(true);
      auto rPhiView = eveMng->SpawnNewViewer("RPhi View");
      rPhiView->SetCameraType(REveViewer::kCameraOrthoXOY);
      rPhiView->AddScene(rPhiEventScene);

      auto pgeoScene = eveMng->SpawnNewScene("Projection Geometry");
       mngRPhi->SetCurrentDepth(-4);
      mngRPhi->ImportElements(b1,pgeoScene );
      rPhiView->AddScene(pgeoScene);
       mngRPhi->SetCurrentDepth(-1);
      mngRPhi->ImportElements(calo3d, rPhiEventScene);
       mngRPhi->SetCurrentDepth(0);
   }
   // Projected RhoZ
   if (1)
   {
       auto rhoZEventScene = eveMng->SpawnNewScene("RhoZ Scene", "RhoZProjected");
       mngRhoZ = new REveProjectionManager(REveProjection::kPT_RhoZ);

       mngRhoZ->GetProjection()->AddPreScaleEntry(0, r - 2, 1.0);
       mngRhoZ->GetProjection()->AddPreScaleEntry(1, 310, 1.0);
       mngRhoZ->GetProjection()->AddPreScaleEntry(0, 370, 0.6);
       mngRhoZ->GetProjection()->AddPreScaleEntry(1, 580, 0.4);

       mngRhoZ->SetImportEmpty(true);
       auto rhoZView = eveMng->SpawnNewViewer("RhoZ View");
       rhoZView->SetCameraType(REveViewer::kCameraOrthoXOY);
       rhoZView->AddScene(rhoZEventScene);

       auto pgeoScene = eveMng->SpawnNewScene("Projection Geometry");
       mngRhoZ->SetCurrentDepth(-4);
       mngRhoZ->ImportElements(b1, pgeoScene);
       rhoZView->AddScene(pgeoScene);
       mngRhoZ->SetCurrentDepth(-1);
       mngRhoZ->ImportElements(calo3d, rhoZEventScene);
       mngRhoZ->SetCurrentDepth(0);
   }
      // collections
   eveMng->SpawnNewScene("Collections", "Collections");


  auto deviator = std::make_shared<FWSelectionDeviator>();
  eveMng->GetSelection()->SetDeviator(deviator);
  eveMng->GetHighlight()->SetDeviator(deviator);

   // Table
   if (1) {
      auto tableScene = eveMng->SpawnNewScene ("Tables", "Tables");
      auto tableView  = eveMng->SpawnNewViewer("Table", "Table View");
      tableView->AddScene(tableScene);
      tableScene->AddElement(viewContext->GetTableViewInfo());
   }

    //((REveViewer*)(eveMng->GetViewers()->FirstChild()))->SetMandatory(false);
}


};

#endif