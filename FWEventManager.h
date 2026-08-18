#ifndef FWVsdEventManager_h
#define FWVsdEventManager_h

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

#include <ROOT/REveElement.hxx>

class CollectionManager;
class VsdProvider;

using namespace ROOT::Experimental;

class EventManager : public REveElement
{
private:
   CollectionManager    *m_collectionMng{nullptr};
   VsdProvider          *m_event{nullptr};
   std::chrono::duration<double> m_deltaTime;
   std::thread *m_timerThread{nullptr};
   std::mutex m_mutex;
   std::condition_variable m_CV;
   bool m_autoplay{false};
   

public:
   EventManager(CollectionManager* m, VsdProvider* e):REveElement("EventManager"), m_collectionMng(m), m_event(e) {}
   virtual ~EventManager() {}

   virtual void GotoEvent(int id);

  void UpdateTitle();

   virtual void NextEvent();
   virtual void PreviousEvent();

   void autoplay_scheduler();

   void autoplay(bool x);

   void playdelay(int x);
   void FilterPublished(const char* data);
};

#endif