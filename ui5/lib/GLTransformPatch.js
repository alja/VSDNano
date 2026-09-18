sap.ui.define([
   'sap/ui/core/mvc/Controller',
   'rootui5/eve7/controller/GL.controller'
], function (Controller, GLController) {
   "use strict";

   // Extends the standard rootui5 GL panel controller (without touching ROOT's own sources) to add a per-panel "Transform" tab for RhoZ/RPhi projection views, controlling their fish-eye distortion (EventManager::set{RhoZ,RPhi}Distortion{Strength,Radius}).

   function findEventManager(mgr)
   {
      let world = mgr && mgr.childs && mgr.childs[0] && mgr.childs[0].childs;
      if (!world) return null;
      for (let i = world.length - 1; i >= 0; --i)
         if (world[i]._typename === "EventManager") return world[i];
      return null;
   }

   // Every value reprojects and re-streams the whole scene, so a drag produces far
   // more updates than the server can process. Keep only the latest value per
   // function and send it once the previous round trip is done, dropping the ones
   // in between. Unlike skipping events outright, the value the slider ends on is
   // always sent, even when it arrives while the server is still busy.
   function makeSender(ctrl)
   {
      const kSendMS = 150; // at most one update per this interval, per function

      let pending = new Map(); // funcName -> value waiting to be sent
      let sent    = new Map(); // funcName -> value sent last
      let timer   = null;

      function flush()
      {
         timer = null;

         if (ctrl.mgr.busyProcessingChanges) { // still redrawing, come back later
            timer = setTimeout(flush, kSendMS);
            return;
         }

         let em = findEventManager(ctrl.mgr);
         if (em) {
            for (let [funcName, value] of pending) {
               if (sent.get(funcName) === value) continue; // nothing new to say
               sent.set(funcName, value);
               ctrl.mgr.SendMIR(funcName + "(" + value + ")", em.fElementId, "EventManager");
            }
         }
         pending.clear();
      }

      return function (funcName, value)
      {
         pending.set(funcName, value);
         if (!timer) timer = setTimeout(flush, kSendMS);
      };
   }

   function makeTransformControls(ctrl, kind)
   {
      let sendMIR = makeSender(ctrl);

      let strengthSlider = new sap.m.Slider({ min: 0, max: 2, step: 0.05, width: "100%", showAdvancedTooltip: true });
      strengthSlider.attachLiveChange(function () { sendMIR("set" + kind + "DistortionStrength", this.getValue()); });
      strengthSlider.attachChange(function () { sendMIR("set" + kind + "DistortionStrength", this.getValue()); });

      let radiusSlider = new sap.m.Slider({ min: 50, max: 800, step: 10, width: "100%", showAdvancedTooltip: true });
      radiusSlider.attachLiveChange(function () { sendMIR("set" + kind + "DistortionRadius", this.getValue()); });
      radiusSlider.attachChange(function () { sendMIR("set" + kind + "DistortionRadius", this.getValue()); });

      let panel = new sap.m.Panel({
         headerText: "Transform",
         width: "16rem",
         content: [
            new sap.m.Label({ text: "Fish-eye Distortion Strength", width: "100%" }),
            strengthSlider,
            new sap.m.Label({ text: "Fish-eye Distortion Radius", width: "100%" }),
            radiusSlider
         ]
      });
      panel.addStyleClass("sapUiTinyMargin");

      let popover = new sap.m.Popover({
         placement: sap.m.PlacementType.Bottom,
         showHeader: false,
         content: [ panel ]
      });

      return { popover: popover, strengthSlider: strengthSlider, radiusSlider: radiusSlider };
   }

   let origCreateScenes = GLController.prototype.createScenes;

   GLController.prototype.createScenes = function ()
   {
      origCreateScenes.apply(this, arguments);

      if (this._transformTabAdded) return;

      let element = this.mgr.GetElement(this.eveViewerId);
      if (!element) return;

      let kind = null;
      if (element.fName.indexOf("RhoZ") === 0) kind = "RhoZ";
      else if (element.fName.indexOf("RPhi") === 0) kind = "RPhi";
      if (!kind) return;

      this._transformTabAdded = true;

      let ctrl = this;
      let t = makeTransformControls(ctrl, kind);

      let infoBtn = new sap.m.Button({
         icon: "sap-icon://message-information",
         tooltip: "Transform",
         press: function (oEvent)
         {
            let em = findEventManager(ctrl.mgr);
            if (em) {
               t.strengthSlider.setValue(kind === "RhoZ" ? em.rhoZDistortionStrength : em.rPhiDistortionStrength);
               t.radiusSlider.setValue(kind === "RhoZ" ? em.rhoZDistortionRadius : em.rPhiDistortionRadius);
            }
            t.popover.openBy(oEvent.getSource());
         }
      });

      ctrl.getView().byId("tbar").insertContent(infoBtn, 1);
   };

   return {};
});
