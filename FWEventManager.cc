#include <iostream>
#include "FWEventManager.h"
#include "FWProvider.h"
#include "FWCollectionManager.h"

#include <ROOT/REveManager.hxx>
#include <ROOT/REveSelection.hxx>
#include <ROOT/REveStraightLineSet.hxx>
#include <ROOT/REveViewer.hxx>
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
    if (m_event->GetEventIdx() == 0)
    {
        id = m_event->GetNumEvents() - 1;
    }
    else
    {
        id = m_event->GetEventIdx() - 1;
    }

    printf("going to previous %d \n", id);
    GotoEvent(id);
}

void EventManager::GotoEvent(int id)
{
    // Selection records point at collection item lists and item indices,
    // which are reused for the new event: without this the outline stays on
    // whatever item now has the selected index.
    ROOT::Experimental::gEve->GetSelection()->ClearSelection();
    ROOT::Experimental::gEve->GetHighlight()->ClearSelection();

    m_event->GotoEvent(id);
    UpdateTitle();
    m_collectionMng->RenewEvent();
    // caloData->DataChanged();

    // Restore RhoZ plane angle was last set for this event. default for 0
    auto it = m_planeAngleByEvent.find(m_event->GetEventIdx());
    setPlaneRotation(it != m_planeAngleByEvent.end() ? it->second : 0.f, true);
}

void EventManager::UpdateTitle()
{
    FWEventId eid = m_event->GetEventId();
    // the GUI (MyNewMain.controller.js showEventInfo) reads idx/nevents/run/lumi/event
    SetTitle(Form("%lld/%lld/%d/%d/%lld", m_event->GetEventIdx(), m_event->GetNumEvents(), eid.m_run, eid.m_lumi, eid.m_event));
    SetName(m_event->GetTitle().c_str()); // provider title is usually the file name
    StampObjProps();
}

void EventManager::NextEvent()
{
    int id = m_event->GetEventIdx() + 1;
    if (id == m_event->GetNumEvents())
    {
        printf("NextEvent: reached last %lld\n", m_event->GetNumEvents());
        id = 0;
    }
    GotoEvent(id);
}

// The projected rotation axis in the RPhi geometry scene, or nullptr.
static REveStraightLineSetProjected *FindRPhiLineAxis()
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
    if (!scene)
        return nullptr;

    // Find LineSetAxis
    for (auto *el : scene->RefChildren())
    {
        if (el->GetName() == "LineSetAxis [P]")
            return static_cast<REveStraightLineSetProjected *>(el);
    }
    return nullptr;
}

void RotateLineAxis(float angle)
{
    REveStraightLineSetProjected* axisp = FindRPhiLineAxis();

    if (axisp)
    {
        auto axis = dynamic_cast<REveStraightLineSet*>(axisp);
        axis->RefMainTrans().UnitTrans();
        axis->RefMainTrans().RotatePF(2, 1, TMath::DegToRad() * angle);
        axis->StampObjProps();
        // the axis only marks a rotated projection plane
        axisp->SetRnrSelf(angle != 0.f);
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
    m_planeAngleByEvent[m_event->GetEventIdx()] = angle;
    StampObjProps();

    if (project) m_collectionMng->m_mngRhoZ->ProjectChildren();
}

void EventManager::setRhoZDistortionStrength(float s)
{
    // Reprojecting the geometry costs ~35 ms against ~0.5 ms for the event
    // scene, so never redo the work for a value we are already showing.
    if (s == m_rhoZDistortionStrength) return;

    m_rhoZDistortionStrength = s;
    float d = 0.005f * s;
    m_collectionMng->m_mngRhoZ->GetProjection()->SetDistortion(d);
    m_collectionMng->m_mngRhoZGeo->GetProjection()->SetDistortion(d);
    m_collectionMng->m_mngRhoZ->ProjectChildren();
    m_collectionMng->m_mngRhoZGeo->ProjectChildren();
    if (m_collectionMng->m_axisRhoZ) m_collectionMng->m_axisRhoZ->UpdateTicks();
    StampObjProps();
}

void EventManager::setRhoZDistortionRadius(float r)
{
    if (r == m_rhoZDistortionRadius) return;

    m_rhoZDistortionRadius = r;
    m_collectionMng->m_mngRhoZ->GetProjection()->SetFixR(r);
    m_collectionMng->m_mngRhoZGeo->GetProjection()->SetFixR(r);
    m_collectionMng->m_mngRhoZ->ProjectChildren();
    m_collectionMng->m_mngRhoZGeo->ProjectChildren();
    if (m_collectionMng->m_axisRhoZ) m_collectionMng->m_axisRhoZ->UpdateTicks();
    StampObjProps();
}

void EventManager::setRPhiDistortionStrength(float s)
{
    if (s == m_rPhiDistortionStrength) return;

    m_rPhiDistortionStrength = s;
    m_collectionMng->m_mngRPhi->GetProjection()->SetDistortion(0.005f * s);
    m_collectionMng->m_mngRPhi->ProjectChildren();
    if (m_collectionMng->m_axisRPhi) m_collectionMng->m_axisRPhi->UpdateTicks();
    StampObjProps();
}

void EventManager::setRPhiDistortionRadius(float r)
{
    if (r == m_rPhiDistortionRadius) return;

    m_rPhiDistortionRadius = r;
    m_collectionMng->m_mngRPhi->GetProjection()->SetFixR(r);
    m_collectionMng->m_mngRPhi->ProjectChildren();
    if (m_collectionMng->m_axisRPhi) m_collectionMng->m_axisRPhi->UpdateTicks();
    StampObjProps();
}

void EventManager::setDarkMode(bool x)
{
    if (x == m_darkMode) return;

    m_darkMode = x;
    // The GL viewers swap background and foreground; elements that follow the
    // foreground colour, such as the projection axes, are recoloured by the client.
    for (auto *el : gEve->GetViewers()->RefChildren())
    {
        if (auto viewer = dynamic_cast<REveViewer *>(el))
            viewer->SetBlackBackground(x);
    }
    // the RPhi rotation axis has a fixed colour
    if (auto axisp = FindRPhiLineAxis())
    {
        axisp->SetMainColor(x ? kWhite : kBlack);
        axisp->StampObjProps();
    }
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
    j["darkMode"] = m_darkMode;
    return res;
}
void EventManager::FilterPublished(const char *data) {}