#ifndef FWEventManager_h
#define FWEventManager_h

#include "VsdBase.h"
#include "VsdProxies.h"
#include "VsdProvider.h"
#include "FWDataCollection.h"
#include "lego_bins.h"
#include "FWVsdInvMassDialog.h"
#include "FWCollectionManager.h"
#include "FWContext.h"

#include "ROOT/REveDataCollection.hxx"
#include "ROOT/REveDataSimpleProxyBuilderTemplate.hxx"
#include "ROOT/REveManager.hxx"
#include "ROOT/RWebWindowsManager.hxx"
#include "ROOT/REveScalableStraightLineSet.hxx"
//#include "ROOT/REveViewContext.hxx"
#include <ROOT/REveGeoShape.hxx>
// #include <ROOT/REveJetCone.hxx>
#include <ROOT/REvePointSet.hxx>
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
#include <ROOT/REveManager.hxx>

// #include "TTree.h"
#include "TGeoTube.h"
// #include "TList.h"
#include "TParticle.h"
//#include "TRandom.h"
#include "TApplication.h"
#include "TMathBase.h"
#include "TMath.h"
#include "TClass.h"
#include "TGeoBBox.h"
#include "TEnv.h"
using namespace ROOT::Experimental;

//==============================================================================
//== Event Manager =============================================================
//==============================================================================

class FWEventManager : public REveElement
{
private:
   FWCollectionManager    *m_collectionMng{nullptr};
   VsdProvider          *m_event{nullptr};
   FWContext          *m_ctx{nullptr};
   std::chrono::duration<double> m_deltaTime;
   std::thread *m_timerThread{nullptr};
   std::mutex m_mutex;
   std::condition_variable m_CV;
   bool m_autoplay{false};

   public:

   FWEventManager(FWCollectionManager *m, VsdProvider *e, FWContext *ctx) : REveElement("EventManager"),
                                                                            m_collectionMng(m),
                                                                            m_event(e),
                                                                            m_ctx(ctx)
   {
       m_deltaTime = std::chrono::milliseconds(1000);
   }

   virtual void GotoEvent(int id)  
   {
      m_event->GotoEvent(id);
      UpdateTitle();
      m_collectionMng->RenewEvent();
      m_ctx->caloData->DataChanged();
   }
  void UpdateTitle()
   {
      // printf("======= update title %lld/%lld event ifnfo run=[%d], lumi=[%d], event = [%lld]\n", m_event->m_eventIdx, m_event->GetNumEvents(),
      //      m_event->m_eventInfo.lumi(), m_event->m_eventInfo.run(), m_event->m_eventInfo.event());
      SetTitle(Form("%lld/%lld/%d/%d/%lld",m_event->m_eventIdx, m_event->GetNumEvents(), m_event->m_eventInfo.lumi() , m_event->m_eventInfo.run(),  m_event->m_eventInfo.event()));
      StampObjProps();
   }
   virtual void NextEvent()
   {
      int id = m_event->m_eventIdx +1;
      if (id ==  m_event->GetNumEvents()) {
         printf("NextEvent: reached last %lld\n", m_event->GetNumEvents());
         id = 0;
      }
      GotoEvent(id);
   }

   virtual void PreviousEvent()
   {
      int id;
      if (m_event->m_eventIdx == 0)
      {
         id = m_event->GetNumEvents() - 1;
      }
      else
      {
         id = m_event->m_eventIdx - 1;
      }

      printf("going to previous %d \n", id);
      GotoEvent(id);
   }

   void autoplay_scheduler()
   {
      while (true)
      {
         bool autoplay;
         {
                std::unique_lock<std::mutex> lock{m_mutex};
                if (!m_autoplay)
                {
                // printf("exit thread pre wait\n");
                return;
                }
                if (m_CV.wait_for(lock, m_deltaTime) != std::cv_status::timeout)
                {
                printf("autoplay not timed out \n");
                if (!m_autoplay)
                {
                    printf("exit thread post wait\n");
                    return;
                }
                else
                {
                    continue;
                }
                }
                autoplay = m_autoplay;
         }
         if (autoplay)
         {
                REveManager::ChangeGuard ch;
                NextEvent();
         }
         else
         {
                return;
         }
      }
   }

   void autoplay(bool x)
   {
      std::cout << "Set autoplay " << x << std::endl;
      static std::mutex autoplay_mutex;
      std::unique_lock<std::mutex> aplock{autoplay_mutex};
      {
         std::unique_lock<std::mutex> lock{m_mutex};

         StampObjProps();
         m_autoplay = x;
         if (m_autoplay)
         {
                if (m_timerThread)
                {
                m_timerThread->join();
                delete m_timerThread;
                m_timerThread = nullptr;
                }
                NextEvent();
                m_timerThread = new std::thread{[this]
                                                { autoplay_scheduler(); }};
         }
         else
         {
                m_CV.notify_all();
         }
      }
   }

   void playdelay(int x)
   {
      printf(">>>>> playdelay %d\n", x);
      std::unique_lock<std::mutex> lock{m_mutex};
      m_deltaTime = std::chrono::milliseconds(int(x));
      StampObjProps();
      m_CV.notify_all();
   }
};
////////////////////////////////////////////////////
#endif