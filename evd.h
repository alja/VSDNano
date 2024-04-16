#include "FWClasses.h"

////////////////////////////////////////////////////
////////////////////////////////////////////////////
void evd(const char* data_path)
{
gSystem->Load("libVsdDict.so");
gSystem->Load("libFWDict.so");

   VsdProvider* prov = new VsdProvider(data_path);
   eveMng = REveManager::Create();


	   ROOT::RWebWindowsManager::SetLoopbackMode(false);
	      ROOT::Experimental::gEve->GetWebWindow()->SetRequireAuthKey(false); 
    ROOT::Experimental::gEve->GetWebWindow()->SetClientVersion("10.3");
    std::string locPath = "ui5";
    eveMng->AddLocation("unidir/", locPath);
    eveMng->SetDefaultHtmlPage("file:unidir/eventDisplay.html");
//  gEnv->SetValue("WebGui.HttpMaxAge", 3600);


   createScenesAndViews();
   auto collectionMng = new CollectionManager(prov);

   auto eventMng = new EventManager(collectionMng, prov);
   eventMng->UpdateTitle();
   eventMng->SetName(data_path);

  auto massDialog = new InvMassDialog();
  eventMng->AddElement(massDialog);
  eveMng->GetWorld()->AddElement(eventMng);

  auto deviator = std::make_shared<FWSelectionDeviator>();
  eveMng->GetSelection()->SetDeviator(deviator);
  eveMng->GetHighlight()->SetDeviator(deviator);
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
