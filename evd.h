#include "FWClasses.h"
#include "TEnv.h"
#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"
#include "TTree.h"
#include "TFile.h"
#include "ROOT/REveManager.hxx"
using namespace ROOT::Experimental;

////////////////////////////////////////////////////
////////////////////////////////////////////////////
void evd(const char *data_path)
{
   gSystem->Load("libVsdDict.so");
   gSystem->Load("libFWDict.so");

   VsdProvider *prov = new VsdProvider(data_path);
   REveManager* eveMng = REveManager::Create();

   ROOT::RWebWindowsManager::SetLoopbackMode(false);
   ROOT::Experimental::gEve->GetWebWindow()->SetRequireAuthKey(false);

   ROOT::Experimental::gEve->GetWebWindow()->SetClientVersion("10.4");
   std::string locPath = "ui5";
   eveMng->AddLocation("unidir/", locPath);
   eveMng->SetDefaultHtmlPage("file:unidir/eventDisplay.html");

   FWContext *ctx = new FWContext();
   ctx->createScenesAndViews();
   auto collectionMng = new FWCollectionManager(prov, ctx);

   auto eventMng = new FWEventManager(collectionMng, prov, ctx);
   eventMng->UpdateTitle();
   eventMng->SetName(data_path);

   auto massDialog = new FWInvMassDialog();
   eventMng->AddElement(massDialog);
   eveMng->GetWorld()->AddElement(eventMng);

   for (auto &vsdc : prov->m_collections)
   {
      // printf("vsd collection ====== %s\n", vsdc->m_name.c_str());
      if (vsdc->m_purpose == "EventInfo")
         continue;
      collectionMng->addCollection(vsdc);
   }
   eventMng->GotoEvent(0);

   //((REveViewer*)(ROOT::Experimental::gEve->GetViewers()->FirstChild()))->SetMandatory(false);

   gEnv->SetValue("WebEve.DisableShow", 1);
   eveMng->Show();
}
