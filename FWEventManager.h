#ifndef FWVsdEventManager_h
#define FWVsdEventManager_h

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

#include <ROOT/REveElement.hxx>

class CollectionManager;
class VsdProvider;


class EventManager : public ROOT::Experimental::REveElement
{
private:
   CollectionManager    *m_collectionMng{nullptr};
   VsdProvider          *m_event{nullptr};
   std::chrono::duration<double> m_deltaTime;
   std::thread *m_timerThread{nullptr};
   std::mutex m_mutex;
   std::condition_variable m_CV;
   bool m_autoplay{false};
   float m_planeAngle{0.f};
   

public:
   EventManager(CollectionManager* m, VsdProvider* e):REveElement("EventManager"), m_collectionMng(m), m_event(e) {}
   virtual ~EventManager() {}
 int WriteCoreJson(nlohmann::json &j, int rnr_offset) override;
   virtual void GotoEvent(int id);

  void UpdateTitle();

   virtual void NextEvent();
   virtual void PreviousEvent();

   void autoplay_scheduler();

   void autoplay(bool x);

   void playdelay(int x);
   void FilterPublished(const char* data);

   // projections
   void setPlaneRotation(float angle, bool project = true);
};

#endif