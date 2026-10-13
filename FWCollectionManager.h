#ifndef FWVCollectionMng_h
#define FWVCollectionMng_h

#include "VsdProxies.h"

#include "FWProvider.h"

#include "FWDataCollection.h"
#include "ROOT/REveDataCollection.hxx"
#include "ROOT/REveDataSimpleProxyBuilderTemplate.hxx"
#include "ROOT/REveManager.hxx"

#include "ROOT/REveCaloData.hxx"
#include "ROOT/REveCalo.hxx"
#include "ROOT/REveProjectionAxis.hxx"



//==============================================================================
//== Collection Manager =============================================================
//==============================================================================
class CollectionManager
{
private:
    FWProvider *m_event{nullptr};
    ROOT::Experimental::REveElement *m_collections{nullptr};



    std::vector<ROOT::Experimental::REveDataProxyBuilderBase *> m_builders;

    bool m_isEventLoading{false}; // don't process model changes when applying filter on new event
public:
    ROOT::Experimental::REveProjectionManager* m_mngRhoZ;
    ROOT::Experimental::REveProjectionManager* m_mngRhoZGeo;
    ROOT::Experimental::REveProjectionManager* m_mngRPhi;
    // Scales of the projected views; their ticks must be rebuilt when a projection changes.
    ROOT::Experimental::REveProjectionAxis* m_axisRPhi{nullptr};
    ROOT::Experimental::REveProjectionAxis* m_axisRhoZ{nullptr};
    ROOT::Experimental::REveViewContext* m_viewContext;
    ROOT::Experimental::REveCaloDataHist* m_caloData;

    CollectionManager(FWProvider *event) : m_event(event)
    {
        m_collections = ROOT::Experimental::gEve->GetScenes()->FindChild("Collections");
    }

    void LoadCurrentEventInCollection(ROOT::Experimental::REveDataCollection *rdc)
    {
        m_isEventLoading = true;
        rdc->ClearItems();
        std::vector<void *> items;
        m_event->GetItems(rdc->GetName(), items);
        // printf("-------- LoadCurrentEventInCollection %s size %lu\n", rdc->GetCName(), items.size());
        std::string t = "dummy";
        for (auto item : items)
        {
            rdc->AddItem(item, t, t);
        }
        rdc->ApplyFilter();
        rdc->GetItemList()->StampObjProps();

        m_isEventLoading = false;
    }

    void RenewEvent()
    {
        for (auto &el : m_collections->RefChildren())
        {
            auto c = dynamic_cast<ROOT::Experimental::REveDataCollection *>(el);
            LoadCurrentEventInCollection(c);
        }

        for (auto proxy : m_builders)
        {
            proxy->Build();
        }
        m_caloData->DataChanged();
    }

    ROOT::Experimental::REveDataProxyBuilderBase *getProxyBuilder(const FWCollectionSpec &spec)
    {
        if (spec.m_proxyBuilder.empty())
            return nullptr;

        // AMT: this is not necessary if ROOT::Experimental::REveCaloData is in globalscope
        if (spec.m_proxyBuilder == "CaloTowerProxyBuilder")
            return new CaloTowerProxyBuilder(m_caloData);

        TClass* pbc = TClass::GetClass(spec.m_proxyBuilder.c_str());
        if (pbc)
           return (ROOT::Experimental::REveDataProxyBuilderBase*)pbc->New();

        printf("can't find proxy builder %s for collection %s\n", spec.m_proxyBuilder.c_str(), spec.m_name.c_str());
        return nullptr;
    }

    void
    addCollection(const FWCollectionSpec &spec)
    {
        FWDataCollection *collection = new FWDataCollection(spec.m_name, spec.m_varConfig);
        m_collections->AddElement(collection);

        TClass* tc  = TClass::GetClass(spec.m_itemClass.c_str());
        if (!tc)
            throw( std::runtime_error("addCollection " +  spec.m_name + ": no class " + spec.m_itemClass) );

        collection->SetItemClass(tc);
        collection->SetMainColor(spec.m_color);
        collection->SetFilterExpr(spec.m_filter.c_str());

        ROOT::Experimental::REveDataProxyBuilderBase *glBuilder = getProxyBuilder(spec);

        // load data
        LoadCurrentEventInCollection(collection);
        if (glBuilder)
        {
            glBuilder->SetCollection(collection);
            glBuilder->SetHaveAWindow(true);

            for (auto &scene : ROOT::Experimental::gEve->GetScenes()->RefChildren())
            {

                // ROOT::Experimental::ROOT::Experimental::REveElement *product = glBuilder->CreateProduct(scene->GetTitle(), m_viewContext);

                if (strncmp(scene->GetCName(), "Table", 5) == 0)
                    continue;
                if (!strncmp(scene->GetCTitle(), "RhoZProjected", 8))
                {
                    ROOT::Experimental::REveElement *product = glBuilder->CreateProduct("RhoZViewType", m_viewContext);
                    m_mngRhoZ->ImportElements(product, scene);
                    continue;
                }
                if (!strncmp(scene->GetCTitle(), "RPhiProjected", 8))
                {
                    ROOT::Experimental::REveElement *product = glBuilder->CreateProduct("RPhiViewType", m_viewContext);
                    m_mngRPhi->ImportElements(product, scene);
                    continue;
                }
                else if ((!strncmp(scene->GetCName(), "Event scene", 8)))
                {
                    ROOT::Experimental::REveElement *product = glBuilder->CreateProduct(scene->GetTitle(), m_viewContext);
                    scene->AddElement(product);
                }
            }
            m_builders.push_back(glBuilder);
            glBuilder->Build();
        }

        // Tables
        auto tableBuilder = new ROOT::Experimental::REveTableProxyBuilder();
        tableBuilder->SetHaveAWindow(true);
        tableBuilder->SetCollection(collection);
        ROOT::Experimental::REveElement *tablep = tableBuilder->CreateProduct("table-type", m_viewContext);
        auto tableMng = m_viewContext->GetTableViewInfo();
        if (collection == m_collections->FirstChild())
        {
            tableMng->SetDisplayedCollection(collection->GetElementId());
        }

        for (auto &scene : ROOT::Experimental::gEve->GetScenes()->RefChildren())
        {
            if (strncmp(scene->GetCTitle(), "Table", 5) == 0)
            {
                scene->AddElement(tablep);
                // tableBuilder->Build(rdc, tablep, m_viewContext );
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

        collection->GetItemList()->SetItemsChangeDelegate([&](ROOT::Experimental::REveDataItemList *collection, const ROOT::Experimental::REveDataCollection::Ids_t &ids)
                                                          { this->ModelChanged(collection, ids); });
        collection->GetItemList()->SetFillImpliedSelectedDelegate([&](ROOT::Experimental::REveDataItemList *collection, ROOT::Experimental::REveElement::Set_t &impSelSet, const std::set<int> &sec_idcs)
                                                                  { this->FillImpliedSelected(collection, impSelSet, sec_idcs); });
    }

    void ModelChanged(ROOT::Experimental::REveDataItemList *itemList, const ROOT::Experimental::REveDataCollection::Ids_t &ids)
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

    void FillImpliedSelected(ROOT::Experimental::REveDataItemList *itemList, ROOT::Experimental::REveElement::Set_t &impSelSet, const std::set<int> &sec_idcs)
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