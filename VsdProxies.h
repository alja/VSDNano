#ifndef VSDProxies_h
#define VSDProxies_h

#include "VsdBase.h"
#include "lego_bins.h"
#include "FWDataCollection.h"

#include "TROOT.h"
#include "TFile.h"
#include "TH2.h"
#include "TFile.h"
#include "ROOT/REveDataCollection.hxx"
#include "ROOT/REveDataSimpleProxyBuilderTemplate.hxx"
#include "ROOT/REveManager.hxx"
#include "ROOT/REveScalableStraightLineSet.hxx"
#include "ROOT/REveViewContext.hxx"
#include <ROOT/REveGeoShape.hxx>
#include <ROOT/REveJetCone.hxx>
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
#include "ROOT/REveStraightLineSet.hxx"
#include "ROOT/REveTrans.hxx"
#include "ROOT/REveGeoShape.hxx"
#include "ROOT/REveBox.hxx"
#include "ROOT/REveCalo.hxx"
#include "ROOT/REveCaloData.hxx"
#include "ROOT/REveSelection.hxx"
#include "ROOT/REveVector.hxx"
#include "ROOT/REveEllipsoid.hxx"
#include <ROOT/RLogger.hxx>
#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "TGeoSphere.h"
#include "TMatrixDEigen.h"
#include "TMatrixDSym.h"

//====================================================================================

//====================================================================================

class HitProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdHit>
{
public:
   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdHit>::BuildItem;
   virtual void BuildItem(const VsdHit &iData, int iIndex, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *vc) override;
 
};

//====================================================================================

class SegmentProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdSegment>
{
public:
   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdSegment>::BuildItem;
   virtual void BuildItem(const VsdSegment &iData, int iIndex, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *vc) override;
};

//====================================================================================

class VertexProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdVertex>
{
public:
   using ROOT::Experimental::REveDataProxyBuilderBase::SetCollection;
   void SetCollection(ROOT::Experimental::REveDataCollection * collection) override
   {
      ROOT::Experimental::REveDataProxyBuilderBase::SetCollection(collection);
      auto fwc = dynamic_cast<FWDataCollection *>(collection);
      /*
      fwc->m_config.push_back({{"val", true}, {"type", "Bool"}, {"name", "DrawEllipse"}});
      fwc->m_config.push_back({{"val", 10}, {"type", "Long"}, {"name", "ScaleEllipse"}});
      fwc->m_config.push_back({{"val", 5}, {"type", "Long"}, {"name", "MarkerSize"}});
      */
      fwc->assertParamter({{"val", true}, {"type", "Bool"}, {"name", "DrawEllipse"}});
      fwc->assertParamter({{"val", true}, {"type", "Bool"}, {"name", "DrawEllipseSphere"}});
      fwc->assertParamter({{"val", 10}, {"type", "Long"}, {"name", "ScaleEllipse"}});
      fwc->assertParamter({{"val", 5}, {"type", "Long"}, {"name", "MarkerSize"}});
   }

   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdVertex>::BuildItem;
   virtual void BuildItem(const VsdVertex &iData, int iIndex, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *vc) override;
};

//====================================================================================

class METProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdMET>
{
public:
   virtual bool HaveSingleProduct() const override { return false; }

   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdMET>::BuildItemViewType;
   virtual void BuildItemViewType(const VsdMET &met, int /*idx*/, ROOT::Experimental::REveElement *iItemHolder,
                                  const std::string &viewType, const ROOT::Experimental::REveViewContext *context) override;
};

//====================================================================================

class CandidateProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdCandidate>
{
   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdCandidate>::BuildItem;
   void BuildItem(const VsdCandidate &el, int /*idx*/, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *context) override;
};

//====================================================================================

class MuonProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdMuon>
{
private:
   ROOT::Experimental::REveTrackPropagator *muonPropagator_g = nullptr;

public:
   void initMuonPropagator()
   {
      if (muonPropagator_g)
         return;
      // AMT this is ugly ... introduce a global contenxt
      muonPropagator_g = new ROOT::Experimental::REveTrackPropagator();
      muonPropagator_g->SetMagFieldObj(new ROOT::Experimental::REveMagFieldDuo(350, -3.5, 2.0));
      muonPropagator_g->SetMaxR(850);
      muonPropagator_g->SetMaxZ(1100);
      muonPropagator_g->SetMaxOrbs(6);
      muonPropagator_g->IncRefCount();
   }

   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdMuon>::BuildItem;
   void BuildItem(const VsdMuon &muon, int /*idx*/, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *context) override;
};

//====================================================================================

class JetProxyBuilder : public ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdJet>
{
   struct Cell
   {
      float thetaMin;
      float thetaMax;
      float phiMin;
      float phiMax;
   };

   void SetCollection(ROOT::Experimental::REveDataCollection * collection) override
   {
      ROOT::Experimental::REveDataProxyBuilderBase::SetCollection(collection);
      collection->SetLayer(-30);
   }
   bool HaveSingleProduct() const override { return false; }

   void makeBarrelCell(Cell &cellData, float &offset, float towerH, float *pnts)
   {
      using namespace TMath;

      float r1 = offset;
      float r2 = r1 + towerH * Sin(cellData.thetaMin);
      float z1In, z1Out, z2In, z2Out;

      z1In = r1 / Tan(cellData.thetaMax);
      z1Out = r2 / Tan(cellData.thetaMax);
      z2In = r1 / Tan(cellData.thetaMin);
      z2Out = r2 / Tan(cellData.thetaMin);

      float cos1 = Cos(cellData.phiMin);
      float sin1 = Sin(cellData.phiMin);
      float cos2 = Cos(cellData.phiMax);
      float sin2 = Sin(cellData.phiMax);

      // 0
      pnts[0] = r1 * cos2;
      pnts[1] = r1 * sin2;
      pnts[2] = z1In;
      pnts += 3;
      // 1
      pnts[0] = r1 * cos1;
      pnts[1] = r1 * sin1;
      pnts[2] = z1In;
      pnts += 3;
      // 2
      pnts[0] = r1 * cos1;
      pnts[1] = r1 * sin1;
      pnts[2] = z2In;
      pnts += 3;
      // 3
      pnts[0] = r1 * cos2;
      pnts[1] = r1 * sin2;
      pnts[2] = z2In;
      pnts += 3;
      //---------------------------------------------------
      // 4
      pnts[0] = r2 * cos2;
      pnts[1] = r2 * sin2;
      pnts[2] = z1Out;
      pnts += 3;
      // 5
      pnts[0] = r2 * cos1;
      pnts[1] = r2 * sin1;
      pnts[2] = z1Out;
      pnts += 3;
      // 6
      pnts[0] = r2 * cos1;
      pnts[1] = r2 * sin1;
      pnts[2] = z2Out;
      pnts += 3;
      // 7
      pnts[0] = r2 * cos2;
      pnts[1] = r2 * sin2;
      pnts[2] = z2Out;

      offset += towerH * Sin(cellData.thetaMin);

   } // end RenderBarrelCell

   void makeEndCapCell(Cell &cellData, float &offset, float towerH, float *pnts)
   {
      using namespace TMath;
      float z1, r1In, r1Out, z2, r2In, r2Out;

      z1 = offset;
      z2 = z1 + towerH;

      r1In = z1 * Tan(cellData.thetaMin);
      r2In = z2 * Tan(cellData.thetaMin);
      r1Out = z1 * Tan(cellData.thetaMax);
      r2Out = z2 * Tan(cellData.thetaMax);

      float cos2 = Cos(cellData.phiMin);
      float sin2 = Sin(cellData.phiMin);
      float cos1 = Cos(cellData.phiMax);
      float sin1 = Sin(cellData.phiMax);

      // 0
      pnts[0] = r1In * cos1;
      pnts[1] = r1In * sin1;
      pnts[2] = z1;
      pnts += 3;
      // 1
      pnts[0] = r1In * cos2;
      pnts[1] = r1In * sin2;
      pnts[2] = z1;
      pnts += 3;
      // 2
      pnts[0] = r2In * cos2;
      pnts[1] = r2In * sin2;
      pnts[2] = z2;
      pnts += 3;
      // 3
      pnts[0] = r2In * cos1;
      pnts[1] = r2In * sin1;
      pnts[2] = z2;
      pnts += 3;
      //---------------------------------------------------
      // 4
      pnts[0] = r1Out * cos1;
      pnts[1] = r1Out * sin1;
      pnts[2] = z1;
      pnts += 3;
      // 5
      pnts[0] = r1Out * cos2;
      pnts[1] = r1Out * sin2;
      pnts[2] = z1;
      pnts += 3;
      // 6
      pnts[0] = r2Out * cos2;
      pnts[1] = r2Out * sin2;
      pnts[2] = z2;
      pnts += 3;
      // 7
      pnts[0] = r2Out * cos1;
      pnts[1] = r2Out * sin1;
      pnts[2] = z2;

      if (z1 > 0)
         offset += towerH * Cos(cellData.thetaMin);
      else
         offset -= towerH * Cos(cellData.thetaMin);

   } // end makeEndCapCell

   using ROOT::Experimental::REveDataSimpleProxyBuilderTemplate<VsdJet>::BuildItemViewType;
   void BuildItemViewType(const VsdJet &dj, int idx, ROOT::Experimental::REveElement *iItemHolder,
                          const std::string &viewType, const ROOT::Experimental::REveViewContext *context) override;

   using ROOT::Experimental::REveDataProxyBuilderBase::LocalModelChanges;
   void LocalModelChanges(int idx, ROOT::Experimental::REveElement *el, const ROOT::Experimental::REveViewContext *ctx) override
   {
      printf("LocalModelChanges jet %s ( %s )\n", el->GetCName(), el->FirstChild()->GetCName());
      ROOT::Experimental::REveJetCone *cone = dynamic_cast<ROOT::Experimental::REveJetCone *>(el->FirstChild());
      cone->SetLineColor(cone->GetMainColor());
   }
};

//==============================================================================
class REveCaloTowerSliceSelector : public ROOT::Experimental::REveCaloDataSliceSelector
{
private:
   ROOT::Experimental::REveDataCollection* fCollection{nullptr};
   ROOT::Experimental::REveCaloDataHist*   fCaloData{nullptr};

public:
   REveCaloTowerSliceSelector(int s, ROOT::Experimental::REveDataCollection* c, ROOT::Experimental::REveCaloDataHist* h):ROOT::Experimental::REveCaloDataSliceSelector(s), fCollection(c), fCaloData(h) {}

   using ROOT::Experimental::REveCaloDataSliceSelector::ProcessSelection;
   void ProcessSelection(ROOT::Experimental::REveCaloData::vCellId_t& sel_cells, UInt_t selectionId, Bool_t multi) override
   {
      std::set<int> item_set;
      ROOT::Experimental::REveCaloData::CellData_t cd;
      for (auto &cellId : sel_cells)
      {
         fCaloData->GetCellData(cellId, cd);

         // loop over enire collection and check its eta/phi range
         for (int t = 0; t < fCollection->GetNItems(); ++t)
         {
            VsdCandidate* tower = (VsdCandidate*) fCollection->GetDataPtr(t);
            if (tower->m_eta > cd.fEtaMin && tower->m_eta < cd.fEtaMax &&
                tower->m_phi > cd.fPhiMin && tower->m_phi < cd.fPhiMax)
                {
                  printf("selected item %d ...\n", t);
               item_set.insert(t);
                }
         }
      }
      ROOT::Experimental::REveSelection* sel = (ROOT::Experimental::REveSelection*)ROOT::Experimental::gEve->FindElementById(selectionId);
      fCollection->GetItemList()->RefSelectedSet() = item_set;
      sel->NewElementPicked(fCollection->GetItemList()->GetElementId(),  multi, true, item_set);
   }

   using ROOT::Experimental::REveCaloDataSliceSelector::GetCellsFromSecondaryIndices;
   void GetCellsFromSecondaryIndices(const std::set<int>& idcs, ROOT::Experimental::REveCaloData::vCellId_t& out) override
   {
      TH2F* hist  =  fCaloData->GetHist(GetSliceIndex());
      std::set<int> cbins;
      float total = 0;
      for( auto &i : idcs ) {
         VsdCandidate* tower = (VsdCandidate*)fCollection->GetDataPtr(i);
         int bin = hist->FindBin(tower->m_eta, tower->m_phi);
         float frac =  tower->m_pt/hist->GetBinContent(bin);
         bool ex = false;
         for (size_t ci = 0; ci < out.size(); ++ci)
         {
            if (out[ci].fTower == bin && out[ci].fSlice == GetSliceIndex())
            {
               float oldv =  out[ci].fFraction;
               out[ci].fFraction = oldv + frac;
               ex = true;
               break;
            }
         }
         if (!ex) {
            out.push_back(ROOT::Experimental::REveCaloData::CellId_t(bin, GetSliceIndex(), frac));
         }
      }
   }
};

//==============================================================================

class CaloTowerProxyBuilder: public ROOT::Experimental::REveDataProxyBuilderBase
{
private:
   ROOT::Experimental::REveCaloDataHist* fCaloData {nullptr};
   TH2F*             fHist {nullptr};
   int               fSliceIndex {-1};

   void assertSlice() {
      if (!fHist) {
         Bool_t status = TH1::AddDirectoryStatus();

         TH1::AddDirectory(kFALSE);  //Keeps histogram from going into memory
         fHist = new TH2F("caloHist", "caloHist", fw3dlego::xbins_n - 1, fw3dlego::xbins, 72, -M_PI, M_PI);
         TH1::AddDirectory(status);
         fSliceIndex = fCaloData->AddHistogram(fHist);

         fCaloData->RefSliceInfo(fSliceIndex)
            .Setup(Collection()->GetCName(),
                   0.,
                   Collection()->GetMainColor(),
                   Collection()->GetMainTransparency());

         fCaloData->GetSelector()->AddSliceSelector(std::unique_ptr<ROOT::Experimental::REveCaloDataSliceSelector>
                                                    (new REveCaloTowerSliceSelector(fSliceIndex, Collection(), fCaloData)));
      }
   }
public:
   CaloTowerProxyBuilder(ROOT::Experimental::REveCaloDataHist* cd) : fCaloData(cd) {}

   using ROOT::Experimental::REveDataProxyBuilderBase::Build;
   void BuildProduct(const ROOT::Experimental::REveDataCollection* collection, ROOT::Experimental::REveElement* product, const ROOT::Experimental::REveViewContext*)override
   {
      assertSlice();
      fHist->Reset();
      if (collection->GetRnrSelf())
      {
         fCaloData->RefSliceInfo(fSliceIndex)
            .Setup(Collection()->GetCName(),
                   0.,
                   Collection()->GetMainColor(),
                   Collection()->GetMainTransparency());


         for (int h = 0; h < collection->GetNItems(); ++h)
         {
            VsdCandidate* tower = (VsdCandidate*)collection->GetDataPtr(h);
            const ROOT::Experimental::REveDataItem* item = Collection()->GetDataItem(h);

            if (!item->GetVisible())
               continue;
            fHist->Fill(tower->m_eta, tower->m_phi, tower->m_pt);
         }
      }
      fCaloData->DataChanged();
   }

   using ROOT::Experimental::REveDataProxyBuilderBase::FillImpliedSelected;
   void FillImpliedSelected(ROOT::Experimental::REveElement::Set_t& impSet, const std::set<int>& sec_idcs, Product*) override
   {
      fCaloData->GetSelector()->SetActiveSlice(fSliceIndex);
      impSet.insert(fCaloData);
      fCaloData->FillImpliedSelectedSet(impSet, sec_idcs);
   }

  using ROOT::Experimental::REveDataProxyBuilderBase::ModelChanges;
   void ModelChanges(const ROOT::Experimental::REveDataCollection::Ids_t& ids, Product* product) override
   {
      BuildProduct(Collection(), nullptr, nullptr);
   }

}; // CaloTowerProxyBuilder
//.....................................................
//==============================================================================
#endif