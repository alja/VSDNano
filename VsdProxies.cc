#include "VsdProxies.h"



ROOT::Experimental::REveGeoShape *getShape(const char *name,
                       TGeoBBox *shape,
                       Color_t color)
{
   ROOT::Experimental::REveGeoShape *egs = new ROOT::Experimental::REveGeoShape(name);
   TColor *c = gROOT->GetColor(color);
   Float_t rgba[4] = {1, 0, 0, 1};
   if (c)
   {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   egs->SetMainColorRGB(rgba[0], rgba[1], rgba[2]);
   egs->SetShape(shape);
   return egs;
}

float EtaToTheta(float eta)
{
   using namespace TMath;

   if (eta < 0)
      return Pi() - 2 * ATan(Exp(-Abs(eta)));
   else
      return 2 * ATan(Exp(-Abs(eta)));
}

void addRhoZEnergyProjection(ROOT::Experimental::REveDataProxyBuilderBase *pb, ROOT::Experimental::REveElement *container,
                             double r_ecal, double z_ecal,
                             double theta_min, double theta_max,
                             double phi)
{
   ROOT::Experimental::REveGeoManagerHolder gmgr(ROOT::Experimental::REveGeoShape::GetGeoManager());
   double z1 = r_ecal / tan(theta_min);
   if (z1 > z_ecal)
      z1 = z_ecal;
   if (z1 < -z_ecal)
      z1 = -z_ecal;
   double z2 = r_ecal / tan(theta_max);
   if (z2 > z_ecal)
      z2 = z_ecal;
   if (z2 < -z_ecal)
      z2 = -z_ecal;
   double r1 = z_ecal * fabs(tan(theta_min));
   if (r1 > r_ecal)
      r1 = r_ecal;
   if (phi < 0)
      r1 = -r1;
   double r2 = z_ecal * fabs(tan(theta_max));
   if (r2 > r_ecal)
      r2 = r_ecal;
   if (phi < 0)
      r2 = -r2;

   if (fabs(r2 - r1) > 1)
   {
      TGeoBBox *sc_box = new TGeoBBox(0., fabs(r2 - r1) / 2, 1);
      ROOT::Experimental::REveGeoShape *element = new ROOT::Experimental::REveGeoShape("r-segment");
      element->SetShape(sc_box);
      ROOT::Experimental::REveTrans &t = element->RefMainTrans();
      t(1, 4) = 0;
      t(2, 4) = (r2 + r1) / 2;
      t(3, 4) = fabs(z2) > fabs(z1) ? z2 : z1;
      pb->SetupAddElement(element, container);
   }
   if (fabs(z2 - z1) > 1)
   {
      TGeoBBox *sc_box = new TGeoBBox(0., 1, (z2 - z1) / 2);
      ROOT::Experimental::REveGeoShape *element = new ROOT::Experimental::REveGeoShape("z-segment");
      element->SetShape(sc_box);
      ROOT::Experimental::REveTrans &t = element->RefMainTrans();
      t(1, 4) = 0;
      t(2, 4) = fabs(r2) > fabs(r1) ? r2 : r1;
      t(3, 4) = (z2 + z1) / 2;
      pb->SetupAddElement(element, container);
   }
};

void HitProxyBuilder::BuildItem(const VsdHit &iData, int iIndex, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *vc)
{
    auto ps = new ROOT::Experimental::REvePointSet("Hit Point");
    // Set point at x, y, z defined in VsdHit
    ps->SetNextPoint(iData.x(), iData.y(), iData.z());

    // Visual styling
    ps->SetMarkerStyle(4);
    ps->SetMarkerSize(4);
    ps->SetMainColor(Collection()->GetMainColor());

    SetupAddElement(ps, iItemHolder);
}

void SegmentProxyBuilder::BuildItem(const VsdSegment &iData, int iIndex, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *vc)
{
    auto ls = new ROOT::Experimental::REveStraightLineSet("Segment Line");

    // start point (x, y, z)
    float x0 = iData.posX();
    float y0 = iData.posY();
    float z0 = iData.posZ();

    // end point: we project the slopes tx and ty over a small delta-Z
    // Adjust 'length' based on your detector geometry (e.g., 5.0 cm)
    float length = 5.0f;
    float x1 = x0 + iData.tx() * length;
    float y1 = y0 + iData.ty() * length;
    float z1 = z0 + length;

    ls->AddLine(x0, y0, z0, x1, y1, z1);
    ls->SetLineWidth(2);
    ls->SetMainColor(Collection()->GetMainColor());

    SetupAddElement(ls, iItemHolder);
}

void VertexProxyBuilder::BuildItem(const VsdVertex &iData, int iIndex, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *vc) 
   {
      auto item = dynamic_cast<FWDataCollection *>(Collection());
      long markerSize = item->getLongParameter("MarkerSize");
      bool drawEllipse = item->getBoolParameter("DrawEllipse");

      // vertex position
      //
      auto ps = new ROOT::Experimental::REvePointSet("vertex pnt");
      ps->SetMainColor(kGreen + 10);
      ps->SetNextPoint(iData.x(), iData.y(), iData.z());
      ps->SetMarkerStyle(4);
      ps->SetMarkerSize(markerSize);
      SetupAddElement(ps, iItemHolder );

      if (drawEllipse)
      {
         TMatrixDSym symMtx(3);
         for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
               // printf("Read error [%d,%d] %g\n", i, j, iData.m_error[i][j]);
               symMtx(i, j) = iData.m_error[i][j];
            }
         // symMtx.Print();

         TMatrixDEigen mtx(symMtx);

         TVectorD eigValsVec(mtx.GetEigenValues());
         if (eigValsVec.Min() <= 0)
         {
            if (eigValsVec.Min() < 0)
               R__LOG_TO_CHANNEL(ROOT::ELogLevel::kError, ROOT::Experimental::REveLog()) << "Negative error matrix " << item->GetName() << " idx = " << iIndex << "\n";
            printf("Non-positive eigenvalue for collection %s idx %d, skipping error ellipsoid\n", item->GetCName(), iIndex);
         }
         else
         {
            eigValsVec = eigValsVec.Sqrt();

            TMatrixD vecEig = mtx.GetEigenVectors();
            // vecEig.Print();

            long scale = item->getLongParameter("ScaleEllipse");
            ROOT::Experimental::REveVector v[3];
            for (int i = 0; i < 3; ++i)
            {
               v[i].Set(vecEig(0, i), vecEig(1, i), vecEig(2, i));
               v[i] *= eigValsVec(i) * scale;
               // v[i].Dump();
            }
            ROOT::Experimental::REveEllipsoid *ell = new ROOT::Experimental::REveEllipsoid("VertexError");
            ell->RefMainTrans().SetPos(iData.x(), iData.y(), iData.z());
            ell->SetLineWidth(2);
            ell->SetBaseVectors(v[0], v[1], v[2]);
            ell->Outline();
            ell->SetMainTransparency(90);
            SetupAddElement(ell, iItemHolder);

            // add TGeoSphere
            if (item->getBoolParameter("DrawEllipseSphere"))
            {
               auto sph = new ROOT::Experimental::REveGeoShape("Sphere");
               sph->SetShape(new TGeoSphere(0.98f, 1.0f));
               sph->SetMainTransparency(95);
               sph->SetMainColor(iItemHolder->GetMainColor());
               sph->SetNSegments(80);

               float m0 = v[0].Mag();
               v[0].Normalize();
               float m1 = v[1].Mag();
               v[1].Normalize();
               float m2 = v[2].Mag();
               v[2].Normalize();

               sph->InitMainTrans();
               sph->RefMainTrans().SetBaseVec(1, v[0].fX, v[0].fY, v[0].fZ);
               sph->RefMainTrans().SetBaseVec(2, v[1].fX, v[1].fY, v[1].fZ);
               sph->RefMainTrans().SetBaseVec(3, v[2].fX, v[2].fY, v[2].fZ);
               sph->RefMainTrans().SetScale(m0, m1, m2);
               sph->RefMainTrans().SetPos(iData.x(), iData.y(), iData.z());
               // last parameter is false to keep the transparency
               SetupAddElement(sph, iItemHolder, false);
            }

         }
      }

/*
      // tracks
      if (item->getConfig()->value<bool>("Draw Tracks"))
      {
         for(reco::Vertex::trackRef_iterator it = iData.tracks_begin() ;
             it != iData.tracks_end()  ; ++it)
         {
            float w = iData.trackWeight(*it);
            if (w < 0.5) continue;
      
            const reco::Track & track = *it->get();
            REveRecTrack t;
            t.fBeta = 1.;
            t.fV = REveVector(track.vx(), track.vy(), track.vz());
            t.fP = REveVector(track.px(), track.py(), track.pz());
            t.fSign = track.charge();
            REveTrack* trk = new REveTrack(&t, context->getTrackPropagator());
            trk->MakeTrack(); 
            SetupAddElement(trk, iItemHolder);
         }
      }
      if (item->getConfig()->value<bool>("Draw Pseudo Track")) {
         REveRecTrack t;
         t.fBeta = 1.;
         t.fV = REveVector(iData.x(), iData.y(), iData.z());
         t.fP = REveVector(-iData.p4().px(), -iData.p4().py(), -iData.p4().pz());
         t.fSign = 1;
         REveTrack* trk = new REveTrack(&t, context->getTrackPropagator());
         trk->SetLineStyle(7);
         trk->MakeTrack();
         SetupAddElement(trk, iItemHolder);
      }
   */
   }


   void METProxyBuilder::BuildItemViewType(const VsdMET &met, int /*idx*/, ROOT::Experimental::REveElement *iItemHolder,
                                  const std::string &viewType, const ROOT::Experimental::REveViewContext *context) 
   {
      using namespace TMath;
      double phi = met.phi();
      double theta = EtaToTheta(met.eta());
      //double theta = 0.f;
      double size = 1.f;

      ROOT::Experimental::REveScalableStraightLineSet *marker = new ROOT::Experimental::REveScalableStraightLineSet("MET marker");
      marker->SetLineWidth(2);
      marker->SetAlwaysSecSelect(false);

      SetupAddElement(marker, iItemHolder);

      float offr = 5;
      float r_ecal = context->GetMaxR() + offr;
      float z_ecal = context->GetMaxZ() + offr;
      float energyScale = 5.f;

      if (viewType.compare(0, 3, "Rho") == 0)
      {
         // body
         double r0;
         if (TMath::Abs(met.eta()) < abs(atan(r_ecal / z_ecal)))
         {
            r0 = r_ecal / sin(theta);
         }
         else
         {
            r0 = z_ecal / fabs(cos(theta));
         }
         marker->SetScaleCenter(0., Sign(r0 * sin(theta), phi), r0 * cos(theta));
         double r1 = r0 + 1;
         marker->AddLine(0., Sign(r0 * sin(theta), phi), r0 * cos(theta),
                         0., Sign(r1 * sin(theta), phi), r1 * cos(theta));
         // arrow pointer
         double r2 = r1 - 0.1;
         double dy = 0.05 * size;
         marker->AddLine(0., Sign(r2 * sin(theta) + dy * cos(theta), phi), r2 * cos(theta) - dy * sin(theta),
                         0., Sign(r1 * sin(theta), phi), r1 * cos(theta));
         dy = -dy;
         marker->AddLine(0., Sign(r2 * sin(theta) + dy * cos(theta), phi), r2 * cos(theta) - dy * sin(theta),
                         0., Sign(r1 * sin(theta), phi), r1 * cos(theta));

         // segment
         addRhoZEnergyProjection(this, iItemHolder, r_ecal - 1, z_ecal - 1,
                                 theta - 0.04, theta + 0.04,
                                 phi);
      }
      else
      {
         // body
         double r0 = r_ecal;
         double r1 = r0 + 1;
         marker->SetScaleCenter(r0 * cos(phi), r0 * sin(phi), 0);
         marker->AddLine(r0 * cos(phi), r0 * sin(phi), 0,
                         r1 * cos(phi), r1 * sin(phi), 0);

         // arrow pointer, xy  rotate offset point ..
         double r2 = r1 - 0.1;
         double dy = 0.05 * size;

         marker->AddLine(r2 * cos(phi) - dy * sin(phi), r2 * sin(phi) + dy * cos(phi), 0,
                         r1 * cos(phi), r1 * sin(phi), 0);
         dy = -dy;
         marker->AddLine(r2 * cos(phi) - dy * sin(phi), r2 * sin(phi) + dy * cos(phi), 0,
                         r1 * cos(phi), r1 * sin(phi), 0);

         // segment
         double min_phi = phi - M_PI / 36 / 2;
         double max_phi = phi + M_PI / 36 / 2;
         ROOT::Experimental::REveGeoManagerHolder gmgr(ROOT::Experimental::REveGeoShape::GetGeoManager());
         ROOT::Experimental::REveGeoShape *element = getShape("spread", new TGeoTubeSeg(r0 - 2, r0, 1, min_phi * 180 / M_PI, max_phi * 180 / M_PI), 0);
         element->SetPickable(kTRUE);
         element->SetMainTransparency(90);
         SetupAddElement(element, iItemHolder);
      }
      // float value = met.et();
      float value = met.pt();
      marker->SetScale(energyScale * value);
   }



   void CandidateProxyBuilder::BuildItem(const VsdCandidate &el, int /*idx*/, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *context) 
   {
      VsdCandidate &cand = (VsdCandidate &)(el); // amt need a const
      // int pdg = 11 * cand.charge();

      float theta = EtaToTheta(cand.eta());
      float pz = cand.pt() / TMath::Tan(theta);
      float px = cand.pt() * TMath::Cos(cand.phi());
      float py = cand.pt() * TMath::Sin(cand.phi());

      ROOT::Experimental::REveRecTrack t;
      t.fBeta = 1.;
      t.fV = ROOT::Experimental::REveVector(cand.posX(), cand.posY(), cand.posZ()); // iData.vx(), iData.vy(), iData.vz());
      t.fP = ROOT::Experimental::REveVector(px, py, pz);
      t.fSign = cand.m_charge;
      ROOT::Experimental::REveTrack *track = new ROOT::Experimental::REveTrack(&t, context->GetPropagator());

      // printf("==============  BUILD track %s (pt=%f, eta=%f) \n", iItemHolder->GetCName(), p.Pt(), p.Eta());
      // auto track = new REveTrack((TParticle *)(x), 1, context->GetPropagator());
      track->MakeTrack();
      SetupAddElement(track, iItemHolder, true);
      // track->SetName(Form("element %s id=%d", iItemHolder->GetCName(), track->GetElementId()));
   }


   void MuonProxyBuilder::BuildItem(const VsdMuon &muon, int /*idx*/, ROOT::Experimental::REveElement *iItemHolder, const ROOT::Experimental::REveViewContext *context) 
   {
      initMuonPropagator();
      float theta = EtaToTheta(muon.eta());
      float pz = muon.pt() / TMath::Tan(theta);
      float px = muon.pt() * TMath::Cos(muon.phi());
      float py = muon.pt() * TMath::Sin(muon.phi());

      ROOT::Experimental::REveRecTrack t;
      t.fBeta = 1.;
      t.fV = ROOT::Experimental::REveVector(muon.posX(), muon.posY(), muon.posZ());
      t.fP = ROOT::Experimental::REveVector(px, py, pz);
      t.fSign = muon.m_charge;

      auto track = new ROOT::Experimental::REveTrack(&t, muonPropagator_g);
      track->MakeTrack();
      track->SetLineWidth(2);

      SetupAddElement(track, iItemHolder, true);
      // track->SetName(Form("element %s id=%d", iItemHolder->GetCName(), track->GetElementId()));
   }


   void JetProxyBuilder::BuildItemViewType(const VsdJet &dj, int idx, ROOT::Experimental::REveElement *iItemHolder,
                          const std::string &viewType, const ROOT::Experimental::REveViewContext *context) 
   {
      auto jet = new ROOT::Experimental::REveJetCone();
      jet->SetCylinder(context->GetMaxR() - 5, context->GetMaxZ());
      jet->AddEllipticCone(dj.eta(), dj.phi(), dj.coneR(), dj.coneR());
      SetupAddElement(jet, iItemHolder, true);
      jet->SetTitle(Form("jet [%d] pt = %f\n", idx, dj.pt()));
      // printf("make jet %d pt == %f\n", idx, dj.pt());
      //printf("make xxx jet %d pt == %f\n", idx, xxx->pt());
      ROOT::Experimental::REveVector av(dj.posX(), dj.posY(), dj.posZ());
      jet->SetApex(av);

      static const float_t offr = 5;
      float r_ecal = context->GetMaxR() + offr;
      float z_ecal = context->GetMaxZ() + offr;
      float energyScale = 5.f;
      float transAngle = abs(atan(r_ecal / z_ecal));
      double theta = EtaToTheta(dj.eta());
      double phi = dj.phi();

      Cell cell;
      // hardcoded cell size
      float ad = 0.02;
      // thetaMin => etaMax, thetaMax => thetaMin
      cell.thetaMax = EtaToTheta(dj.eta() - ad);
      cell.thetaMin = EtaToTheta(dj.eta() + ad);
      cell.phiMin = phi - ad;
      cell.phiMax = phi + ad;
      float pnts[24];

      // an example of slices
      std::vector<float> sliceVals;
      sliceVals.push_back(dj.pt() * (1 - dj.hadFraction()));
      sliceVals.push_back(dj.pt() * dj.hadFraction());

      if (theta < transAngle || (3.14 - theta) < transAngle)
      {
         float offset = TMath::Sign(context->GetMaxZ(), dj.eta());
         for (auto &val : sliceVals)
         {
            offset += TMath::Sign(offr, dj.eta());
            makeEndCapCell(cell, offset, TMath::Sign(val * energyScale, dj.eta()), &pnts[0]);
            ROOT::Experimental::REveBox *reveBox = new ROOT::Experimental::REveBox();
            reveBox->SetVertices(pnts);
            SetupAddElement(reveBox, iItemHolder, true);
         }
      }
      else
      {
         float offset = context->GetMaxR();
         for (auto &val : sliceVals)
         {
            offset += offr;
            makeBarrelCell(cell, offset, val * energyScale, &pnts[0]);
            auto reveBox = new ROOT::Experimental::REveBox();
            reveBox->SetVertices(pnts);
            SetupAddElement(reveBox, iItemHolder, true);
            reveBox->SetTitle(Form("jet %d", idx)); // amt this is workaround and should be unnecessary
         }
      }
   }