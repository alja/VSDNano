#ifndef VSDInvMassDialog_h
#define VSDInvMassDialog_h

#include "ROOT/REveElement.hxx"
#include "ROOT/REveManager.hxx"
#include "ROOT/REveScene.hxx"
#include "TClass.h"
#include "VsdBase.h"

using namespace ROOT::Experimental;

class FWVsdInvMassDialog : public REveElement
{
   public:
   void Calculate()
   {
      fTitle = "<pre>";
      printf("FWWebInvMassDialog::calculate() .... \n");
      double sum_len = 0;
      double sum_len_xy = 0;
      int n = 0;
      REveVector first, second, sum;
      addLine("");
      addLine("--------------------------------------------------+--------------");
      addLine("       px          py          pz          pT     | Collection");
      addLine("--------------------------------------------------+--------------");

      TClass *rc_class = TClass::GetClass(typeid(VsdCandidate));
      auto s = ROOT::Experimental::gEve->GetScenes()->FindChild("Collections");
      for (auto &c : s->RefChildren())
      {
         REveDataCollection *coll = (REveDataCollection *)(c);
         auto items = coll->GetItemList();
         for (auto &au : items->RefAunts())
         {
            if (au == (REveAunt*)ROOT::Experimental::gEve->GetSelection())
            {
                std::cout << c->GetName() << " " << items->GetImpliedSelected() << " --- " << items->RefSelectedSet().size() << "\n";
                for (auto &ss : items->RefSelectedSet())
                {
                    TString line;
                    TClass *model_class = coll->GetItemClass();
                    // std::cout << "item with type " << model_class->GetName() << "\n";
                    void *model_data = const_cast<void *>(coll->GetDataPtr(ss));
                    REveVector v;

                    VsdCandidate *rc = reinterpret_cast<VsdCandidate *>(model_class->DynamicCast(rc_class, model_data));

                    float theta = EtaToTheta(rc->eta());
                    float pz = rc->pt() / TMath::Tan(theta);
                    float px = rc->pt() * TMath::Cos(rc->phi());
                    float py = rc->pt() * TMath::Sin(rc->phi());

                    if (rc != nullptr)
                    {
                        v.Set(px, py, pz);
                        rc->dump();

                        sum += v;
                        sum_len += TMath::Sqrt(v.Mag2());
                        sum_len_xy += TMath::Sqrt(v.Perp2());

                        line = TString::Format("  %+10.3f  %+10.3f  %+10.3f  %10.3f", v.fX, v.fY, v.fZ, TMath::Sqrt(v.Perp2()));
                    }
                    else
                    {
                        line = TString::Format("  -------- not a VsdCandidate --------");
                    }
                    line += TString::Format("  | %s[%d]", coll->GetCName(), ss);

                    addLine(line);

                    if (n == 0)
                        first = v;
                    else if (n == 1)
                        second = v;
                }

                break;
            }
         }
      }

      addLine("--------------------------------------------------+--------------");
      addLine(TString::Format(
          "  %+10.3f  %+10.3f  %+10.3f  %10.3f  | Sum", sum.fX, sum.fY, sum.fZ, TMath::Sqrt(sum.Perp2())));
      addLine("");
      addLine(TString::Format("m  = %10.3f", TMath::Sqrt(TMath::Max(0.0, sum_len * sum_len - sum.Mag2()))));
      addLine(TString::Format("mT = %10.3f", TMath::Sqrt(TMath::Max(0.0, sum_len_xy * sum_len_xy - sum.Perp2()))));
      addLine(TString::Format("HT = %10.3f", sum_len_xy));

      if (n == 2)
      {
        // addLine(TString::Format("deltaPhi  = %+6.4f", deltaPhi(first.Phi(), second.Phi()))); //AMT deltaPhi exisiting only in data formats
         addLine(TString::Format("deltaEta  = %+6.4f", first.Eta() - second.Eta()));
        // addLine(TString::Format("deltaR    = % 6.4f", deltaR(first.Eta(), first.Phi(), second.Eta(), second.Phi())));
      }

      fTitle += "</pre>";
      StampObjProps();
   }

   void
   addLine(const TString &line)
   {
      fTitle += "\n";
      fTitle += line.Data();
   }
   virtual int WriteCoreJson(nlohmann::json &j, int rnr_offset) override
   {
     int ret = REveElement::WriteCoreJson(j, rnr_offset);
   
      std::cout << j.dump(4);
      j["UT_PostStream"] = "UT_refresh_invmass_dialog";
      return ret;
   }
float EtaToTheta(float eta)
{
   using namespace TMath;

   if (eta < 0)
      return Pi() - 2 * ATan(Exp(-Abs(eta)));
   else
      return 2 * ATan(Exp(-Abs(eta)));
}
};

#endif