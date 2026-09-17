#include <iostream>
#include "FWEventManager.h"
#include "VsdProvider.h"
#include "FWCollectionManager.h"

#include <ROOT/REveManager.hxx>
#include <ROOT/REveStraightLineSet.hxx>
#include "TFile.h"

using namespace ROOT::Experimental;

void EventManager::autoplay(bool x)
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

void EventManager::playdelay(int x)
{
    printf(">>>>> playdelay %d\n", x);
    std::unique_lock<std::mutex> lock{m_mutex};
    m_deltaTime = std::chrono::milliseconds(int(x));
    StampObjProps();
    m_CV.notify_all();
}

void EventManager::autoplay_scheduler()
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
            ROOT::Experimental::REveManager::ChangeGuard ch;
            NextEvent();
        }
        else
        {
            return;
        }
    }
}

void EventManager::PreviousEvent()
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

void EventManager::GotoEvent(int id)
{
    m_event->GotoEvent(id);
    UpdateTitle();
    m_collectionMng->RenewEvent();
    // caloData->DataChanged();
    setPlaneRotation(0, true);
}

void EventManager::UpdateTitle()
{
    // printf("======= update title %lld/%lld event ifnfo run=[%d], lumi=[%d], event = [%lld]\n", m_event->m_eventIdx, m_event->GetNumEvents(),
    //      m_event->m_eventInfo.lumi(), m_event->m_eventInfo.run(), m_event->m_eventInfo.event());
    SetTitle(Form("%lld/%lld/%d/%d/%lld", m_event->m_eventIdx, m_event->GetNumEvents(), m_event->m_eventInfo.lumi(), m_event->m_eventInfo.run(), m_event->m_eventInfo.event()));
    SetName(m_event->m_title); // VSD provider stores file name in title
    StampObjProps();
}

void EventManager::NextEvent()
{
    int id = m_event->m_eventIdx + 1;
    if (id == m_event->GetNumEvents())
    {
        printf("NextEvent: reached last %lld\n", m_event->GetNumEvents());
        id = 0;
    }
    GotoEvent(id);
}

void RotateLineAxis(float angle)
{
    REveScene *scene = nullptr;

    // Find scene
    for (auto *el : gEve->GetScenes()->RefChildren())
    {
        if (el->GetName() == TString("Projection Geometry RPhi"))
        {
            scene = static_cast<REveScene *>(el);
            break;
        }
    }

    printf("Found scene: %s\n", scene->GetName());

    // Find LineSetAxis
    REveStraightLineSetProjected* axisp = nullptr;
    for (auto *el : scene->RefChildren())
    {
        if (el->GetName() == "LineSetAxis [P]")
        {
            axisp = static_cast<REveStraightLineSetProjected *>(el);
            break;
        }
    }

    if (axisp)
    {
        auto axis = dynamic_cast<REveStraightLineSet*>(axisp);
        axis->RefMainTrans().UnitTrans();
        axis->RefMainTrans().RotatePF(2, 1, TMath::DegToRad() * angle);
        axis->StampObjProps();
        axisp->StampObjProps();
    }
    else
        printf("LineSetAxis not found\n");
}


void EventManager::setPlaneRotation(float angle, bool project)
{
    float rad = TMath::DegToRad()*angle;
    REveRhoZProjection* p =  dynamic_cast<ROOT::Experimental::REveRhoZProjection*>(m_collectionMng->m_mngRhoZ->GetProjection());
    REveVector nVec(TMath::Sin(rad), TMath::Cos(rad), 0);
    p->SetPlaneNormal(nVec);

    printf("EventManager::setPlaneRotation\n");
    //p->fProjectedPlaneNormal.Dump();

    RotateLineAxis(angle);
    m_planeAngle = angle;
    StampObjProps();

    if (project) m_collectionMng->m_mngRhoZ->ProjectChildren();
}

void EventManager::setRhoZDistortionStrength(float s)
{
    m_rhoZDistortionStrength = s;
    float d = 0.005f * s;
    m_collectionMng->m_mngRhoZ->GetProjection()->SetDistortion(d);
    m_collectionMng->m_mngRhoZGeo->GetProjection()->SetDistortion(d);
    m_collectionMng->m_mngRhoZ->ProjectChildren();
    m_collectionMng->m_mngRhoZGeo->ProjectChildren();
    StampObjProps();
}

void EventManager::setRhoZDistortionRadius(float r)
{
    m_rhoZDistortionRadius = r;
    m_collectionMng->m_mngRhoZ->GetProjection()->SetFixR(r);
    m_collectionMng->m_mngRhoZGeo->GetProjection()->SetFixR(r);
    m_collectionMng->m_mngRhoZ->ProjectChildren();
    m_collectionMng->m_mngRhoZGeo->ProjectChildren();
    StampObjProps();
}

void EventManager::setRPhiDistortionStrength(float s)
{
    m_rPhiDistortionStrength = s;
    m_collectionMng->m_mngRPhi->GetProjection()->SetDistortion(0.005f * s);
    m_collectionMng->m_mngRPhi->ProjectChildren();
    StampObjProps();
}

void EventManager::setRPhiDistortionRadius(float r)
{
    m_rPhiDistortionRadius = r;
    m_collectionMng->m_mngRPhi->GetProjection()->SetFixR(r);
    m_collectionMng->m_mngRPhi->ProjectChildren();
    StampObjProps();
}

int EventManager::WriteCoreJson(nlohmann::json &j, int rnr_offset)
{
    int res = REveElement::WriteCoreJson(j, -1);
    j["planeAngle"] = std::round(m_planeAngle * 100.0f) / 100.0f;
    j["rhoZDistortionStrength"] = m_rhoZDistortionStrength;
    j["rhoZDistortionRadius"] = m_rhoZDistortionRadius;
    j["rPhiDistortionStrength"] = m_rPhiDistortionStrength;
    j["rPhiDistortionRadius"] = m_rPhiDistortionRadius;
    return res;
}
void EventManager::FilterPublished(const char *data) {}