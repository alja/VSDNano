#ifndef FWCollectionManager_h
#define FWCollectionManager_h

#include "ROOT/REveManager.hxx"
#include "FWContext.h"
//==============================================================================
//== Collection Manager =============================================================
//==============================================================================
class FWCollectionManager
{
private:
    VsdProvider *m_event{nullptr};
    FWContext *m_ctx{nullptr};
    REveElement *m_collections{nullptr};

    std::vector<REveDataProxyBuilderBase *> m_builders;

    bool m_isEventLoading{false}; // don't process model changes when applying filter on new event
public:
    FWCollectionManager(VsdProvider *event, FWContext* ctx) : m_event(event), m_ctx(ctx)
    {
        m_collections = ROOT::Experimental::gEve->GetScenes()->FindChild("Collections");
    }

    void LoadCurrentEventInCollection(REveDataCollection *rdc)
    {
        m_isEventLoading = true;
        rdc->ClearItems();
        VsdCollection *vsdc = m_event->RefColl(rdc->GetName());
        std::string cname = rdc->GetName();
        // printf("-------- LoadCurrentEventInCollection %s size %lu\n", rdc->GetCName(), vsdc->m_list.size());
        std::string t = "dummy";
        for (auto vsd : vsdc->m_list)
        {
            // printf ("add item tp REveColl\n"); vsd->dump();
            rdc->AddItem(vsd, t, t);
        }
        rdc->ApplyFilter();
        rdc->GetItemList()->StampObjProps();

        m_isEventLoading = false;
    }

    void RenewEvent()
    {
        for (auto &el : m_collections->RefChildren())
        {
            auto c = dynamic_cast<REveDataCollection *>(el);
            LoadCurrentEventInCollection(c);
        }

        for (auto proxy : m_builders)
        {
            proxy->Build();
        }
    }

    REveDataProxyBuilderBase *getProxyBuilderFromVSD(VsdCollection *vsdc)
    {
        if (vsdc->m_purpose == "Candidate")
            return new CandidateProxyBuilder();
        else if (vsdc->m_purpose == "Jet")
            return new JetProxyBuilder();
        else if (vsdc->m_purpose == "MET")
            return new METProxyBuilder();
        else if (vsdc->m_purpose == "Muon")
            return new MuonProxyBuilder();
        else if (vsdc->m_purpose == "Vertex")
            return new VertexProxyBuilder();
        else if (vsdc->m_purpose == "CaloTower")
            return new CaloTowerProxyBuilder(m_ctx->caloData);

        std::cout << typeid(vsdc).name() << '\n';

        // amt alternative way
        std::string pbn = vsdc->m_purpose + "ProxyBuilder";
        // TClass* pbc = TClass::GetClass(pbn.c_str());

        printf("can't find proxy for purpose %s \n", vsdc->m_purpose.c_str());
        return nullptr;
    }

    void
    addCollection(VsdCollection *vsdc)
    {
        FWDataCollection *collection = new FWDataCollection(vsdc->m_name, vsdc->m_varConfig);
        m_collections->AddElement(collection);
        std::string class_name = "Vsd" + vsdc->m_type; // !!! This works beacuse it is a root macro

        // std::cout << "addCollection class name " << class_name << "\n";

        TClass* tc  = TClass::GetClass(class_name.c_str());
        if (!tc) {
            // class_name = "VSD" + vsdc->m_purpose; // !!! This works beacuse it is a root macro
            class_name = vsdc->m_purpose;
            tc  = TClass::GetClass(class_name.c_str());
            if (!tc)
            throw( std::runtime_error("addCollection " +  vsdc->m_name ) );
        }

        collection->SetItemClass(TClass::GetClass(class_name.c_str()));
        collection->SetMainColor(vsdc->m_color);
        collection->SetFilterExpr(vsdc->m_filter.c_str());

        REveDataProxyBuilderBase *glBuilder = getProxyBuilderFromVSD(vsdc);

        // load data
        LoadCurrentEventInCollection(collection);
        glBuilder->SetCollection(collection);
        glBuilder->SetHaveAWindow(true);

        for (auto &scene : ROOT::Experimental::gEve->GetScenes()->RefChildren())
        {

            // REveElement *product = glBuilder->CreateProduct(scene->GetTitle(), m_ctx->viewContext);

            if (strncmp(scene->GetCName(), "Table", 5) == 0)
                continue;
            if (!strncmp(scene->GetCTitle(), "RhoZProjected", 8))
            {
                REveElement *product = glBuilder->CreateProduct("RhoZViewType", m_ctx->viewContext);
                m_ctx->mngRhoZ->ImportElements(product, scene);
                continue;
            }
            if (!strncmp(scene->GetCTitle(), "RPhiProjected", 8))
            {
                REveElement *product = glBuilder->CreateProduct("RPhiViewType", m_ctx->viewContext);
                m_ctx->mngRPhi->ImportElements(product, scene);
                continue;
            }
            else if ((!strncmp(scene->GetCName(), "Event scene", 8)))
            {
                REveElement *product = glBuilder->CreateProduct(scene->GetTitle(), m_ctx->viewContext);
                scene->AddElement(product);
            }
        }
        m_builders.push_back(glBuilder);
        glBuilder->Build();

        // Tables
        auto tableBuilder = new REveTableProxyBuilder();
        tableBuilder->SetHaveAWindow(true);
        tableBuilder->SetCollection(collection);
        REveElement *tablep = tableBuilder->CreateProduct("table-type", m_ctx->viewContext);
        auto tableMng = m_ctx->viewContext->GetTableViewInfo();
        if (collection == m_collections->FirstChild())
        {
            tableMng->SetDisplayedCollection(collection->GetElementId());
        }

        for (auto &scene : ROOT::Experimental::gEve->GetScenes()->RefChildren())
        {
            if (strncmp(scene->GetCTitle(), "Table", 5) == 0)
            {
                scene->AddElement(tablep);
                // tableBuilder->Build(rdc, tablep, m_ctx->viewContext );
                break;
            }
        }
        tableMng->AddDelegate([=]()
                              { tableBuilder->ConfigChanged(); });
        m_builders.push_back(tableBuilder);

        // set tooltip expression for items

        auto tableEntries = tableMng->RefTableEntries(collection->GetItemClass()->GetName());
        int N = TMath::Min(int(tableEntries.size()), 3);
        for (int t = 0; t < N; t++)
        {
            auto te = tableEntries[t];
            collection->GetItemList()->AddTooltipExpression(te.fName, te.fExpression);
        }

        collection->setGLBuilder(glBuilder);

        collection->GetItemList()->SetItemsChangeDelegate([&](REveDataItemList *collection, const REveDataCollection::Ids_t &ids)
                                                          { this->ModelChanged(collection, ids); });
        collection->GetItemList()->SetFillImpliedSelectedDelegate([&](REveDataItemList *collection, REveElement::Set_t &impSelSet, const std::set<int> &sec_idcs)
                                                                  { this->FillImpliedSelected(collection, impSelSet, sec_idcs); });
    }

    void ModelChanged(REveDataItemList *itemList, const REveDataCollection::Ids_t &ids)
    {
        if (m_isEventLoading)
            return;

        for (auto proxy : m_builders)
        {
            if (proxy->Collection()->GetItemList() == itemList)
            {
                printf("Model changes check proxy %s: \n", proxy->Collection()->GetCName());
                proxy->ModelChanges(ids);
            }
        }
    }

    void FillImpliedSelected(REveDataItemList *itemList, REveElement::Set_t &impSelSet, const std::set<int> &sec_idcs)
    {

        for (auto proxy : m_builders)
        {
            if (proxy->Collection()->GetItemList() == itemList)
            {
                proxy->FillImpliedSelected(impSelSet, sec_idcs);
            }
        }
    }
};
#endif
